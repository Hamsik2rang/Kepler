#pragma once

#include "Renderer/GraphicsContext.h"
#include <Windows.h>
#include <vulkan/vulkan.h>



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
	VkInstance m_pInstance;
	VkPhysicalDevice m_pPhysicalDevice;
	VkDevice m_pLogicalDevice;

	VulkanDebug* m_pDebugger;

	std::vector<std::string> m_supportedInstanceExtensions;
	std::vector<const char*> m_enabledDeviceExtensions;
	std::vector<const char*> m_enabledInstanceExtensions;
	
	bool createInstance(const std::string& title);
	bool checkValidationLayerSupport();

};

}