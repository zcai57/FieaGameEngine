#include "Support/AttributeBox.h"
#include "FieaGameEngine/ContentService.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace std::string_literals;

    RTTI_DEFINITIONS(AttributeBox);

    bool AttributeBox::CheckSame(const AttributeBox& rhs)
    {
        bool same = true;
        if (ExternalInteger != rhs.ExternalInteger ||
            ExternalFloat != rhs.ExternalFloat ||
            ExternalString != rhs.ExternalString)
        {
            same = false;
        }
        // One is nullptr, the other is not
        if ((ExternalPointer == nullptr) != (rhs.ExternalPointer == nullptr))
        {
            same = false;
        } 
        // Check equality when non null
        else if (ExternalPointer && rhs.ExternalPointer && *ExternalPointer != *rhs.ExternalPointer)
        {
            same = false;
        }
        for (size_t i = 0; i < ARRAY_SIZE; ++i)
        {
            if (ExternalIntegerArray[i] != rhs.ExternalIntegerArray[i] ||
                ExternalFloatArray[i] != rhs.ExternalFloatArray[i] ||
                ExternalStringArray[i] != rhs.ExternalStringArray[i])
            {
                same = false;
            }
            // One is nullptr, the other is not
            if ((ExternalPointerArray[i] == nullptr) != (rhs.ExternalPointerArray[i] == nullptr)) {
                same = false;
            }
            // Check equality when non null
            else if (ExternalPointerArray[i] && rhs.ExternalPointerArray[i] &&
                *ExternalPointerArray[i] != *rhs.ExternalPointerArray[i])
            {
                same = false;
            }
        }
        return same;
    }

    AttributeBox::ClassDef AttributeBox::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalInteger), Datum::Type::Int, 1, "Int"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalFloat), Datum::Type::Float, 1, "Float"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalString), Datum::Type::String, 1, "String"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalPointer), Datum::Type::Pointer, 1, "FooPtr"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalIntegerArray), Datum::Type::Int, ARRAY_SIZE, "IntArr"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalFloatArray), Datum::Type::Float, ARRAY_SIZE, "FloatArr"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalStringArray), Datum::Type::String, ARRAY_SIZE, "StringArr"s });
        ExternalData.Signatures.push_back({ offsetof(AttributeBox, ExternalPointerArray), Datum::Type::Pointer, ARRAY_SIZE, "FooPtrArr"s });
        
        ExternalData.Signatures.push_back({ 0, Datum::Type::Int, (size_t)0, "IntInternal"s });
        
        return ExternalData;
    }

    bool AttributeBox::operator==(const AttributeBox& rhs)
    {
        return CheckSame(rhs);
    }

    bool AttributeBox::operator!=(const AttributeBox& rhs)
    {
        return !CheckSame(rhs);
    }

    // Scope overrides
    [[nodiscard]] AttributeBox* AttributeBox::Clone() const
    {
        return new AttributeBox(*this);
    }

    // RTTI overrides
    std::string AttributeBox::ToString() const
    {
        return "AttributeBox"s;
    }

    bool AttributeBox::Equals(const RTTI* rhs) const
    {
        // this would happen without the override, but included for clarity
        return Scope::Equals(rhs);

        // if AttributeBox had fields that are NOT tracked by Scope, then there would be additional checks here
    }
}