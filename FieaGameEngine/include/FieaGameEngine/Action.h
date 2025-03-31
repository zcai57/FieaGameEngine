#pragma once
#include "ContentService.h"
#include <chrono>

namespace Fiea::Engine
{
    using namespace Content;
    class Action : public Attributed
    {
        RTTI_DECLARATIONS(Action, Attributed);
        
    public:
        Action() : Attributed(&ClassDefinitions, Action::TypeIdClass()) {};
        Action(const Action& rhs) = default;
        Action(Action&& rhs) noexcept = default;
        Action& operator=(const Action& rhs) = default;
        Action& operator=(Action&& rhs) noexcept = default;
        virtual ~Action() = default;

        virtual bool Update();

        [[nodiscard]]
        virtual bool Init();
        virtual bool CleanUp();
        // Scope overrides
        [[nodiscard]] Action* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;
    protected:
        Action(Action::ClassDef(*getDef)(), RTTI::IdType id) : Attributed(getDef, id) {};

        static ClassDef ClassDefinitions();
        string _ActionName;
        bool ActionFinished;
        bool ActionInited = false;
        std::chrono::system_clock::time_point startTime;
    private:
    };
    FACTORY_DECLARE(Action, Scope);

}


