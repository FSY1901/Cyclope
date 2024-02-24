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
			if (HasComponent<PlayerComponent>()) {
				auto p = GetComponent<PlayerComponent>();
			}
			else {
				std::cout << "No Player Component";
			}
			//std::cout << p.health << std::endl;
		}

		void OnUpdate(float dt) {
			
		}

		void OnEvent(Event& e) {
			EventDispatcher dispatcher(e);
			//dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Script::Scroll));
			dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Script::Scroll));
		}

		bool Scroll(MouseScrolledEvent& e) {
			std::cout << "Scrolling" << std::endl;
			return true;
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