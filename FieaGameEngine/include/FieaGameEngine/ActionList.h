#pragma once
#include "FieaGameEngine/Content/Attributed.h"
#include "FieaGameEngine/Action.h"
namespace Fiea::Engine
{
    using namespace Content;
    class ActionList : public Attributed
    {
        RTTI_DECLARATIONS(ActionList, Attributed);
    public:
        ActionList() : Attributed(&ClassDefinitions, ActionList::TypeIdClass()) {};
        ActionList(const ActionList& rhs) = default;
        ActionList(ActionList&& rhs) noexcept = default;
        ActionList& operator=(const ActionList& rhs) = default;
        ActionList& operator=(ActionList&& rhs) noexcept = default;


        virtual bool Update();
        [[nodiscard]] ActionList* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;
        void AddAction(Action& action);
    protected:
        ActionList(ActionList ::ClassDef(*getDef)(), RTTI::IdType id) : Attributed(getDef, id) {};
        static ClassDef ClassDefinitions();
        std::chrono::system_clock::time_point startTime;
    private:
    };
}


