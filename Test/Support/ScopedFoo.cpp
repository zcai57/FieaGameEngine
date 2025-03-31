#include "ScopedFoo.h"
namespace Fiea::Engine
{
    FACTORY_DEFINE_ABSTRACT(Tests::Support::ScopedFoo);
}

namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(ScopedFoo);
    FACTORY_DEFINE(ScopedFoo);
    ScopedFoo::ScopedFoo(int32_t data) : _Data(new int32_t(data))
    {
    }
    // Initializer is better cuz in bg, this is doing
    // Foo::Foo(const Foo& rhs) : _Data(0)
    // The rule of three
    ScopedFoo::ScopedFoo(const ScopedFoo& rhs) : _Data(new int32_t(*rhs._Data))
    {
        //_Data = new int32_t(rhs.Data());
    };
    ScopedFoo& ScopedFoo::operator=(const ScopedFoo& rhs)
    {
        if (this != &rhs) {
            *_Data = *rhs._Data;
        }

        return *this;
    }

    ScopedFoo::~ScopedFoo()
    {
        delete _Data;
        //delete this;
    };
    bool ScopedFoo::operator==(const ScopedFoo& rhs) const
    {
        return *_Data == *rhs._Data;
    }

    bool ScopedFoo::operator!=(const ScopedFoo& rhs) const
    {
        return !operator==(rhs);
    }

    /// @brief Retrieves a copy of the data held by this foo
    /// @return A copy of the data
    int32_t ScopedFoo::Data() const
    {
        return *_Data;
    }

    /// @brief Sets the data within this Foo
    /// @param data The data to set
    void ScopedFoo::SetData(int32_t data)
    {
        *_Data = data;
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
