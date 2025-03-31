#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/RTTI.h"

namespace Fiea::Engine::Tests::Support
{
    class Foo final : public RTTI
    {
        RTTI_DECLARATIONS(Foo, RTTI);
    public:
        explicit Foo(int32_t data = 0);
        Foo(const Foo& rhs);
        Foo& operator=(const Foo& rhs);
        virtual ~Foo();

        bool operator==(const Foo& rhs) const;
        bool operator!=(const Foo& rhs) const;

        std::int32_t Data() const;
        void SetData(int32_t data);

        virtual std::string ToString() const override;
    private:
        int32_t* _Data;
    };
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::Foo;

    template<>
    inline std::wstring ToString<Foo>(const Foo& t)
    {
        RETURN_WIDE_STRING("Foo:" << t.Data());
    }

    template<>
    inline std::wstring ToString<Foo>(const Foo* t)
    {
        RETURN_WIDE_STRING("Foo:" << t);
    }

    template<>
    inline std::wstring ToString<Foo>(Foo* t)
    {
        RETURN_WIDE_STRING("Foo:" << t);
    }
}