#include "kepch.h"

#include "VulkanContext.h"
#include "VulkanDebug.h"

namespace kepler {




#ifdef _DEBUG
static bool enableValidationLayers = true;

const std::vector<const char*> validationLayers
{
	"VK_LAYER_KHRONOS_validation",
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(_WIN32)
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
};

#else
static bool enableValidationLayers = false;
#endif



VulkanContext::VulkanContext(const HWND hWnd)
	: m_pDebugger{ nullptr }
	, m_pLogicalDevice{ nullptr }
	, m_pInstance{ nullptr }
	, m_pPhysicalDevice{ nullptr }
	, m_hWnd(hWnd)
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

	if (enableValidationLayers)
	{
		m_pDebugger = new VulkanDebug();
		m_pDebugger->SetupDebugMessenger(m_pInstance);
	}

	if(createSurface())

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
	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		KEPLER_CORE_CRITICAL("vaildation layer requested, but not available.");
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


	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		m_pDebugger->PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &m_pInstance) != VK_SUCCESS)
	{
		KEPLER_CORE_CRITICAL("failed to create instance!");
		return false;
	}

	return true;
}

bool VulkanContext::createSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = m_hWnd;
	createInfo.hinstance = GetModuleHandle(nullptr);
	
	if (vkCreateWin32SurfaceKHR(m_pInstance, &createInfo, nullptr, &m_pSurface) != VK_SUCCESS)
	{
		KEPLER_CORE_CRITICAL("Cannot create Vulkan surface!");
		return false;
	}

	return true;
}

bool VulkanContext::checkValidationLayerSupport()
{
	uint32 layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool bHasLayerFound = false;
		for (const auto& layerProperty : availableLayers)
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

std::vector<const char*> VulkanContext::getRequiredExtensions()
{
	return std::vector<const char*>();
}

void VulkanContext::pickPhysicalDevice()
{
	uint32_t gpuCount = 0;
	vkEnumeratePhysicalDevices(m_pInstance, &gpuCount, nullptr);
	if (gpuCount == 0)
	{
		KEPLER_CORE_CRITICAL("No device with Vulkan support found");
	}

	std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
	if (vkEnumeratePhysicalDevices(m_pInstance, &gpuCount, physicalDevices.data()) != VK_SUCCESS)
	{
		KEPLER_CORE_CRITICAL("Could not enumerate physical devices");
	}

	uint32_t selectedDevice = 0;
	for (const auto& device : physicalDevices)
	{
		if (isDeviceSuitable(device))
		{
			m_pPhysicalDevice = device;
			break;
		}
	}
}

bool VulkanContext::isDeviceSuitable(VkPhysicalDevice pDevice)
{
	QueueFamilyIndices indices = FindQueueFamilies(pDevice);	
	
	VkPhysicalDeviceProperties prop{};
	vkGetPhysicalDeviceProperties(pDevice, &prop);



	bool bExtensionSupport = checkDeviceExtensionSupport(pDevice);
	bool bSwapChainAdequate = false;
	if (bExtensionSupport)
	{
		
	}


	return false;
}

bool VulkanContext::checkDeviceExtensionSupport(VkPhysicalDevice pDevice)
{
	static const std::vector<const char*> deviceExtensions{ 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
		if (requiredExtensions.empty())
		{
			break;
		}
	}

	return requiredExtensions.empty();
}

VulkanContext::QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice pDevice)
{
	QueueFamilyIndices indices{};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, m_pSurface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
		}
		i++;

		if (indices.isComplete())
		{
			break;
		}
	}

	return indices;
}
}