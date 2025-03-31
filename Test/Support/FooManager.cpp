#include "FooManager.h"
#include "Foo.h"
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine::Test::Support
{
    /**
     * bool Registration()
     * Register IFoomaker and IFooTaker
     * \return true
     */
    FIEA_SERVICE_BIND2(FooManager, IFooMaker, IFooTaker);
    /**
     * Create FooManager
     * \return FooManager address
     */
    void* FooManager::CreateFooMgr()
    {
        return new FooManager();
    }
    /**
     * Destroy FooManager
     * \param instance
     */
    void FooManager::DestroyFooMgr(void* instance)
    {
        delete reinterpret_cast<FooManager*>(instance);
    }
    /**
     * Provide FooMaker interface
     * \param instance
     * \return FooMaker interface address
     */
    void* FooManager::ProvideFooMaker(void* instance) {
        FooManager* FooMgr = reinterpret_cast<FooManager*>(instance);
        return static_cast<IFooMaker*>(FooMgr);
    }
    /**
     * Provide FooTaker interface
     * \param instance
     * \return FooTaker interface address
     */
    void* FooManager::ProvideFooTaker(void* instance) {
        FooManager* FooMgr = reinterpret_cast<FooManager*>(instance);
        return static_cast<IFooTaker*>(FooMgr);
    }
    /**
     * FooMaker make
     * \param name
     * \param value
     */
    void FooManager::Make(const char* name, int32_t value)
    {
        _FooName = name;
        _FooInt = value;
    }
    /**
     * FooMaker take
     * \param name
     * \return stored int value
     */
    int32_t FooManager::Take(const char* name)
    {
        return _FooInt;
    }
    /**
     * FooMaker ServiceProvider
     * \return 
     */
    ServiceProvider GetFooMakerProvider()
    {
        return { FooManager::CreateFooMgr, FooManager::DestroyFooMgr, FooManager::ProvideFooMaker};
    }
    /**
     * FooTaker ServiceProvider
     * \return 
     */
    ServiceProvider GetFooTakerProvider()
    {
        return { FooManager::CreateFooMgr, FooManager::DestroyFooMgr, FooManager::ProvideFooTaker };
    }
}