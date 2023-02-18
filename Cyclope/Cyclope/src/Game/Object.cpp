#include "Object.h"

namespace Cyclope {

	Object::Object()
	{
		
	}

	Object::Object(Transform transform, std::string name)
	{
		this->transform = transform;
		this->name = name;
	}

	Object::Object(Vector3f position, Vector3f scale, Quaternion rotation, std::string name)
	{
		transform.position = position;
		transform.scale = scale;
		transform.rotation = rotation;
		this->name = name;
	}

	Object::~Object() {
		for (auto c : m_components) {
			delete c;
		}
	}

	////////////////////////////
	/////COMPONENT
	////////////////////////////

	Component::Component() {

	}

	const char* Component::GetName() {
		return typeid(*this).name();
	}

	Object* Component::GetObject() {
		return object;
	}

}