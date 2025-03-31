#include "FieaGameEngine/FactoryService.h"
#include "FieaGameEngine/ContentService.h"
namespace Fiea::Engine{
    RTTI_DEFINITIONS(IFactoryBase);
    //RTTI_DEFINITIONS_Templated(IAbstractFactory<BaseType>, BaseType);*/

    //RTTI_DEFINITIONS_TEMPLATED(IAbstractFactory<Fiea::Engine::Content::Scope>);
    FIEA_SERVICE_BIND1(FactoryService, IFactoryService);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="baseId"></param>
    /// <param name="factory"></param>
    void FactoryService::RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory)
    {
        //FIEA_ASSERT(_FactoryMap.find(baseId) == _FactoryMap.end());
        if (_FactoryMap.find(baseId) == _FactoryMap.end())
        {
            _FactoryMap.emplace(baseId, std::unordered_map<std::string, std::unique_ptr<IFactoryBase>>());
        }
        string factoryName = factory->GetProductName();
        auto& temp = _FactoryMap.at(baseId);
        FIEA_ASSERT(temp.find(factoryName) == temp.end());
        temp.insert({ factoryName, std::move(factory) });
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="baseId"></param>
    /// <param name="productName"></param>
    /// <returns></returns>
    IFactoryBase* FactoryService::GetFactory(RTTI::IdType baseId, const string& productName)
    {
        FIEA_ASSERT(_FactoryMap.find(baseId) != _FactoryMap.end());
        auto& temp = _FactoryMap.at(baseId);
        FIEA_ASSERT(temp.find(productName) != temp.end());
        return temp.at(productName).get();
    }

}
