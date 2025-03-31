#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/Types.h"
#include <queue>
namespace Fiea::Engine
{
    FACTORY_DEFINE_ABSTRACT(Content::Scope);
}
namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Scope);
    FACTORY_DEFINE(Scope);
    /// <summary>
    /// Default constructor
    /// </summary>
    Scope::Scope() : Parent(nullptr)
    {
    }
    /// <summary>
    /// Constructor to reserve space with input value
    /// </summary>
    /// <param name="value"></param>
    Scope::Scope(int32_t value) : Parent(nullptr)
    {
        // Reserve space
        _LookUpVector.reserve(value);
        _DatumTable.reserve(value);
    }
    /// <summary>
    /// Copy constructor for scope
    /// </summary>
    /// <param name=""></param>
    Scope::Scope(const Scope& other)
    {
        Parent = nullptr;
        DeepCopy(other);
    }
    /// <summary>
    /// Assignment operator for scope
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Scope& Scope::operator=(const Scope& rhs)
    {
        Parent = nullptr;
        // Check sameness
        if (*this != rhs)
        {
            Clear();
            DeepCopy(rhs);
        }
        return *this;
    }

    /// <summary>
    /// Deep copy from another scope its Datum
    /// </summary>
    /// <param name="other"></param>
    void Scope::DeepCopy(const Scope& other)
    {
        // Loop through table
        for (auto& i : other._DatumTable)
        {
            string curKey = i.first;
            const Datum& curDt = i.second;
            Datum dt;
            if (curDt._Type == Datum::Type::Table)
            {
                // Set type to init space
                dt.SetType(Datum::Type::Table);
                for (size_t i = 0; i < curDt.Size(); ++i)
                {
                    Scope* sc = (*(curDt.GetPointer<Scope*>(i)))->Clone();
                    dt.PushPointer<Scope*>(sc);
                }
            }
            else {
                // Copy datum and insert
                dt = curDt;
            }
            _DatumTable.insert({ curKey, dt });
            _LookUpVector.push_back(curKey);
        }
    }

    /// <summary>
    /// Move constructor
    /// </summary>
    /// <param name="other"></param>
    Scope::Scope(Scope&& other) noexcept
        : _DatumTable(std::move(other._DatumTable)), _LookUpVector(std::move(other._LookUpVector))
    {
        Parent = nullptr;
        //other.Clear();
        other._DatumTable.clear();
        other._LookUpVector.clear();
    }

    /// <summary>
    /// Move Assignment operator
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    Scope& Scope::operator=(Scope&& other) noexcept
    {
        Parent = nullptr;
        // Check sameness
        if (this != &other)  
        {
            // Clear old self first
            Clear();
            // move containers
            _DatumTable = std::move(other._DatumTable);
            _LookUpVector = std::move(other._LookUpVector);
            // clear other's containers
            other._DatumTable.clear();
            other._LookUpVector.clear();
        }
        return *this;
    }

    /// <summary>
    /// Destructor
    /// </summary>
    Scope::~Scope()
    {
        if (Parent != nullptr)
        {
            // Set orphan
            Scope* discard = Orphan();
        }
        // Destruct objects
        Clear();
    }

    /// <summary>
    /// Double Equal operator for Scope
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    bool Scope::operator==(const Scope& rhs) const
    {
        bool same = true;
        if (Size() != rhs.Size()) return false;
        for (size_t i = 0; i < Size(); ++i)
        {
            // Compare value and key
            if (_LookUpVector[i] != rhs._LookUpVector[i] ||
                _DatumTable.at(_LookUpVector[i]) != rhs._DatumTable.at(_LookUpVector[i]))
            {
                same = false;
                break;
            }
        }
        return same;
    }

    /// <summary>
    /// Not equal operator
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    bool Scope::operator!=(const Scope& rhs) const
    {
        bool same = true;
        if (Size() != rhs.Size()) return true;
        for (size_t i = 0; i < Size(); ++i)
        {
            // Compare value and key
            if (_LookUpVector[i] != rhs._LookUpVector[i] ||
                _DatumTable.at(_LookUpVector[i]) != rhs._DatumTable.at(_LookUpVector[i]))
            {
                same = false;
                break;
            }
        }
        return !same;
    }


    /// <summary>
    /// Return the Datum at the requested key, or nullptr if not found
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    Datum* Scope::Find(const std::string& key)
    {
        if (_DatumTable.find(key) == _DatumTable.end())
        {
            return nullptr;
        }
        return &_DatumTable[key];
    }

    /// <summary>
    /// Const version of look up Datum table with string key
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    const Datum* Scope::Find(const std::string& key) const
    {
        if (_DatumTable.find(key) == _DatumTable.end())
        {
            return nullptr;
        }
        return &_DatumTable.at(key);
    }

    /// <summary>
    /// Return a reference to the Datum at this key. create a new Datum, if one is not found
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    Datum& Scope::Append(const std::string& key)
    {
        // If Datum with key doesn't exist, create new one
        if (_DatumTable.find(key) == _DatumTable.end())
        {
            Datum newDatum;
            _DatumTable.insert({key, std::move(newDatum) });
            _LookUpVector.push_back(key);
        }
        // return Datum stored with key
        return _DatumTable.at(key);
    }

    /// <summary>
    /// Bracket operator with string
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    Datum& Scope::operator[](const std::string& key)
    {
        return Append(key);
    }

    /// <summary>
    /// Bracket operator with string const version
    /// </summary>
    /// <param name="key"></param>
    /// <returns>Datum&</returns>
    const Datum& Scope::operator[](const std::string& key) const
    {
        return _DatumTable.at(key);
    }

    /// <summary>
    /// Bracket operator with size_t
    /// </summary>
    /// <param name="idx"></param>
    /// <returns>Datum&</returns>
    Datum& Scope::operator[](size_t idx)
    {
        FIEA_ASSERT(idx < _LookUpVector.size());
        return _DatumTable.at(_LookUpVector.at(idx));
    }

    /// <summary>
    /// Bracket operator with size_t const version
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    const Datum& Scope::operator[](size_t idx) const
    {
        //FIEA_ASSERT(idx < _LookUpVector.size());
        return _DatumTable.at(_LookUpVector.at(idx));
    }

    /// <summary>
    /// the number of key->Datum pairs currently held
    /// </summary>
    /// <returns></returns>
    size_t Scope::Size() const
    {
        return _LookUpVector.size();
    }

    /// <summary>
    /// get rid of all contained key->Datum pairs, including any necessary destruction
    /// </summary>
    void Scope::Clear()
    {

        for (auto& i : _DatumTable)
        {
            Datum& temp = i.second;
            if (temp.IsType(Datum::Type::Table))
            {
               size_t size = temp.Size();
               // Use while loop and loop backwards to avoid looping and reindexing array
               while (size > 0)
               {
                   --size;
                   Scope* child = temp.GetPointer<Scope*>(size);
                   if (child != nullptr) delete child;
               }
            }
        }
        // Clear containers
        _LookUpVector.clear();
        _DatumTable.clear();
    }

    void Scope::RemoveDatum(const string& key)
    {
        Datum* dt = Find(key);
        if (dt != nullptr)
        {
            // Remove from Datum Table
            _DatumTable.erase(key);
            // Remove from LookUpVector
            auto it = _LookUpVector.end();
            while (it != _LookUpVector.begin()) {
                --it;  // Move to the previous element.
                if (*it == key) {
                    it = _LookUpVector.erase(it);
                }
            }
        }
        dt->~Datum();
    }

    /// <summary>
    /// For each of these methods, use a Table Datum, if one exists. Otherwise, create it
    /// </summary>
    /// <param name="key"></param>
    /// <param name="scope"></param>
    /// <returns></returns>
    Scope& Scope::AppendScope(const std::string& key, Scope* scope)
    {
        // If key found, get the Table Datum and push in scope
        if (Find(key) != nullptr)
        {
            Datum& dt = this->Append(key);
            FIEA_ASSERT(dt.IsType(Datum::Type::Table));
            dt.PushPointer(scope);
        }
        else {
            // Create a Table Datum
            Datum& dt = Append(key);
            FIEA_ASSERT(dt.IsType(Datum::Type::Unknown));
            dt.SetType(Datum::Type::Table);
            dt.PushPointer(scope);
        }
        return *this;
    }

    /// <summary>
    /// Assume parentage of the provided child
    /// </summary>
    /// <param name="key"></param>
    /// <param name="child"></param>
    void Scope::Adopt(const std::string& key, Scope& child)
    {
        // Check is child has Parent, if has, Orphan the child
        if (child.Parent != nullptr)
        {
            Scope* childPtr = child.Orphan();
            FIEA_ASSERT(childPtr == &child);
        }
        // Append scope to children Datum
        this->AppendScope(key, &child);
        //child.Parent = this;
        child.SetParent(*this);
    }

    Scope* Scope::GetParent() const
    {
        return Parent;
    }

    Datum* Content::Scope::Search(const string& key, Scope* curScope)
    {
        while (curScope != nullptr)
        {
            if (curScope->Find(key) != nullptr)
            {
                return curScope->Find(key);
            }
            curScope = curScope->GetParent();
        }
        return nullptr;
    }

    /// <summary>
    /// can assume that the Scope is currently a Root Scope
    /// </summary>
    /// <param name="parent"></param>
    void Scope::SetParent(Scope& parent)
    {
        FIEA_ASSERT(Parent == nullptr);
        FIEA_ASSERT(!parent.IsDescendentOf(*this));
        // Add self to Parent's child datum
        if (Parent == nullptr && !parent.IsDescendentOf(*this)) {
            Parent = &parent;
        }
    }

    /// <summary>
    /// remove parentage and return the pointer to the caller, since they will now own this Root Scope
    /// </summary>
    /// <returns></returns>
    Scope* Scope::Orphan()
    {
        if (Parent != nullptr)
        {
            // Current Parent's children
            size_t idx = 0;
            Datum* children = Parent->FindContainedScope(*this, idx);
            children->RemoveAt(idx);
            Parent = nullptr;
        }
        return this;
    }

    /// <summary>
    /// search (non-recursive) for a nested Scope which is the SAME (not just equal)
    ///  as the provided scope and return the Datum holding it. 
    /// Populate idx with the index within the Datum
    /// </summary>
    /// <param name="child"></param>
    /// <param name="idx"></param>
    /// <returns></returns>
    Datum* Scope::FindContainedScope(const Scope& child, size_t& idx)
    {
        // BFS iterative approach using queue
        std::queue<Scope*> dtQueue;
        dtQueue.push(this);
        while (!dtQueue.empty())
        {
            Scope* current = dtQueue.front();
            dtQueue.pop();
            for (auto& i : _DatumTable) {
                if (i.second.IsType(Datum::Type::Table)) {
                    Datum* children = &(i.second);
                    for (size_t i = 0; i < children->Size(); ++i)
                    {
                        // If found child in Datum
                        if (children->GetPointer<Scope*>(i) == &child) {
                            idx = i;
                            return children;
                        }
                        // Push in all children
                        dtQueue.push(children->GetPointer<Scope*>(i));
                    }
                }
            }
            
        }
        return nullptr;
    }

    /// <summary>
    /// Const version of FindContainedScope
    /// </summary>
    /// <param name="child"></param>
    /// <param name="idx"></param>
    /// <returns></returns>
    const Datum* Scope::FindContainedScope(const Scope& child, size_t& idx) const
    {
        std::queue<const Scope*> dtQueue;
        dtQueue.push(this);
        while (!dtQueue.empty())
        {
            const Scope* current = dtQueue.front();
            dtQueue.pop();
            for (const auto& i : _DatumTable) {
                if (i.second._Type == Datum::Type::Table) {
                    const Datum* children = &(i.second);
                    for (size_t j = 0; j< children->Size(); ++j)
                    {
                        // If found child in Datum
                        if (*(children->GetPointer<Scope*>(j)) == &child) {
                            idx = j;
                            return children;
                        }
                        // Push in all children
                        Scope* childScope = *(children->GetPointer<Scope*>(j));
                        if(childScope != nullptr && childScope != this)  dtQueue.push(childScope);
                    }
                }

            }

        }
        return nullptr;
    }

    /// <summary>
    /// check if current scope is ancestor of another scope
    /// </summary>
    /// <param name="descendent"></param>
    /// <returns></returns>
    bool Scope::IsAncestorOf(const Scope& descendent) const
    {
        if (descendent.IsDescendentOf(*this)) return true;
        return false;
    }
    
    /// <summary>
    /// check if current scope is descendent of another scope
    /// </summary>
    /// <param name="ancestor"></param>
    /// <returns></returns>
    bool Scope::IsDescendentOf(const Scope& ancestor) const
    {
        Scope* temp = this->Parent;
        while (temp != nullptr)
        {
            if (*temp == ancestor) return true;
            temp = temp->Parent;
        }
        return false;
    }
    /// <summary>
    /// Get key at index in LookUpVector
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    string Scope::GetKey(size_t idx)
    {
        return _LookUpVector[idx];
    }
}
