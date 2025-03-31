#include "FieaGameEngine/Actions/TimedAction.h"

namespace Fiea::Engine::Actions
{
    RTTI_DEFINITIONS(TimedAction);
    using namespace std::string_literals;
    /// <summary>
    /// Class attribute ActionList
    /// </summary>
    /// <returns></returns>
    TimedAction::ClassDef TimedAction::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ 0, Datum::Type::Table, 0, "ActionList"s });

        return ExternalData;
    }
    /// <summary>
    /// Standard ActionList add action
    /// Add action to ActionList Datum
    /// </summary>
    /// <param name="action"></param>
    void TimedAction::AddAction(Action& action)
    {
        Datum* actionDatum = Find("ActionList"s);
        FIEA_ASSERT(actionDatum != nullptr);
        if (actionDatum != nullptr)
        {
            this->Adopt("ActionList", action);
        }
    }

    void TimedAction::SetUp(int32_t duration)
    {
        Duration = duration;
    }

    TimedAction* TimedAction::Clone() const
    {
        return new TimedAction(*this);
    }

    std::string TimedAction::ToString() const
    {
        return "TimedAction"s;
    }

    bool TimedAction::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }
    /// <summary>
    /// Update override
    /// Run actionList until certain Duration have passed
    /// </summary>
    /// <returns></returns>
    bool TimedAction::Update()
    {
        if (ActionFinished) return false;
        // Start timer inside Update if Action not Inited 
        if (!ActionInited)
        {
            startTime = std::chrono::system_clock::now();
            ActionInited = true;
        }

        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        //auto elapsed = (std::chrono::milliseconds)0;

        if (elapsed < std::chrono::milliseconds(Duration))
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
                    
                }
            }
            return false;
        }
        // Update ActionList until Duration have passed
        return true;
    }

    bool TimedAction::Init()
    {
        ActionFinished = false;
        
        return true;
    }
    /// <summary>
    /// Properly reset state for reruns
    /// </summary>
    /// <returns></returns>
    bool TimedAction::CleanUp()
    {
        ActionFinished = true;
        ActionInited = false;
        return true;
    }
}
