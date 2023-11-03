#include "kepch.h"

#include "Renderer2D.h"
#include "RenderState.h"

namespace kepler {

static constexpr uint32_t MAX_INSTANCE_COUNT{ 1000 };
static int drawCallsCount = 0;
static int trianglesCount = 0;
static int vertexCount = 0;

// Batch Rendering시 데이터 단위가 되는 Batch Data
struct BatchData
{
	std::shared_ptr<ITexture2D> pTexture;
	std::string vertexShader;
	std::string pixelShader;
	std::shared_ptr<IVertexArray> pVertexArray;
	std::vector<Mat44f> worldMatrices;
	char* pInstBuffer;
	size_t instanceCount;
	size_t sizePerInstance;
	bool bUVFlipX;
	bool bUVFlipY;
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

Renderer2D* Renderer2D::s_pInstance = nullptr;

Renderer2D::Renderer2D()
	: m_pGraphicsAPI{ IGraphicsAPI::Create() }
	, m_renderLog{}
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
		ShaderCache::Load(eShaderType::Vertex, "../Resources/Shaders/HLSL/VS2DSolidInst.hlsl");
		ShaderCache::Load(eShaderType::Pixel, "../Resources/Shaders/HLSL/PS2DSolidInst.hlsl");

		// Texture Shader - 텍스처가 주어진 사각형을 그릴 때 사용
		ShaderCache::Load(eShaderType::Vertex, "../Resources/Shaders/HLSL/VS2DTextureInst.hlsl");
		ShaderCache::Load(eShaderType::Pixel, "../Resources/Shaders/HLSL/PS2DTextureInst.hlsl");

		// Non-Batch Solid Shader
		ShaderCache::Load(eShaderType::Vertex, "../Resources/Shaders/HLSL/VSSolid.hlsl");
		ShaderCache::Load(eShaderType::Pixel, "../Resources/Shaders/HLSL/PSSolid.hlsl");

		// Non-Batch Texture Shader
		ShaderCache::Load(eShaderType::Vertex, "../Resources/Shaders/HLSL/VSTexture.hlsl");
		ShaderCache::Load(eShaderType::Pixel, "../Resources/Shaders/HLSL/PSTexture.hlsl");
	}
}

void Renderer2D::BeginScene(OrthographicCamera& camera)
{
	s_data.sceneData.viewProjection = camera.GetViewProjectionMatrix();
	s_data.sceneData.aspect = camera.GetAspect();
	drawCallsCount = 0;
	trianglesCount = 0;
	vertexCount = 0;

}

void Renderer2D::EndScene()
{
	// 이번 프레임에 그리기 예약된 Batch Data들을 모두 출력합니다.
	Flush();
}

// TODO: Batch Rendering 구현해야 함
void Renderer2D::Flush()
{
	// RenderProfiler
	//int drawCallsCount = 0;
	//int trianglesCount = 0;
	//int vertexCount = 0;

	for (const auto& batchData : s_data.batchObjects)
	{
		ShaderCache::GetShader(batchData.vertexShader)->Bind();
		ShaderCache::GetShader(batchData.pixelShader)->Bind();
		auto shaderDesc = IRenderState::Get()->GetShaderState();
		shaderDesc.pVertexShader->SetMatrix("g_ViewProjection", s_data.sceneData.viewProjection.Transpose());
		shaderDesc.pVertexShader->SetArray("g_Worlds", &batchData.worldMatrices[0], sizeof(Mat44f) * batchData.worldMatrices.size());

		// TODO: 데이터를 어떻게 넘겨줘야 할까?
		std::shared_ptr<IInstanceBuffer> pIB = IInstanceBuffer::Create(
			batchData.pInstBuffer,
			static_cast<UINT>(batchData.sizePerInstance * batchData.instanceCount),
			static_cast<UINT>(batchData.instanceCount),
			eBufferUsage::Default
		);

		if (batchData.pTexture)
		{
			batchData.pTexture->Bind(0);
			pIB->SetLayout({
				{"INST_COLOR", 0, eShaderDataType::Float4, 0, sizeof(float) * 4},
				// sizeof(bool) == 4 in HLSL
				{"INST_BOOL", 0, eShaderDataType::Bool, sizeof(float) * 4, sizeof(int32_t)},
				{"INST_BOOL", 1, eShaderDataType::Bool, sizeof(float) * 4 + sizeof(int32_t), sizeof(int32_t)}
				});
		}
		else
		{
			pIB->SetLayout({
				{"INST_COLOR", 0, eShaderDataType::Float4, 0, sizeof(float) * 4}
				});
		}
		drawCallsCount++;
		trianglesCount += static_cast<int>((batchData.pVertexArray->GetIndexBuffer()->GetCount() / 3) * batchData.instanceCount);
		vertexCount += static_cast<int>(4 * batchData.instanceCount);

		m_pGraphicsAPI->DrawIndexedInstanced(batchData.pVertexArray, pIB);
		delete[] batchData.pInstBuffer;
	}
	m_renderLog.trianglesCount.Add(static_cast<float>(trianglesCount));
	m_renderLog.vertexCount.Add(static_cast<float>(vertexCount));
	m_renderLog.drawCallsCount.Add(static_cast<float>(drawCallsCount));

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
		data.vertexShader = "VS2DSolidInst";
		data.pixelShader = "PS2DSolidInst";
		data.pTexture = nullptr;

		s_data.batchObjects.push_back(data);
		index = (int)s_data.batchObjects.size() - 1;
	}

	// 정점, 인덱스 데이터 추가
	struct QuadVertex
	{
		Vec3f pos;
		Vec4f color;
	};

	float positions[]{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	uint32_t indices[]{
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<IVertexBuffer> pVB = IVertexBuffer::Create(positions, sizeof(positions), eBufferUsage::Default);
	pVB->SetLayout({
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 }
		});
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
	pVA->AddVertexBuffer(pVB);
	pVA->SetIndexBuffer(pIB);

	s_data.batchObjects[index].worldMatrices.push_back(transform);
	s_data.batchObjects[index].pVertexArray = pVA;

	// REFACTOR: 개선 방안 찾아보기
	if (!s_data.batchObjects[index].pInstBuffer)
	{
		s_data.batchObjects[index].sizePerInstance = sizeof(color);
		s_data.batchObjects[index].pInstBuffer = new char[sizeof(color) * MAX_INSTANCE_COUNT];
		s_data.batchObjects[index].instanceCount = 0;
	}

	char* pInstBuffer = s_data.batchObjects[index].pInstBuffer;
	size_t instanceIndex = s_data.batchObjects[index].instanceCount;
	size_t instanceSize = s_data.batchObjects[index].sizePerInstance;

	::memcpy(pInstBuffer + (instanceIndex * instanceSize), &color, instanceSize);
	s_data.batchObjects[index].instanceCount++;
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
		data.vertexShader = "VS2DTextureInst";
		data.pixelShader = "PS2DTextureInst";
		data.pTexture = texture;

		s_data.batchObjects.push_back(data);
		index = (int)s_data.batchObjects.size() - 1;
	}

	float positions[12]{
		0.5f,	0.5f,	0.0f,
		0.5f,	-0.5f,	0.0f,
		-0.5f,	-0.5f,	0.0f,
		-0.5f,	0.5f,	0.0f
	};

	float UVs[8]{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	// 인스턴스 데이터를 담는 구조체
	struct InstData
	{
		Vec4f color;
		int32_t bUVFlipX;
		int32_t bUVFlipY;
	};

	// Texture Flip
	s_data.batchObjects[index].bUVFlipX = bFlipX;
	s_data.batchObjects[index].bUVFlipY = bFlipY;

	uint32_t indices[6]{
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<IVertexBuffer> pVB = IVertexBuffer::Create(positions, sizeof(positions), eBufferUsage::Default);
	pVB->SetLayout({
		{ "POSITION", 0, eShaderDataType::Float3, 0, sizeof(float) * 3 },
		{ "TEXCOORD", 0, eShaderDataType::Float2, sizeof(float) * 3, sizeof(float) * 2}
		});
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
	pVA->AddVertexBuffer(pVB);
	pVA->SetIndexBuffer(pIB);

	s_data.batchObjects[index].pVertexArray = pVA;
	s_data.batchObjects[index].worldMatrices.push_back(transform);
	size_t instanceSize = sizeof(InstData);

	// REFACTOR: 개선 방안 찾아보기
	if (!s_data.batchObjects[index].pInstBuffer)
	{
		s_data.batchObjects[index].pInstBuffer = new char[instanceSize * MAX_INSTANCE_COUNT];
		s_data.batchObjects[index].sizePerInstance = instanceSize;
		s_data.batchObjects[index].instanceCount = 0;
	}
	char* pInstBuffer = s_data.batchObjects[index].pInstBuffer;
	size_t instanceCount = s_data.batchObjects[index].instanceCount;
	InstData data{ color, bFlipX, bFlipY };

	// 인스턴스 데이터를 인스턴스 버퍼의 해당 인스턴스 인덱스에 카피
	::memcpy(pInstBuffer + (instanceSize * instanceCount), &data, sizeof(data));
	s_data.batchObjects[index].instanceCount++;
}

void Renderer2D::DrawNonBatchedQuad(const Vec2f& position, const float rotation, const Vec2f& size, const Vec4f& color)
{
	Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });

	ShaderCache::GetShader("VSSolid")->Bind();
	ShaderCache::GetShader("PSSolid")->Bind();

	// TODO: empty shader!!
	auto shaderDesc = IRenderState::Get()->GetShaderState();
	shaderDesc.pVertexShader->SetMatrix("g_World", transform);
	shaderDesc.pVertexShader->SetMatrix("g_ViewProjection", s_data.sceneData.viewProjection.Transpose());

	float positions[]{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	Vec4f colors[]{ color, color, color, color };

	uint32_t indices[]{
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<IVertexBuffer> pPosBuffer = IVertexBuffer::Create(positions, sizeof(positions), eBufferUsage::Default);
	pPosBuffer->SetLayout({
		{ "POSITION", 0, eShaderDataType::Float3, 0, sizeof(float) * 3 }
		});
	std::shared_ptr<IVertexBuffer> pColorBuffer = IVertexBuffer::Create(colors, sizeof(colors), eBufferUsage::Default);
	pColorBuffer->SetLayout({
		{"COLOR", 0, eShaderDataType::Float4, 0, sizeof(Vec4f)}
		});
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
	pVA->AddVertexBuffer(pPosBuffer);
	pVA->AddVertexBuffer(pColorBuffer);
	pVA->SetIndexBuffer(pIB);

	m_pGraphicsAPI->DrawIndexed(pVA);
	drawCallsCount++;
	trianglesCount += 2;
	vertexCount += 4;
}

void Renderer2D::DrawNonBatchedQuad(const Vec2f& position, const float rotation, const Vec2f& size, const std::shared_ptr<ITexture2D>& texture, bool bFilpX, bool bFlipY, const Vec4f& color)
{
	Mat44f transform = math::GetWorldMatrix({ position.x, position.y, 0.0f }, Quaternion::FromEuler({ 0.0f,  0.0f, rotation }), { size.x, size.y, 1.0f });

	ShaderCache::GetShader("VSTexture")->Bind();
	ShaderCache::GetShader("PSTexture")->Bind();

	auto shaderDesc = IRenderState::Get()->GetShaderState();
	shaderDesc.pVertexShader->SetMatrix("g_World", transform);
	shaderDesc.pVertexShader->SetMatrix("g_ViewProjection", s_data.sceneData.viewProjection.Transpose());

	texture->Bind();

	float positions[]{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float uvs[]{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	Vec4f colors[]{ color, color, color, color };

	uint32_t indices[]{
		0, 1, 2,
		0, 2, 3
	};

	std::shared_ptr<IVertexBuffer> pPosBuffer = IVertexBuffer::Create(positions, sizeof(positions), eBufferUsage::Default);
	pPosBuffer->SetLayout({
		{ "POSITION", 0, eShaderDataType::Float3, 0, sizeof(float) * 3 }
		});
	std::shared_ptr<IVertexBuffer> pColorBuffer = IVertexBuffer::Create(colors, sizeof(colors), eBufferUsage::Default);

	std::shared_ptr<IVertexBuffer> pUVBuffer = IVertexBuffer::Create(uvs, sizeof(uvs), eBufferUsage::Default);
	pUVBuffer->SetLayout({
		{ "TEXCOORD", 0, eShaderDataType::Float2, 0, sizeof(float) * 2}

		});
	pColorBuffer->SetLayout({
		{"COLOR", 0, eShaderDataType::Float4, 0, sizeof(Vec4f)}
		});
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	std::shared_ptr<IVertexArray> pVA = IVertexArray::Create();
	pVA->AddVertexBuffer(pPosBuffer);
	pVA->AddVertexBuffer(pUVBuffer);
	pVA->AddVertexBuffer(pColorBuffer);
	pVA->SetIndexBuffer(pIB);

	m_pGraphicsAPI->DrawIndexed(pVA);
	drawCallsCount++;
	trianglesCount += 2;
	vertexCount += 4;
}
}