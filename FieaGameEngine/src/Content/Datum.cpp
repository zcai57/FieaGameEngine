#include "FieaGameEngine/RTTI.h"
#include "FieaGameEngine/ContentService.h"

namespace Fiea::Engine::Content
{
    /// <summary>
    /// Default constructor
    /// </summary>
    Datum::Datum() : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0) {
        _DatumUnion.Int = nullptr;
        _DatumUnion.Float = nullptr;
        _DatumUnion.String = nullptr;
    }
    bool Datum::operator==(const Datum& other) const
    {
        if (_Type == other._Type && Size() == other.Size() &&
            Capacity() == other.Capacity())
        {
            bool same = true;
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                if (_Type == Int) 
                {
                    if (Get<int>() != other.Get<int>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Float)
                {
                    if (Get<float>() != other.Get<float>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == String)
                {
                    if (Get<string>() != other.Get<string>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Vec4)
                {
                    if (Get<Vec4Type>() != other.Get<Vec4Type>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Mat4x4)
                {
                    if (Get<Mat4x4Type>() != other.Get<Mat4x4Type>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Pointer)
                {
                    if (*GetPointer<RTTI*>() != *other.GetPointer<RTTI*>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Table)
                {   // const version of GetPointer is called, deref twice
                    if (**GetPointer<Scope*>() != **other.GetPointer<Scope*>())
                    {
                        same = false;
                        break;
                    }
                }
            }
            if (same) return true;
        }
        return false;
    }
    bool Datum::operator!=(const Datum& other) const
    {
        if (_Type == other._Type && Size() == other.Size() &&
            Capacity() == other.Capacity())
        {
            bool same = true;
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                if (_Type == Int)
                {
                    if (Get<int>() != other.Get<int>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == Float)
                {
                    if (Get<float>() != other.Get<float>())
                    {
                        same = false;
                        break;
                    }
                }
                else if (_Type == String)
                {
                    if (Get<string>() != other.Get<string>())
                    {
                        same = false;
                        break;
                    }
                }
            }
            if (same) return false;
        }
        return true;
    }

    Datum::Datum(const Datum& other)
    {
        _Type = other._Type;
        if (_Type != Unknown) _Allocated = true;
        _ElementNum = other._ElementNum;
        _ArrayElemCount = other._ArrayElemCount;
        _DatumUnion.Int = nullptr;

        if (_Type == Int)
        {
            _DatumUnion.Int = reinterpret_cast<int32_t*>(::operator new(_ArrayElemCount * sizeof(int32_t)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Int[i] = other._DatumUnion.Int[i];
            }
        }
        else if (_Type == Float)
        {
            _DatumUnion.Float = reinterpret_cast<float*>(::operator new(_ArrayElemCount * sizeof(float)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Float[i] = other._DatumUnion.Float[i];
            }
        }
        else if (_Type == String)
        {
            _DatumUnion.String = reinterpret_cast<string*>(::operator new(_ArrayElemCount * sizeof(string)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.String[i]) string(other._DatumUnion.String[i]);
            }
        }
        else if (_Type == Vec4)
        {
            _DatumUnion.Vec = reinterpret_cast<Vec4Type*>(::operator new(_ArrayElemCount * sizeof(Vec4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.Vec[i]) Vec4Type(other._DatumUnion.Vec[i]);
            }
        }
        else if (_Type == Mat4x4)
        {
            _DatumUnion.Mat = reinterpret_cast<Mat4x4Type*>(::operator new(_ArrayElemCount * sizeof(Mat4x4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.Mat[i]) Mat4x4Type(other._DatumUnion.Mat[i]);
            }
        }
        else if (_Type == Pointer)
        {
            _DatumUnion.Pointer = reinterpret_cast<RTTI**>(::operator new(_ArrayElemCount * sizeof(RTTI)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Pointer[i] = other._DatumUnion.Pointer[i];
            }
        }
        else if (_Type == Table)
        {
            _DatumUnion.Table = reinterpret_cast<Scope**>(::operator new(_ArrayElemCount * sizeof(Scope)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Table[i] = other._DatumUnion.Table[i];
            }
        }
       
    }

    Datum& Datum::operator=(const Datum& other)
    {
        if (this != &other) {
            // Free existing memory before taking ownership
            FreeDatum();
        }
        else {
            return *this;
        }

        _Type = other._Type;
        if (_Type != Unknown) _Allocated = true;
        _ElementNum = other._ElementNum;
        _ArrayElemCount = other._ArrayElemCount;
        _DatumUnion.Int = nullptr;
        if (_Type == Int)
        {
            _DatumUnion.Int = reinterpret_cast<int32_t*>(::operator new(_ArrayElemCount * sizeof(int32_t)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Int[i] = other._DatumUnion.Int[i];
            }
        }
        else if (_Type == Float)
        {
            _DatumUnion.Float = reinterpret_cast<float*>(::operator new(_ArrayElemCount * sizeof(float)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Float[i] = other._DatumUnion.Float[i];
            }
        }
        else if (_Type == String)
        {
            _DatumUnion.String = reinterpret_cast<string*>(::operator new(_ArrayElemCount * sizeof(string)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.String[i]) string(other._DatumUnion.String[i]);
            }
        }
        else if (_Type == Vec4)
        {
            _DatumUnion.Vec = reinterpret_cast<Vec4Type*>(::operator new(_ArrayElemCount * sizeof(Vec4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.Vec[i]) Vec4Type(other._DatumUnion.Vec[i]);
            }
        }
        else if (_Type == Mat4x4)
        {
            _DatumUnion.Mat = reinterpret_cast<Mat4x4Type*>(::operator new(_ArrayElemCount * sizeof(Mat4x4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                //_DatumUnion.String[i] = other._DatumUnion.String[i];
                new (&_DatumUnion.Mat[i]) Mat4x4Type(other._DatumUnion.Mat[i]);
            }
        }
        else if (_Type == Pointer)
        {
            _DatumUnion.Pointer = reinterpret_cast<RTTI**>(::operator new(_ArrayElemCount * sizeof(RTTI)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Pointer[i] = other._DatumUnion.Pointer[i];
            }
        }
        else if (_Type == Table)
        {
            _DatumUnion.Table = reinterpret_cast<Scope**>(::operator new(_ArrayElemCount * sizeof(Scope)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.Table[i] = other._DatumUnion.Table[i];
            }
        }
        return *this;
    }

    /// <summary>
    /// Move semantic assign
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(Datum&& other) noexcept
    {
        // Taking ownership of other
        _Type = other._Type;
        if (_Type != Unknown) _Allocated = true;
        _ElementNum = other._ElementNum;
        _ArrayElemCount = other._ArrayElemCount;
        _DatumUnion.Int = nullptr;
        if (_Type == Int)
        {
            _DatumUnion.Int = other._DatumUnion.Int;
            other._DatumUnion.Int = nullptr;
        }
        else if (_Type == Float)
        {
            _DatumUnion.Float = other._DatumUnion.Float;
            other._DatumUnion.Float = nullptr;
        }
        else if (_Type == String)
        {
            _DatumUnion.String = other._DatumUnion.String;
            other._DatumUnion.String = nullptr;
        }
        else if (_Type == Vec4)
        {
            _DatumUnion.Vec = other._DatumUnion.Vec;
            other._DatumUnion.Vec = nullptr;
        }
        else if (_Type == Mat4x4)
        {
            _DatumUnion.Mat = other._DatumUnion.Mat;
            other._DatumUnion.Mat = nullptr;
        }
        else if (_Type == Pointer)
        {
            _DatumUnion.Pointer = other._DatumUnion.Pointer;
            other._DatumUnion.Pointer = nullptr;
        } 
        else if (_Type == Table)
        {
            _DatumUnion.Table = other._DatumUnion.Table;
            other._DatumUnion.Table = nullptr;
        }
        other._ElementNum = 0;
        other._ArrayElemCount = 0;
        other.SetType(Unknown);
        other._Allocated = false;
        other._IsExternal = false;
    }

    /// <summary>
    /// Copy constructor string
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(const string other) : _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(String);
        Push(other);
    }

    /// <summary>
    /// Copy constructor single int
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(const int32_t other): _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Int);
        Push(other);
    }
    
    /// <summary>
    /// Copy constructor single float
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(const float other): _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Float);
        Push(other);
    }

    Datum::Datum(const Vec4Type other): _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Vec4);
        Push(other);
    }

    Datum::Datum(const Mat4x4Type other): _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Mat4x4);
        Push(other);
    }

    /// <summary>
    /// init Datum and copy 1 RTTI*
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(RTTI* other) : _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Pointer);
        PushPointer(other);
    }

    /// <summary>
    /// init Datum and copy 1 Scope*
    /// </summary>
    /// <param name="other"></param>
    Datum::Datum(Scope* other) : _ElementNum(size_t(0)), _ArrayElemCount(size_t(0))
    {
        SetType(Table);
        PushPointer(other);
    }

    /// <summary>
    /// Internal helper free/deallocate
    /// </summary>
    void Datum::FreeDatum()
    {
        Clear();
        if (_Type == Type::Int && _DatumUnion.Int != nullptr) free(_DatumUnion.Int);
        if (_Type == Type::Float && _DatumUnion.Float != nullptr) free(_DatumUnion.Float);
        if (_Type == Type::String && _DatumUnion.String != nullptr) ::operator delete(_DatumUnion.String);
        if (_Type == Type::Vec4 && _DatumUnion.Vec != nullptr) ::operator delete(_DatumUnion.Vec);
        if (_Type == Type::Mat4x4 && _DatumUnion.Mat != nullptr) ::operator delete(_DatumUnion.Mat);
        if (_Type == Type::Pointer && _DatumUnion.Pointer != nullptr) ::operator delete(_DatumUnion.Pointer);
        if (_Type == Type::Table && _DatumUnion.Table != nullptr) ::operator delete(_DatumUnion.Table);
        //if (_Type == Type::String && _DatumUnion.String != nullptr) free(_DatumUnion.String);
    }
    /// <summary>
    /// Remove element at a specific index. Only used in Scope so Table type only.
    /// </summary>
    /// <param name="index"></param>
    void Datum::RemoveAt(size_t index)
    {
        if (index >= _ElementNum) {
            return; 
        }
        FIEA_ASSERT(_Type == Datum::Type::Table);
        _DatumUnion.Table[index] = nullptr;
        for (size_t i = index; i < _ElementNum - 1; ++i)
        {
            _DatumUnion.Table[i] = _DatumUnion.Table[i + 1];
        }
        _ElementNum -= 1;
    }

    /// <summary>
    /// Destructor
    /// </summary>
    Datum::~Datum()
    {
        FreeDatum();
    }

    /// <summary>
    /// Move semantics assign
    /// </summary>
    /// <param name="other"></param>
    /// <returns>Datum&</returns>
    Datum& Datum::operator=(Datum&& other) noexcept
    {
        // Self-assignment check
        if (this != &other) {
            // Free existing memory before taking ownership
            FreeDatum();
        }
        else {
            return *this;
        }
        // Taking ownership
        _Type = other._Type;
        if (_Type != Unknown) _Allocated = true;
        _ElementNum = other._ElementNum;
        _ArrayElemCount = other._ArrayElemCount;
        _DatumUnion.Int = nullptr;

        if (_Type == Int)
        {
            _DatumUnion.Int = other._DatumUnion.Int;
            other._DatumUnion.Int = nullptr;
        }
        else if (_Type == Float)
        {
            _DatumUnion.Float = other._DatumUnion.Float;
            other._DatumUnion.Float = nullptr;
        }
        else if (_Type == String)
        {
            _DatumUnion.String = other._DatumUnion.String;
            other._DatumUnion.String = nullptr;
        }
        else if (_Type == Vec4)
        {
            _DatumUnion.Vec = other._DatumUnion.Vec;
            other._DatumUnion.Vec = nullptr;
        }
        else if (_Type == Mat4x4)
        {
            _DatumUnion.Mat = other._DatumUnion.Mat;
            other._DatumUnion.Mat = nullptr;
        }
        else if (_Type == Pointer)
        {
            _DatumUnion.Pointer = other._DatumUnion.Pointer;
            other._DatumUnion.Pointer = nullptr;
        }
        else if (_Type == Table)
        {
            _DatumUnion.Table = other._DatumUnion.Table;
            other._DatumUnion.Table = nullptr;
        }
        other._ElementNum = 0;
        other._ArrayElemCount = 0;
        other.SetType(Unknown);
        other._Allocated = false;
        other._IsExternal = false;
        return *this;
    }
    
    /// <summary>
    /// Assign to int32_t
    /// </summary>
    /// <param name="other"></param>
    /// <returns>Datum&</returns>
    Datum& Datum::operator=(const int32_t other)
    {
        // Clear old self
        FreeDatum();
        _Type = Type::Int;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Int = reinterpret_cast<int32_t*>(::operator new(sizeof(int32_t)));
        Push(other);
        return *this;
    }

    /// <summary>
    /// Assign to float
    /// </summary>
    /// <param name="other"></param>
    /// <returns>Datum&</returns>
    Datum& Datum::operator=(const float other)
    {
        // Clear old self;
        FreeDatum();
        _Type = Type::Float;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Float = reinterpret_cast<float*>(::operator new(sizeof(float)));
        Push(other);
        return *this;
    }

    /// <summary>
    /// Assign to string
    /// </summary>
    /// <param name="other"></param>
    /// <returns>Datum&</returns>
    Datum& Datum::operator=(const string other)
    {
        // Clear old self;
        FreeDatum();
        _Type = Type::String;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.String = reinterpret_cast<string*>(::operator new(sizeof(string)));
        Push(other);
        return *this;
    }

    Datum& Datum::operator=(const Vec4Type other)
    {
        FreeDatum();
        _Type = Type::Vec4;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Vec = reinterpret_cast<Vec4Type*>(::operator new(sizeof(Vec4Type)));
        Push(other);
        return *this;
    }

    Datum& Datum::operator=(const Mat4x4Type other)
    {
        FreeDatum();
        _Type = Type::Mat4x4;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Mat = reinterpret_cast<Mat4x4Type*>(::operator new(sizeof(Mat4x4Type)));
        Push(other);
        return *this;
    }

    Datum& Datum::operator=(RTTI* other)
    {
        // Clear old self;
        FreeDatum();
        _Type = Type::Pointer;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Pointer = reinterpret_cast<RTTI**>(::operator new(sizeof(RTTI*)));
        PushPointer(other);
        return *this;
    }

    Datum& Datum::operator=(Scope* other)
    {
        // Clear old self;
        FreeDatum();
        _Type = Type::Table;
        _ElementNum = 0;
        _ArrayElemCount = defaultSize;
        _DatumUnion.Table = reinterpret_cast<Scope**>(::operator new(sizeof(Scope*)));
        PushPointer(other);
        return *this;
    }
    
    
    /// <summary>
    /// Set type to not unknown and allocate memory
    /// </summary>
    /// <param name="tp"></param>
    void Datum::SetType(Type tp)
    {
        // if not Unknown
        if (tp != Unknown) {
            FreeDatum();
            _Allocated = true;
            if (tp == Int) {
                _DatumUnion.Int = reinterpret_cast<int32_t*>(::operator new(defaultSize * sizeof(int32_t)));
            }else if (tp == String)
            {
                _DatumUnion.String = reinterpret_cast<string*>(::operator new(defaultSize * sizeof(string)));
            }else if (tp == Float)
            {
                _DatumUnion.Float = reinterpret_cast<float*>(::operator new(defaultSize * sizeof(float)));
            }
            else if (tp == Pointer)
            {
                _DatumUnion.Pointer = reinterpret_cast<RTTI**>(::operator new(defaultSize * sizeof(RTTI*)));
            }
            else if (tp == Table)
            {
                _DatumUnion.Table = reinterpret_cast<Scope**>(::operator new(defaultSize * sizeof(Scope*)));
            }
            else if (tp == Vec4)
            {
                _DatumUnion.Vec = reinterpret_cast<Vec4Type*>(::operator new(defaultSize * sizeof(Vec4Type)));
            }
            else if (tp == Mat4x4)
            {
                _DatumUnion.Mat = reinterpret_cast<Mat4x4Type*>(::operator new(defaultSize * sizeof(Mat4x4Type)));
            }
        }
        _Type = tp;
        _ArrayElemCount = defaultSize;
    }

    /// <summary>
    /// Check is type
    /// </summary>
    /// <param name="tp"></param>
    /// <returns>bool</returns>
    bool Datum::IsType(Type tp)
    {
        return tp == _Type;
    }

    void Datum::SetStorageLocation(void* data, Type tp, size_t count)
    {
        if (_Allocated) return;
        _IsExternal = true;
        _ElementNum = count;
        _ArrayElemCount = count;
        if (tp == Int)
        {
            _DatumUnion.Int = reinterpret_cast<int32_t*>(data);
        } 
        else if (tp == Float)
        {
            _DatumUnion.Float = reinterpret_cast<float*>(data);
        }
        else if (tp == String)
        {
            _DatumUnion.String = reinterpret_cast<string*>(data);
        }
        else if (tp == Vec4)
        {
            _DatumUnion.Vec = reinterpret_cast<Vec4Type*>(data);
        }
        else if (tp == Mat4x4)
        {
            _DatumUnion.Mat = reinterpret_cast<Mat4x4Type*>(data);
        }
        else if (tp == Pointer)
        {
            _DatumUnion.Pointer = reinterpret_cast<RTTI**>(data);
        }
    }
    
    /// <summary>
    /// safely remove the last item from the datum's storage, including destruction (if necessary)
    /// </summary>
    void Datum::Pop()
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        
        size_t elementSize = Size();
        
        if (_Type == String)
        {
            // destruct allocated string
            _DatumUnion.String[_ElementNum - 1].~string();
        }
        _ElementNum -= 1;
    }

    /// <summary>
    /// safely remove ALL items from the datum's storage
    /// </summary>
    void Datum::Clear()
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == String)
        {
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                _DatumUnion.String[i].~basic_string();
            }
        }
        /*else if (_Type == Vec4)
        {
            
            _DatumUnion.Vec[_ElementNum - 1].~Vec4Type();
        }
        else if (_Type == Mat4x4)
        {
            _DatumUnion.Mat[_ElementNum - 1].~Mat4x4Type();
        }*/
        _ElementNum = 0;
    }

    /// <summary>
    /// The number of actual elements in the datum
    /// </summary>
    /// <returns>size_t</returns>
    size_t Datum::Size() const
    {
        return _ElementNum;
    }

    /// <summary>
    /// the number of elements the datum may store without allocating more storage
    /// </summary>
    /// <returns></returns>
    size_t Datum::Capacity() const
    {
        if (_IsExternal)
        {
            return _ElementNum;
        }
        return _ArrayElemCount - _ElementNum;
    }

}
