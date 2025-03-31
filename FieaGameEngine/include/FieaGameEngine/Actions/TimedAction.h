#include "FieaGameEngine/Action.h"

namespace Fiea::Engine::Actions
{
    class TimedAction : public Action
    {
        RTTI_DECLARATIONS(TimedAction, Action);
    public:
        TimedAction() : Action(&ClassDefinitions, TimedAction::TypeIdClass()) {};

        [[nodiscard]] TimedAction* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        virtual bool Update() override;
        virtual bool Init() override;
        virtual bool CleanUp() override;
        void AddAction(Action& action);
        void SetUp(int32_t duration);

        //string Key;
        //int32_t Amount;
        int32_t Duration;
    protected:
        static ClassDef ClassDefinitions();
    private:
        //std::chrono::system_clock::time_point startTime;
    };
}

