#include "Cyclope.h"
#include "GUI.h"
namespace Scripting {
	
	REGISTER_COMPONENT(PlayerComponent);
	struct PlayerComponent {

		PlayerComponent() = default;
		PlayerComponent(const PlayerComponent&) = default;

		float health = 100.0f;
	};

	REGISTER_GUI_FUNCTION(PlayerComponent, 
	{
		ImGui::DragFloat("health", &e.GetComponent<PlayerComponent>().health);
	})

	REGISTER_COMPONENT(TestC);
	struct TestC {

		float value = 100.0f;
	};

	REGISTER_GUI_FUNCTION(TestC,
	{
		ImGui::DragFloat("value", &e.GetComponent<TestC>().value);
	})

	using namespace Cyclope;

	REGISTER_SCRIPT(Script);
	class Script : public Cyclope::ScriptableEntity {

	public:
		TransformComponent transform;
		
		void OnCreate() {
			transform = GetComponent<TransformComponent>();
			//auto p = GetComponent<PlayerComponent>();
			//std::cout << p.health << std::endl;
		}

		void OnUpdate(float dt) {
			std::cout << "Q" << std::endl;
		}

	};

	REGISTER_SCRIPT(Script2);
	class Script2 : public Cyclope::ScriptableEntity {
	public:
		void OnUpdate(float dt) {
			std::cout << "F" << std::endl;
		}
	};

}