#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ContentService.h"
#include <functional>
#include "Tests.h"
#include "Support/Foo.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace Content;

    class AttributeBox : public Attributed
    {
        RTTI_DECLARATIONS(AttributeBox, Attributed);

    public:
        static const std::size_t ARRAY_SIZE = 5;
        
        // Rule-of-5 methods
        AttributeBox() : Attributed(&ClassDefinitions, AttributeBox::TypeIdClass()) {};
        AttributeBox(const AttributeBox& rhs) = default;
        AttributeBox(AttributeBox&& rhs) noexcept = default;
        AttributeBox& operator=(const AttributeBox& rhs) = default;
        AttributeBox& operator=(AttributeBox&& rhs) noexcept = default;
        virtual ~AttributeBox() = default;
        bool operator==(const AttributeBox& rhs);
        bool operator!=(const AttributeBox& rhs);

        // Scope overrides
        [[nodiscard]] AttributeBox* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        int32_t ExternalInteger{};
        float ExternalFloat{};
        std::string ExternalString{};
        Foo* ExternalPointer{};

        int32_t ExternalIntegerArray[ARRAY_SIZE]{};
        float ExternalFloatArray[ARRAY_SIZE]{};
        std::string ExternalStringArray[ARRAY_SIZE]{};
        Foo* ExternalPointerArray[ARRAY_SIZE]{};
    protected:
        AttributeBox(Attributed::ClassDef(*getDef)(), RTTI::IdType id) : Attributed(getDef, id) {};
        static ClassDef ClassDefinitions();
    private:
        bool CheckSame(const AttributeBox& rhs);
    };
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::AttributeBox;

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox& t)
    {
        RETURN_WIDE_STRING(t.ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }
}