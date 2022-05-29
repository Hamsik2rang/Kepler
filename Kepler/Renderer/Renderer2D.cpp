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

	struct BatchData
	{
		std::shared_ptr<ITexture2D> pTexture;
		std::shared_ptr<IShader> pVertexShader;
		std::shared_ptr<IShader> pPixelShader;
		std::vector<std::pair<std::shared_ptr<IVertexArray>, Mat44f&>> vt;
	};

	struct RenderData
	{
		struct SceneData
		{
			Mat44f viewProjection;
			float aspect;
		} sceneData{};

		std::list<BatchData> batchList;
	};

	static Quad s_quad;
	static RenderData s_data;

	Renderer2D::~Renderer2D()
	{
		if (s_pInstance)
		{
			delete s_pInstance;
			s_pInstance = nullptr;
		}
	}

	Renderer2D* Renderer2D::Get()
	{
		if (!s_pInstance)
		{
			Init();
		}

		return s_pInstance;
	}

	void Renderer2D::Init()
	{
		if (!s_pInstance)
		{
			s_pInstance = new Renderer2D;

			ShaderCache::Load(eShaderType::Vertex, "Resources/Shaders/HLSL/VSSolid.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "Resources/Shaders/HLSL/PSSolid.hlsl");

			ShaderCache::Load(eShaderType::Vertex, "Resources/Shaders/HLSL/VSTexture.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "Resources/Shaders/HLSL/PSTexture.hlsl");

			ShaderCache::Load(eShaderType::Vertex, "Resources/Shaders/HLSL/VSSolidTexture.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "Resources/Shaders/HLSL/PSSolidTexture.hlsl");
		}
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_data.sceneData.viewProjection = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
		// Batch Data들을 모두 출력합니다.
		Flush();
	}

	void Renderer2D::Flush()
	{
		//...

		s_data.batchList.clear();
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color)
	{
		// Solid Shader
		
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const Vec4f& color)
	{
		// Solid Shader
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color)
	{
		// Solid Shader
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const Vec4f& color)
	{
		// Solid Shader
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot)
	{
		// Texture Shader


	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot)
	{
		// Texture Shader
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot)
	{
		// Texture Shader


	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot)
	{
		// Texture Shader


	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot, const Vec4f& color)
	{
		// Texture Color Shader
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot, const Vec4f& color)
	{
		// Texture Color Shader


	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot, const Vec4f& color)
	{
		// Texture Color Shader


	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot, const Vec4f& color)
	{
		// Texture Color Shader


	}


	void Renderer2D::DrawQuad(const Mat44f& transform, const Vec4f& color)
	{
		// Solid Shader
		BatchData data{};
		data.pVertexShader = ShaderCache::GetShader("VSSolid");
		data.pPixelShader = ShaderCache::GetShader("PSSolid");

		struct QuadVertex
		{
			Vec3f pos;
			Vec4f color;
		};

		QuadVertex vertices[4]{
			{ { 0.5f,  0.5f, 0.0f }, color },
			{ { 0.5f,  -0.5f, 0.0f }, color },
			{ { -0.5f,  -0.5f, 0.0f }, color },
			{ { -0.5f,  0.5f, 0.0f }, color }
		};

		uint32_t indices[]{
			0, 1, 2,
			0, 2, 3
		};

		std::shared_ptr<IVertexBuffer> pVB = IVertexBuffer::Create(vertices, sizeof(vertices), eBufferUsage::Default);
		pVB->SetLayout({
			{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 },
			{ "COLOR", 0, kepler::eShaderDataType::Float4, 12, sizeof(float) * 4 }
			});
		std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
		std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
		pVA->AddVertexBuffer(pVB);
		pVA->SetIndexBuffer(pIB);

		//...
	}

	void Renderer2D::DrawQuad(const Mat44f& transform, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot)
	{
		// Texture Shader
		BatchData data{};
		data.pVertexShader = ShaderCache::GetShader("VSTexture");
		data.pPixelShader = ShaderCache::GetShader("PSTexture");

	}

	void Renderer2D::DrawQuad(const Mat44f& transform, const std::shared_ptr<ITexture2D>& texture, const uint32_t slot, const Vec4f& color)
	{
		// Texture Color Shader
		BatchData data{};
		data.pVertexShader = ShaderCache::GetShader("VSSolidTexture");
		data.pPixelShader = ShaderCache::GetShader("PSSolidTexture");



	}
}