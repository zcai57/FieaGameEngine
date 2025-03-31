#include "CppUnitTest.h"
#include "Tests.h"
#include "AttributeBox.h"
#include "AttributeBoxPlus.h"
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine::Tests::Support
{
	using namespace std::string_literals;
	TEST_CLASS(AtributeBoxTests)
	{
	public:
		TEST_MEMCHECK;
		void TwoWayBindingTest(AttributeBox& test1)
		{
			// Ext Int test
			test1.ExternalInteger = 1;
			Datum& IntD = test1[string("Int")];
			Assert::AreEqual(IntD.Get<int32_t>(), 1);

			IntD.Set<int32_t>(2);
			Assert::AreEqual(test1.ExternalInteger, 2);

			// Ext Int Arr
			test1.ExternalIntegerArray[1] = 5;
			Datum& IntArr = test1[string("IntArr")];
			Assert::AreEqual(IntArr.Get<int32_t>(1), 5);

			IntArr.Set<int32_t>(2, 1_z);
			Assert::AreEqual(test1.ExternalIntegerArray[1], 2);

			// Ext Float test
			test1.ExternalFloat = 1.0f;
			Datum& FloatD = test1["Float"];
			Assert::AreEqual(FloatD.Get<float>(), 1.0f);

			FloatD.Set<float>(2.0f);
			Assert::AreEqual(test1.ExternalFloat, 2.0f);

			// Ext Float arr test
			test1.ExternalFloatArray[2] = 3.0f;
			Datum& FloatArrD = test1["FloatArr"];
			Assert::AreEqual(FloatArrD.Get<float>(2), 3.0f);

			FloatArrD.Set<float>(4.0f, 2);
			Assert::AreEqual(test1.ExternalFloatArray[2], 4.0f);

			// Ext String
			string testStr = "Hello";
			test1.ExternalString = testStr;
			Datum& StringD = test1["String"];
			Assert::AreEqual(StringD.Get<string>(), testStr);

			string testStr1 = "HelloWorld";
			StringD.Set<string>(testStr1);
			Assert::AreEqual(test1.ExternalString, testStr1);

			// Ext String Arr
			test1.ExternalStringArray[1] = testStr;
			Datum& StringArrD = test1["StringArr"];
			Assert::AreEqual(StringArrD.Get<string>(1), testStr);

			StringArrD.Set<string>(testStr1, 1);
			Assert::AreEqual(test1.ExternalStringArray[1], testStr1);

			// Ext Foo
			Foo* testFoo = new Foo(5);
			test1.ExternalPointer = testFoo;
			Datum& PointerD = test1["FooPtr"];
			Assert::IsTrue(PointerD.GetPointer<RTTI*>() == (const RTTI*)testFoo);

			Foo* testFoo1 = new Foo(0);
			PointerD.SetPointer<RTTI*>(testFoo1);
			Assert::IsTrue(test1.ExternalPointer == testFoo1);

			// Ext Foo Arr
			test1.ExternalPointerArray[1] = testFoo;
			Datum& PointerArrD = test1["FooPtrArr"];
			Assert::IsTrue(PointerArrD.GetPointer<RTTI*>(1) == (const RTTI*)testFoo);

			PointerArrD.SetPointer<RTTI*>(testFoo1, 1);
			Assert::IsTrue(test1.ExternalPointerArray[1] == testFoo1);

			delete testFoo;
			delete testFoo1;
		}

		TEST_METHOD(CtorBindTest)
		{
			AttributeBox test1;
			TwoWayBindingTest(test1);
			ServiceMgr::Reset();
		}

		TEST_METHOD(CopyConstructor)
		{
			// Test two way binding after copy construction
			{
				AttributeBox test1;
				AttributeBox test2(test1);
				TwoWayBindingTest(test2);
			}

			// Test copy values
			{
				AttributeBox test1;
				test1.ExternalInteger = 1;
				test1.ExternalFloat = 2.0f;
				test1.ExternalString = string("Hello");
				Foo* testFoo = new Foo(5);
				test1.ExternalPointer = testFoo;
				test1.ExternalFloatArray[2] = 3.0f;

				// Test copy construction
				AttributeBox test2(test1);
				Assert::AreEqual(test2.ExternalInteger, 1);
				Assert::AreEqual(test2.ExternalFloat, 2.0f);
				Assert::AreEqual(test2.ExternalString, string("Hello"));
				Assert::AreEqual(test2.ExternalPointer->ToString(), testFoo->ToString());
				Assert::AreEqual(test2.ExternalFloatArray[2], 3.0f);
				Assert::IsTrue(test1 == test2);

				// Test assignment operator
				AttributeBox test3;
				test3 = test1;
				Assert::AreEqual(test3.ExternalInteger, 1);
				Assert::AreEqual(test3.ExternalFloat, 2.0f);
				Assert::AreEqual(test3.ExternalString, string("Hello"));
				Assert::AreEqual(test3.ExternalPointer->ToString(), testFoo->ToString());
				Assert::AreEqual(test3.ExternalFloatArray[2], 3.0f);
				Assert::IsTrue(test1 == test3);

				delete testFoo;
			}
			ServiceMgr::Reset();
		}
		TEST_METHOD(MoveTest)
		{
			AttributeBox test1;
			test1.ExternalInteger = 1;
			test1.ExternalString = string("Hello");
			Foo* testFoo = new Foo(5);
			test1.ExternalPointer = testFoo;

			AttributeBox test2 = std::move(test1);

			Assert::AreEqual(test1.ExternalInteger, 1);
			Assert::AreEqual(test2.ExternalString, string("Hello"));
			Assert::AreNotEqual(test1.ExternalString, string("Hello"));
			//Assert::AreEqual(test2.ExternalPointer, testFoo);
			Assert::IsTrue(test1.ExternalPointer == nullptr);

			AttributeBox test3;
			test3 = std::move(test2);
			Assert::AreEqual(test2.ExternalInteger, 1);
			Assert::AreEqual(test3.ExternalString, string("Hello"));
			Assert::AreNotEqual(test2.ExternalString, string("Hello"));
			//Assert::AreEqual(test3.ExternalPointer, testFoo);
			Assert::IsTrue(test2.ExternalPointer == nullptr);

			delete testFoo;
			ServiceMgr::Reset();
		}
	};
	TEST_CLASS(AttributeBoxPlusTests)
	{
		TEST_MEMCHECK;
		void TwoWayBindingTest(AttributeBoxPlus& test1)
		{
			// Ext Int test
			test1.ExternalInteger = 1;
			Datum& IntD = test1[string("Int")];
			Assert::AreEqual(IntD.Get<int32_t>(), 1);

			IntD.Set<int32_t>(2);
			Assert::AreEqual(test1.ExternalInteger, 2);

			// Ext Int Arr
			test1.ExternalIntegerArray[1] = 5;
			Datum& IntArr = test1[string("IntArr")];
			Assert::AreEqual(IntArr.Get<int32_t>(1), 5);

			IntArr.Set<int32_t>(2, 1_z);
			Assert::AreEqual(test1.ExternalIntegerArray[1], 2);

			// Ext Float test
			test1.ExternalFloat = 1.0f;
			Datum& FloatD = test1["Float"];
			Assert::AreEqual(FloatD.Get<float>(), 1.0f);

			FloatD.Set<float>(2.0f);
			Assert::AreEqual(test1.ExternalFloat, 2.0f);

			// Ext Float arr test
			test1.ExternalFloatArray[2] = 3.0f;
			Datum& FloatArrD = test1["FloatArr"];
			Assert::AreEqual(FloatArrD.Get<float>(2), 3.0f);

			FloatArrD.Set<float>(4.0f, 2);
			Assert::AreEqual(test1.ExternalFloatArray[2], 4.0f);

			// Ext String
			string testStr = "Hello";
			test1.ExternalString = testStr;
			Datum& StringD = test1["String"];
			Assert::AreEqual(StringD.Get<string>(), testStr);

			string testStr1 = "HelloWorld";
			StringD.Set<string>(testStr1);
			Assert::AreEqual(test1.ExternalString, testStr1);

			// Ext String Arr
			test1.ExternalStringArray[1] = testStr;
			Datum& StringArrD = test1["StringArr"];
			Assert::AreEqual(StringArrD.Get<string>(1), testStr);

			StringArrD.Set<string>(testStr1, 1);
			Assert::AreEqual(test1.ExternalStringArray[1], testStr1);

			// Ext Foo
			Foo* testFoo = new Foo(5);
			test1.ExternalPointer = testFoo;
			Datum& PointerD = test1["FooPtr"];
			Assert::IsTrue(PointerD.GetPointer<RTTI*>() == (const RTTI*)testFoo);

			Foo* testFoo1 = new Foo(0);
			PointerD.SetPointer<RTTI*>(testFoo1);
			Assert::IsTrue(test1.ExternalPointer == testFoo1);

			// Ext Foo Arr
			test1.ExternalPointerArray[1] = testFoo;
			Datum& PointerArrD = test1["FooPtrArr"];
			Assert::IsTrue(PointerArrD.GetPointer<RTTI*>(1) == (const RTTI*)testFoo);

			PointerArrD.SetPointer<RTTI*>(testFoo1, 1);
			Assert::IsTrue(test1.ExternalPointerArray[1] == testFoo1);
			
			// Extra String test
			test1.ExtraString = "Hello"s;
			Datum& extraD = test1["ExtraString"];
			Assert::AreEqual(extraD.Get<string>(), "Hello"s);

			extraD.Set<string>("HelloWorld"s);
			Assert::AreEqual(test1.ExtraString, "HelloWorld"s);

			delete testFoo;
			delete testFoo1;
		}
		TEST_METHOD(TwoWayBindingTest)
		{
			AttributeBoxPlus test1;
			TwoWayBindingTest(test1);
			ServiceMgr::Reset();
		}
		TEST_METHOD(CopyConstructor)
		{
			// Test two way binding after copy construction
			{
				AttributeBoxPlus test1;
				AttributeBoxPlus test2(test1);
				TwoWayBindingTest(test2);
			}

			// Test copy values
			{
				AttributeBoxPlus test1;
				test1.ExternalInteger = 1;
				test1.ExternalFloat = 2.0f;
				test1.ExternalString = string("Hello");
				Foo* testFoo = new Foo(5);
				test1.ExternalPointer = testFoo;
				test1.ExternalFloatArray[2] = 3.0f;

				// Test copy construction
				AttributeBoxPlus test2(test1);
				Assert::AreEqual(test2.ExternalInteger, 1);
				Assert::AreEqual(test2.ExternalFloat, 2.0f);
				Assert::AreEqual(test2.ExternalString, string("Hello"));
				Assert::AreEqual(test2.ExternalPointer->ToString(), testFoo->ToString());
				Assert::AreEqual(test2.ExternalFloatArray[2], 3.0f);
				Assert::IsTrue(test1 == test2);

				// Test assignment operator
				AttributeBoxPlus test3;
				test3 = test1;
				Assert::AreEqual(test3.ExternalInteger, 1);
				Assert::AreEqual(test3.ExternalFloat, 2.0f);
				Assert::AreEqual(test3.ExternalString, string("Hello"));
				Assert::AreEqual(test3.ExternalPointer->ToString(), testFoo->ToString());
				Assert::AreEqual(test3.ExternalFloatArray[2], 3.0f);
				Assert::IsTrue(test1 == test3);

				delete testFoo;
			}
			ServiceMgr::Reset();
		}
		TEST_METHOD(MoveTest)
		{
			AttributeBoxPlus test1;
			test1.ExternalInteger = 1;
			test1.ExternalString = string("Hello");
			Foo* testFoo = new Foo(5);
			test1.ExternalPointer = testFoo;

			AttributeBoxPlus test2 = std::move(test1);

			Assert::AreEqual(test1.ExternalInteger, 1);
			Assert::AreEqual(test2.ExternalString, string("Hello"));
			Assert::AreNotEqual(test1.ExternalString, string("Hello"));
			//Assert::AreEqual(test2.ExternalPointer, testFoo);
			Assert::IsTrue(test1.ExternalPointer == nullptr);

			AttributeBoxPlus test3;
			test3 = std::move(test2);
			Assert::AreEqual(test2.ExternalInteger, 1);
			Assert::AreEqual(test3.ExternalString, string("Hello"));
			Assert::AreNotEqual(test2.ExternalString, string("Hello"));
			//Assert::AreEqual(test3.ExternalPointer, testFoo);
			Assert::IsTrue(test2.ExternalPointer == nullptr);

			delete testFoo;
			ServiceMgr::Reset();
		}
	};
}
