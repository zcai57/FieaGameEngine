#pragma once
#include "Types.h"
#include "RTTI.h"
#include "ServiceMgr.h"

// Generate the ENTIRE class declaration for a ProductTypeFactory that derives from IAbstractFactory<BaseType>
// We'll typically place this invocation in a header, right after the declaration of the ProductType class
#define FACTORY_DECLARE(ProductType, BaseType)                                      \
class ProductType##Factory : public IAbstractFactory<BaseType>                               \
{                                                                                      \
RTTI_DECLARATIONS(ProductType##Factory, IAbstractFactory<BaseType>);                         \
public:                                                                                \
    ProductType##Factory() = default;                                                           \
    ~ProductType##Factory() = default;                                                          \
    inline virtual [[nodiscard]] BaseType* Create() override { return new ProductType(); }        \
    inline virtual std::string GetProductName() override { return #ProductType; }      \
};
// Thin wrapper around the RTTI defs for a ProductTypeFactory
// We'll typically place this invocation in a cpp, as we do for RTTI_DEFINITIONS (probably right next to the RTTI defs for the associated ProductType)
#define FACTORY_DEFINE(ProductType)                                         \
RTTI_DEFINITIONS(ProductType##Factory)


// Thin wrapper around the RTTI defs for an IAbstractFactory<BaseType>
// We'll typically place this once in the cpp for the BaseType class
// Note that the specialization we are creating will need to appear in the same namespace as the IAbstractFactory template declaration!
#define FACTORY_DEFINE_ABSTRACT(BaseType)                                   \
RTTI_DEFINITIONS_TEMPLATED(IAbstractFactory<BaseType>)

namespace Fiea::Engine {
    //class Factory : public IFactoryBase {
    //    virtual string GetProductName() override; // The string name for this factory
    //};
    struct IFactoryBase: public RTTI {
        FIEA_SERVICE_INTERFACE;
        RTTI_DECLARATIONS(IFactoryBase, RTTI);
    public:
        virtual string GetProductName() = 0;
    };
    //RTTI type, child of IFactoryBase
    template <typename BaseType>
    struct IAbstractFactory : public IFactoryBase {
        FIEA_SERVICE_INTERFACE;
        RTTI_DECLARATIONS(IAbstractFactory, IFactoryBase);
        // a create function for the abstract factory that produces a default-constructed sub-class
    public:
        [[nodiscard]] virtual BaseType* Create() = 0;
    };

    struct IFactoryService
    {
        FIEA_SERVICE_INTERFACE;
    public:
        // thin wrapper for RegisterFactory
        template <typename BaseType> 
        void Register(std::unique_ptr<IAbstractFactory<BaseType>>&& concreteFactory)
        {
            RegisterFactory(BaseType::TypeIdClass(), std::move(concreteFactory));
        }
        // thin wrapper for GetFactory, which calls Create if a factory is found
        template <typename BaseType>
        BaseType* Create(const string& productName) {
            IFactoryBase* bs = GetFactory(BaseType::TypeIdClass(), productName);
            if (bs != nullptr) return reinterpret_cast<IAbstractFactory<BaseType>*>(bs)->Create();
            return nullptr;
        }
    protected:
        // associate this factory with an abstract factory for the class w/ the given id
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) = 0;
        // retrieve a factory associated with abstract factory for the class w/ the given id, matching the provided productName
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const string& productName) = 0;
    
    };
    class FactoryService : public IFactoryService
    {
    protected:
        // associate this factory with an abstract factory for the class w/ the given id
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) override;
        // retrieve a factory associated with abstract factory for the class w/ the given id, matching the provided productName
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const string& productName) override;
    private:
        static bool _Registration();
        static inline bool _registered = _Registration();
        std::unordered_map<RTTI::IdType, std::unordered_map<string, std::unique_ptr<IFactoryBase>>> _FactoryMap;
    };
}
