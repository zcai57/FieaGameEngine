#include "FieaGameEngine/Actions/DelayAction.h"

namespace Fiea::Engine::Actions
{
    RTTI_DEFINITIONS(DelayAction);
    using namespace std::string_literals;
    DelayAction* DelayAction::Clone() const
    {
        return new DelayAction(*this);
    }

    std::string DelayAction::ToString() const
    {
        return "DelayAction"s;
    }

    bool DelayAction::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    /// <summary>
    /// Update override
    /// Delay by a Duration, then execute actionList
    /// </summary>
    /// <returns></returns>
    bool DelayAction::Update()
    {
        if (ActionFinished || !ActionInited) return false;
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);

        // If Elapsed time passed duration, start executing actions
        if (elapsed > std::chrono::milliseconds(Duration))
        {
            Datum* actionList = Find("ActionList"s);
            FIEA_ASSERT(actionList != nullptr);
            if (actionList != nullptr)
            {
                for (size_t i = 0; i < actionList->Size(); ++i)
                {
                    Action* curAction = reinterpret_cast<Action*>(actionList->GetPointer<Scope*>(i));
                    if (!curAction->Init()) continue;
                    bool updated = curAction->Update();
                    if (updated) curAction->CleanUp();
                }
            }
            /// After executing, increase repeatCount, reset startTime timer
            RepeatCount += 1;
            startTime = std::chrono::system_clock::now();
            return false;
        }
        return false;
    }

    /// <summary>
    /// Init the timer of current Action
    /// </summary>
    /// <returns></returns>
    bool DelayAction::Init()
    {
        ActionFinished = false;
        if (!ActionInited)
        {
            startTime = std::chrono::system_clock::now();
            ActionInited = true;
        }
        return ActionInited;
    }

    bool DelayAction::CleanUp()
    {
        ActionFinished = true;
        return true;
    }
    /// <summary>
    /// Add action to ActionList Datum table
    /// </summary>
    /// <param name="action"></param>
    void DelayAction::AddAction(Action& action)
    {
        Datum* actionDatum = Find("ActionList"s);
        FIEA_ASSERT(actionDatum != nullptr);
        if (actionDatum != nullptr)
        {
            this->Adopt("ActionList", action);
        }
    }
    /// <summary>
    /// Set up duration for DelayAction
    /// </summary>
    /// <param name="duration"></param>
    void DelayAction::SetUp(int32_t duration)
    {
        Duration = duration;
    }
    /// <summary>
    /// ClassDefinite includes 
    /// ClassAttribute ActionList
    /// </summary>
    /// <returns></returns>
    DelayAction::ClassDef DelayAction::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "ActionList"s });

        return ExternalData;
    }
}