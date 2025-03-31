#include "FieaGameEngine/Action.h"
namespace Fiea::Engine
{
    using namespace std::string_literals;
    RTTI_DEFINITIONS(Action);
    FACTORY_DEFINE(Action);
    FACTORY_DEFINE_ABSTRACT(Action);
    bool Action::Update()
    {
        return true;
    }
    /// <summary>
    /// Action Init set states
    /// </summary>
    /// <returns></returns>
    bool Action::Init()
    {
        ActionFinished = false;
        ActionInited = true;
        return true;
    }
    /// <summary>
    /// Clean up states
    /// </summary>
    /// <returns></returns>
    bool Action::CleanUp()
    {
        ActionFinished = true;
        ActionInited = false;
        return true;
    }

    Action* Action::Clone() const
    {
        return new Action(*this);
    }

    std::string Action::ToString() const
    {
        return "Action"s;
    }

    bool Action::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    Action::ClassDef Action::ClassDefinitions()
    {
        return ClassDef();
    }
}
