#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../Maths/Maths.h"
#include "../Rendering/Camera.h"
#include "ScriptableEntity.h"

namespace Cyclope {

	class Scene;

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct TransformComponent{

		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Quaternion rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::mat4_cast(rotation);

			return glm::translate(glm::mat4(1.0f), position)
				* rot
				* glm::scale(glm::mat4(1.0f), scale);;
		}

	};

	struct CameraComponent {

		Camera camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator Camera& () { return camera; }
		operator const Camera& () const { return camera; }

	};

	struct NativeScriptComponent {

		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)() = nullptr;
		void (*DestroyScript)(NativeScriptComponent*);

		std::string scriptName = "None";

		template<typename T>
		void Bind(const std::string& name)
		{
			scriptName = name;
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}

		void Unbind() {
			scriptName = "None";
			InstantiateScript = nullptr;
			DestroyScript = nullptr;
		}

	};

}

#endif