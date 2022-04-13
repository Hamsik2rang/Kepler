#include "kepch.h"

#include "Core/LayerStack.h"

namespace kepler {

	LayerStack::LayerStack()
	{
		m_lastLayerIterator = m_pLayers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_pLayers)
		{
			if (layer)
			{
				delete layer;
				layer = nullptr;
			}
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// TODO: 임시 코드. 나중에 리팩토링 해야합니다.
		if (m_lastLayerIterator == m_pLayers.end())
		{
			m_lastLayerIterator = m_pLayers.begin();
		}
		// 일반 Layer는 스택처럼 쌓여야 함
		if (!m_pLayers.empty())
		{
			m_lastLayerIterator++;
		}
		m_lastLayerIterator = m_pLayers.emplace(m_lastLayerIterator, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		// 오버레이는 어떤 다른 일반 레이어보다 위에 존재해야 함
		m_pLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_pLayers.begin(), m_pLayers.end(), layer);
		// 인자로 받은 찾고자 하는 레이어가 레이어 스택 안에 존재하는 경우
		if (it != m_pLayers.end())	
		{
			// 해당 레이어를 제거하고 레이어 스택의 오버레이를 제외한 top index를 한칸 내림(Stack의 pop과 동일)
			m_pLayers.erase(it);
			m_lastLayerIterator--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		// 모든 오버레이는 다른 어떤 일반 레이어보다도 위에 있으므로 lastPushedLayer부터 찾으면 됨
		auto it = std::find(m_lastLayerIterator, m_pLayers.end(), overlay);
		// 인자로 받은 오버레이가 스택 안에 존재하는 경우
		if (it != m_pLayers.end())
		{
			// 해당 오버레이를 제거
			m_pLayers.erase(it);
		}
	}
}