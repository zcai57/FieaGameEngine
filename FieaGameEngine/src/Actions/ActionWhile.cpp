#include "FieaGameEngine/Actions/ActionWhile.h"
#include "FieaGameEngine/Actions/ActionWhileIncrement.h"
#include "FieaGameEngine/ActionList.h"

namespace Fiea::Engine::Actions
{
    RTTI_DEFINITIONS(ActionWhile);
    using namespace std::string_literals;

    ActionWhile* ActionWhile::Clone() const
    {
        return new ActionWhile(*this);
    }
    std::string ActionWhile::ToString() const
    {
        return "ActionWhile"s;
    }

    bool ActionWhile::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    /// <summary>
    /// Update override
    /// </summary>
    /// <returns></returns>
    bool ActionWhile::Update()
    {
        Datum* Preamble = Find("Preamble"s);
        Datum* Increment = Find("Increment"s);
        Datum* Condition = Find("Condition"s);
        Datum* ActionList = Find("ActionList"s);

       /* FIEA_ASSERT(Preamble != nullptr && Preamble->Size() > 0);*/
        FIEA_ASSERT(Increment != nullptr && Increment->Size() > 0);
        FIEA_ASSERT(Condition != nullptr && Condition->Size() > 0);
        //FIEA_ASSERT(ActionList != nullptr && ActionList->Size() > 0);

        if (Preamble != nullptr)
        {
            reinterpret_cast<Action*>(Preamble->GetPointer<Scope*>())->Update();
        }
        while (Condition->Get<int32_t>() < _WhileCondition)
        {
            if (ActionList != nullptr)
            {
                reinterpret_cast<Fiea::Engine::ActionList*>(ActionList->GetPointer<Scope*>())->Update();
            }
            reinterpret_cast<Action*>(Increment->GetPointer<Scope*>())->Update();
        }

        return true;
    }

    bool ActionWhile::Init()
    {
        return ActionInited;
    }

    bool ActionWhile::CleanUp()
    {
        return true;
    }

    /// <summary>
    /// Set up function to properly set up class
    /// </summary>
    /// <param name="Condition"></param>
    /// <param name="increment"></param>
    /// <param name="pre"></param>
    /// <param name="list"></param>
    void ActionWhile::SetCondition(int32_t Condition, int32_t increment, Action* pre, ActionList* list)
    {
        // Set preable
        Datum* preamble = Find("Preamble"s);
        //preamble->PushPointer<Scope*>(pre);
        this->Adopt("Preamble"s, *pre);

        // Set ActionList
        Datum* actionList = Find("ActionList"s);
        this->Adopt("ActionList"s, *list);

        Datum* dCondition = Find("Condition"s);
        counter = 0;
        FIEA_ASSERT(dCondition->Get<int32_t>() == size_t(0));
        _WhileCondition = Condition;
        
        Datum* dIncrement = Find("Increment"s);
        FIEA_ASSERT(dIncrement != nullptr);
        ActionWhileIncrement* newIncrement = new ActionWhileIncrement(dCondition, increment);
        this->Adopt("Increment"s, *newIncrement);
        // Give reference to ActionWhileIncrement datum
        //reinterpret_cast<ActionWhileIncrement*>(dIncrement->Get<Scope*>())->setCurrentCondition(dCondition);
        ActionInited = true;
    }

    /// <summary>
    /// Class definition includes
    /// Datum Table: Preamble
    /// Datum Int: Condition
    /// Datum Table: ActionList
    /// Datum Table: Increment
    /// </summary>
    /// <returns></returns>
    ActionWhile::ClassDef ActionWhile::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "Preamble"s });
        ExternalData.Signatures.push_back({ offsetof(ActionWhile, counter), Datum::Type::Int, 1, "Condition"s });
        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "ActionList"s });
        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "Increment"s});

        return ExternalData;
    }
}
