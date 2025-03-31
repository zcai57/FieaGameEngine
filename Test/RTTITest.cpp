#include "CppUnitTest.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "Support/Empty.h"
#include "Support/EmptyChild.h"

using namespace Fiea::Engine::Tests::Support;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	TEST_CLASS(RTTITest)
	{
	public:
		TEST_METHOD(IsTest)
		{
			Empty e1;
			EmptyChild e2(2);
			Empty e3 = e2;
			Assert::IsTrue(e2.getValue() == 2);
			Assert::IsTrue(e3.getValue() == 0);
			Assert::IsTrue(e1.Is(e3.TypeIdInstance()));	
			Assert::IsFalse(e3.Is(e2.TypeIdInstance()));
		}
		TEST_METHOD(AsTest)
		{
			EmptyChild e2(2);
			Empty* e3 = e2.As<Empty>();
			Assert::IsTrue(e3->getValue() == 2);
			Assert::IsTrue(e3->Is(e2.TypeIdClass()));
		}
		TEST_METHOD(EqualToStringTest)
		{
			EmptyChild e2(2);
			Empty* e3 = e2.As<Empty>();
			Assert::IsTrue(e2.ToString() == string("2"));
			Assert::IsTrue(e3->ToString() == string("2"));

		}
	};
}