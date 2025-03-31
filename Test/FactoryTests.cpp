#pragma once
#include "CppUnitTest.h"
#include "Tests.h"
#include "Support/ScopedFoo.h"
#include "FieaGameEngine/FactoryService.h"
#include "FieaGameEngine/ContentService.h"

using namespace Fiea::Engine::Content;
using namespace Fiea::Engine::Tests::Support;

namespace Fiea::Engine::Tests
{
    using namespace std::string_literals;
//    class ScopeFactory : public IAbstractFactory<Scope>
//    {
//        RTTI_DECLARATIONS(ScopeFactory, IAbstractFactory<Scope>);
//    public:
//        ScopeFactory() = default;
//        ~ScopeFactory() = default;
//        
//        inline virtual Scope* Create() override
//        {
//            return &_Scope;
//        };
//        inline virtual string GetProductName() override
//        {
//            return "Scope"s;
//        };
//    private:
//        Scope _Scope;
//    };
//
//    class ScopedFooFactory : public IAbstractFactory<ScopedFoo> {
//        RTTI_DECLARATIONS(ScopeFactory, IAbstractFactory<ScopedFoo>);
//    public:
//        ScopedFooFactory() = default;
//        ~ScopedFooFactory() = default;
//
//        inline virtual ScopedFoo* Create() override
//        {
//            return &_ScopedFoo;
//        };
//        inline virtual string GetProductName() override
//        {
//            return "ScopedFoo"s;
//        };
//    private:
//        ScopedFoo _ScopedFoo;
//    };
//
//    RTTI_DEFINITIONS(ScopeFactory);
//    RTTI_DEFINITIONS(ScopedFooFactory);
    
    TEST_CLASS(FactoryTest)
    {
    public:
        TEST_MEMCHECK;
        TEST_METHOD(ScopeFactoryTest)
        {
            ScopeFactory s;
            IAbstractFactory<Scope>* absScope = new ScopeFactory();
            Scope* test1 = s.Create();
            Assert::AreEqual(test1->TypeIdClass(), test1->TypeIdInstance());
            Assert::AreEqual(s.GetProductName(), "Scope"s);
            Assert::AreNotEqual(absScope->TypeIdInstance(), absScope->TypeIdClass());
            Assert::AreEqual(absScope->TypeIdInstance(), s.TypeIdInstance());
            delete absScope;

           ScopedFooFactory sf;
           Scope* test2 = sf.Create();
           ScopedFoo* test3 = reinterpret_cast<ScopedFoo*>(test2);
           Assert::AreEqual(test3->TypeIdClass(), test3->TypeIdInstance());
           Assert::AreEqual(sf.GetProductName(), "ScopedFoo"s);
           delete test1;
           //delete test2;
           delete test3;
        }

        TEST_METHOD(ServiceTest)
        {
            IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
            //fs->Register(std::make_unique<ScopeFactory>());
            fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopeFactory>())));
            Scope* test1 = fs->Create<Scope>("Scope"s);
            Assert::AreEqual(test1->TypeIdInstance(), Scope::TypeIdClass());
           

            fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopedFooFactory>())));
            Scope* test2 = fs->Create<Scope>("ScopedFoo"s);
            Assert::AreEqual(test2->TypeIdInstance(), ScopedFoo::TypeIdClass());
            Assert::AreNotEqual(test2->TypeIdInstance(), Scope::TypeIdClass());
            ScopedFoo* test3 = reinterpret_cast<ScopedFoo*>(test2);
            test3->SetData(3);
            Assert::AreEqual(test3->Data(), 3);
            
            ServiceMgr::Reset();
            delete test1;
            delete test3;
        }
    };
}
