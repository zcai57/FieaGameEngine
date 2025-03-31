#pragma once
#include "FieaGameEngine/Action.h"
#include "FieaGameEngine/ActionList.h"
namespace Fiea::Engine::Actions
{
    class ActionWhile : public Action
    {
        RTTI_DECLARATIONS(ActionWhile, Action);
    public:
        ActionWhile() : Action(&ClassDefinitions, ActionWhile::TypeIdClass()) {};

        [[nodiscard]] ActionWhile* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        virtual bool Update() override;
        virtual bool Init() override;
        virtual bool CleanUp() override;
        // Call this to properly init this Action
        void SetCondition(int32_t Condition, int32_t increment, Action* pre = nullptr, ActionList* list = nullptr);
        int32_t counter;
    protected:
        static ClassDef ClassDefinitions();
    private:
        int32_t _WhileCondition;
    };
    
}

