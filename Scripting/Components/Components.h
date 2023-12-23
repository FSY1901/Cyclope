#include "Cyclope.h"

namespace Scripting {
	
	REGISTER_COMPONENT(PlayerComponent);
	struct PlayerComponent {

		PlayerComponent() = default;
		PlayerComponent(const PlayerComponent&) = default;

		float health = 100.0f;
	};

	REGISTER_COMPONENT(TestC);
	struct TestC {

		float health = 100.0f;
	};

	using namespace Cyclope;

	REGISTER_SCRIPT(Script);
	class Script : public Cyclope::ScriptableEntity {

	public:
		TransformComponent transform;
		
		void OnCreate() {
			transform = GetComponent<TransformComponent>();
		}

		void OnUpdate(float dt) {
			if (Input::KeyPressed(Key::Q)) {
				std::cout << "Q" << std::endl;
			}
		}

	};

}