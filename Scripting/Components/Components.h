#include "Scripting.h"

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

}