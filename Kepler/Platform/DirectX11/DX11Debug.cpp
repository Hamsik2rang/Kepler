#include "kepch.h"

#include "DX11Debug.h"
#include "DX11Context.h"

namespace kepler {

DX11Debug::DX11Debug()
	: m_pDebug{ nullptr }
	, m_pInfoQueue{ nullptr }
{

}

DX11Debug::~DX11Debug()
{
	if (m_pInfoQueue)
	{
		m_pInfoQueue->Release();
		m_pInfoQueue = nullptr;
	}
	if (m_pDebug)
	{
		m_pDebug->Release();
		m_pDebug = nullptr;
	}
}

bool DX11Debug::Init()
{
	ID3D11Device* pDevice = static_cast<ID3D11Device*>(IGraphicsContext::Get()->GetDevice());
	if (SUCCEEDED(pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug)))
	{
		if (SUCCEEDED(pDevice->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&m_pInfoQueue)))
		{
#ifdef _DEBUG
			m_pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			m_pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			return true;
		}
	}

	return false;
}

void DX11Debug::CheckReferenceCount()
{
	if (m_pDebug)
	{
		m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
}
}