#include "kepch.h"

#include "VulkanDebug.h"
#include "Core/Base.h"

namespace kepler {

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebug::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	{
		KEPLER_CORE_INFO("[INFO] {0}", pCallbackData->pMessage);
	}
	else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	{
		KEPLER_CORE_TRACE("[TRACE] {0}", pCallbackData->pMessage);
	}
	else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		KEPLER_CORE_WARNING("[WARNING] {0}", pCallbackData->pMessage);
	}
	else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		KEPLER_CORE_ERROR("[ERROR] {0}", pCallbackData->pMessage);
	}

	return VK_FALSE;
}

bool VulkanDebug::SetupDebugMessenger(VkInstance instance)
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	
	createInfo.pfnUserCallback = VulkanDebug::DebugCallback;
	createInfo.pUserData = nullptr; // Optional

	if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool VulkanDebug::DestroyDebugMessenger(VkInstance instance)
{
	destroyDebugUtilsMessengerEXT(instance, m_debugMessenger, nullptr);
}

VkResult VulkanDebug::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void VulkanDebug::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

}