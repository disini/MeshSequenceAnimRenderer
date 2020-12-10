#include "Common/Common.h"
#include "Common/Log.h"

#include "Demo/DVKCommon.h"
#include "Demo/DVKCamera.h"

#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

//#include "Demo/DemoBase.h"

//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES// force GLM to use a version of vec2 and mat4 that has the alignment requirements already specified for us.


//#define VK_USE_PLATFORM_XCB
//#define VK_USE_PLATFORM_XCB_KHR
#include <utils/util_init.hpp>
#include <utils/util.hpp>
//#include <util_init.hpp>
//#include <glm/detail/setup.hpp>
#include <optional>
//#include "G:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.28.29333\include\optional"

#include <glm/glm.hpp>
#include <glm/ext.hpp> // perspective, translate, rotate

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <endian.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
#include <tinyobj/tiny_obj_loader_me.h>


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


#include <stdio.h>


#include <QtCore/qfileinfo.h>
#include <QtCore/QVector>
#include <QtCore/QCollator>
#include <QtCore/QTimer>
#include <QtCore/QDir>
//#include <QWidget>

#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <zlib.h>

#include "utils/GlobalManager.h"


#include <chrono>

// #define NDEBUG // not debug

using namespace std;
using namespace chrono;
using namespace tinyobj;

const int WIDTH = 1600;
const int HEIGHT = 900;

//const std::string MODEL_PATH = "models/viking_room.obj";
//const std::string TEXTURE_PATH = "textures/viking_room.png";

#ifdef __linux__
const std::string MODEL_PATH_PREFIX = "/media/liusheng/Apps/projects/3DMM/body/mesh_sorted/";
#elif _WIN32
//const std::string MODEL_PATH_PREFIX = "D:/projects/3DMM/obj/";
//const std::string MODEL_PATH_PREFIX = "D:/projects/3DMM/body/mesh/";
const std::string MODEL_PATH_PREFIX = "D:/projects/3DMM/body/mesh_sorted/";
#else
const std::string MODEL_PATH_PREFIX = "";
#endif

#ifdef __linux__
const std::string FIRST_MODEL_PATH = "/media/liusheng/Apps/projects/3DMM/body/wangzhi/nvrentou0001.obj";
#elif _WIN32
const std::string FIRST_MODEL_PATH = "D:/projects/3DMM/body/wangzhi/nvrentou0001.obj";
#else
const std::string FIRST_MODEL_PATH = "";
#endif

const std::string MODEL_PATH_POSTFIX = "_mesh.obj";

#ifdef __linux__
const std::string TEXTURE_PATH = "/media/liusheng/Apps/projects/3DMM/body/wangzhi/difuse.jpg";
#elif _WIN32
//const std::string TEXTURE_PATH = "K:/Projects/moviebook/creativity/LSVulkanKit/Obj_Sequence_Renderer/ObjS_Renderer/App01/textures/viking_room.png";
const std::string TEXTURE_PATH = "D:/projects/3DMM/body/wangzhi/difuse.jpg";
#else
const std::string TEXTURE_PATH = "";
#endif


const int MAX_FRAMES_IN_FLIGHT = 2;

const int MAX_OBJS_PRELOAD = 100;

//const std::vector<const char*> validationLayers = {
//    "VK_LAYER_KHRONOS_validation"
//};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

int curLodingModelIndex = 0;

int curLodedModelIndex = 0;

int totalModelsNumber = 0;
//sample_info renderInfo = {};


char curOutputImgNameStr[20] = "00000000000";

//std::vector<float> initializedVertices;
//std::vector<uint32> initializedIndices;
//vk_demo::DVKMesh* initializedMesh;
//vk_demo::DVKBoundingBox initializedBounding;
//int32 initializedVerticesDataStride;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    };


    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator == (const Vertex & other) const  {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1 )) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct SingleMeshData {
    std::string fileName;
    std::string fileIndex;
//        tinyobj::attrib_t attrib;
//        std::vector<tinyobj::shape_t> shapes;
//        std::vector<tinyobj::material_t> materials;
//        std::string warn;
//        std::string err;
    std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
//        uniqueVertices.
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

};



class TextureModule : public DemoBase
{
public:
	TextureModule(int32 width, int32 height, const char* title, const std::vector<std::string>& cmdLine)
		: DemoBase(width, height, title, cmdLine)
	{
        
	}
    
	virtual ~TextureModule()
	{

	}

	virtual bool PreInit() override
	{
		return true;
	}

	virtual bool Init() override
	{
		DemoBase::Setup();
		DemoBase::Prepare();

		LoadAssets();
		CreateGUI();
		CreateUniformBuffers();
		CreateDescriptorSetLayout();
        CreateDescriptorSet();
		CreatePipelines();
		SetupCommandBuffers();
        if(!getFilesList())
            return false;
//        if(!loadFirstModel())
//            return false;
        if(!initRenderInfo())
            return false;
//        curVkDevice = m_Device;
		m_Ready = true;

		m_NeedSaveImage = true;

		return true;
	}

	virtual void Exist() override
	{
		DemoBase::Release();

		DestroyAssets();
		DestroyGUI();
        DestroyDescriptorSetLayout();
		DestroyPipelines();
		DestroyUniformBuffers();
	}

	virtual void Loop(float time, float delta) override
	{
        if(!loadRestModels1())
            return;

        if (!m_Ready) {
			return;
		}
		Draw(time, delta);
	}

private:
    
	struct MVPBlock
	{
		Matrix4x4 model;
		Matrix4x4 view;
		Matrix4x4 projection;
	};

	struct ParamBlock
	{
		Vector3 lightDir;
		float curvature;

		Vector3 lightColor;
		float exposure;

		Vector2 curvatureScaleBias;
		float blurredLevel;
		float padding;
	};

//    void Console()
//    {
//        AllocConsole();
//        FILE *pFileCon = NULL;
//        pFileCon = freopen("CONOUT$", "w", stdout);
//
//        COORD coordInfo;
//        coordInfo.X = 130;
//        coordInfo.Y = 9000;
//
//        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
//        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),ENABLE_QUICK_EDIT_MODE| ENABLE_EXTENDED_FLAGS);
//    }

//    bool loadFirstModel(){
//
//        auto start = system_clock::now();
//        int i = 0;
//
//        cout << "loadModels() : loading model number :  " << i << endl;// uniqueVertices.size == 3566
////            auto objMeshData = std::make_unique<SingleMeshData>();
//        SingleMeshData* objMeshData = new SingleMeshData();
//        std::string path = FIRST_MODEL_PATH;
//        cout << "loadFirstModel() : currrent model path is " << path << endl;
//        if(!loadModel1(path, objMeshData, i))
//        {
//            throw std::runtime_error("failed to load model number " + to_string(i));
//        }
//
//        meshSequenceData[std::to_string(i)] = objMeshData;
//
////        if(objMeshData != nullptr)
////        {
////            delete objMeshData;
////            objMeshData = nullptr;
////        }
//
//        loadedModelCount++;
//
//        auto end = system_clock::now();
//        auto duration = duration_cast<microseconds>(end - start);
//        cout << "total loding time cost : " << double(duration.count()) *microseconds::period::num / microseconds::period::den << "seconds" << endl;
//        return true;
//    }

    bool loadRestModels1() {

        gIsFirstModel = false;

        curLoadingModelIndex = loadedModelCount;

        if(curLoadingModelIndex >= objFileCount)
        {
//            usleep(33000);//force changing fps!
            return true;
//            curLoadingModelIndex %= objFileCount;
        }
        cout << "loadModels() : loading model number :  " << curLoadingModelIndex << endl;// uniqueVertices.size == 3566
        std::string path = (std::string)((const char*)(objFileList[curLoadingModelIndex].absoluteFilePath()).toLocal8Bit());
        cout << "loadRestModels1() : currrent model path is " << path << endl;
//        if(!loadModel1(path, objMeshData, curLoadingModelIndex))

        vk_demo::DVKCommandBuffer* cmdBuffer = vk_demo::DVKCommandBuffer::Create(m_VulkanDevice, m_CommandPool);
//        m_CmdBuffer = cmdBuffer;

        vk_demo::DVKModel* model = vk_demo::DVKModel::LoadFromFile(
//			"assets/models/head.obj",
//                "assets/models/wangzhi/nvrentou0001.obj",
                path,
     m_VulkanDevice,
                cmdBuffer,
//                m_CmdBuffer,
                { VertexAttribute::VA_Position, VertexAttribute::VA_UV0, VertexAttribute::VA_Normal, VertexAttribute::VA_Tangent }, false
        );

        SingleMeshData1* objMeshData = new SingleMeshData1();
        objMeshData->vertices = model->curMeshVertices;
//        objMeshData->indices = model->curMeshIndices;
        if (gIsFirstModel) {
            objMeshData->indices = model->curMeshIndices;
        }
        else
        {
//            objMeshData->indices = m_Model->initializedIndices;
            objMeshData->indices = initializedIndices;
        }

        meshSequenceData1[std::to_string(curLoadingModelIndex)] = objMeshData;

        //        if(curLoadingModelIndex > 2)
//        {
//            clearOldFrameData();
//        if(objMeshData != nullptr)
//        {
//            delete objMeshData;
//            objMeshData = nullptr;
//        }
//        }

        loadedModelCount++;

//        delete cmdBuffer;
        return true;
    }


//    bool loadRestModels(){
//
//        auto start = system_clock::now();
//        curLoadingModelIndex = loadedModelCount;
//
//
//        if(curLoadingModelIndex >= objFileCount)
//        {
////            usleep(33000);//force changing fps!
//            return true;
////            curLoadingModelIndex %= objFileCount;
//        }
//
//        cout << "loadModels() : loading model number :  " << curLoadingModelIndex << endl;// uniqueVertices.size == 3566
////            auto objMeshData = std::make_unique<SingleMeshData>();
//        SingleMeshData* objMeshData = new SingleMeshData();
//        std::string path = (std::string)((const char*)(objFileList[curLoadingModelIndex].absoluteFilePath()).toLocal8Bit());
//        cout << "loadRestModels() : currrent model path is " << path << endl;
//        if(!loadModel1(path, objMeshData, curLoadingModelIndex))
//        {
//            throw std::runtime_error("failed to loadwenjianming  model number " + to_string(curLoadingModelIndex));
//        }
//
//        meshSequenceData[std::to_string(curLoadingModelIndex)] = objMeshData;
//
////        if(curLoadingModelIndex > 2)
////        {
////            clearOldFrameData();
//            if(objMeshData != nullptr)
//            {
//                delete objMeshData;
//                objMeshData = nullptr;
//            }
////        }
//
//        loadedModelCount++;
//
//        auto end = system_clock::now();
//        auto duration = duration_cast<microseconds>(end - start);
//        cout << "total loding time cost : " << double(duration.count()) *microseconds::period::num / microseconds::period::den << "seconds" << endl;
//        return true;
//    }

    bool getFilesList() {
        path = QString::fromLocal8Bit(MODEL_PATH_PREFIX.c_str());
        objFileList.clear();
        QDir dir(path);
        if(!dir.exists())
            return false;

        QStringList filter;
        filter<<"*.obj";
        dir.setNameFilters(filter);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setSorting(QDir::NoSort);  // will sort manually with std::sort
//        dir.setSorting(QDir::Name | QDir::Reversed);
        objFileList = dir.entryInfoList();// get file info list;
        objStringList = dir.entryList();

        QCollator collator;
        collator.setNumericMode(true);

        std::sort(objStringList.begin(),
                  objStringList.end(),
                  [& collator](const QString &file1, const QString &file2)
                  {
                      return collator.compare(file1, file2) < 0;
                  });

//        objFileCount = objFileList.count();
        objFileCount = objStringList.count();

        return true;
    }

//    bool loadModel1(const std::string path, SingleMeshData* data, uint32_t modelIndex) {
//        tinyobj::attrib_t attrib;
//        std::vector<tinyobj::shape_t> shapes;
//        std::vector<tinyobj::material_t> materials;
//        std::string warn, err;
//        bool isFirstModel = true;
//        int curCount = 0;
//        if(modelIndex > 0)
//        {
//            isFirstModel = false;
//        }
//
//        gIsFirstModel = isFirstModel;
//
//        cout << "loadModel1() : currrent model path is " << path << endl;
////        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
////        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
//        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), NULL, true, true, !isFirstModel)) {
//
//            throw std::runtime_error(warn + err);
//            return false;
//        }
//
////        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
//
//        if(modelIndex == 0 && shapes.size() > 0)
//        {
//            initializedShapes = shapes;
//            initializedAttrib = attrib;
//        }
//
//        std::vector<tinyobj::shape_t> tempShapes = initializedShapes;
//
////        for (const auto& shape : shapes) {
//        for (const auto& shape : tempShapes) {
//            if(isFirstModel)
//                initializedFaces = shape.mesh.indices;
//
//            for (const auto& index : shape.mesh.indices) {
//                Vertex vertex = {};
//
//                if(index.vertex_index >= 0 && attrib.vertices.size() > 0)
//                {
//                    vertex.pos = {
//                        attrib.vertices[3 * index.vertex_index + 0],
//                        attrib.vertices[3 * index.vertex_index + 1],
//                        attrib.vertices[3 * index.vertex_index + 2]
//                    };
//                }
//
//
//                if(index.texcoord_index >= 0 && attrib.texcoords.size() > 0)
//                {
//                    vertex.texCoord = {
//                        attrib.texcoords[2 * index.texcoord_index + 0],
//                        //attrib.texcoords[2 * index.texcoord_index + 1]
//                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//                    };
//                }
//                else
//                {
//                    if(!isFirstModel)
//                    {
//                        vertex.texCoord = {
//                            initializedAttrib.texcoords[2 * initializedFaces[curCount].texcoord_index + 0],
//                            1.0f - initializedAttrib.texcoords[2 * initializedFaces[curCount].texcoord_index + 1]
//                        };
//                    }
//                }
//
//                if(index.vertex_index >= 0 && attrib.colors.size() > 0)
//                {
////                vertex.color = { 1.0f, 1.0f, 1.0f };
//                    vertex.color = { attrib.colors[3 * index.vertex_index + 0], attrib.colors[3 * index.vertex_index + 1], attrib.colors[3 * index.vertex_index + 2] };
//
//                }
//
////                if (data->uniqueVertices.count(vertex) == 0) {// if not contained yet
////                    data->uniqueVertices[vertex] = static_cast<uint32_t>(data->vertices.size());
////                    data->vertices.push_back(vertex);
////                }
//
//                data->vertices.push_back(vertex);
//                //indices.push_back(indices.size());
//                if(isFirstModel)
//                {
////                    data->indices.push_back(data->uniqueVertices[vertex]);
//                    data->indices.push_back(data->indices.size());
//                }
//
//                curCount++;
//            }
//
//
//        }
//
////        indices = data->indices;
//
//        if(isFirstModel)
//        {
////            vertices = data->vertices;
//            initializedVertices = data->vertices;
//            initializedIndices = data->indices;
//        }
//        else
//        {
//            data->indices = initializedIndices;
//        }
//
//        cout << "vertices.size == " << data->vertices.size() << endl;// vertices.size == 11484(before); 3566(after)
//        cout << "indices.size == " << data->indices.size() << endl;// indices.size ==11484
//        cout << "uniqueVertices.size == " << data->uniqueVertices.size() << endl;// uniqueVertices.size == 3566
//        cout << "model " << modelIndex << " loaded successfully!" << "!\n" << endl;// uniqueVertices.size == 3566
//
//        if(modelIndex == MAX_OBJS_PRELOAD - 1)
//        {
//            cout << "index == " << (MAX_OBJS_PRELOAD - 1) << "! The last model data was loaded yet!" << "!\n" << endl;
//        }
//
//
//        return true;
//    }



    bool createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//        bufferInfo.size = sizeof(vertices[0]) * vertices.size();
        bufferInfo.size = size;
//        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if(vkCreateBuffer(curVkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(curVkDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
//        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(curVkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
            return false;
        }
        vkBindBufferMemory(curVkDevice, buffer, bufferMemory, 0);

        return true;

    }

    VkCommandBuffer beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(curVkDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    bool endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;


        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(curVkDevice, commandPool, 1, &commandBuffer);

        return true;
    }



    bool copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = 0;// start copy from start of src;
        copyRegion.dstOffset = 0;
//        copyRegion.dstOffset = currOffset;
        copyRegion.size = size;

        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);

        return true;
    }


    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        renderInfo.memory_properties = memProperties;
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        //renderInfo.memory_properties = memProperties;

        throw std::runtime_error("failed to find suitable memory type!");

    }

    bool updateVertexBuffer() {

        if(curFrameNum >= objFileCount) {
            return false;
        }
//        else
//        {
//            curFrameNum %= objFileCount;
//        }

        std::string keyStr = to_string(curFrameNum);
//        keyStr = "0";
        cout << " updateVertexBuffer() : keyStr == " << keyStr << endl;
//        curVertices = meshSequenceData[keyStr]->vertices;
        curVertices = meshSequenceData1[keyStr]->vertices;
        void* data;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        vertexBufferSize = m_Model->m_VertexBufferSize;
        indexBufferSize = m_Model->m_IndexBufferSize;
        createBuffer(vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        vkMapMemory(curVkDevice, stagingBufferMemory, 0, vertexBufferSize, 0, &data);
        memcpy(data, curVertices.data(), (size_t)vertexBufferSize);
//        vkFlushMappedMemoryRanges()
//        vkUnmapMemory(device, vertexBufferMemory);
        vkUnmapMemory(curVkDevice, stagingBufferMemory);

//        copyBuffer(stagingBuffer, vertexBuffer, vertexBufferSize);
        copyBuffer(stagingBuffer, m_Model->m_VertexBuffer, vertexBufferSize);

        vkDestroyBuffer(curVkDevice, stagingBuffer, nullptr);
        vkFreeMemory(curVkDevice, stagingBufferMemory, nullptr);

        return true;
    }


	void Draw(float time, float delta)
	{
		int32 bufferIndex = DemoBase::AcquireBackbufferIndex();

//        bool hovered = UpdateUI(time, delta);
//		if (!hovered) {
		if (true) {
			m_ViewCamera.Update(time, delta);
            m_ViewCamera.autoRotate(1, 0.1);
//            m_ParamData.lightDir.Set(std::cos(time), 0, std::sin(time));
//            m_ParamData.lightDir.Set(std::cos(time), std::sin(time), -1);
            m_ParamData.lightDir.Set(std::cos(curFrameNum * 0.02), std::sin(curFrameNum * 0.02), -1);
		}

		UpdateUniformBuffers(time, delta);
		
		DemoBase::Present(bufferIndex);

//		if(curFrameNum == 10)
//        {
//        write_png(renderInfo, string("output/png/frame_" + formatNumByDigit(curFrameNum)).c_str());
//            write_png(renderInfo, string("frame_" + formatNumByDigit(curFrameNum)).c_str());
//        }

        curFrameNum++;

        if(!updateVertexBuffer())
            return;
	}
    
	bool UpdateUI(float time, float delta)
	{
		m_GUI->StartFrame();
        
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			ImGui::Text("SSS Demo");

			for (int32 i = 0; i < m_Model->meshes.size(); ++i)
			{
				vk_demo::DVKMesh* mesh = m_Model->meshes[i];
				ImGui::Text("%-20s Tri:%d", mesh->linkNode->name.c_str(), mesh->triangleCount);
			}

			ImGui::SliderFloat("Curvature", &(m_ParamData.curvature),       0.0f, 10.0f);
			ImGui::SliderFloat2("CurvatureBias", (float*)&(m_ParamData.curvatureScaleBias), 0.0f, 1.0f);

			ImGui::SliderFloat("BlurredLevel", &(m_ParamData.blurredLevel), 0.0f, 12.0f);
			ImGui::SliderFloat("Exposure", &(m_ParamData.exposure),         0.0f, 10.0f);

			ImGui::SliderFloat3("LightDirection", (float*)&(m_ParamData.lightDir), -10.0f, 10.0f);
			ImGui::ColorEdit3("LightColor", (float*)&(m_ParamData.lightColor));

			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
		}
        
		bool hovered = ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered() || ImGui::IsRootWindowOrAnyChildHovered();

		m_GUI->EndFrame();
        
		if (m_GUI->Update()) {
			SetupCommandBuffers();
		}

		return hovered;
	}

	void LoadAssets()
	{
        gIsFirstModel = true;

        vk_demo::DVKCommandBuffer* cmdBuffer = vk_demo::DVKCommandBuffer::Create(m_VulkanDevice, m_CommandPool);
		m_CmdBuffer = cmdBuffer;

		m_Model = vk_demo::DVKModel::LoadFromFile(
//			"assets/models/head.obj",
    "assets/models/wangzhi/nvrentou0001.obj",
			m_VulkanDevice,
			cmdBuffer,
			{ VertexAttribute::VA_Position, VertexAttribute::VA_UV0, VertexAttribute::VA_Normal, VertexAttribute::VA_Tangent }
		);

//		m_TexDiffuse       = vk_demo::DVKTexture::Create2D("assets/textures/head_diffuse.jpg", m_VulkanDevice, cmdBuffer);
//		m_TexNormal        = vk_demo::DVKTexture::Create2D("assets/textures/head_normal.jpg", m_VulkanDevice, cmdBuffer);
        m_TexDiffuse       = vk_demo::DVKTexture::Create2D("assets/models/wangzhi/head_diffuse.jpg", m_VulkanDevice, cmdBuffer);
        m_TexNormal        = vk_demo::DVKTexture::Create2D("assets/models/wangzhi/head_normal.jpg", m_VulkanDevice, cmdBuffer);
		m_TexCurvature     = vk_demo::DVKTexture::Create2D("assets/textures/curvatureLUT.png", m_VulkanDevice, cmdBuffer);
		m_TexPreIntegrated = vk_demo::DVKTexture::Create2D("assets/textures/preIntegratedLUT.png", m_VulkanDevice, cmdBuffer);

//        SingleMeshData* objMeshData = new SingleMeshData();
//
//        meshSequenceData[std::to_string(curLoadingModelIndex)] = objMeshData;
//
        loadedModelCount++;
        m_FirstModel = m_Model;

		delete cmdBuffer;
	}

	void DestroyAssets()
	{
		delete m_Model;

		delete m_TexDiffuse;
		delete m_TexNormal;
		delete m_TexCurvature;
		delete m_TexPreIntegrated;
	}
    
	void SetupCommandBuffers()
	{
		VkCommandBufferBeginInfo cmdBeginInfo;
		ZeroVulkanStruct(cmdBeginInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);

		VkClearValue clearValues[2];
		clearValues[0].color        = { {0.2f, 0.2f, 0.2f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBeginInfo;
		ZeroVulkanStruct(renderPassBeginInfo, VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO);
        renderPassBeginInfo.renderPass      = m_RenderPass;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues    = clearValues;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent.width  = m_FrameWidth;
        renderPassBeginInfo.renderArea.extent.height = m_FrameHeight;
        
		for (int32 i = 0; i < m_CommandBuffers.size(); ++i)
		{
            renderPassBeginInfo.framebuffer = m_FrameBuffers[i];
            
			VERIFYVULKANRESULT(vkBeginCommandBuffer(m_CommandBuffers[i], &cmdBeginInfo));
			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            
            VkViewport viewport = {};
            viewport.x        = 0;
            viewport.y        = m_FrameHeight;
            viewport.width    = m_FrameWidth;
            viewport.height   = -(float)m_FrameHeight;    // flip y axis
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            
            VkRect2D scissor = {};
            scissor.extent.width  = m_FrameWidth;
            scissor.extent.height = m_FrameHeight;
            scissor.offset.x      = 0;
            scissor.offset.y      = 0;
            
            vkCmdSetViewport(m_CommandBuffers[i], 0, 1, &viewport);
            vkCmdSetScissor(m_CommandBuffers[i], 0, 1, &scissor);
            
            vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->pipeline);
            vkCmdBindDescriptorSets(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->pipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
            
            for (int32 meshIndex = 0; meshIndex < m_Model->meshes.size(); ++meshIndex) {
                m_Model->meshes[meshIndex]->BindDrawCmd(m_CommandBuffers[i]);
            }
			
			m_GUI->BindDrawCmd(m_CommandBuffers[i], m_RenderPass);

			vkCmdEndRenderPass(m_CommandBuffers[i]);
			VERIFYVULKANRESULT(vkEndCommandBuffer(m_CommandBuffers[i]));
		}
	}
    
	void CreateDescriptorSet()
	{
		VkDescriptorPoolSize poolSizes[2];
		poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = 2;
		poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = 4;
        
		VkDescriptorPoolCreateInfo descriptorPoolInfo;
		ZeroVulkanStruct(descriptorPoolInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO);
		descriptorPoolInfo.poolSizeCount = 2;
		descriptorPoolInfo.pPoolSizes    = poolSizes;
		descriptorPoolInfo.maxSets       = 1;
		VERIFYVULKANRESULT(vkCreateDescriptorPool(m_Device, &descriptorPoolInfo, VULKAN_CPU_ALLOCATOR, &m_DescriptorPool));
        
        VkDescriptorSetAllocateInfo allocInfo;
        ZeroVulkanStruct(allocInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO);
        allocInfo.descriptorPool     = m_DescriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts        = &m_DescriptorSetLayout;
        VERIFYVULKANRESULT(vkAllocateDescriptorSets(m_Device, &allocInfo, &m_DescriptorSet));
        
        VkWriteDescriptorSet writeDescriptorSet;
        
        ZeroVulkanStruct(writeDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
        writeDescriptorSet.dstSet          = m_DescriptorSet;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.pBufferInfo     = &(m_MVPBuffer->descriptor);
        writeDescriptorSet.dstBinding      = 0;
        vkUpdateDescriptorSets(m_Device, 1, &writeDescriptorSet, 0, nullptr);
        
        ZeroVulkanStruct(writeDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
        writeDescriptorSet.dstSet          = m_DescriptorSet;
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeDescriptorSet.pBufferInfo     = &(m_ParamBuffer->descriptor);
        writeDescriptorSet.dstBinding      = 1;
        vkUpdateDescriptorSets(m_Device, 1, &writeDescriptorSet, 0, nullptr);

		std::vector<vk_demo::DVKTexture*> textures = { m_TexDiffuse, m_TexNormal, m_TexCurvature, m_TexPreIntegrated };
		for (int32 i = 0; i < 4; ++i)
		{
			ZeroVulkanStruct(writeDescriptorSet, VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET);
			writeDescriptorSet.dstSet          = m_DescriptorSet;
			writeDescriptorSet.descriptorCount = 1;
			writeDescriptorSet.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDescriptorSet.pBufferInfo     = nullptr;
			writeDescriptorSet.pImageInfo      = &(textures[i]->descriptorInfo);
			writeDescriptorSet.dstBinding      = 2 + i;
			vkUpdateDescriptorSets(m_Device, 1, &writeDescriptorSet, 0, nullptr);
		}
	}
    
	void CreatePipelines()
	{
		VkVertexInputBindingDescription vertexInputBinding = m_Model->GetInputBinding();
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributs = m_Model->GetInputAttributes();
		
		vk_demo::DVKGfxPipelineInfo pipelineInfo;
        pipelineInfo.vertShaderModule = vk_demo::LoadSPIPVShader(m_Device, "assets/shaders/11_Texture/texture.vert.spv");
		pipelineInfo.fragShaderModule = vk_demo::LoadSPIPVShader(m_Device, "assets/shaders/11_Texture/texture.frag.spv");
		m_Pipeline = vk_demo::DVKGfxPipeline::Create(m_VulkanDevice, m_PipelineCache, pipelineInfo, { vertexInputBinding }, vertexInputAttributs, m_PipelineLayout, m_RenderPass);
		
		vkDestroyShaderModule(m_Device, pipelineInfo.vertShaderModule, VULKAN_CPU_ALLOCATOR);
		vkDestroyShaderModule(m_Device, pipelineInfo.fragShaderModule, VULKAN_CPU_ALLOCATOR);
	}
    
	void DestroyPipelines()
	{
        delete m_Pipeline;
        m_Pipeline = nullptr;
	}
	
	void CreateDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding layoutBindings[6] = { };
		layoutBindings[0].binding 			 = 0;
		layoutBindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBindings[0].descriptorCount    = 1;
		layoutBindings[0].stageFlags 		 = VK_SHADER_STAGE_VERTEX_BIT;
		layoutBindings[0].pImmutableSamplers = nullptr;

		layoutBindings[1].binding 			 = 1;
		layoutBindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBindings[1].descriptorCount    = 1;
		layoutBindings[1].stageFlags 		 = VK_SHADER_STAGE_FRAGMENT_BIT;
		layoutBindings[1].pImmutableSamplers = nullptr;

		for (int32 i = 0; i < 4; ++i)
		{
			layoutBindings[2 + i].binding 			 = 2 + i;
			layoutBindings[2 + i].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			layoutBindings[2 + i].descriptorCount    = 1;
			layoutBindings[2 + i].stageFlags 		 = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[2 + i].pImmutableSamplers = nullptr;
		}
		
		VkDescriptorSetLayoutCreateInfo descSetLayoutInfo;
		ZeroVulkanStruct(descSetLayoutInfo, VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO);
		descSetLayoutInfo.bindingCount = 6;
		descSetLayoutInfo.pBindings    = layoutBindings;
		VERIFYVULKANRESULT(vkCreateDescriptorSetLayout(m_Device, &descSetLayoutInfo, VULKAN_CPU_ALLOCATOR, &m_DescriptorSetLayout));
        
		VkPipelineLayoutCreateInfo pipeLayoutInfo;
		ZeroVulkanStruct(pipeLayoutInfo, VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO);
		pipeLayoutInfo.setLayoutCount = 1;
		pipeLayoutInfo.pSetLayouts    = &m_DescriptorSetLayout;
		VERIFYVULKANRESULT(vkCreatePipelineLayout(m_Device, &pipeLayoutInfo, VULKAN_CPU_ALLOCATOR, &m_PipelineLayout));
	}
    
	void DestroyDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, VULKAN_CPU_ALLOCATOR);
		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, VULKAN_CPU_ALLOCATOR);
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, VULKAN_CPU_ALLOCATOR);
	}
	
	void UpdateUniformBuffers(float time, float delta)
	{
		m_MVPData.view = m_ViewCamera.GetView();
		m_MVPData.projection = m_ViewCamera.GetProjection();

		m_MVPBuffer->CopyFrom(&m_MVPData, sizeof(MVPBlock));
		m_ParamBuffer->CopyFrom(&m_ParamData, sizeof(ParamBlock));
	}
    
	void CreateUniformBuffers()
	{
		vk_demo::DVKBoundingBox bounds = m_Model->rootNode->GetBounds();
		Vector3 boundSize   = bounds.max - bounds.min;
        Vector3 boundCenter = bounds.min + boundSize * 0.5f;
        float cameraPosZ = 0.0f;

//		m_MVPData.model.AppendRotation(180, Vector3::UpVector);
//        m_MVPData.model.AppendRotation(180, Vector3::RightVector);
        m_MVPData.model.AppendRotation(180, Vector3::ForwardVector);
//        m_MVPData.model.AppendTranslation(Vector3(0.0f, boundCenter.y * 2.0f, 0));// move model to the coordinates center
        m_MVPData.model.AppendTranslation(Vector3(0.0f, boundCenter.y * 2.0f, -boundCenter.z));// move model to the coordinates center

		m_ViewCamera.Perspective(PI / 4, GetWidth(), GetHeight(), 0.1f, 1000.0f);
        cameraPosZ = boundCenter.z - boundSize.Size() * 0.9f;
//		m_ViewCamera.SetPosition(boundCenter.x, boundCenter.y, boundCenter.z - boundSize.Size() * 2.5f);
//        m_ViewCamera.SetPosition(boundCenter.x, boundCenter.y * (-3.0f), cameraPosZ);
//        m_ViewCamera.SetPosition(boundCenter.x, boundCenter.y + m_MVPData.model.GetPosition().y, cameraPosZ);
//        m_ViewCamera.SetPosition(boundCenter.x, 0, cameraPosZ);
        m_ViewCamera.SetPosition(boundCenter.x, boundCenter.y, cameraPosZ);
//		m_ViewCamera.LookAt(boundCenter);
//        m_ViewCamera.LookAt(boundCenter.x, boundCenter.y, -boundSize.Size());
//        m_ViewCamera.LookAt(boundCenter.x, boundCenter.y, -boundSize.z);
        m_ViewCamera.LookAt(boundCenter.x, boundCenter.y, 0);
//        m_ViewCamera.LookAt(0, 0, 0);



		
		m_MVPBuffer = vk_demo::DVKBuffer::CreateBuffer(
			m_VulkanDevice, 
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			sizeof(MVPBlock),
			&(m_MVPData)
		);
		m_MVPBuffer->Map();

		m_ParamData.blurredLevel = 2.0;
		m_ParamData.curvature = 3.5;
		m_ParamData.curvatureScaleBias.x = 0.101;
		m_ParamData.curvatureScaleBias.y = -0.001;
		m_ParamData.exposure = 1.0;
		m_ParamData.lightColor.Set(240.0f / 255.0f, 200.0f / 255.0f, 166.0f / 255.0f);
		m_ParamData.lightDir.Set(1, 0, -1.0);
		m_ParamData.lightDir.Normalize();
		m_ParamData.padding = 0.0;
		m_ParamBuffer = vk_demo::DVKBuffer::CreateBuffer(
			m_VulkanDevice, 
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			sizeof(ParamBlock),
			&(m_ParamData)
		);
		m_ParamBuffer->Map();
	}
	
	void DestroyUniformBuffers()
	{
		m_MVPBuffer->UnMap();
		delete m_MVPBuffer;
		m_MVPBuffer = nullptr;

		m_ParamBuffer->UnMap();
		delete m_ParamBuffer;
		m_ParamBuffer = nullptr;
	}

	void CreateGUI()
	{
		m_GUI = new ImageGUIContext();
		m_GUI->Init("assets/fonts/Ubuntu-Regular.ttf");
	}

	void DestroyGUI()
	{
		m_GUI->Destroy();
		delete m_GUI;
	}

private:

	bool 							m_Ready = false;
    
	vk_demo::DVKCamera				m_ViewCamera;

	MVPBlock 						m_MVPData;
	vk_demo::DVKBuffer*				m_MVPBuffer;

	ParamBlock						m_ParamData;
	vk_demo::DVKBuffer*				m_ParamBuffer = nullptr;

	vk_demo::DVKTexture*			m_TexDiffuse = nullptr;
	vk_demo::DVKTexture*			m_TexNormal = nullptr;
	vk_demo::DVKTexture*			m_TexCurvature = nullptr;
	vk_demo::DVKTexture*			m_TexPreIntegrated = nullptr;
	
    vk_demo::DVKGfxPipeline*           m_Pipeline = nullptr;

	vk_demo::DVKModel*				m_Model = nullptr;

    VkDescriptorSetLayout 			m_DescriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout 				m_PipelineLayout = VK_NULL_HANDLE;
	VkDescriptorPool                m_DescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSet 				m_DescriptorSet = VK_NULL_HANDLE;
    
	ImageGUIContext*				m_GUI = nullptr;

    vk_demo::DVKCommandBuffer* m_CmdBuffer = nullptr;



    //----------------------------------------------------------20201016-------------------------------------//

//    std::unordered_map<std::string, std::unique_ptr<SingleMeshData>> meshSequenceData;
    std::unordered_map<std::string, const SingleMeshData*> meshSequenceData;
    std::unordered_map<std::string, const SingleMeshData1*> meshSequenceData1;


    QString objFileName;
    QStringList objStringList;
    QFileInfoList objFileList;
    QString path;
    QString presetLabelsListStr;
    QStringList presetLabelsListQ;
    QVector<QString> presetLabelsList;
    QList<QFileInfo> *objFileInfo;
//
    uint32_t objFileCount = 0;
//        QPixmap pix;
//        About about;
//    int imageAngle;
//    int index;
    uint32_t loadedModelCount = 0;
    uint32_t curLoadingModelIndex = 0;



 std::vector<VkImage> mMappableImages;
 std::vector<VkDeviceMemory> mMappableMemories;

 vk_demo::DVKModel*	m_FirstModel = nullptr;

};

std::shared_ptr<AppModuleBase> CreateAppMode(const std::vector<std::string>& cmdLine)
{
	return std::make_shared<TextureModule>(1600, 900, "Texture", cmdLine);
}
