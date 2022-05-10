#pragma once

#include <list>

#include "Core/Layer.hpp"

namespace kepler {

	class LayerStack
	{
	private:
		std::list<Layer*>			m_pLayers;
		std::list<Layer*>::iterator m_lastLayerIterator;
	public:
		LayerStack();
		~LayerStack();

		// NOTICE: Layer, Overlay Push �� new Ű���带 �̿��� �����ڸ� ���� �����ؾ� �մϴ�!
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		
		// LayerStack ��ȸ�� ���� Iterator ����
		[[nodiscard]] inline std::list<Layer*>::iterator begin() { return m_pLayers.begin(); }
		[[nodiscard]] inline std::list<Layer*>::iterator end() { return m_pLayers.end(); }
	};

}