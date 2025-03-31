#include "AttributeBoxPlus.h"
namespace Fiea::Engine::Tests::Support {
    using namespace std::string_literals;
    RTTI_DEFINITIONS(AttributeBoxPlus);

    AttributeBoxPlus* AttributeBoxPlus::Clone() const
    {
        return new AttributeBoxPlus(*this);
    }
    std::string AttributeBoxPlus::ToString() const
    {
        return "AttributeBoxPlus"s;
    }
    bool AttributeBoxPlus::Equals(const RTTI* rhs) const
    {
        return AttributeBox::Equals(rhs);;
    }
    /// <summary>
    /// Build ClassDef with AttributeBox's Attribute then self Attribute
    /// </summary>
    /// <returns></returns>
    Attributed::ClassDef AttributeBoxPlus::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData = AttributeBox::ClassDefinitions();
        ExternalData.Signatures.push_back({ offsetof(AttributeBoxPlus, ExtraString), Datum::Type::String, 1, "ExtraString"s });
        return ExternalData;
    }
}
