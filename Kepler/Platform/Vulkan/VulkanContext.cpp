#include "kepch.h"

#include "VulkanContext.h"
#include "VulkanDebug.h"

namespace kepler {

#ifdef _DEBUG
static bool enableValidationLayer = true;
#else
static bool enableValidationLayer = false;
#endif

VulkanContext::VulkanContext(const HWND hWnd)
	: m_pDebugger{ nullptr }
	, m_pLogicalDevice{ nullptr }
	, m_pInstance{ nullptr }
	, m_pPhysicalDevice{ nullptr }
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
	if (m_pLogicalDevice)
	{
		vkDestroyDevice(m_pLogicalDevice, nullptr);
		m_pLogicalDevice = nullptr;
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
		KEPLER_CORE_CRITICAL("Cannot create vulkan instance");
		return false;
	}

	if (enableValidationLayer)
	{
		m_pDebugger = new VulkanDebug();
		m_pDebugger->SetupDebugMessenger(m_pInstance);
	}

	uint32_t gpuCount = 0;
	vkEnumeratePhysicalDevices(m_pInstance, &gpuCount, nullptr);
	if (gpuCount == 0)
	{
		KEPLER_CORE_CRITICAL("No device with Vulkan support found");
		return false;
	}

	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	if (vkEnumeratePhysicalDevices(m_pInstance, &gpuCount, physicalDevices.data()) != VK_SUCCESS)
	{
		KEPLER_CORE_CRITICAL("Could not enumerate physical devices");
		return false;
	}

	uint32_t selectedDevice = 0;

	//...


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
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = title.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Kepler Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	std::vector<const char*> instanceExtensions;
	if (enableValidationLayer)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		instanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	};
#if defined(_WIN32)
	instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif


	uint32_t extCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	if (extCount > 0)
	{
		std::vector<VkExtensionProperties> extensions(extCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extensions.data()) == VK_SUCCESS)
		{
			for (const VkExtensionProperties& extension : extensions)
			{
				m_supportedInstanceExtensions.push_back(extension.extensionName);
			}
		}
	}

	if (m_enabledInstanceExtensions.size() > 0)
	{
		for (const char* enabledExtension : m_enabledInstanceExtensions)
		{
			if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), enabledExtension) == m_supportedInstanceExtensions.end())
			{
				KEPLER_CORE_WARNING("Enabled instance extension \" {0} \" is not present at instance level\n", enabledExtension);
			}
			instanceExtensions.push_back(enabledExtension);
		}
	}

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.pApplicationInfo = &appInfo;

	if (instanceExtensions.size() > 0)
	{
		if (enableValidationLayer)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
		createInfo.ppEnabledExtensionNames = instanceExtensions.data();
	}

	const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
	if (enableValidationLayer)
	{
		uint32_t instanceLayerCount = 0;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());

		bool validationLayerPresent = false;
		for (const VkLayerProperties& layer : instanceLayerProperties)
		{
			if (::strcmp(layer.layerName, validationLayerName) == 0)
			{
				validationLayerPresent = true;
				break;
			}
		}

		if (validationLayerPresent)
		{
			createInfo.ppEnabledLayerNames = &validationLayerName;
			createInfo.enabledLayerCount = 1;
		}
		else
		{
			KEPLER_CORE_WARNING("Validation Layer VK_LAYER_KHRONOS_validation not present, validation is disabled.");
		}
	}


	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_pInstance);
	if (result != VK_SUCCESS)
	{
		return false;
	}

	return true;
}


}