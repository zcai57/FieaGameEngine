#include "FieaGameEngine/ServiceMgr.h"
#include "FieaGameEngine/Types.h"
namespace Fiea::Engine
{
    /**
     * Reset _Implement map to empty map
     */
    void ServiceMgr::Reset()
    {
        for (auto it_i = Instance()->_ImplementMap.begin(); it_i != Instance()->_ImplementMap.end(); ++it_i)
        {
            // for every implementation, call destroy
            for (auto it_s = Instance()->_ServiceMap.begin(); it_s != Instance()->_ServiceMap.end(); ++it_s)
            {
                // If this implementation uses it_s interface
                if (it_s->second.Create == it_i->first) {
                    it_s->second.Destroy(it_i->second);
                    break;
                } 
            }
        }
        // create function address, foomanager
        std::unordered_map<void*, void*> emptyImplement;
        //std::swap(Instance()->_ServiceMap, emptyService);
        std::swap(Instance()->_ImplementMap, emptyImplement);
    }

    /*const ServiceProvider* ServiceMgr::GetProvider(InterfaceId id)
    {
        return nullptr;
    }*/
    /*void* ServiceMgr::GetImplementation(const ServiceProvider& provider)
    {
        return nullptr;
    }*/
}