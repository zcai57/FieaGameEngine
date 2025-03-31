#pragma once
#include "FieaGameEngine/RTTI.h"
namespace Fiea::Engine::Tests::Support {
    class Empty : public RTTI
    {
        RTTI_DECLARATIONS(Empty, RTTI);
    public:
        virtual int32_t getValue();
    };
}

