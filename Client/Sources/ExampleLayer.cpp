#include "ExampleLayer.h"


using namespace kepler;

void ExampleLayer::OnAttach()
{
	// ���̴� ���α׷� �ε��ϱ� -----------------------------
	// ���� ���� ���̴� ���α׷��� �ε��մϴ�. 
	// Kepler Engine�� ��� ���̴��� ShaderCache ��ü�� �����մϴ�.
	// ShaderCache��ü�� ���̴��� �ε�� ���� ���������� GetShader()�� ���� ������ ���̴��� ĳ���մϴ�.
	// 
	// ���������� kepler engine�� �⺻������ ������ �ִ� ���̴��� ����ϹǷ�, kepler ������Ʈ ���� resource ��θ� �����մϴ�.
	// NOTE: ��� ���̴��� ���̴� ĳ�ø� ���� �ε��մϴ�. �ܵ������� �������� �ʽ��ϴ�.
	// ���� ���̴� ���α׷��� ���� �������� ������ �ʿ��� ��� ������ ���� �����մϴ�.
	// 
	// 1. ���� ���� ���̴��� �ε��մϴ�. �̹� �ε��� �� �ִ� ���̴���� �ٽ� �ҷ��� �ʿ䰡 �����ϴ�.
	// ���̴� �ε�� ShaderCache::Load()�� �̿��մϴ�.
	// ex.
	// ShaderCache::Load(type, path);
	// or
	// ShaderCache::Load(type, name, path);
	//
	// * ���̴� �ε� �� ���̴� �̸��� �������� ������ �ҷ��� ���̴��� �̸��� �ڵ����� ���� �̸��� �˴ϴ�.
	// ex.	VSTest.hlsl				-> VSTest
	//		PSVolumetricShadow.hlsh -> PSVolumetricShadow
	// 
	// 2. ShaderCache::GetShader()�� ���� �ش� ���̴��� ���� shared_ptr�� �޾ƿɴϴ�.
	// std::shared_ptr<IShader> pShader = ShaderCache::GetShader(name);
	//

	ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTest.hlsl");
	ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTest.hlsl");

	ShaderCache::Load(eShaderType::Vertex, "../Kepler/Resources/Shaders/HLSL/VSTexture.hlsl");
	ShaderCache::Load(eShaderType::Pixel, "../Kepler/Resources/Shaders/HLSL/PSTexture.hlsl");

	//---------------------------------------------------

	// ���� �迭 �����ϱ� ----------------------------------
	// �׷��� ���� �迭�� �����մϴ�. 
	// ������ �����̹Ƿ� ������ Wrapping ���� ���� ���� �����͸� �����մϴ�.
	// �������� ���� �����ʹ� ���� �迭�� ǥ���Ǹ�, ���� �迭�� ���� ���� ���� ����(DirectX11 ���� �ִ� 16��)�� �ϳ��� Index Buffer�� �����˴ϴ�.
	// �� ���� ���۴� ������ �����Ϳ� ���� ���̾ƿ��� �����ؾ� �մϴ�.
	// 
	// 1. Vertex Array�� �����մϴ�. Vertex Array���� ���� ����(�ִ� 16��) Vertex Buffer�� �ϳ��� Index Buffer�� ���ϴ�.
	m_pVertexArray[0] = IVertexArray::Create();
	m_pVertexArray[1] = IVertexArray::Create();

	// 2-1.������ ���� �����͸� ����ü �������� ������ �� �ֽ��ϴ�.
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
	// 2-2. Vertex Array�� �� Vertex Buffer�� �����մϴ�.
	std::shared_ptr<IVertexBuffer> pTextureColorVB = IVertexBuffer::Create(reinterpret_cast<float*>(vertices), sizeof(vertices), eBufferUsage::Default);
	// 2-3. �ش� Vertex Buffer�� ���̾ƿ�(Shader ���� Vertex Input Layout���� ��Ī ���̾ƿ�)�� ������ �ݴϴ�.
	pTextureColorVB->SetLayout({
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 },
		{ "TEXCOORD", 0, kepler::eShaderDataType::Float2, 12, sizeof(float) * 2 },
		{ "COLOR", 0, kepler::eShaderDataType::Float4, 20, sizeof(float) * 4 }
		});
	// 2-4. Vertex Buffer�� Vertex Array�� �߰��մϴ�.
	m_pVertexArray[0]->AddVertexBuffer(pTextureColorVB);

	// 3-1. �� �����͸� ������ ���� ���� Vertex Buffer�� ������ ���� �ֽ��ϴ�.
	// ���� ��ǥ ������
	float textureVertices[] = {
		  0.25f,  0.25f, 0.0f,  // top right
		 0.25f, -0.25f, 0.0f,  // bottom right
		-0.25f, -0.25f, 0.0f,  // bottom left
		-0.25f,  0.25f, 0.0f   // top left
	};
	// �ؽ�ó ��ǥ ������
	float textureUV[]{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, 
		0.0f, 0.0f
	};
	
	// 3-2. ���� ��ǥ�� ���� Vertex Buffer�� �����մϴ�
	std::shared_ptr<IVertexBuffer> pTextureVertex = IVertexBuffer::Create(textureVertices, sizeof(vertices), eBufferUsage::Immutable);
	// 3-3. ���� ���̾ƿ��� �����մϴ�.
	pTextureVertex->SetLayout({
		{ "POSITION", 0, kepler::eShaderDataType::Float3, 0, sizeof(float) * 3 }
		});
	// 3-4. Vertex Buffer�� Vertex Array�� �߰��մϴ�.
	m_pVertexArray[1]->AddVertexBuffer(pTextureVertex);

	// �ؽ�ó ��ǥ Vertex Buffer�� ���������� ������ ��, ���̾ƿ��� �����մϴ�. ���� Vertex Array�� �߰��մϴ�.
	std::shared_ptr<IVertexBuffer> pTextureUV = IVertexBuffer::Create(textureUV, sizeof(textureUV), eBufferUsage::Immutable);
	pTextureUV->SetLayout({ 
		{ "TEXCOORD", 0, kepler::eShaderDataType::Float2, 0, sizeof(float) * 2 }
		});
	m_pVertexArray[1]->AddVertexBuffer(pTextureUV);

	// Index Buffer ������. 
	uint32_t indices[6]{
		0, 1, 2,
		0, 2, 3
	};
	// Index Buffer�� �����մϴ�. 
	// NOTE: Index Buffer���� Vertex Buffer�� �޸� ���� ũ��(size)�� �ƴ� �ε��� ����(count)�� ���ϴ�.
	std::shared_ptr<IIndexBuffer> pIB = IIndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t), eBufferUsage::Default);
	// 1~3���� ������ Vertex Array�� Index Buffer�� ����(set)�մϴ�.
	m_pVertexArray[0]->SetIndexBuffer(pIB);
	m_pVertexArray[1]->SetIndexBuffer(pIB);
	//---------------------------------------------------

	// ���̴��� ����� �ؽ�ó�� ������ �� ������ ���������ο� ���ε��մϴ�. 
	// �ؽ�ó�� ���ε� �� ������ ���̴� ���α׷��� � �ؽ�ó�� ���ν�ų�� ����(register number)�� �����մϴ�
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