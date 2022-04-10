#pragma once

#include <vector>

#include "Core/Layer.hpp"

namespace kepler {

	class LayerStack
	{
	private:
		std::vector<Layer*> m_pLayers;
		std::vector<Layer*>::iterator m_lastPushedLayer;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(const Layer* layer);
		void PushOverlay(const Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		
		// LayerStack 순회를 위한 Iterator 구현
		inline std::vector<Layer*>::iterator begin() { return m_pLayers.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_pLayers.end(); }
	};

}