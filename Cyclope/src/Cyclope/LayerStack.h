#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace Cyclope {

	class CYCLOPE_API LayerStack {

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;


	};

}

#endif