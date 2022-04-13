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
		// TODO: �ӽ� �ڵ�. ���߿� �����丵 �ؾ��մϴ�.
		if (m_lastLayerIterator == m_pLayers.end())
		{
			m_lastLayerIterator = m_pLayers.begin();
		}
		// �Ϲ� Layer�� ����ó�� �׿��� ��
		if (!m_pLayers.empty())
		{
			m_lastLayerIterator++;
		}
		m_lastLayerIterator = m_pLayers.emplace(m_lastLayerIterator, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		// �������̴� � �ٸ� �Ϲ� ���̾�� ���� �����ؾ� ��
		m_pLayers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_pLayers.begin(), m_pLayers.end(), layer);
		// ���ڷ� ���� ã���� �ϴ� ���̾ ���̾� ���� �ȿ� �����ϴ� ���
		if (it != m_pLayers.end())	
		{
			// �ش� ���̾ �����ϰ� ���̾� ������ �������̸� ������ top index�� ��ĭ ����(Stack�� pop�� ����)
			m_pLayers.erase(it);
			m_lastLayerIterator--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		// ��� �������̴� �ٸ� � �Ϲ� ���̾�ٵ� ���� �����Ƿ� lastPushedLayer���� ã���� ��
		auto it = std::find(m_lastLayerIterator, m_pLayers.end(), overlay);
		// ���ڷ� ���� �������̰� ���� �ȿ� �����ϴ� ���
		if (it != m_pLayers.end())
		{
			// �ش� �������̸� ����
			m_pLayers.erase(it);
		}
	}
}