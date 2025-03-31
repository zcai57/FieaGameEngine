#pragma once
#include "FieaGameEngine/Action.h"

namespace Fiea::Engine::Actions
{
    //using namespace Fiea::Engine;
    class ActionIncrement : public Action
    {
        RTTI_DECLARATIONS(ActionIncrement, Action);
    public:
        ActionIncrement() : Action(&ClassDefinitions, ActionIncrement::TypeIdClass()) {};

        [[nodiscard]] Action* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        virtual bool Update() override;
        virtual bool Init() override;
        virtual bool CleanUp() override;
        virtual void SetIncrement(string key, float amount);
       
    protected:
        static ClassDef ClassDefinitions();
    private: 
        string Key;
        float Amount;
    };
}
