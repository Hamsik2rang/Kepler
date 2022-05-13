#pragma once
#include <Kepler.h>


class ExampleLayer: public kepler::Layer
{
private:
	std::shared_ptr<IVertexArray> m_pVertexArray;
	std::vector<std::shared_ptr<IShader>> v;
public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnEvent(kepler::Event& e) override;
};