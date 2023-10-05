#ifndef LAYER_H
#define LAYER_H

#include "Core.h"

namespace Cyclope {

	class CYCLOPE_API Layer {

	public:
		Layer() {}
		virtual ~Layer() {};

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(float dt) {};
		virtual void OnImGuiRender() {};

	};

}

#endif // !LAYER_H