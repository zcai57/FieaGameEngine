#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/RTTI.h"
#include "FieaGameEngine/FactoryService.h"

namespace Fiea::Engine::Tests::Support
{
    class ScopedFoo final : public Fiea::Engine::Content::Scope
    {
        RTTI_DECLARATIONS(ScopedFoo, Scope);
    public:
        explicit ScopedFoo(int32_t data = 0);
        ScopedFoo(const ScopedFoo& rhs);
        ScopedFoo& operator=(const ScopedFoo& rhs);
        virtual ~ScopedFoo() override;
        [[nodiscard]] virtual Scope* Clone() const override {
            return new ScopedFoo(*this);
        }

        bool operator==(const ScopedFoo& rhs) const;
        bool operator!=(const ScopedFoo& rhs) const;

        std::int32_t Data() const;
        void SetData(int32_t data);

    private:
        int32_t* _Data;
    };
    FACTORY_DECLARE(ScopedFoo, Fiea::Engine::Content::Scope);
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::ScopedFoo;

    template<>
    inline std::wstring ToString<ScopedFoo>(const ScopedFoo& t)
    {
        RETURN_WIDE_STRING("Foo:" << t.Data());
    }

    template<>
    inline std::wstring ToString<ScopedFoo>(const ScopedFoo* t)
    {
        RETURN_WIDE_STRING("Foo:" << t);
    }

    template<>
    inline std::wstring ToString<ScopedFoo>(ScopedFoo* t)
    {
        RETURN_WIDE_STRING("Foo:" << t);
    }
}