#include "FieaGameEngine/Action.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"

namespace Fiea::Engine::Actions
{
    RTTI_DEFINITIONS(ActionIncrement);
    using namespace std::string_literals;
    Action* ActionIncrement::Clone() const
    {
        return new ActionIncrement(*this);
    }

    std::string ActionIncrement::ToString() const
    {
        return "ActionIncrement"s;
    }

    bool ActionIncrement::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }


    /// <summary>
    /// Update function override
    /// </summary>
    /// <returns></returns>
    bool ActionIncrement::Update()
    {
        Scope* parent = GetParent();
        Datum* dt = parent->Search(Key, parent);

        FIEA_ASSERT(dt->IsType(Datum::Type::Float));
        // Increment All Elements
        for (size_t i = 0; i < dt->Size(); ++i)
        {
            dt->Set<float>(Amount + dt->Get<float>(i), i);
        }
        
        return true;
    }

    /// <summary>
    /// SetUp function need to be called to properly init class
    /// </summary>
    /// <param name="key"></param>
    /// <param name="amount"></param>
    void ActionIncrement::SetIncrement(string key, float amount)
    {
        Key = key;
        Amount = amount;
        ActionInited = true;
    }


    bool ActionIncrement::Init()
    {
        return ActionInited;
    }

    bool ActionIncrement::CleanUp()
    {
        return true;
    }

    /// <summary>
    /// Class Defintion with class attribute
    /// string Key
    /// float Amount
    /// </summary>
    /// <returns></returns>
    ActionIncrement::ClassDef ActionIncrement::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ offsetof(ActionIncrement, Key), Datum::Type::String, 1, "Key"s });
        ExternalData.Signatures.push_back({ offsetof(ActionIncrement, Amount), Datum::Type::Float, 1, "Amount"s });

        return ClassDef();
    }
}

