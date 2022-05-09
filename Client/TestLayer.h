#pragma once

#include "Core/Layer.hpp"
#include "Platform/DirectX11/DX11Model.h"

using namespace kepler;

class TestLayer : public kepler::Layer
{
private:
	DX11Model** m_ppModel = nullptr;
	int modelCount = 0;
public:
	TestLayer();
	virtual ~TestLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnEvent(kepler::Event& e) override;
};