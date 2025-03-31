#pragma once
#include "FieaGameEngine/Types.h"
#include "../../packages/glm.1.0.1/build/native/include/glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../../packages/glm.1.0.1/build/native/include/glm/gtx/string_cast.hpp"

namespace Fiea::Engine {
    class RTTI;
}

namespace Fiea::Engine::Content
{
    using Vec4Type = glm::f32vec4;
    using Mat4x4Type = glm::f32mat4x4;
    class Scope;
    //Unknown: unassigned - has no values, yet
    //Int : signed, 32 - bit integer values
    //Float : single - precision floating point values
    //String : an STL string
    class Datum
    {
    public:
        // Core Implementation
        Datum(); // default ctor, what "type" is it?
        // rule-of-3 methods, if needed
        /*bool operator==(const Datum&) const;
        bool operator!=(const Datum&) const;*/
        ~Datum();
        // Casts & Convenience methods for supported types
        //    - For each, you may choose to write explicit overloads or use templates
        //    - Make implementations for all supported types (int32_t, float, std::string)
        // typecast constructors
        //    - something like Datum d(27); produces a Datum of Int-type which stores the value 27
        // initializer-list constructors for collections of each supported type
        //    - something like Datum d { 1, 2, 3 }; produces a Datum of Int-type which stores those three values
        // operator= for right-hand values of each supported type
        //    - something like d = 1; produces a Datum of Int-type which has a single value of 1 (ONLY IF d was Unknown-type or Int-type!)
        //    - don't forget the case where the Datum already has stuff in it!

        // Datum "types"
        // nested type (why?)... use the typenames from "Supported types" above
        enum Type {
            Unknown,
            Int,
            Float,
            String,
            Pointer,
            Table,
            Vec4,
            Mat4x4,
        }; 
        Datum(const Datum& other);
        Datum& operator=(const Datum& other);
        // Move semantics
        Datum(Datum&& other) noexcept;
        Datum& operator=(Datum&& other) noexcept;
        bool operator==(const Datum&) const;
        bool operator!=(const Datum&) const;
        Datum(const string);
        Datum(const int32_t);
        Datum(const float);
        Datum(const Vec4Type);
        Datum(const Mat4x4Type);
        Datum(RTTI*);
        Datum(Scope*);
        Datum& operator=(const int32_t other);
        Datum& operator=(const float other);
        Datum& operator=(const string other);
        Datum& operator=(const Vec4Type other);
        Datum& operator=(const Mat4x4Type other);
        Datum& operator=(Fiea::Engine::RTTI* other);
        Datum& operator=(Scope* other);

        // string based modifiers - either
        //    - make templated methods, which accept a native type (not a Datum::Type)
        //    - make methods with the Datum::Type in the name, like SetIntFromString
        template<typename T>
        void SetFromString(const string&, size_t idx = 0);
        template<typename T>
        string GetToString(size_t idx = 0) const;
        template<typename T>
        void PushFromString(const string&);

        // Initializer list constructor
        template<typename T>
        Datum(std::initializer_list<T> init_list);

        void SetType(Type); // simple setter
        // we'll offer this, instead of a getter 
        // because the only anticipated operation would be the compare
        bool IsType(Type); 

        // Accessors & modifiers
        //    - In this section, where you see TYPE, you may attempt to use templates for these methods, or you may provide overloads
        template<typename T>
        void Set(const T& val, size_t idx = 0); // just sets a value at an index, may NOT grow the size!
        
        // Set for RTTI* and Scope*, take a pointer as T
        template<typename T>
        void SetPointer(T val, size_t idx = 0);

        // Set external storage for int, float, string types
        template<typename T>
        void SetStorage(T* items, size_t count = 1);

        // Set external storage for RTTI* types
        template<typename T>
        void SetStoragePtr(T** items, size_t count = 1);

        void SetStorageLocation(void* location, Type tp, size_t count = 1);

        template<typename T>
        T& Get(size_t idx = 0); // simple mutable getter. if you are not using templates, use function names GetAs*, where * is a Datum::Type, like GetAsInt
        
        template<typename T>
        T GetPointer(size_t idx = 0); // Getter for RTTI* and Scope*


        template<typename T>
        const T& Get(size_t idx = 0) const; // simple const getter

        template<typename T>
        T const *GetPointer(size_t idx = 0) const; // Getter for RTTI* and Scope*

        template<typename T>
        void Push(const T& val); // pushes a new value to the back on the datum's storage, grows the size and CAN grow the capacity
        
        template<typename T>
        void PushPointer(T val);
        void Pop(); // safely remove the last item from the datum's storage, including destruction (if necessary)
        void Clear(); // safely remove ALL items from the datum's storage
        
        // Additional methods
        size_t Size() const; // the number of actual elements in the datum
        size_t Capacity() const; // the number of elements the datum may store without allocating more storage
        void RemoveAt(size_t index); //Remove element at a specific index. Only used in Scope so Table type only.
 private:
        friend class Scope;
        Type _Type;
        bool _Allocated = false;
        bool _IsExternal = false;
        union _Union
        {
            int32_t* Int;
            float* Float;
            string* String;
            Vec4Type* Vec;
            Mat4x4Type* Mat;
            Fiea::Engine::RTTI** Pointer;
            Scope** Table;
        } _DatumUnion;
        int32_t defaultSize = 5;
        size_t _ElementNum;     // number of actual elements inside
        size_t _ArrayElemCount; // number of allocated space
        void FreeDatum();
        
    };

    
    // simple mutable getter. if you are not using templates, 
    // use function names GetAs*, where * is a Datum::Type, like GetAsInt
    // Get for int32_t
    template <>
    inline int32_t& Datum::Get(size_t idx) 
    {
        return _DatumUnion.Int[idx];
    };

    /// <summary>
    /// Get for float
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline float& Datum::Get(size_t idx) 
    {
        return _DatumUnion.Float[idx];
    };


    /// <summary>
    /// Get for string
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline string& Datum::Get(size_t idx) 
    {
        return _DatumUnion.String[idx];
    };

    /// <summary>
    /// Get for Vec4Type
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline Vec4Type& Datum::Get(size_t idx)
    {
        return _DatumUnion.Vec[idx];
    };

    /// <summary>
    /// Get for Mat4x4Type
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline Mat4x4Type& Datum::Get(size_t idx)
    {
        return _DatumUnion.Mat[idx];
    };

    /// <summary>
    /// Get for RTTI*
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline Fiea::Engine::RTTI* Datum::GetPointer(size_t idx)
    {
        return _DatumUnion.Pointer[idx];
    };

    /// <summary>
    /// Get for Scope*
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline Scope* Datum::GetPointer(size_t idx)
    {
        return _DatumUnion.Table[idx];
    };

    /// <summary>
    /// const Get for int
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline const int32_t& Datum::Get(size_t idx) const
    {
        return _DatumUnion.Int[idx];
    };

    /// <summary>
    /// const Get for float
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline const float& Datum::Get(size_t idx) const
    {
        return _DatumUnion.Float[idx];
    };

    /// <summary>
    /// const Get for string
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline const string& Datum::Get(size_t idx) const
    {
        return _DatumUnion.String[idx];
    };

    /// <summary>
    /// Vec Get
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline const Vec4Type& Datum::Get(size_t idx) const
    {
        return _DatumUnion.Vec[idx];
    };

    /// <summary>
    /// Mat get
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline const Mat4x4Type& Datum::Get(size_t idx) const
    {
        return _DatumUnion.Mat[idx];
    };

    /// <summary>
    /// Get for RTTI*
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline Fiea::Engine::RTTI* const* Datum::GetPointer(size_t idx) const
    {
        return &_DatumUnion.Pointer[idx];
    };

    /// <summary>
    /// Get for Scope*
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template <>
    inline Scope* const* Datum::GetPointer(size_t idx) const
    {
        return &_DatumUnion.Table[idx];
    };
    
    /// <summary>
    /// Set for int
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::Set(const int32_t& val, size_t idx)
    {
        _DatumUnion.Int[idx] = val;
    }

    /// <summary>
    /// Set for float
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::Set(const float& val, size_t idx)
    {
        _DatumUnion.Float[idx] = val;
    }

    /// <summary>
    /// Set for string
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::Set(const string& val, size_t idx)
    {
        _DatumUnion.String[idx] = val;
    }

    /// <summary>
    /// Set for Vec4
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::Set(const Vec4Type& val, size_t idx)
    {
        _DatumUnion.Vec[idx] = val;
    }

    /// <summary>
    /// Set for Mat4
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::Set(const Mat4x4Type& val, size_t idx)
    {
        _DatumUnion.Mat[idx] = val;
    }

    /// <summary>
    /// SetPointer for RTTI*    
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::SetPointer(RTTI* val, size_t idx)
    {
        _DatumUnion.Pointer[idx] = val;
    }

    /// <summary>
    /// SetPointer for Scope*
    /// </summary>
    /// <param name="val"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::SetPointer(Scope* val, size_t idx)
    {
        _DatumUnion.Table[idx] = val;
    }


    /// <summary>
    /// Push for int type
    /// </summary>
    /// <param name="val"></param>
    template<>
    inline void Datum::Push(const int32_t& val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Int);
        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            //_DatumUnion.Int = (int32_t*)realloc(_DatumUnion.Int, sizeof(int32_t) * _ArrayElemCount);
            int32_t* temp = reinterpret_cast<int32_t*>(::operator new(_ArrayElemCount * sizeof(int32_t)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                temp[i] = _DatumUnion.Int[i];
            }
            //Clear();
            ::operator delete(_DatumUnion.Int);
            _DatumUnion.Int = temp;
        }
        _DatumUnion.Int[_ElementNum] = val;
        _ElementNum += 1;
    }

    /// <summary>
    /// Push for int type
    /// </summary>
    /// <param name="val"></param>
    template<>
    inline void Datum::Push(const float& val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Float);
        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            /*_DatumUnion.Float = (float*)realloc(_DatumUnion.Float, sizeof(float) * _ArrayElemCount);*/
            float* temp = reinterpret_cast<float*>(::operator new(_ArrayElemCount * sizeof(float)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                temp[i] = _DatumUnion.Float[i];
            }
            //Clear();
            ::operator delete(_DatumUnion.Float);
            _DatumUnion.Float = temp;
        }
        _DatumUnion.Float[_ElementNum] = val;
        _ElementNum += 1;
    }

    /// <summary>
    /// Push for string type
    /// </summary>
    /// <param name="val"></param>
    template<>
    inline void Datum::Push(const string& val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(String);
        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            //string* temp = new string[_ArrayElemCount];
            
            string* temp = reinterpret_cast<string*>(::operator new(_ArrayElemCount * sizeof(string)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                new (&temp[i]) string(_DatumUnion.String[i]);
                _DatumUnion.String[i].~basic_string();
            }
            //Clear();
            ::operator delete(_DatumUnion.String);
            _DatumUnion.String = temp;
        }
        //_DatumUnion.String[_ElementNum] = val;
        new (&_DatumUnion.String[_ElementNum]) string(val);
        _ElementNum += 1;
    }

    template<>
    inline void Datum::Push(const Vec4Type& val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Vec4);

        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            Vec4Type* temp = reinterpret_cast<Vec4Type*>(::operator new(_ArrayElemCount * sizeof(Vec4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                new (&temp[i]) Vec4Type(_DatumUnion.Vec[i]);
                _DatumUnion.Vec[i].~Vec4Type();
            }
            //Clear();
            ::operator delete(_DatumUnion.Vec);
            _DatumUnion.Vec = temp;
        }
        new (&_DatumUnion.Vec[_ElementNum]) Vec4Type(val);
        _ElementNum += 1;
    }

    template<>
    inline void Datum::Push(const Mat4x4Type& val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Mat4x4);

        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;

            Mat4x4Type* temp = reinterpret_cast<Mat4x4Type*>(::operator new(_ArrayElemCount * sizeof(Mat4x4Type)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                new (&temp[i]) Mat4x4Type(_DatumUnion.Mat[i]);
                _DatumUnion.Mat[i].~Mat4x4Type();
            }
            //Clear();
            ::operator delete(_DatumUnion.Mat);
            _DatumUnion.Mat = temp;
        }
        //_DatumUnion.String[_ElementNum] = val;
        new (&_DatumUnion.Mat[_ElementNum]) Mat4x4Type(val);
        _ElementNum += 1;
    }
    

    /// <summary>
    /// Push for RTTI* type
    /// </summary>
    /// <param name="val"></param>
    template<>
    inline void Datum::PushPointer(Fiea::Engine::RTTI* val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Pointer);
        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            //string* temp = new string[_ArrayElemCount];

            RTTI** temp = reinterpret_cast<RTTI**>(::operator new(_ArrayElemCount * sizeof(RTTI*)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
               temp[i] = _DatumUnion.Pointer[i];
                //_DatumUnion.String[i].~basic_string();
            }
            //Clear();
            ::operator delete(_DatumUnion.Pointer);
            _DatumUnion.Pointer = temp;
        }
        _DatumUnion.Pointer[_ElementNum] = val;
        _ElementNum += 1;
    }

    /// <summary>
    /// Push for Scope* type
    /// </summary>
   /// <param name="val"></param>
    template<>
    inline void Datum::PushPointer(Scope* val)
    {
        if (_IsExternal)
        {
            // throw some exception
            return;
        }
        if (_Type == Unknown) SetType(Table);
        if (Capacity() < 1)
        {
            size_t size = (size_t)0;
            _ArrayElemCount *= 2;
            //string* temp = new string[_ArrayElemCount];

            Scope** temp = reinterpret_cast<Scope**>(::operator new(_ArrayElemCount * sizeof(Scope*)));
            for (size_t i = 0; i < _ElementNum; ++i)
            {
                temp[i] = _DatumUnion.Table[i];
                //_DatumUnion.String[i].~basic_string();
            }
            //Clear();
            ::operator delete(_DatumUnion.Table);
            _DatumUnion.Table = temp;
        }
        _DatumUnion.Table[_ElementNum] = val;
        _ElementNum += 1;
    }
    

    /// <summary>
    /// Datum Initializer list int32_t
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="init_list"></param>
    template<>
    inline Datum::Datum(std::initializer_list<int32_t> init_list) : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Int);
        for (const int32_t& value : init_list) {
            Push(value);
        }
    }

    template<>
    inline Datum::Datum(std::initializer_list<float> init_list): _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Float);
        for (const float& value : init_list) {
            Push(value);
        }
    }


    template<>
    inline Datum::Datum(std::initializer_list<string> init_list): _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(String);
        for (const string& value : init_list) {
            Push(value);
        }
    }

    template<>
    inline Datum::Datum(std::initializer_list<Vec4Type> init_list) : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Vec4);
        for (const Vec4Type& value : init_list) {
            Push(value);
        }
    }

    template<>
    inline Datum::Datum(std::initializer_list<Mat4x4Type> init_list) : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Mat4x4);
        for (const Mat4x4Type& value : init_list) {
            Push(value);
        }
    }


    template<>
    inline Datum::Datum(std::initializer_list<RTTI*> init_list) : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Pointer);
        for (RTTI* value : init_list) {
            PushPointer(value);
        }
    }

    template<>
    inline Datum::Datum(std::initializer_list<Scope*> init_list) : _Type(Unknown), _ElementNum((size_t)0), _ArrayElemCount((size_t)0)
    {
        SetType(Table);
        for (Scope* value : init_list) {
            PushPointer(value);
        }
    }
    
    /// <summary>
    /// Set from string for int32_t
    /// </summary>
    /// <param name="str"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::SetFromString<int32_t>(const string& str, size_t idx)
    {
        int32_t strInt = std::stoi(str);
        Set<int32_t>(strInt, idx);
    }

    /// <summary>
    /// Set from string for float
    /// </summary>
    /// <param name="str"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::SetFromString<float>(const string& str, size_t idx)
    {
        float strFl = std::stof(str);
        Set<float>(strFl, idx);
    }
    /// <summary>
    /// SetFromString Vec4Type
    /// </summary>
    /// <param name="str"></param>
    /// <param name="idx"></param>
    template<>
    inline void Datum::SetFromString<Vec4Type>(const string& str, size_t idx)
    {
        float a, b, c, d;
        int result = sscanf(str.c_str(), "(%f, %f, %f, %f)", &a, &b, &c, &d);
        if (result == 4) {
            Set<Vec4Type>(Vec4Type(a, b, c, d), idx);
        }
        else {
            //throw something 
        }
    }

    template<>
    inline void Datum::SetFromString<Mat4x4Type>(const string& str, size_t idx)
    {
        float f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16;
        int result = sscanf(str.c_str(), "((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
            &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15, &f16);
        if (result == 16)
        {
            Set<Mat4x4Type>(Mat4x4Type(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16), idx);
        }
        else {
            // throw something
        }

    }

    /// <summary>
    /// GetToString for int32_t
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline string Datum::GetToString<int32_t>(size_t idx) const
    {
        return std::to_string(Get<int32_t>(idx));
    }

    /// <summary>
    /// GetToString for float
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline string Datum::GetToString<float>(size_t idx) const
    {
        return std::to_string(Get<float>(idx));
    }
    /// <summary>
    /// GetToString for Vec4
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline string Datum::GetToString<Vec4Type>(size_t idx) const
    {
        return glm::to_string(Get<Vec4Type>(idx));
    }

    /// <summary>
    /// GetToString for Mat4x4
    /// </summary>
    /// <param name="idx"></param>
    /// <returns></returns>
    template<>
    inline string Datum::GetToString<Mat4x4Type>(size_t idx) const
    {
        return glm::to_string(Get<Mat4x4Type>(idx));
    }
    
    /// <summary>
    /// PushFromString for int32_t
    /// </summary>
    /// <param name="str"></param>
    template<>
    inline void Datum::PushFromString<int32_t>(const string& str)
    {
        int32_t strInt = std::stoi(str);
        Push<int32_t>(strInt);
    }

    /// <summary>
    /// PushFromString for float
    /// </summary>
    /// <param name="str"></param>
    template<>
    inline void Datum::PushFromString<float>(const string& str)
    {
        float strFl = std::stof(str);
        Push<float>(strFl);
    }


    /// <summary>
    /// Push from string for Vec4
    /// </summary>
    /// <param name="str"></param>
    template<>
    inline void Datum::PushFromString<Vec4Type>(const string& str)
    {
        float a, b, c, d;
        int result = sscanf(str.c_str(), "(%f, %f, %f, %f)", &a, &b, &c, &d);
        if (result == 4) {
            Push<Vec4Type>(Vec4Type(a, b, c, d));
        }
        else {
            //throw something 
        }
    }

    template<>
    inline void Datum::PushFromString<Mat4x4Type>(const string& str)
    {
        float f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16;
        int result = sscanf(str.c_str(), "((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
            &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15, &f16);
        if (result == 16)
        {
            Push<Mat4x4Type>(Mat4x4Type(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16));
        }
        else {
            // throw something
        }
        
    }

    /// <summary>
    ///  SetStorage may only be called on a Datum that does not already 
    ///  have allocated internal storage (this is to simplify the implementation)
    ///  Enable data two way binding for Datum
    /// </summary>
    /// <param name="items"></param>
    /// <param name="count"></param>
    template<>
    inline void Datum::SetStorage(int32_t* items, size_t count) 
    {
        if (_Allocated) return;
        _IsExternal = true;
        _ElementNum = count;
        _ArrayElemCount = count;
        /*_DatumUnion.Int = reinterpret_cast<int32_t*>(::operator new(count * sizeof(int32_t*)));*/
        _DatumUnion.Int = items;
    }


    /// <summary>
    /// Set storage for float type
    /// </summary>
    /// <param name="items"></param>
    /// <param name="count"></param>
    template<>
    inline void Datum::SetStorage(float* items, size_t count)
    {
        if (_Allocated) return;
        _IsExternal = true;
        _ElementNum = count;
        _ArrayElemCount = count;
        _DatumUnion.Float = items;
    }

    /// <summary>
    /// Set storage for string type
    /// </summary>
    /// <param name="items"></param>
    /// <param name="count"></param>
    template<>
    inline void Datum::SetStorage(string* items, size_t count)
    {
        if (_Allocated) return;
        _IsExternal = true;
        _ElementNum = count;
        _ArrayElemCount = count;
        _DatumUnion.String = items;
    }


    template<>
    inline void Datum::SetStoragePtr(RTTI** items, size_t count)
    {
        if (_Allocated) return;
        _IsExternal = true;
        _ElementNum = count;
        _ArrayElemCount = count;
        _DatumUnion.Pointer = items;
    }
}

