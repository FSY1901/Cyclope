#ifndef LAYER_H
#define LAYER_H

#include "Core.h"

#include "Events/Event.h"

namespace Cyclope {

	class CYCLOPE_API Layer {

	public:
		Layer() {}
		virtual ~Layer() {};

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(float dt) {};
		virtual void OnEvent(Event& e) {};
		virtual void OnImGuiRender() {};

	};

}

#endif // !LAYER_H