#pragma once

#include <unordered_map>
#include "../Game/Entity.h"

namespace Cyclope {
	using AddComponentFunction = void(*)(Entity& e);
	using CopyComponentFunction = void(*)(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap);
	using HasComponentFunction = bool(*)(Entity& e);
	using RemoveComponentFunction = void(*)(Entity& e);
	using AddNativeScriptFunction = void(*)(Entity& e);
	using ComponentFunctions = struct { 
		AddComponentFunction AddComponent; 
		CopyComponentFunction CopyComponent;
		HasComponentFunction HasComponent;
		RemoveComponentFunction RemoveComponent;
	};
	using ComponentRegistry = std::unordered_map<size_t, ComponentFunctions>;
	using NativeScriptRegistry = std::unordered_map<size_t, AddNativeScriptFunction>;
	using ComponentNamesList = std::vector<std::string>;
	using NativeScriptNamesList = std::vector<std::string>;

	extern CYCLOPE_API ComponentRegistry& componentRegistry();
	extern CYCLOPE_API NativeScriptRegistry& nativeScriptRegistry();
	extern CYCLOPE_API ComponentNamesList& componentNamesList();
	extern CYCLOPE_API NativeScriptNamesList& nativeScriptNamesList(); //= std::vector<std::string>{"None"};

	inline uint8_t RegisterComponent(const std::string& name, AddComponentFunction aFunc, CopyComponentFunction cFunc, HasComponentFunction hFunc, RemoveComponentFunction rFunc) {
		componentNamesList().push_back(name);
		size_t tag = std::hash<std::string>()(name);
		return componentRegistry().insert(ComponentRegistry::value_type({tag, {aFunc, cFunc, hFunc, rFunc}})).second;
	}

	inline uint8_t RegisterNativeScript(const std::string& name, AddNativeScriptFunction func) {
		nativeScriptNamesList().push_back(name);
		size_t tag = std::hash<std::string>()(name);
		return nativeScriptRegistry().insert(NativeScriptRegistry::value_type({tag, func})).second;
	}

#define REGISTER_COMPONENT(TYPE)									\
	struct TYPE;													\
	namespace {														\
		const uint8_t reg_##TYPE									\
		{ Cyclope::RegisterComponent(								\
			(#TYPE),												\
			[](Cyclope::Entity& e){e.AddComponent<TYPE>();},		\
			[](entt::registry& dst, entt::registry& src, const std::unordered_map<Cyclope::UUID, entt::entity>& enttMap){ \
			auto view = src.view<TYPE>();		\
			for (auto e : view) {		\
			Cyclope::UUID uuid = src.get<Cyclope::IDComponent>(e).id;						\
			entt::entity id = enttMap.at(uuid);	\
			auto& component = src.get<TYPE>(e);	\
			dst.emplace_or_replace<TYPE>(id, component);\
			}\
			},		\
			[](Cyclope::Entity& e){return e.HasComponent<TYPE>();},	\
			[](Cyclope::Entity& e){e.RemoveComponent<TYPE>();})		\
		};															\
	}

#define REGISTER_SCRIPT(TYPE)									\
	class TYPE;													\
	namespace {													\
		const uint8_t reg_##TYPE								\
		{ Cyclope::RegisterNativeScript(						\
			(#TYPE),											\
			[](Cyclope::Entity& e){e.GetComponent<Cyclope::NativeScriptComponent>().Bind<TYPE>(#TYPE);})	\
		};														\
	}

}
