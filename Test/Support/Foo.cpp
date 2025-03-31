#include "Foo.h"

namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(Foo);
    Foo::Foo(int32_t data) : _Data(new int32_t(data))
    {
    }
    // Initializer is better cuz in bg, this is doing
    // Foo::Foo(const Foo& rhs) : _Data(0)
    // The rule of three
    Foo::Foo(const Foo& rhs) : _Data(new int32_t(*rhs._Data))
    {
        //_Data = new int32_t(rhs.Data());
    };
    Foo& Foo::operator = (const Foo& rhs)
    {
        if (this != &rhs) {
            *_Data = *rhs._Data;
        }
        
        return *this;
    }
    
    
    
    Foo::~Foo()
    {
        delete _Data;
        //delete this;
    };
    bool Foo::operator==(const Foo& rhs) const
    {
        return *_Data == *rhs._Data;
    }

    bool Foo::operator!=(const Foo& rhs) const
    {
        return !operator==(rhs);
    }

    /// @brief Retrieves a copy of the data held by this foo
    /// @return A copy of the data
    int32_t Foo::Data() const
    {
        return *_Data;
    }

    /// @brief Sets the data within this Foo
    /// @param data The data to set
    void Foo::SetData(int32_t data)
    {
        *_Data = data;
    }
    std::string Foo::ToString() const
    {
        return std::to_string(*_Data);
    }
    /// <summary>
    /// Helper function
    /// </summary>
    /// <param name="ptr"></param>
    /// <returns></returns>
    static int GetInt(int32_t* ptr) {
        _ASSERT(ptr != nullptr);
        return *ptr;
    }
}