#pragma once
#include "AttributeBox.h"

namespace Fiea::Engine::Tests::Support {
    class AttributeBoxPlus : public AttributeBox
    {
        RTTI_DECLARATIONS(AttributeBoxPlus, AttributeBox);
    public:
        AttributeBoxPlus() : AttributeBox(&ClassDefinitions, AttributeBoxPlus::TypeIdClass()) {};
        AttributeBoxPlus(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus(AttributeBoxPlus&& rhs) noexcept = default;
        AttributeBoxPlus& operator=(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus& operator=(AttributeBoxPlus&& rhs) noexcept = default;
        virtual ~AttributeBoxPlus() = default;// Scope overrides

        [[nodiscard]] AttributeBoxPlus* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;
        string ExtraString;
    private:
        inline static ClassDef ExternalData;
        static Attributed::ClassDef ClassDefinitions();
    };
}


