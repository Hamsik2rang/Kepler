#include "kepch.h"

#include "Renderer2D.h"

namespace kepler {

	struct Quad
	{
		struct QuadVertex
		{
			Vec3f position;
			Vec2f uv;
			Vec4f color;
		};

		QuadVertex vertices[4]{
			{{ 0.5f,  0.5f, 0.0f }, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },  // top right
			{{ 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },  // bottom right
			{{ -0.5f, -0.5f, 0.0f }, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },  // bottom left
			{{ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} }   // top left
		};
		uint32_t indices[6]{
			0, 1, 2,
			0, 2, 3 
		};
	};

	struct RenderData
	{
		// Vertex Array container

	};

	struct SceneData
	{
		Mat44f viewProjection;
		float aspect;
	};

	static RenderData s_renderData;
	static SceneData s_sceneData;


	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_sceneData.viewProjection = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
		//TODO: Batch Rendering
	}
}