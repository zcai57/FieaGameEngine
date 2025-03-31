#include "CppUnitTest.h"
#include "Support/Foo.h"
#include "Support/ScopedFoo.h"
#include "FieaGameEngine/ContentService.h"
#include "Tests.h"


using namespace Fiea::Engine::Content;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ScopeTestClass) {
        TEST_MEMCHECK;
        TEST_METHOD(ConstructorTest)
        {
            Scope s;
            Assert::IsTrue(s.Size() == 0_z);
        }
        TEST_METHOD(ValueConstructorTest)
        {
            Scope s(5);
            Assert::IsTrue(s.Size() == 0_z);
        }
        TEST_METHOD(AppendTest)
        {
            Scope s;
            string key1 = string("Datum1");
            s.Append(key1);
            Assert::IsTrue(s[key1] == s[0]);
            Assert::IsTrue(s.Size() == 1_z);
            Assert::IsFalse(s[key1] != s[0]);

            string key2 = string("Datum2");
            s.Append(key2);
            Assert::IsTrue(s[key2] == s[1]);
            Assert::IsFalse(s[key2] != s[1]);
            Assert::IsTrue(s.Size() == 2_z);
            // first el and second el have different addresses
            Assert::IsTrue(&s[key2] != &s[0]);
        }

        TEST_METHOD(ClearFindTest)
        {
            Scope s;
            string key1 = string("Datum1");
            s.Append(key1);
            Assert::IsTrue(s.Find(key1) != nullptr);
            Assert::IsTrue(*s.Find(key1) == s[key1]);
            s.Clear();
            Assert::IsTrue(s.Find(key1) == nullptr);
        }
        TEST_METHOD(BracketTest)
        {
            Scope s;
            string key1 = string("Datum1");
            s[key1];
            Assert::IsTrue(s.Size() == 1_z);
            Assert::IsTrue(s[key1] == s[0]);

            const Scope s1(5);
            Assert::IsTrue(s1.Find(key1) == nullptr);
            Assert::ExpectException<std::out_of_range>([&]() {s1[key1]; });
            Assert::ExpectException<std::out_of_range>([&]() {s1[0]; });
        }
        TEST_METHOD(EqualityOperatorTest)
        {
            Scope s;
            string key1 = string("Datum1");
            Scope s1;
            s.Append(key1);
            s1.Append(key1);
            Assert::IsTrue(s == s1);

            s.Append(string("Datum2"));
            Assert::IsTrue(s != s1);

            s1.Append(string("Datum3"));
            Assert::IsTrue(s != s1);
            Assert::IsFalse(s == s1);
        }
        TEST_METHOD(AdoptOrphanTest)
        {
            Scope s;
            // heap allocation to test if s handle child memory when destruct
            Scope* s1 = new Scope();
            s.Adopt(string("Child"), *s1);
            Assert::IsTrue(s1->IsDescendentOf(s));
            Assert::IsTrue(s.IsAncestorOf(*s1));
            Assert::IsTrue(s1->GetParent() == &s);

            Scope* s2 = s1->Orphan();

            Assert::IsFalse(s1->IsDescendentOf(s));
            Assert::IsFalse(s.IsAncestorOf(*s1));
            Assert::IsTrue(s1->GetParent() == nullptr);
            delete s2;
        }
        TEST_METHOD(FindContainedTest)
        {
            Scope s;
            Scope* s1 = new Scope();
            Scope* s2 = new Scope();
            s.Adopt(string("Child"), *s1);
            s.Adopt(string("Child"), *s2);
            size_t idx1;
            size_t idx2;
            Assert::IsTrue(s.Append(string("Child")).Size() == 2_z);
            Datum* test1 = s.FindContainedScope(*s1, idx1);
            Datum* test2 = s.FindContainedScope(*s2, idx2);
            Assert::IsTrue(idx1 == 0);
            Assert::IsTrue(idx2 == 1);
            Assert::IsNotNull(test1);
            Assert::IsNotNull(test2);
            Assert::IsTrue(test1 == test2);
            //delete s1;

        }
        TEST_METHOD(CopyAssignTest)
        {
            Scope* scope = new ScopedFoo(5); // store a ScopedFoo as a Scope
            Scope* child1 = new Scope();
            string key = string("Child");
            scope->Adopt(key, *child1);
            
            string key1 = string("SomeOtherType");
            Datum& dt = scope->Append(key1);
            dt.SetType(Datum::Type::Int);
            Assert::IsTrue(scope->Size() == 2_z);

            Scope* scope2 = new Scope(*scope); // invoke copy construction
            Assert::IsTrue(scope2->Is(Scope::TypeIdClass())); // yep
            Assert::IsTrue(scope2->Is(ScopedFoo::TypeIdClass())); 
            Assert::IsTrue(scope->Size() == scope2->Size());
            Assert::IsTrue(scope->Append(key) == scope2->Append(key));

            Scope scope3;
            scope3 = *scope;
            //scope3 = new Scope(*scope);
            Assert::IsTrue(scope3.Is(Scope::TypeIdClass()));
            Assert::IsTrue(scope->Size() == scope2->Size());
            Assert::IsTrue(scope->Append(key) == scope3.Append(key));

            delete scope2;
            delete scope;
        }
        TEST_METHOD(DoubleNestingTest)
        {
            Scope* scope = new Scope();
            Scope* prevParent = new Scope();

            Scope* scope1 = new Scope();
            prevParent->Adopt(string("OldParentage"), *scope1);
            size_t index;
            // Cannot find not nested scope
            Assert::IsNull(scope->FindContainedScope(*prevParent, index));

            string key = string("Child");
            scope->Adopt(key, *scope1);
            Assert::IsNotNull(scope->Find(key));
            
            string key1 = string("GrandChild");
            Scope* scope2 = new Scope();
            scope1->Adopt(key1, *scope2);
            Assert::IsNotNull(scope1->Find(key1));

            Assert::IsTrue(scope2->IsDescendentOf(*scope));
            Assert::IsTrue(scope1->IsDescendentOf(*scope));
            Assert::IsTrue(scope2->IsDescendentOf(*scope1));

            delete scope;
            delete prevParent;
        }
        TEST_METHOD(MoveTest)
        {
            Scope* scope = new Scope();
            Scope* scope1 = new Scope();
            string key = string("Child");
            Datum& dt = scope->Append(key);
            *scope1 = std::move(*scope);
            Assert::IsTrue(scope1->Find(key) != nullptr);
            Assert::IsTrue(scope->Find(key) == nullptr);
            Assert::IsTrue(scope1->Size() == 1_z);
            Assert::IsTrue(dt == scope1->Append(key));

            Scope* scope2 = new Scope(std::move(*scope1));
            Assert::IsTrue(scope2->Find(key) != nullptr);
            Assert::IsTrue(scope1->Find(key) == nullptr);
            Assert::IsTrue(scope2->Size() == 1_z);
            Assert::IsTrue(dt == scope2->Append(key));

            delete scope;
            delete scope2;
            delete scope1;
        }
        TEST_METHOD(ConstTest)
        {
            {
                const Scope* s = new Scope(5);
                size_t size = 0;
                s->FindContainedScope(*s, size);
                Assert::AreEqual(size, 0_z);
                delete s;
            }

            {
                Scope* s1 = new Scope(5);
                s1->Append(string("test"));
                Scope* s2 = new Scope(5);
                s1->Adopt(string("child"), *s2);
                Scope* s3 = new Scope(5);
                s1->Adopt(string("child"), *s3);

                // Copy a nested scope into const scope
                // s and s1 reference same memory
                const Scope* s(s1);
                size_t size = 10;
                size_t size1 = 10;
                Assert::IsNotNull(s->Find(string("test")));
                Assert::IsNotNull(s->FindContainedScope(*s2, size));
                Assert::IsNotNull(s->FindContainedScope(*s3, size1));
                Assert::AreEqual(size, 0_z);
                Assert::AreEqual(size1, 1_z);
                
                //delete s1;
                delete s;
            }
        }
    };   
}