#pragma once

#include "Kepler.h"

class VulkanTestLayer : public kepler::Layer
{
public:
	VulkanTestLayer();
	~VulkanTestLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGUIRender() override;
	virtual void OnEvent(kepler::Event& e) override;

private:
	std::shared_ptr<kepler::IVertexBuffer> m_pVertexBuffer;
	std::shared_ptr<kepler::IIndexBuffer> m_pIndexBuffer;
	std::shared_ptr<kepler::IVertexArray> m_pVertexArray;
	std::shared_ptr<kepler::ITexture2D> m_pTexture0;
	std::shared_ptr<kepler::ISampler> m_pSampler;
	kepler::Timer m_timer;
};

