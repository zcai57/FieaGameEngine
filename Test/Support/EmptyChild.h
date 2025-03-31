#pragma once
#include "Support/Empty.h"
namespace Fiea::Engine::Tests::Support {
    class EmptyChild : public Empty
    {
        RTTI_DECLARATIONS(EmptyChild, Empty);
    public:
        EmptyChild(int32_t value);
        virtual int32_t getValue() override;
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;
    private:
        int32_t _value;
    };
}

