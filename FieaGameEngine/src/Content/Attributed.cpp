#include "FieaGameEngine/ContentService.h"
namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Attributed);
    /// <summary>
    /// Attribute constructor that take a function pointer.
    /// Used for child construction
    /// </summary>
    /// <param name="getDef"></param>
    /// <param name="id"></param>
    Attributed::Attributed(Attributed::ClassDef(*getDef)(), RTTI::IdType id)
    {
        ITypeRegistrar* typeRegister = ServiceMgr::ProvideInterface<ITypeRegistrar>();
        ITypeProvider* typeProvider = ServiceMgr::ProvideInterface<ITypeProvider>();
        // If no classDef exist, register one
        if (typeProvider->ProvideClassDef(id) == nullptr)
        {
            typeRegister->RegisterClassDef(getDef(), id);
        }
        _classDef = typeProvider->ProvideClassDef(id);
        Retarget(false);
    }

    /// <summary>
    /// Copy Constructor.
    /// </summary>
    /// <param name="rhs"></param>
    Attributed::Attributed(const Attributed& rhs) : _classDef(rhs._classDef), Scope(rhs)
    {   
        // Two way binding
        Retarget(true);
    }
    
    /// <summary>
    /// Move constructor. Bind then set old address to nullptr
    /// </summary>
    /// <param name="rhs"></param>
    Attributed::Attributed(Attributed&& rhs) noexcept : _classDef(rhs._classDef), Scope(std::move(rhs))
    {
        Retarget(true);
        // Set rhs.pointer member field to nullptr
        for (const auto& signature : _classDef->Signatures)
        {

            if (signature.type == Datum::Type::Pointer)
            {
                void* memberAddress = reinterpret_cast<char*>(&rhs) + signature.offset;
                std::memset(memberAddress, 0, sizeof(void*) * signature.size);
            }
        }
    }

    /// <summary>
    /// Assignment operator. Check if assign to self, clear old self, copy and bind
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Attributed& Attributed::operator=(const Attributed& rhs)
    {
        if (&rhs != this)
        {
            // Clear old self;
            this->Clear();
            if (rhs._classDef != nullptr)
            {
                _classDef = rhs._classDef;
            }
            Scope::operator=(rhs);
            Retarget(true);
        }
        return *this;
    }

    /// <summary>
    /// Move assignment. 
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Attributed& Attributed::operator=(Attributed&& rhs) noexcept
    {
        if (&rhs != this) {
            // Clear old self;
            this->Clear();
            if (rhs._classDef != nullptr)
            {
                _classDef = rhs._classDef;
            }
            Scope::operator=(std::move(rhs));
            Retarget(true);
        }
        // Set rhs.pointer member field to nullptr
        for (const auto& signature : _classDef->Signatures)
        {
            if (signature.type == Datum::Type::Pointer)
            {
                void* memberAddress = reinterpret_cast<char*>(&rhs) + signature.offset;
                std::memset(memberAddress, 0, sizeof(void*) * signature.size);
            }
        }
        return *this;
    }

    /// <summary>
    /// Enable two way binding for all _classDef
    /// </summary>
    void Attributed::Retarget(bool copy)
    {
        // Iterate over the ClassDef and perform the binding
        for (auto i : _classDef->Signatures)
        {
            // Internal
            if (i.offset == 0 && i.size == 0)
            {
                if (copy) continue;
                Datum& temp = this->Append(i.name);
                temp.SetType(i.type);
            }
            // External
            else {
                Datum* temp = &this->Append(i.name);
                void* memLocation = reinterpret_cast<char*>(this) + i.offset;
                temp->SetStorageLocation(memLocation, i.type, i.size);
            }
        }
    }

    /// <summary>
    /// Given a string, return a bool indicating whether it is the name of an attribute for this object.
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    bool Attributed::IsAttribute(const string name)
    {
        //if (IsClassAttribute(name)) return false;
        if (this->Find(name) != nullptr) return true;
        return false;
    }

    /// <summary>
    /// Given a string, return a bool indicating whether it is the name of a class attribute for this object.
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    bool Attributed::IsClassAttribute(const string name)
    {
        for (auto i : _classDef->Signatures)
        {
            if (name == i.name) {
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// Given a string, return a bool indicating whether it is the name of an instance
    /// attribute for this object. Note, you might find it easier to implement IsAttribute
    /// and IsClassAttribute, and then implement IsInstanceAttribute using those two.
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    bool Attributed::IsInstanceAttribute(const string name)
    {
        if (IsAttribute(name) && !IsClassAttribute(name)) return true;
        return false;
    }

    /// <summary>
    /// Given a string (which must not be the name of a class attribute),
    /// append a Datum to this Scope with the given name, and 
    /// return a reference to that Datum. If the Datum already existed, return it.
    /// This should be a thin wrapper around Scope::Append, 
    /// with only checks for accidentally appending a name which is a class attribute.
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    Datum& Attributed::AppendInstanceAttribute(const string name)
    {
        if (IsClassAttribute(name)) {
            FIEA_ASSERT(this->Find(name) != nullptr);
            return *this->Find(name);
        }
        return this->Append(name);
    }
    
    /// <summary>
    /// these methods might return Vector objects (or references) 
    /// to identify the full lists of attributes
    /// </summary>
    /// <returns></returns>
    std::vector<string> Attributed::Attributes()
    {
        std::vector<string> attributes;
        for (size_t i = 0; i < this->Size(); ++i)
        {
            string curName = this->GetKey(i);
            if (IsAttribute(curName)) attributes.push_back(curName);
        }
        return attributes;
    }
    
    /// <summary>
    /// class attributes and isntance attributes of the object/class. Carefully
    /// </summary>
    /// <returns></returns>
    std::vector<string> Attributed::ClassAttributes()
    {
        std::vector<string> attributes;
        for (size_t i = 0; i < this->Size(); ++i)
        {
            string curName = this->GetKey(i);
            if (IsClassAttribute(curName)) attributes.push_back(curName);
        }
        return attributes;
    }

    /// <summary>
    /// Return Instance Attributes
    /// </summary>
    /// <returns></returns>
    std::vector<string> Attributed::InstanceAttributes()
    {
        std::vector<string> attributes;
        for (size_t i = 0; i < this->Size(); ++i)
        {
            string curName = this->GetKey(i);
            if (IsInstanceAttribute(curName)) attributes.push_back(curName);
        }
        return attributes;
    }
}
