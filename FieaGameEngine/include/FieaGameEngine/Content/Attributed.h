#pragma once
#include "Scope.h"
#include "Datum.h"
#include <functional>

namespace Fiea::Engine::Content
{
    class Attributed : public Scope
    {
        RTTI_DECLARATIONS(Attributed, Scope);
    public:
        struct Signature
        {
            size_t offset;
            Datum::Type type;
            size_t size;
            string name;
        };
        struct ClassDef
        {
            std::vector<Signature> Signatures;
        };
        Attributed() = delete;
        Attributed(Attributed::ClassDef(*getDef)(), RTTI::IdType id);
        Attributed(const Attributed& rhs);
        Attributed(Attributed&& rhs) noexcept;
        Attributed& operator=(const Attributed& rhs);
        Attributed& operator=(Attributed&& rhs) noexcept;
        virtual ~Attributed() = default;

        // Given a string, return a bool indicating whether it is the name of an attribute for this object.
        bool IsAttribute(const string); 
        // Given a string, return a bool indicating whether it is the name of a class attribute for this object.
        bool IsClassAttribute(const string); 
        // Given a string, return a bool indicating whether it is the name of an instance
        // attribute for this object. Note, you might find it easier to implement IsAttribute
        // and IsClassAttribute, and then implement IsInstanceAttribute using those two.
        bool IsInstanceAttribute(const string); 
        // Given a string (which must not be the name of a class attribute),
        // append a Datum to this Scope with the given name, and 
        // return a reference to that Datum. If the Datum already existed, return it.
        // This should be a thin wrapper around Scope::Append, 
        // with only checks for accidentally appending a name which is a class attribute.
        Datum& AppendInstanceAttribute(const string); 
        // these methods might return Vector objects (or references) 
        // to identify the full lists of attributes
        std::vector<string> Attributes();
        // class attributes and isntance attributes of the object/class. Carefully
        std::vector<string> ClassAttributes();
        // consider the correct data type and constness for the return values of these methods
        std::vector<string> InstanceAttributes(); 
    private:
        const ClassDef* _classDef;
        void Retarget(bool copy);
    };
}


