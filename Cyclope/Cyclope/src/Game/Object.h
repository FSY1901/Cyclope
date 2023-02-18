#ifndef OBJECT_H
#define OBJECT_H

#include "../Cyclope/Core.h"
#include "../Maths/Maths.h"

#include <string>
#include <vector>
#include <memory>

namespace Cyclope {

    struct API Transform {
        Vector3f position;
        Vector3f scale;
        Quaternion rotation;
    };

    class API Component;

	class API Object {

    public:
        Object();
        Object(Transform transform, std::string name);
        Object(Vector3f position, Vector3f scale, Quaternion rotation, std::string name);

        ~Object();

        Transform transform;
        std::string name;

#pragma region Component Related
        std::vector<Component*> m_components;

        template<class T> bool AddComponent() {
            Component* c = new T();
            if (this->HasComponent<T>()) {
                return false;
            }
            c->object = this;
            m_components.push_back(c);
            return true;
        }

        template<class T> bool HasComponent() {
            Component* c = new T();
            const char* ID = c->GetName();
            delete c;
            for (int i = 0; i < m_components.size(); ++i) {
                c = m_components[i];
                auto ID2 = c->GetName();
                if (ID == ID2) {
                    return true;
                }
            }
            return false;
        }

        template<class T> T* GetComponent() {
            Component* c = new T();
            auto name = c->GetName();
            delete c;
            for (auto& comp : m_components) {
                if (name == comp->GetName()) {
                    return (T*)comp;
                }
            }
            return nullptr;
        }

        template<class T> void RemoveComponent() {
            Component* c = new T();
            for (auto& comp : m_components) {
                int index = 0;
                if (comp->GetName() == c->GetName()) {
                    m_components.erase(m_components.begin() + index);
                    delete c;
                    return;
                }
                index++;
            }
            delete c;
        }
#pragma endregion


    private:

        friend class Scene;
        friend class Application;

	};

    class API Component {

    public:
        Component();
        /// <summary>
        /// Gets called on the first frame.
        /// </summary>
        virtual void Start() {}
        /// <summary>
        /// Gets called every frame.
        /// </summary>
        virtual void Update() {}

        virtual Component* Clone() = 0;

        const char* GetName();

        Object* GetObject();

    protected:
        Object* object;

        friend class Object;
        extern friend class Scene;

    };

}

#endif // !OBJECT_H
