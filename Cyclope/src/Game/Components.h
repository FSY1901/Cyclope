#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../Maths/Maths.h"
#include "../Rendering/Camera.h"
#include "ScriptableEntity.h"
#include "../Cyclope/UUID.h"
#include "../Rendering/Mesh.h"

namespace Cyclope {

	class Scene;

	struct IDComponent {
		UUID id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

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

	struct DirectionalLightComponent {
		Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 ambient = Vector3(0.2f, 0.2f, 0.2f);
		float ambientValue = 0.2f;
	};

	struct PointLightComponent {
		Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 ambient = Vector3(0.2f, 0.2f, 0.2f);
		float ambientValue = 0.2f;

		float radius = 5.0f;
		float intensity = 1.0f;
		float cutOff = 0.001f;
	};

	struct SpotLightComponent {
		Vector3 diffuse = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 ambient = Vector3(0.2f, 0.2f, 0.2f);
		float ambientValue = 0.2f;

		float intensity = 1.0f;
		float cutOff = 12.5f; //in Degrees
		float outerCutOff = 17.5f; //in Degrees
		float linear = 0.0f;
		float quadratic = 0.05f;
	};

	struct ModelRendererComponent {
		//Material Properties
		Shared<Shader> shader;//TODO: Materials --> will store shaders
		Vector3 diffuse = Vector3(1.0f);
		Vector3 specular = Vector3(0.5f);
		float shininess = 32.0f;

		//Model/Mesh
		Shared<Model> model;
	};

}

#endif