#include "FieaGameEngine/ActionList.h"

namespace Fiea::Engine
{
    RTTI_DEFINITIONS(ActionList);
    using namespace std::string_literals;
    /// <summary>
    /// ActionList Update
    /// Update all actions in list in a sequential order
    /// </summary>
    /// <returns></returns>
    bool ActionList::Update()
    {
        Datum* actionList = Find("ActionList"s);
        FIEA_ASSERT(actionList != nullptr);
        bool AllFinished = true;
        if (actionList != nullptr)
        {
            for (size_t i = 0; i < actionList->Size(); ++i)
            {
                Action* curAction = reinterpret_cast<Action*>(actionList->GetPointer<Scope*>(i));
                if (!curAction->Init()) continue;
                //FIEA_ASSERT(actionList->GetPointer<Scope*>(i)->TypeIdInstance() == Action::TypeIdClass());
                bool updated = curAction->Update(); 
                if (!updated) AllFinished = false;
            }
        }
        return AllFinished;
    }

    /// <summary>
    /// Add action into ActionList
    /// </summary>
    /// <param name="action"></param>
    void ActionList::AddAction(Action& action)
    {
        Datum* actionDatum = Find("ActionList"s);
        FIEA_ASSERT(actionDatum != nullptr);
        if (actionDatum != nullptr)
        {
            this->Adopt("ActionList", action);
        }
    }

    ActionList* ActionList::Clone() const
    {
        return new ActionList(*this);
    }
    std::string ActionList::ToString() const
    {
        return "ActionList"s;
    }

    bool ActionList::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    /// <summary>
    /// Class Attribute ActionList
    /// </summary>
    /// <returns></returns>
    ActionList::ClassDef Fiea::Engine::ActionList::ClassDefinitions()
    {
        ClassDef ExternalData;

        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "ActionList"s });
        return ExternalData;
    }
}
