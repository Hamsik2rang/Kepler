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

		// NOTICE: Layer, Overlay Push 시 new 키워드를 이용해 생성자를 직접 전달해야 합니다!
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		
		// LayerStack 순회를 위한 Iterator 구현
		[[nodiscard]] inline std::list<Layer*>::iterator begin() { return m_pLayers.begin(); }
		[[nodiscard]] inline std::list<Layer*>::iterator end() { return m_pLayers.end(); }
	};

}