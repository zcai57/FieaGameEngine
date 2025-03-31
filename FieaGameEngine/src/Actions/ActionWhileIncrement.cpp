#include "FieaGameEngine/Actions/ActionWhileIncrement.h"

namespace Fiea::Engine::Actions
{
    RTTI_DEFINITIONS(ActionWhileIncrement);
    using namespace std::string_literals;

    Action* ActionWhileIncrement::Clone() const
    {
        return new ActionWhileIncrement(*this);
    }

    std::string ActionWhileIncrement::ToString() const
    {
        return "ActionWhileIncrement"s;
    }

    bool ActionWhileIncrement::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    bool ActionWhileIncrement::Update()
    {
        int32_t currentNum = currentCondition.Get<int32_t>();
        currentCondition.Set<int32_t>(currentNum + Increment);
        return true;
    }

    bool ActionWhileIncrement::Init()
    {
        return ActionInited;
    }

    bool ActionWhileIncrement::CleanUp()
    {
        return true;
    }
 
   /* void ActionWhileIncrement::setCurrentCondition(Datum* condition, int32_t increment)
    {
        currentCondition = *condition;
        ActionInited = true;
    }*/
    ActionWhileIncrement::ClassDef ActionWhileIncrement::ClassDefinitions()
    {
        ClassDef ExternalDef;
        return ExternalDef;
    }
}
