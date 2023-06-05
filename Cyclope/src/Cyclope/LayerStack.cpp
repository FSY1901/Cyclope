#include "LayerStack.h"

namespace Cyclope {

	LayerStack::LayerStack() {

	}

	LayerStack::~LayerStack() {
		for (Layer* l : m_Layers)
			delete l;
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.push_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer) {

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end())
			m_Layers.erase(it);

	}

}