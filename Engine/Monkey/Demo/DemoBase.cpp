﻿#include "DemoBase.h"
#include "DVKDefaultRes.h"
#include "DVKCommand.h"
#include <utils/util_init.hpp>
//#include "../../../3rdparty/utils/util_init.hpp"
#include "GlobalManager.h"

void DemoBase::Setup()
{
	auto vulkanRHI    = GetVulkanRHI();
	auto vulkanDevice = vulkanRHI->GetDevice();

	m_SwapChain     = vulkanRHI->GetSwapChain();

    curVkDevice = m_Device		= vulkanDevice->GetInstanceHandle();
    physicalDevice = m_PhysicalDevice = vulkanDevice->GetPhysicalHandle();
//    renderInfo.gpus = physicalDevices = vulkanRHI->GetPhysicalDevices();
    physicalDevices = vulkanRHI->GetPhysicalDevices();
	m_VulkanDevice	= vulkanDevice;
	graphicsQueue = m_GfxQueue		= vulkanDevice->GetGraphicsQueue()->GetHandle();
	presentQueue = m_PresentQueue	= vulkanDevice->GetPresentQueue()->GetHandle();
    
	m_FrameWidth	= vulkanRHI->GetSwapChain()->GetWidth();
	m_FrameHeight	= vulkanRHI->GetSwapChain()->GetHeight();
}

void DemoBase::SetupIfNeedSaveImage(bool isNeed) {
    m_NeedSaveImage = isNeed;
}

int32 DemoBase::AcquireBackbufferIndex()
{
	int32 backBufferIndex = m_SwapChain->AcquireImageIndex(&m_PresentComplete);
	return backBufferIndex;
}

void DemoBase::Present(int backBufferIndex)
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType 				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pWaitDstStageMask 	= &m_WaitStageMask;									
	submitInfo.pWaitSemaphores 		= &m_PresentComplete;
	submitInfo.waitSemaphoreCount 	= 1;
	submitInfo.pSignalSemaphores 	= &m_RenderComplete;
	submitInfo.signalSemaphoreCount = 1;											
	submitInfo.pCommandBuffers 		= &(m_CommandBuffers[backBufferIndex]);
	submitInfo.commandBufferCount 	= 1;												
	
    vkResetFences(m_Device, 1, &(m_Fences[backBufferIndex]));

	VERIFYVULKANRESULT(vkQueueSubmit(m_GfxQueue, 1, &submitInfo, m_Fences[backBufferIndex]));
    vkWaitForFences(m_Device, 1, &(m_Fences[backBufferIndex]), true, MAX_uint64);
    
    // present
    m_SwapChain->Present(m_VulkanDevice->GetGraphicsQueue(), m_VulkanDevice->GetPresentQueue(), &m_RenderComplete);

    renderInfo.current_buffer = backBufferIndex;

//    curFrameNum++;
}

uint32 DemoBase::GetMemoryTypeFromProperties(uint32 typeBits, VkMemoryPropertyFlags properties)
{
	uint32 memoryTypeIndex = 0;
	GetVulkanRHI()->GetDevice()->GetMemoryManager().GetMemoryTypeFromProperties(typeBits, properties, &memoryTypeIndex);
	return memoryTypeIndex;
}

void DemoBase::DestroyPipelineCache()
{
	VkDevice device = GetVulkanRHI()->GetDevice()->GetInstanceHandle();
	vkDestroyPipelineCache(device, m_PipelineCache, VULKAN_CPU_ALLOCATOR);
	m_PipelineCache = VK_NULL_HANDLE;
}

void DemoBase::CreatePipelineCache()
{
	VkDevice device = GetVulkanRHI()->GetDevice()->GetInstanceHandle();

	VkPipelineCacheCreateInfo createInfo;
    ZeroVulkanStruct(createInfo, VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO);
    VERIFYVULKANRESULT(vkCreatePipelineCache(device, &createInfo, VULKAN_CPU_ALLOCATOR, &m_PipelineCache));
}

void DemoBase::CreateFences()
{
	VkDevice device  = GetVulkanRHI()->GetDevice()->GetInstanceHandle();
    int32 frameCount = GetVulkanRHI()->GetSwapChain()->GetBackBufferCount();
        
	VkFenceCreateInfo fenceCreateInfo;
	ZeroVulkanStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        
    m_Fences.resize(frameCount);
	for (int32 i = 0; i < m_Fences.size(); ++i) {
		VERIFYVULKANRESULT(vkCreateFence(device, &fenceCreateInfo, VULKAN_CPU_ALLOCATOR, &m_Fences[i]));
	}

	VkSemaphoreCreateInfo createInfo;
	ZeroVulkanStruct(createInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
	vkCreateSemaphore(device, &createInfo, VULKAN_CPU_ALLOCATOR, &m_RenderComplete);
}

void DemoBase::DestroyFences()
{
	VkDevice device = GetVulkanRHI()->GetDevice()->GetInstanceHandle();

	for (int32 i = 0; i < m_Fences.size(); ++i) {
		vkDestroyFence(device, m_Fences[i], VULKAN_CPU_ALLOCATOR);
	}

	vkDestroySemaphore(device, m_RenderComplete, VULKAN_CPU_ALLOCATOR);
}

void DemoBase::CreateDefaultRes()
{
	vk_demo::DVKCommandBuffer* cmdbuffer = vk_demo::DVKCommandBuffer::Create(GetVulkanRHI()->GetDevice(), m_CommandPool);
	vk_demo::DVKDefaultRes::Init(GetVulkanRHI()->GetDevice(), cmdbuffer);
	delete cmdbuffer;
}

void DemoBase::DestroyDefaultRes()
{
	vk_demo::DVKDefaultRes::Destroy();
}

void DemoBase::CreateCommandBuffers()
{
	VkDevice device = GetVulkanRHI()->GetDevice()->GetInstanceHandle();
        
	VkCommandPoolCreateInfo cmdPoolInfo;
	ZeroVulkanStruct(cmdPoolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
	cmdPoolInfo.queueFamilyIndex = GetVulkanRHI()->GetDevice()->GetPresentQueue()->GetFamilyIndex();
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VERIFYVULKANRESULT(vkCreateCommandPool(device, &cmdPoolInfo, VULKAN_CPU_ALLOCATOR, &m_CommandPool));

    commandPool = m_CommandPool;

	VkCommandPoolCreateInfo computePoolInfo;
	ZeroVulkanStruct(computePoolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
	computePoolInfo.queueFamilyIndex = GetVulkanRHI()->GetDevice()->GetComputeQueue()->GetFamilyIndex();
	computePoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	VERIFYVULKANRESULT(vkCreateCommandPool(device, &computePoolInfo, VULKAN_CPU_ALLOCATOR, &m_ComputeCommandPool));

    VkCommandBufferAllocateInfo cmdBufferInfo;
    ZeroVulkanStruct(cmdBufferInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
    cmdBufferInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferInfo.commandBufferCount = 1;
    cmdBufferInfo.commandPool        = m_CommandPool;
        
    m_CommandBuffers.resize(GetVulkanRHI()->GetSwapChain()->GetBackBufferCount());
    for (int32 i = 0; i < m_CommandBuffers.size(); ++i) {
        vkAllocateCommandBuffers(device, &cmdBufferInfo, &(m_CommandBuffers[i]));
    }
}

void DemoBase::DestroyCommandBuffers()
{
	VkDevice device = GetVulkanRHI()->GetDevice()->GetInstanceHandle();
    for (int32 i = 0; i < m_CommandBuffers.size(); ++i) {
        vkFreeCommandBuffers(device, m_CommandPool, 1, &(m_CommandBuffers[i]));
    }

	vkDestroyCommandPool(device, m_CommandPool, VULKAN_CPU_ALLOCATOR);
    
    vkDestroyCommandPool(device, m_ComputeCommandPool, VULKAN_CPU_ALLOCATOR);
}


bool DemoBase::initRenderInfo()
{

    init_instance_extension_names(renderInfo);
    init_device_extension_names(renderInfo);
//          return false;
    renderInfo.save_images = true;
//        renderInfo
//    renderInfo.device = curVkDevice;
//    renderInfo.width = WIDTH;
//    renderInfo.height = HEIGHT;
    renderInfo.format = m_SwapChain->GetColorFormat();
//    renderInfo.swap_chain = swapChain;
//    renderInfo.graphics_queue = graphicsQueue;
//    renderInfo.present_queue = presentQueue;
    renderInfo.memory_properties = gMemProperties;

    renderInfo.device = m_Device;
    renderInfo.physicalDevice = m_PhysicalDevice;
    renderInfo.gpus = physicalDevices;
    renderInfo.width = m_Width;
    renderInfo.height = m_Height;
//    renderInfo.format = m_SwapChain;
    renderInfo.swap_chain = m_SwapChain->GetInstanceHandle();
    renderInfo.cmd_pool = m_CommandPool;
    renderInfo.graphics_queue = m_GfxQueue;
    renderInfo.present_queue = m_PresentQueue;

    db_swapChainImages = GetVulkanRHI()->GetBackbufferImages();
    db_swapChainImageViews = GetVulkanRHI()->GetBackbufferViews();

    for (int i = 0; i < db_swapChainImages.size(); ++i) {
        swap_chain_buffer sc_buffer;
        sc_buffer.image = db_swapChainImages[i];
        sc_buffer.view = db_swapChainImageViews[i];
        renderInfo.buffers.push_back(sc_buffer);
    }

    init_command_buffer(renderInfo);

    return true;
}
