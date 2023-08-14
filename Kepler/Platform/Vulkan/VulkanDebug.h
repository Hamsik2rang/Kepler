#pragma once

#include <vulkan/vulkan.h>

namespace kepler {

class VulkanDebug
{
public:
	VulkanDebug();
	~VulkanDebug();

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	bool SetupDebugMessenger(VkInstance instance);
	void DestroyDebugMessenger(VkInstance instance);
private:
	VkResult createDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pCallback);
	
	void destroyDebugUtilsMessengerEXT(
		VkInstance instance, 
		VkDebugUtilsMessengerEXT callback, 
		const VkAllocationCallbacks* pAllocator);


	VkDebugUtilsMessengerEXT m_debugMessenger;
};
}