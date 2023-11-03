#pragma once

#include "Renderer/GraphicsContext.h"
#include <Windows.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace kepler {

class VulkanDebug;


class VulkanContext : public IGraphicsContext
{
public:
	VulkanContext(const HWND hWnd);
	~VulkanContext();

	virtual bool Init(const WindowData& data);
	virtual void Cleanup();
	virtual void SwapBuffer();

	inline virtual void* GetDevice() { return m_pLogicalDevice; }
	inline virtual void* GetDeviceContext() { return m_pInstance; }
	inline virtual void* GetSwapChain() { return nullptr; }

private:
	struct QueueFamilyIndices
	{
		int32 graphicsFamily = -1;
		int32 presentFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	HWND m_hWnd;

	VkInstance m_pInstance;
	VkPhysicalDevice m_pPhysicalDevice;
	VkDevice m_pLogicalDevice;
	VkSurfaceKHR m_pSurface;

	VulkanDebug* m_pDebugger;

	std::vector<std::string> m_supportedInstanceExtensions;
	std::vector<const char*> m_enabledDeviceExtensions;
	std::vector<const char*> m_enabledInstanceExtensions;

	bool createInstance(const std::string& title);
	bool createSurface();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice pDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice pDevice);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice pDevice);
};

}