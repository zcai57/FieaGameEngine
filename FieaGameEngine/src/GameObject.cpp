#include "FieaGameEngine/GameObject.h"
#include "FieaGameEngine/Action.h"

namespace Fiea::Engine
{
    RTTI_DEFINITIONS(GameObject);
    FACTORY_DEFINE(GameObject);
    FACTORY_DEFINE_ABSTRACT(GameObject);
    using namespace std::string_literals;

    /// <summary>
    /// Clone function for Cloneable pattern
    /// </summary>
    /// <returns></returns>
    GameObject* GameObject::Clone() const
    {
        return new GameObject(*this);
    }

    /// <summary>
    /// To String for RTTI override
    /// </summary>
    /// <returns></returns>
    std::string GameObject::ToString() const
    {
        return "GameObject"s;
    }

    /// <summary>
    /// RTTI equals override
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    bool GameObject::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }
    
    /// <summary>
    /// Add GameObject Child
    /// </summary>
    /// <param name="child"></param>
    void GameObject::AddChild(GameObject& child)
    {
        FIEA_ASSERT(Find("Children"s) != nullptr);
        if (Find("Children"s) != nullptr)
        {
            this->Adopt("Children", child);
        }   
    }

    void GameObject::AddAction(Action& action)
    {
        
        Datum* actionDatum = Find("Action"s);
        FIEA_ASSERT(actionDatum != nullptr);
        if (actionDatum != nullptr)
        {
            this->Adopt("Action", action);
        }
    }

    /// <summary>
    /// Remove Gameobject Child by index
    /// </summary>
    /// <param name="idx"></param>
    void GameObject::RemoveChild(size_t idx)
    {
        Datum* children = Find("Children");
        FIEA_ASSERT(children != nullptr);
        if (children != nullptr)
        {
            FIEA_ASSERT(idx < children->Size());
            Scope* s = children->GetPointer<Scope*>(idx);
            delete s;
            //children->SetPointer<Scope*>(nullptr, idx);
            children->RemoveAt(idx);
        }
    }

    /// <summary>
    /// Register Class Attributes
    /// Vect4Type: Position, Rotation, Scales
    /// TableDatum(Internal): Children
    /// </summary>
    /// <returns></returns>
    GameObject::ClassDef GameObject::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ offsetof(GameObject, Transform) + offsetof(GameObject::_Transform, Position), Datum::Type::Vec4, 1, "Position"s});
        ExternalData.Signatures.push_back({ offsetof(GameObject, Transform) + offsetof(GameObject::_Transform, Rotation), Datum::Type::Vec4, 1, "Rotation"s});
        ExternalData.Signatures.push_back({ offsetof(GameObject, Transform) + offsetof(GameObject::_Transform, Scales), Datum::Type::Vec4, 1, "Scales"s});
        
        ExternalData.Signatures.push_back({ size_t(0), Datum::Type::Table, size_t(0), "Children"s });
        ExternalData.Signatures.push_back({ size_t(0), Datum::Type::Table, size_t(0), "Action"s });
        return ExternalData;
    }

    /// <summary>
    /// Call update on all children
    /// </summary>
    void GameObject::Update()
    {
        Datum* Children = Find("Children"s);
        FIEA_ASSERT(Children != nullptr);
        FIEA_ASSERT(Children->IsType(Datum::Type::Table));
        if (Children != nullptr)
        {
            for (size_t i = 0; i < Children->Size(); ++i)
            {
                FIEA_ASSERT(Children->GetPointer<Scope*>(i)->TypeIdInstance() == GameObject::TypeIdClass());
                reinterpret_cast<GameObject*>(Children->GetPointer<Scope*>(i))->Update();
            }
        }
        Datum* dAction = Find("Action"s);
        FIEA_ASSERT(dAction != nullptr);
        FIEA_ASSERT(dAction->IsType(Datum::Type::Table));
        if (dAction != nullptr)
        {
            int32_t i = (int32_t)(dAction->Size() - 1);
            while(i >= 0)
            {
                Action* curAction = reinterpret_cast<Action*>(dAction->GetPointer<Scope*>(i));
                // If cur Action not correctly inited, continue
                if (!curAction->Init()) {
                    i--;
                    continue;
                }
                bool actionFinished = curAction->Update();
                if (actionFinished)
                {
                    delete curAction;
                    dAction->RemoveAt(i);
                }
                i--;
            }
        }
    }

    /// <summary>
    /// Get Child Count helper for Gameobject
    /// </summary>
    /// <returns></returns>
    size_t GameObject::GetChildCount()
    {
        Datum* Children = Find("Children"s);
        FIEA_ASSERT(Children != nullptr);
        if (Children != nullptr) {
            return Children->Size();
        }
        return (size_t)0;
    }

    size_t GameObject::GetActionCount()
    {        Datum* Actions = Find("Action"s);
        FIEA_ASSERT(Actions != nullptr);
        if (Actions != nullptr)
        {
            return Actions->Size();
        }
        return (size_t)0;
    }

    /// <summary>
    /// Get child by index
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    GameObject* GameObject::GetChild(size_t idx)
    {
        Datum* Children = Find("Children"s);
        FIEA_ASSERT(Children != nullptr);
        if (Children != nullptr) {
            return reinterpret_cast<GameObject*>(Children->GetPointer<Scope*>(idx));
        }
        return nullptr;
    }

    /// <summary>
    /// ForEach Child do void function(GameObject*)
    /// </summary>
    /// <param name="func"></param>
    void GameObject::ForEachChild(void(*func)(GameObject*))
    {
        Datum* Children = Find("Children"s);
        FIEA_ASSERT(Children != nullptr);
        if (Children != nullptr) {
            for (size_t i = 0; i < Children->Size(); ++i)
            {
                func(reinterpret_cast<GameObject*>(Children->GetPointer<Scope*>(i)));
            }
        }
    }
    

}
