#include "kepch.h"

#include "Renderer2D.h"

namespace kepler {

	Renderer2D* Renderer2D::s_pInstance = nullptr;

	struct BatchData
	{
		std::shared_ptr<ITexture2D> pTexture;
		std::shared_ptr<IShader> pVertexShader;
		std::shared_ptr<IShader> pPixelShader;
		std::vector<std::pair<std::shared_ptr<IVertexArray>, Mat44f>> vt;
	};

	struct RenderData
	{
		struct SceneData
		{
			Mat44f viewProjection;
			float aspect;
		} sceneData{};

		std::vector<BatchData> batchObjects;
	};

	static RenderData s_data{};

	Renderer2D::Renderer2D()
		:m_pGraphicsAPI{ IGraphicsAPI::Create() }
	{

	}

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

			ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSSolid.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSSolid.hlsl");

			ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTexture.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTexture.hlsl");
		}
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_data.sceneData.viewProjection = camera.GetViewProjectionMatrix();
		s_data.sceneData.aspect = camera.GetAspect();
	}

	void Renderer2D::EndScene()
	{
		// Batch Data���� ��� ����մϴ�.
		Flush();
	}

	void Renderer2D::Flush()
	{
		//...
		for (const auto& batchData : s_data.batchObjects)
		{
			batchData.pVertexShader->Bind();
			batchData.pPixelShader->Bind();
			if (batchData.pTexture)
			{
				batchData.pTexture->Bind(0);
			}
			auto viewProjTranspose = s_data.sceneData.viewProjection.Transpose();
			batchData.pVertexShader->SetMatrix("g_ViewProjection", viewProjTranspose);
			//batchData.pVertexShader->SetMatrix("g_ViewProjection", Mat44f::Identity);

			for (const auto& vt : batchData.vt)
			{
				Vec4f pos0{ 0.5f, 0.5f, 0.0f, 1.0f };
				Vec4f pos1{ 0.5f, -0.5f, 0.0f, 1.0f };
				Vec4f pos2{ -0.5f, -0.5f, 0.0f, 1.0f };
				Vec4f pos3{ -0.5f, 0.5f, 0.0f, 1.0f };
				auto result = pos0 * vt.second * s_data.sceneData.viewProjection;
				KEPLER_TRACE("{0}, {1}, {2}, {3}", result.x, result.y, result.z, result.w);
				result = pos1 * vt.second * s_data.sceneData.viewProjection;
				KEPLER_TRACE("{0}, {1}, {2}, {3}", result.x, result.y, result.z, result.w);
				result = pos2 * vt.second * s_data.sceneData.viewProjection;
				KEPLER_TRACE("{0}, {1}, {2}, {3}", result.x, result.y, result.z, result.w);
				result = pos3 * vt.second * s_data.sceneData.viewProjection;
				KEPLER_TRACE("{0}, {1}, {2}, {3}", result.x, result.y, result.z, result.w);
				batchData.pVertexShader->SetMatrix("g_World", vt.second.Transpose());
				vt.first->Bind();
				m_pGraphicsAPI->DrawIndexed(vt.first);
			}
		}

		s_data.batchObjects.clear();
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::Identity, { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::Identity, { size.x , size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::FromEuler({ 0.0f, rotation, 0.0f }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, color);
	}


	// �ؽ�ó�� �������� ���� Solid Quad
	void Renderer2D::DrawQuad(const Mat44f& transform, const Vec4f& color)
	{
		// Solid Object�� ��ġ ������ �ȿ� �߰��� �� �ִ��� �ľ�
		int index = -1;
		for (int i = 0; i < s_data.batchObjects.size(); i++)
		{
			if (!s_data.batchObjects[i].pTexture)
			{
				index = i;
				break;
			}
		}

		// �߰����� �ʾҴٸ� Shader, Texture(nullptr)�� ������ ��ġ ������ �߰�
		if (index < 0)
		{
			BatchData data{};
			data.pVertexShader = ShaderCache::GetShader("VSSolid");
			data.pPixelShader = ShaderCache::GetShader("PSSolid");
			data.pTexture = nullptr;

			s_data.batchObjects.push_back(data);
			index = s_data.batchObjects.size() - 1;
		}

		// ����, �ε��� ������ �߰�
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

		s_data.batchObjects[index].vt.push_back(std::make_pair(pVA, transform));
	}

	void Renderer2D::DrawQuad(const Mat44f& transform, const std::shared_ptr<ITexture2D>& texture, const Vec4f& color)
	{
		int index = -1;
		for (int i = 0; i < s_data.batchObjects.size(); i++)
		{
			if (s_data.batchObjects[i].pTexture == texture)
			{
				index = i;
				break;
			}
		}

		if (index < 0)
		{
			BatchData data{};
			data.pVertexShader = ShaderCache::GetShader("VSTexture");
			data.pPixelShader = ShaderCache::GetShader("PSTexture");
			data.pTexture = texture;

			s_data.batchObjects.push_back(data);
			index = s_data.batchObjects.size() - 1;
		}

		struct QuadVertex
		{
			Vec3f pos;
			Vec2f uv;
			Vec4f color;
		};

		QuadVertex vertices[4]{
			{ { 0.5f,  0.5f, 0.0f }, {1.0f, 0.0f}, color },
			{ { 0.5f,  -0.5f, 0.0f }, {1.0f, 1.0f}, color },
			{ { -0.5f,  -0.5f, 0.0f }, {0.0f, 1.0f}, color },
			{ { -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f}, color }
		};

		uint32_t indices[]{
			0, 1, 2,
			0, 2, 3
		};

		std::shared_ptr<IVertexBuffer> pVB = IVertexBuffer::Create(vertices, sizeof(vertices), eBufferUsage::Default);
		pVB->SetLayout({
			{ "POSITION", 0, eShaderDataType::Float3, 0, sizeof(float) * 3 },
			{ "TEXCOORD", 0, eShaderDataType::Float2, sizeof(float) * 3, sizeof(float) * 2},
			{ "COLOR", 0, eShaderDataType::Float4, sizeof(float) * 5, sizeof(float) * 4}
			});
		std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
		std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
		pVA->AddVertexBuffer(pVB);
		pVA->SetIndexBuffer(pIB);

		s_data.batchObjects[index].vt.push_back(std::make_pair(pVA, transform));
	}
}