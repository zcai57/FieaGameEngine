#pragma once
#include "FieaGameEngine/RTTI.h"
#include "Datum.h"
#include "FieaGameEngine/FactoryService.h"
namespace Fiea::Engine::Content
{
    /*  All Scope object must have RTTI capabilities(thus Scope must inherit from RTTI)
        Scope contains mappings from string to Datum
        Pairs of string->Datum must be referenceable, by index, in the order they were added
        Scope must be able to hold any number of string->Datum pairs
        Lookups by name or index should all be O(1)
    */
    class Scope : public RTTI
    {
        RTTI_DECLARATIONS(Scope, RTTI);
    public:
        Scope();
        Scope(int32_t value);
        Scope(const Scope&);
        Scope& operator=(const Scope& rhs);
        Scope(Scope&&) noexcept;
        Scope& operator=(Scope&& rhs) noexcept;
        virtual ~Scope();

        
        [[nodiscard]] virtual Scope* Clone() const {
            return new Scope(*this);
        }

        bool operator==(const Scope& rhs) const;
        bool operator!=(const Scope& rhs) const;
        Datum* Find(const std::string& key); // return the Datum at the requested key, or nullptr if not found
        const Datum* Find(const std::string& key) const;
        Datum& Append(const std::string& key); // return a reference to the Datum at this key. create a new Datum, if one is not found
        Datum& operator[](const std::string& key); // syntactic sugar to wrap Append
        const Datum& operator[](const std::string& key) const;
        Datum& operator[](size_t idx); // retrieve a Datum by index. out-of-range requests do not append
        const Datum& operator[](size_t idx) const;
        size_t Size() const; // the number of key->Datum pairs currently held
        void Clear(); // get rid of all contained key->Datum pairs, including any necessary destruction
        void RemoveDatum(const string& key);

        // for each of these methods, use a Table Datum, if one exists. Otherwise, create it
        Scope& AppendScope(const std::string& key, Scope* scope = nullptr); // Add a reference to the provided Scope to a Table-type Datum at 'key'
        void Adopt(const std::string& key, Scope& child); // Assume parentage of the provided child

        Scope* GetParent() const;
        Datum* Search(const string& key, Scope* curScope);
        
        [[nodiscard]] Scope* Orphan(); // remove parentage and return the pointer to the caller, since they will now own this Root Scope

        // search (non-recursive) for a nested Scope which is the SAME (not just equal) as the provided scope and return the Datum holding it.
        // Populate idx with the index within the Datum
        Datum* FindContainedScope(const Scope& child, size_t& idx);
        const Datum* FindContainedScope(const Scope& child, size_t& idx) const;

        // hint: one of these should be easy to implement... is there some way to implement the tougher one by using the easier one?
        bool IsAncestorOf(const Scope& descendent) const;
        bool IsDescendentOf(const Scope& ancestor) const;

    protected:
        // Return key at index
        string GetKey(size_t idx);
    private:
        std::vector<string> _LookUpVector;
        std::unordered_map<string, Datum> _DatumTable;
        Scope* Parent;
        std::string ChildKey;
        void SetParent(Scope& parent); // can assume that the Scope is currently a Root Scope
        void DeepCopy(const Scope& other);
    };

    FACTORY_DECLARE(Scope, Scope);
}

