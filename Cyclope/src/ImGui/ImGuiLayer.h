#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Layer.h"

namespace Cyclope {

	class CYCLOPE_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	};

}

#endif // !IMGUILAYER_H