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
	};

}