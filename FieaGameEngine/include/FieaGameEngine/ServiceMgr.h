#pragma once
#include "FieaGameEngine/Types.h"
/**
 * Generate unique interface id.
 */
#define FIEA_SERVICE_INTERFACE                             \
static inline ServiceMgr::InterfaceId GetInterfaceId()     \
{                                                          \
    static char _unused{};                                 \
    return (ServiceMgr::InterfaceId)&_unused;            \
}

/**
 * Bind class with 1 interface
 */
#define FIEA_SERVICE_BIND1(_impl, _intf)                   \
bool _impl::_Registration() {                                                            \
    auto create = []() -> void* { return new _impl(); };                                 \
    auto destroy = [](void* instance) { delete reinterpret_cast<_impl*>(instance); };     \
    auto provide = [](void* instance) -> void* {                                         \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf*>(FooMgr); };                                               \
    ServiceMgr::RegisterInterfaceProvider<_intf>(create, destroy, provide);              \
    return true;                                                                         \
}
/**
 * Bind Class with 2 interfaces
 */
#define FIEA_SERVICE_BIND2(_impl, _intf1, _intf2)                                        \
bool _impl::_Registration() {                                                            \
    auto create = []() -> void* { return new _impl(); };                                 \
    auto destroy = [](void* instance) { delete reinterpret_cast<_impl*>(instance); };     \
    auto provide1 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf1*>(FooMgr); };                                              \
    auto provide2 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf2*>(FooMgr); };                                              \
    ServiceMgr::RegisterInterfaceProvider<_intf1>(create, destroy, provide1);            \
    ServiceMgr::RegisterInterfaceProvider<_intf2>(create, destroy, provide2);            \
    return true;                                                                         \
}
/**
 * Bind Class with 4 interfaces
 */
#define FIEA_SERVICE_BIND4(_impl, _intf1, _intf2, _intf3, _intf4)                          \
bool _impl::_Registration() {                                                            \
    auto create = []() -> void* { return new _impl(); };                                 \
    auto destroy = [](void* instance) { delete reinterpret_cast<_impl*>(instance); };     \
    auto provide1 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf1*>(FooMgr); };                                              \
    auto provide2 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf2*>(FooMgr); };                                              \
    auto provide3 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf3*>(FooMgr); };                                              \
    auto provide4 = [](void* instance) -> void* {                                        \
    _impl* FooMgr = reinterpret_cast<_impl*>(instance);                                  \
    return static_cast<_intf4*>(FooMgr); };                                              \
    ServiceMgr::RegisterInterfaceProvider<_intf1>(create, destroy, provide1);            \
    ServiceMgr::RegisterInterfaceProvider<_intf2>(create, destroy, provide2);            \
    ServiceMgr::RegisterInterfaceProvider<_intf3>(create, destroy, provide3);            \
    ServiceMgr::RegisterInterfaceProvider<_intf4>(create, destroy, provide4);            \
    return true;                                                                       \
}
//ServiceMgr::RegisterInterfaceProvider<_intf3>(create, destroy, \
//    [](void*) -> void* { return ServiceMgr::ProvideInterface<_intf3>(); });              \
//    ServiceMgr::RegisterInterfaceProvider<_intf4>(create, destroy, \
//        [](void*) -> void* { return ServiceMgr::ProvideInterface<_intf4>(); });            \

namespace Fiea::Engine {
    using CreateFunc = void* (*) ();
    using DestroyFunc = void (*) (void*);
    using ProvideFunc = void* (*) (void*);

    struct ServiceProvider
    {
        CreateFunc Create;
        DestroyFunc Destroy;
        ProvideFunc Provide;  
    };
    

	class ServiceMgr : public Singleton<ServiceMgr, true>
	{
	public:
        using InterfaceId = size_t;
        // we'll use this as a way to uniquely refer to a particular interface 
		ServiceMgr() = default;
        ~ServiceMgr() = default;
        static void Reset();
        template <typename I>
        static void RegisterInterfaceProvider(CreateFunc create, DestroyFunc destroy, ProvideFunc provide);
        template<typename I>
        static I* ProvideInterface();
    private:
        std::unordered_map<InterfaceId, ServiceProvider> _ServiceMap;
        std::unordered_map<void*, void*> _ImplementMap;
        //Just a helper to do the lookup
        //const ServiceProvider* GetProvider(InterfaceId id);
        //A helper to get the implementation pointer... or create a new one, if necessary
        //void* GetImplementation(const ServiceProvider & provider);
	};
    /**
     * Creates a mapping from and InterfaceId to a ServiceProvider struct
     */
    template <typename I> 
    static void ServiceMgr::RegisterInterfaceProvider(CreateFunc create, DestroyFunc destroy, ProvideFunc provide) 
    {
        Instance();
        const ServiceMgr::InterfaceId id = I::GetInterfaceId();
        ServiceProvider serviceProvider = {create, destroy, provide};
        Instance()->_ServiceMap.insert({ id, serviceProvider });
        ServiceProvider& sProvider = Instance()->_ServiceMap[id];
    };

    /**
     * Create instance and provide interface if no instance with requested interface existed
     * \return I*
     */
    template<typename I> 
    static I* ServiceMgr::ProvideInterface() {
        const InterfaceId id = I::GetInterfaceId();
        ServiceProvider& sProvider = Instance()->_ServiceMap[id];
        void* providerKey = sProvider.Create;
        // Assert interface is already registered
        FIEA_ASSERT(Instance()->_ServiceMap.find(id) != Instance()->_ServiceMap.end());

        // Check if an instance already exists
        auto it = Instance()->_ImplementMap.find(providerKey);
        // if found in implementMap, return value
        if (it != Instance()->_ImplementMap.end()) {
            return reinterpret_cast<I*>(sProvider.Provide(it->second));
        }

        // if not in implementMap, create implement and put in. 
        void* implement = sProvider.Create();
        Instance()->_ImplementMap.insert({ providerKey, implement });
        return reinterpret_cast<I*>(sProvider.Provide(implement));
    };

}