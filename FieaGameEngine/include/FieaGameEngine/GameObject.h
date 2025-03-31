#pragma once
#include "ContentService.h"
#include "Action.h"

using namespace Fiea::Engine::Content;
namespace Fiea::Engine
{
    class GameObject : public Attributed
    {
        RTTI_DECLARATIONS(GameObject, Attributed);
    public:
        GameObject() : Attributed(&ClassDefinitions, GameObject::TypeIdClass()) {};
        GameObject(const GameObject& rhs) = default;
        GameObject(GameObject&& rhs) noexcept = default;
        GameObject& operator=(const GameObject& rhs) = default;
        GameObject& operator=(GameObject&& rhs) noexcept = default;
        virtual ~GameObject() = default;
        struct _Transform
        {
            Vec4Type Position;
            Vec4Type Rotation;
            Vec4Type Scales;
        } Transform;
        virtual void Update();
        // Rule-of-5 methods
        
       /* bool operator==(const GameObject& rhs);
        bool operator!=(const GameObject& rhs);*/

        // Scope overrides
        [[nodiscard]] GameObject* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        void AddChild(GameObject& child);
        void AddAction(Action& action);
        void RemoveChild(size_t idx);
        size_t GetChildCount();
        size_t GetActionCount();
        GameObject* GetChild(size_t idx);
        void ForEachChild(void (*func)(GameObject*));
    protected:
        static ClassDef ClassDefinitions();
    };
    FACTORY_DECLARE(GameObject, Scope);
}

