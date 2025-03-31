#include "Tests.h"
#include "Support/Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(FooTests)
    {
        TEST_MEMCHECK;
        TEST_METHOD(Constructor)
        {
            const Foo a;
            Assert::AreEqual(0, a.Data());

            const Foo b(10);
            Assert::AreEqual(10, b.Data());
        }

        TEST_METHOD(CopySemantics)
        {
            const Foo a(10);
            Assert::AreEqual(10, a.Data());

            const Foo b(a);
            Assert::AreEqual(a, b);

            Foo c;
            c = a;
            Assert::AreEqual(c, a);
        }

        TEST_METHOD(EqualityOperators)
        {
            const Foo a(10);
            const Foo b(a);

            // Assert::AreNotEqual does not invoke operator!=
            Assert::AreEqual(a, b);
            Assert::IsTrue(a == b);
            Assert::IsFalse(a != b);

            const Foo c;
            Assert::AreNotEqual(a, c);
            Assert::IsTrue(a != c);
            Assert::IsFalse(a == c);
        }

        TEST_METHOD(SetData)
        {
            Foo a;
            Assert::AreEqual(0, a.Data());

            const int data = 10;
            a.SetData(data);
            Assert::AreEqual(data, a.Data());
        }
    };
}