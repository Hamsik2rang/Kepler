#pragma once

#include "Core/Base.h"
#include "Core/Renderer/GraphicsAPI.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Camera.hpp"
#include "Core/Window.h"

#include "KeplerMath.h"


namespace kepler {

class Renderer
{
private:
	static Renderer* s_pInstance;
	IGraphicsAPI* m_pGraphicsAPI;

	Renderer();
	~Renderer();

public:
	static Renderer* Get();
	static void Init();

	inline eGraphicsAPI GetAPI() const { return m_pGraphicsAPI->GetAPI(); }

	void SetViewport(const uint32_t width, const uint32_t height, const float minDepth, const float maxDepth);

	void BeginScene(Camera& camera);
	void EndScene();

	// TODO: should be going to define additional function overloadings(T, TR, TRS, shader program, etc.)
	void Submit(std::shared_ptr<IVertexArray>& pVertexArray, const Mat44f& transform = Mat44f::Identity);
};
}