#pragma once

#include "Engine.h"

#include "Common/Common.h"
#include "Common/Log.h"

#include "Vulkan/VulkanCommon.h"

#include "Application/AppModuleBase.h"
#include "Application/GenericWindow.h"
#include "Application/GenericApplication.h"

#include <vector>

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES// force GLM to use a version of vec2 and mat4 that has the alignment requirements already specified for us.




#include <optional>
//#include "G:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.28.29333\include\optional"

#include <glm/glm.hpp>
#include <glm/ext.hpp> // perspective, translate, rotate


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <endian.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image/stb_image.h>

//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
//#include <tinyobj/tiny_obj_loader_me.h>

//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>


#include <fstream>
#include <stdexcept>
#include <algorithm>
//#include <functional>
#include <chrono>

#include <vector>
#include <cstring>
#include <cstdlib>
#include <array>
#include <cstdint>
#include <optional>
//#include "G:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.28.29333\include\optional"
#include <set>
#include <unordered_map>
//#include <unique_ptr>
#include <memory>

//#include <QtCore/qfileinfo.h>

#include <stdio.h>
//#include <QtCore/QVector>
//#include <QtCore/QCollator>
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <zlib.h>
//#include <QtCore/QTimer>
//#include <QtCore/QDir>
#include <chrono>
 
#include <string>

#include <utils/util_init.hpp>
#include <utils/util.hpp>

#include <utils/formatUtils.h>



class DemoBase : public AppModuleBase
{
public:
	DemoBase(int32 width, int32 height, const char* title, const std::vector<std::string>& cmdLine)
		: AppModuleBase(width, height, title)
		, m_Device(VK_NULL_HANDLE)
		, m_VulkanDevice(nullptr)
		, m_GfxQueue(VK_NULL_HANDLE)
		, m_PresentQueue(VK_NULL_HANDLE)
		, m_FrameWidth(0)
		, m_FrameHeight(0)
		, m_PipelineCache(VK_NULL_HANDLE)
		, m_PresentComplete(VK_NULL_HANDLE)
		, m_RenderComplete(VK_NULL_HANDLE)
		, m_CommandPool(VK_NULL_HANDLE)
		, m_WaitStageMask(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
		, m_SwapChain(VK_NULL_HANDLE)
	{

	}

	virtual ~DemoBase()
	{

	}

	VkDevice GetDeviceHandle()
	{
		return GetVulkanRHI()->GetDevice()->GetInstanceHandle();
	}

	virtual bool PreInit() override
	{

		return true;
	}
	
	virtual bool Init() override
	{

		return true;
	}

	virtual void Loop(float time, float delta) override
	{

	}

	virtual void Exist() override
	{

	}
    
    virtual void UpdateFPS(float time, float delta)
    {
        m_FrameCounter  += 1;
        m_LastFrameTime += delta;
        if (m_LastFrameTime >= 1.0f) 
		{
            m_LastFPS = m_FrameCounter;
            m_FrameCounter  = 0;
            m_LastFrameTime = 0.0f;
        }
    }
    
	void Setup();

	void SetupIfNeedSaveImage(bool isNeed = false);

	void Prepare() override
	{
        AppModuleBase::Prepare();
		CreateFences();
		CreateCommandBuffers();
		CreatePipelineCache();
		CreateDefaultRes();
	}

	void Release() override
	{
        AppModuleBase::Release();
		DestroyDefaultRes();
		DestroyFences();
		DestroyCommandBuffers();
		DestroyPipelineCache();
	}

	void Present(int backBufferIndex);

	int32 AcquireBackbufferIndex();

	uint32 GetMemoryTypeFromProperties(uint32 typeBits, VkMemoryPropertyFlags properties);

    bool initRenderInfo();

private:



	void CreateDefaultRes();

	void DestroyDefaultRes();

	void CreateCommandBuffers();

	void DestroyCommandBuffers();

	void CreateFences();

	void DestroyFences();

	void DestroyPipelineCache();

	void CreatePipelineCache();



    
protected:


	typedef std::shared_ptr<VulkanSwapChain> VulkanSwapChainRef;
    
	VkDevice						m_Device;
	VkPhysicalDevice                m_PhysicalDevice;
	std::shared_ptr<VulkanDevice>	m_VulkanDevice;
	VkQueue							m_GfxQueue;
	VkQueue							m_PresentQueue;

	int32							m_FrameWidth;
	int32							m_FrameHeight;
    
	VkPipelineCache                 m_PipelineCache;
    
	std::vector<VkFence> 			m_Fences;
	VkSemaphore 					m_PresentComplete;
	VkSemaphore 					m_RenderComplete;

	VkCommandPool					m_CommandPool;
	VkCommandPool					m_ComputeCommandPool;
	std::vector<VkCommandBuffer>	m_CommandBuffers;
    
	VkPipelineStageFlags			m_WaitStageMask;
    
	VulkanSwapChainRef				m_SwapChain;

	bool m_NeedSaveImage = false;
    
    int32                           m_FrameCounter = 0;
    float                           m_LastFrameTime = 0.0f;
    float                           m_LastFPS = 0.0f;

    size_t currentFrame = 0;
    int lastFrameNum = 0;
    int curFrameNum = 0;

    VkSwapchainKHR db_swapChain;
    std::vector<VkImage> db_swapChainImages;
    VkFormat db_swapChainImageFormat;
    VkExtent2D db_swapChainExtent;
    std::vector<VkImageView> db_swapChainImageViews;
    std::vector<VkFramebuffer> db_swapChainFramebuffers;

    sample_info renderInfo = {};

    VkSwapchainCreateInfoKHR swapChainCreateInfo = {};

    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    std::vector<VkPhysicalDevice> physicalDevices;
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkDevice curVkDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    uint32_t availableValidationLayerFoundCount = 0;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    uint32_t mipLevels = 0;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;


//    std::vector<Vertex> initializedVertices;
//    std::vector<uint32_t> initializedIndices;
//    std::vector<index_t> initializedFaces;
//    std::vector<tinyobj::shape_t> initializedShapes;
//    tinyobj::attrib_t initializedAttrib;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkDeviceSize vertexBufferSize;
    VkDeviceSize indexBufferSize;

//    VkBuffer stagingBuffer;
//    VkDeviceMemory stagingBufferMemory;

//    std::vector<Vertex> firstVertices;
//    std::vector<Vertex> curVertices;
    std::vector<float> curVertices;



    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;


    /*VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;*/

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

    uint32_t imageIndex = 0;
    bool framebufferResized = false;



};
