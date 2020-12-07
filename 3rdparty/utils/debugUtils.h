#pragma once

#ifndef _DEBUG_UTILS_
#define _DEBUG_UTILS_

#include <cstdlib>
#include <vector>
#include <iostream>

//#include "window/Window.h"
//#include "input/UserInput.h"
//#include "utility/Timer.h"
//#include "vulkan/Renderer.h"
//#include "input/ArcBallCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const bool g_vulkanDebugCallBackEnabled = true; // requires VK_EXT_debug_utils extension

extern VkDebugUtilsMessengerCreateInfoEXT debugMsgCreateInfo;

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif



    bool checkValidationLayerSupport();
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    bool DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    bool populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool setupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger);



#endif

