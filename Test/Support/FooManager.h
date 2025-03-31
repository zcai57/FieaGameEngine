#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ServiceMgr.h"
namespace Fiea::Engine::Test::Support
{
    struct IFooMaker 
    {
        FIEA_SERVICE_INTERFACE;
        virtual void Make(const char* name, int32_t value) = 0;
    };

    struct IFooTaker
    {
        FIEA_SERVICE_INTERFACE;
        virtual int32_t Take(const char* name) = 0;
    };

    class FooManager : IFooMaker, IFooTaker
    {
    public:
        virtual void Make(const char* name, int32_t value) override;
        virtual int32_t Take(const char* name) override;
        static void* CreateFooMgr();
        static void DestroyFooMgr(void* ptr);
        static void* ProvideFooMaker(void* instance);
        static void* ProvideFooTaker(void* instance);
    private:
        static bool _Registration();
        static inline bool _registered = _Registration();
        int32_t _FooInt;
        const char* _FooName;
    };
    //Implement the method in FooManager.cpp to return a ServiceProvider which provides an IFooMaker (by value)
    ServiceProvider GetFooMakerProvider();
    // Implement the method in FooManager.cpp to return a ServiceProvider which provides an IFooTaker(by value)
    ServiceProvider GetFooTakerProvider();
}


