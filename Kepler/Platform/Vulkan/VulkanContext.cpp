#include "kepch.h"

#include "VulkanContext.h"
#include "VulkanDebug.h"

namespace kepler {

VulkanContext::VulkanContext(const HWND hWnd)
{

}

VulkanContext::~VulkanContext()
{
	VulkanContext::Cleanup();
#ifdef _DEBUG
	m_pDebugger->DestroyDebugMessenger(m_pInstance);
	delete m_pDebugger;
	m_pDebugger = nullptr;
#endif
	if (m_pDevice)
	{
		vkDestroyDevice(m_pDevice, nullptr);
		m_pDevice = nullptr;
	}
	if (m_pInstance)
	{
		vkDestroyInstance(m_pInstance, nullptr);
		m_pInstance = nullptr;
	}
}

bool VulkanContext::Init(const WindowData& data)
{
	if (createInstance(data.title))
	{
		return false;
	}


	return true;
}

void VulkanContext::Cleanup()
{

}

void VulkanContext::SwapBuffer()
{

}

bool VulkanContext::createInstance(const std::string& title)
{
#ifdef _DEBUG
	bool enableValidationLayer = true;
#else
	bool enableValidationLayer = false;
#endif

	if (enableValidationLayer && !checkValidationLayerSupport())
	{
		return false;
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = title.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Kepler Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
#ifdef _DEBUG
	std::vector<const char*> extensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	createInfo.enabledExtensionCount = 1;
	createInfo.ppEnabledExtensionNames = extensions.data();
#endif

	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_pInstance);
	if (result != VK_SUCCESS)
	{
		return false;
	}
#ifdef _DEBUG
	m_pDebugger = new VulkanDebug();
	m_pDebugger->SetupDebugMessenger(m_pInstance);
#endif
	return true;
}

bool VulkanContext::checkValidationLayerSupport()
{
	static const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	uint32_t instanceLayerCount = 0;
	std::vector<VkLayerProperties> layerProperties;
	VkResult result = VK_INCOMPLETE;

	do
	{
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		if (result != VK_SUCCESS || !instanceLayerCount)
		{
			return false;
		}
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	for (const char* layerName : validationLayers)
	{
		bool bHasLayerFound = false;

		for (const auto& layerProperty : layerProperties)
		{
			if (strcmp(layerName, layerProperty.layerName) == 0)
			{
				bHasLayerFound = true;
				break;
			}
		}

		if (!bHasLayerFound)
		{
			return false;
		}
	}

	return true;
}

}