#include "kepch.h"

#include "Renderer2D.h"

namespace kepler {

	Renderer2D* Renderer2D::s_pInstance = nullptr;
	// Batch Rendering시 데이터 단위가 되는 Batch Data
	struct BatchData
	{
		std::shared_ptr<ITexture2D> pTexture;
		std::string vertexShader;
		std::string pixelShader;
		std::vector<std::pair<std::shared_ptr<IVertexArray>, Mat44f>> vt;
	};

	// 화면 렌더링에 필요한 정보들
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
			// Renderer2D는 엔진 내장 쉐이더를 이용해 간단히 그려줌.
			// Solid Shader - 텍스처 없이 간단한 사각형을 그릴 때 사용
			ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSSolid.hlsl");
			ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSSolid.hlsl");
			// Texture Shader - 텍스처가 주어진 사각형을 그릴 때 사용
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
		// 이번 프레임에 그리기 예약된 Batch Data들을 모두 출력합니다.
		Flush();
	}

	// TODO: Batch Rendering 구현해야 함
	void Renderer2D::Flush()
	{
		for (const auto& batchData : s_data.batchObjects)
		{
			ShaderCache::GetShader(batchData.vertexShader)->Bind();
			ShaderCache::GetShader(batchData.pixelShader)->Bind();
			if (batchData.pTexture)
			{
				batchData.pTexture->Bind(0);
			}
			ShaderCache::GetLastCachedShader(eShaderType::Vertex)->SetMatrix("g_ViewProjection", s_data.sceneData.viewProjection.Transpose());

			for (const auto& vt : batchData.vt)
			{
				ShaderCache::GetLastCachedShader(eShaderType::Vertex)->SetMatrix("g_World", vt.second.Transpose());
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
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, bFlipX, bFlipY, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::Identity, { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, bFlipX, bFlipY, color);
	}

	void Renderer2D::DrawQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, bFlipX, bFlipY, color);
	}

	void Renderer2D::DrawQuad(const Vec3f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color)
	{
		Mat44f transform = math::GetWorldMatrix(position, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, bFlipX, bFlipY, color);
	}

	// 텍스처가 지정되지 않은 Solid Quad
	void Renderer2D::DrawQuad(const Mat44f& transform, const Vec4f& color)
	{
		// Solid Object가 배치 데이터 안에 추가된 적 있는지 파악
		int index = -1;
		for (int i = 0; i < s_data.batchObjects.size(); i++)
		{
			if (!s_data.batchObjects[i].pTexture)
			{
				index = i;
				break;
			}
		}

		// 추가되지 않았다면 Shader, Texture(nullptr)을 지정해 배치 데이터 추가
		if (index < 0)
		{
			BatchData data{};
			data.vertexShader = "VSSolid";
			data.pixelShader = "PSSolid";
			data.pTexture = nullptr;

			s_data.batchObjects.push_back(data);
			index = s_data.batchObjects.size() - 1;
		}

		// 정점, 인덱스 데이터 추가
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
	
	// 텍스처가 지정된 Texture Quad
	void Renderer2D::DrawQuad(const Mat44f& transform, const std::shared_ptr<ITexture2D>& texture, bool bFlipX, bool bFlipY, const Vec4f& color)
	{
		int index = -1;
		// 전달받은 텍스처가 이미 등록된 텍스처인지 확인
		for (int i = 0; i < s_data.batchObjects.size(); i++)
		{
			if (s_data.batchObjects[i].pTexture == texture)
			{
				index = i;
				break;
			}
		}

		// 새로운 텍스처일 경우 배치 데이터셋을 새로 추가
		if (index < 0)
		{
			BatchData data{};
			data.vertexShader = "VSTexture";
			data.pixelShader = "PSTexture";
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

		// Texture Flip
		if (bFlipX)
		{
			for (int i = 0; i < 4; i++)
			{
				vertices[i].uv.x *= -1;
			}
		}
		if (bFlipY)
		{
			for (int i = 0; i < 4; i++)	
			{
				vertices[i].uv.y *= -1;
			}
		}

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