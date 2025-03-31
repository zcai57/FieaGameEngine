#include "EmptyChild.h"
namespace Fiea::Engine::Tests::Support {
    RTTI_DEFINITIONS(EmptyChild);
    EmptyChild::EmptyChild(int32_t value) : _value(value)
    {
    }

    int32_t EmptyChild::getValue()
    {
        return _value;
    }
    std::string EmptyChild::ToString() const
    {
        return std::to_string(_value);
    }
    bool EmptyChild::Equals(const RTTI* rhs) const
    {
        return this == rhs;
    }
}
