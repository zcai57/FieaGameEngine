#pragma once
#include "FieaGameEngine/Action.h"
#include <chrono>
namespace Fiea::Engine::Tests::Support {
    class testAction : public Action
    {
        RTTI_DECLARATIONS(testAction, Action);
    public:
        testAction() : Action(&ClassDefinitions, testAction::TypeIdClass()) {};

        [[nodiscard]] Action* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        virtual bool Update() override;
        virtual bool Init() override;
        virtual bool CleanUp() override;
        string Key;
        string Value;
        int32_t Duration;
    protected:
        static ClassDef ClassDefinitions();
    private:
        //std::chrono::system_clock::time_point startTime;
    };
}


