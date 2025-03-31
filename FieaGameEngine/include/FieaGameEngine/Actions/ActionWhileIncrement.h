#pragma once
#include "FieaGameEngine/Action.h"

namespace Fiea::Engine::Actions
{
    //using namespace Fiea::Engine;
    class ActionWhileIncrement : public Action
    {
        RTTI_DECLARATIONS(ActionWhileIncrement, Action);
    public:
        ActionWhileIncrement(Datum* dt, int32_t increment) : Action(&ClassDefinitions, ActionWhileIncrement::TypeIdClass()), currentCondition(*dt), Increment(increment) { ActionInited = true; };

        [[nodiscard]] Action* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        virtual bool Update() override;
        virtual bool Init() override;
        virtual bool CleanUp() override;
        int32_t Increment;
        Datum& currentCondition;
    protected:
        static ClassDef ClassDefinitions();
    private:
        
    };
}

