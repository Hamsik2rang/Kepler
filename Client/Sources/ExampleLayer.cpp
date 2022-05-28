#include "ExampleLayer.h"


using namespace kepler;

void ExampleLayer::OnAttach()
{
	// 쉐이더 프로그램 로딩하기 -----------------------------
	// 가장 먼저 쉐이더 프로그램을 로드합니다. 
	// Kepler Engine의 모든 쉐이더는 ShaderCache 객체가 관리합니다.
	// ShaderCache객체는 쉐이더의 로드와 가장 마지막으로 GetShader()를 통해 참조된 쉐이더를 캐싱합니다.
	// 
	// 예제에서는 kepler engine이 기본적으로 제공해 주는 쉐이더를 사용하므로, kepler 프로젝트 안의 resource 경로를 지정합니다.
	// NOTE: 모든 쉐이더는 쉐이더 캐시를 통해 로드합니다. 단독적으로 생성하지 않습니다.
	// 만약 쉐이더 프로그램에 대한 직접적인 참조가 필요한 경우 다음과 같이 참조합니다.
	// 
	// 1. 가장 먼저 쉐이더를 로드합니다. 이미 로드한 적 있는 쉐이더라면 다시 불러올 필요가 없습니다.
	// 쉐이더 로드는 ShaderCache::Load()를 이용합니다.
	// ex.
	// ShaderCache::Load(type, path);
	// or
	// ShaderCache::Load(type, name, path);
	//
	// * 쉐이더 로드 시 쉐이더 이름을 지정하지 않으면 불러온 쉐이더의 이름은 자동으로 파일 이름이 됩니다.
	// ex.	VSTest.hlsl				-> VSTest
	//		PSVolumetricShadow.hlsh -> PSVolumetricShadow
	// 
	// 2. ShaderCache::GetShader()를 통해 해당 쉐이더에 대한 shared_ptr를 받아옵니다.
	// std::shared_ptr<IShader> pShader = ShaderCache::GetShader(name);
	//

	ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTest.hlsl");
	ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTest.hlsl");

	ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTexture.hlsl");
	ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTexture.hlsl");

	//---------------------------------------------------

	// 정점 배열 구성하기 ----------------------------------
	// 그려낼 정점 배열을 구성합니다. 
	// 간단한 예제이므로 별도의 Wrapping 없이 직접 정점 데이터를 구성합니다.
	// 렌더링될 정점 데이터는 정점 배열로 표현되며, 정점 배열은 여러 개의 정점 버퍼(DirectX11 기준 최대 16개)와 하나의 Index Buffer로 구성됩니다.
	// 각 정점 버퍼는 버퍼의 데이터에 대한 레이아웃을 정의해야 합니다.
	// 
	// 1. Vertex Array를 생성합니다. Vertex Array에는 여러 개의(최대 16개) Vertex Buffer와 하나의 Index Buffer가 들어갑니다.
	m_pVertexArray[0] = IVertexArray::Create();
	m_pVertexArray[1] = IVertexArray::Create();

	// 2-1.별도의 정점 데이터를 구조체 형식으로 정의할 수 있습니다.
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
	// 2-2. Vertex Array에 들어갈 Vertex Buffer를 생성합니다.
	std::shared_ptr<IVertexBuffer> pTextureColorVB = IVertexBuffer::Create(reinterpret_cast<float*>(vertices), sizeof(vertices), eBufferUsage::Default);
	// 2-3. 해당 Vertex Buffer의 레이아웃(Shader 안의 Vertex Input Layout와의 매칭 레이아웃)을 지정해 줍니다.
	pTextureColorVB->SetLayout({
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 },
		{ "TEXCOORD", 0, kepler::eShaderDataType::Float2, 12, sizeof(float) * 2 },
		{ "COLOR", 0, kepler::eShaderDataType::Float4, 20, sizeof(float) * 4 }
		});
	// 2-4. Vertex Buffer를 Vertex Array에 추가합니다.
	m_pVertexArray[0]->AddVertexBuffer(pTextureColorVB);

	// 3-1. 각 데이터를 구분해 여러 개의 Vertex Buffer로 분할할 수도 있습니다.
	// 정점 좌표 데이터
	float textureVertices[] = {
		  0.25f,  0.25f, 0.0f,  // top right
		 0.25f, -0.25f, 0.0f,  // bottom right
		-0.25f, -0.25f, 0.0f,  // bottom left
		-0.25f,  0.25f, 0.0f   // top left
	};
	// 텍스처 좌표 데이터
	float textureUV[]{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, 
		0.0f, 0.0f
	};
	
	// 3-2. 정점 좌표를 위한 Vertex Buffer를 생성합니다
	std::shared_ptr<IVertexBuffer> pTextureVertex = IVertexBuffer::Create(textureVertices, sizeof(vertices), eBufferUsage::Immutable);
	// 3-3. 정점 레이아웃을 설정합니다.
	pTextureVertex->SetLayout({
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 }
		});
	// 3-4. Vertex Buffer를 Vertex Array에 추가합니다.
	m_pVertexArray[1]->AddVertexBuffer(pTextureVertex);

	// 텍스처 좌표 Vertex Buffer도 마찬가지로 생성한 후, 레이아웃을 설정합니다. 이후 Vertex Array에 추가합니다.
	std::shared_ptr<IVertexBuffer> pTextureUV = IVertexBuffer::Create(textureUV, sizeof(textureUV), eBufferUsage::Immutable);
	pTextureUV->SetLayout({ 
		{ "TEXCOORD", 0, kepler::eShaderDataType::Float2, 0, sizeof(float) * 2 }
		});
	m_pVertexArray[1]->AddVertexBuffer(pTextureUV);

	// Index Buffer 데이터. 
	uint32_t indices[6]{
		0, 1, 2,
		0, 2, 3
	};
	// Index Buffer를 생성합니다. 
	// NOTE: Index Buffer에는 Vertex Buffer와 달리 버퍼 크기(size)가 아닌 인덱스 개수(count)가 들어갑니다.
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	// 1~3에서 구성한 Vertex Array에 Index Buffer를 지정(set)합니다.
	m_pVertexArray[0]->SetIndexBuffer(pIB);
	m_pVertexArray[1]->SetIndexBuffer(pIB);
	//---------------------------------------------------

	// 쉐이더에 사용할 텍스처를 생성한 후 렌더링 파이프라인에 바인딩합니다. 
	// 텍스처를 바인딩 할 때에는 쉐이더 프로그램의 어떤 텍스처와 매핑시킬지 슬롯(register number)을 지정합니다
	m_pTexture = ITexture2D::Create(eTextureDataType::Float, "Assets/Textures/2k_earth_daymap.jpg");
	m_pTexture->Bind(0);

	m_timer.Start();
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate()
{
	float curTime = m_timer.Elapsed();

	ShaderCache::GetShader("VSTest")->Bind();
	ShaderCache::GetShader("PSTest")->Bind();
	
	ShaderCache::GetShader("PSTest")->SetFloat("g_Time", curTime);
	m_pVertexArray[0]->Bind();
	
	Renderer::Get()->Submit(m_pVertexArray[0]);

	ShaderCache::GetShader("VSTexture")->Bind();
	ShaderCache::GetShader("PSTexture")->Bind();
	m_pVertexArray[1]->Bind();
	
	Renderer::Get()->Submit(m_pVertexArray[1]);
}

void ExampleLayer::OnEvent(Event& e)
{
	KEPLER_TRACE("{0}", e);
}