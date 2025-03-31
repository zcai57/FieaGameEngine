#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Types.h"
#include "Support/AttributeBoxPlus.h"
#include "Support/AttributeBox.h"
#include "FieaGameEngine/ContentService.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;
using namespace Fiea::Engine::Tests::Support;
namespace Fiea::Engine::Tests
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
		TEST_METHOD(HelperTest)
		{
			AttributeBox test1;
			Assert::IsTrue(test1.IsClassAttribute("String"s));
			Assert::IsTrue(test1.IsClassAttribute("Float"s));
			Assert::IsTrue(test1.IsClassAttribute("Int"s));
			Assert::IsTrue(test1.IsClassAttribute("FooPtr"s));
			Assert::IsTrue(test1.IsClassAttribute("StringArr"s));
			Assert::IsTrue(test1.IsClassAttribute("FloatArr"s));
			Assert::IsTrue(test1.IsClassAttribute("IntArr"s));
			Assert::IsTrue(test1.IsClassAttribute("FooPtrArr"s));

			AttributeBoxPlus test2;
			Assert::IsTrue(test2.IsClassAttribute("ExtraString"));

			test1.Append("Hello"s);
			Assert::IsTrue(test1.IsAttribute("Hello"s));
			Assert::IsTrue(test1.IsAttribute("String"s));
			Assert::IsTrue(test1.IsInstanceAttribute("Hello"s));
			Assert::IsFalse(test1.IsInstanceAttribute("String"s));

			test1.AppendInstanceAttribute("String"s);
			Assert::IsFalse(test1.IsInstanceAttribute("String"s));

			Assert::IsFalse(test1.IsInstanceAttribute("Yo"s));
			test1.AppendInstanceAttribute("Yo"s);
			Assert::IsTrue(test1.IsInstanceAttribute("Yo"s));

			std::vector<string> attContainer = test1.Attributes();
			std::vector<string> classAttContainer = test1.ClassAttributes();
			std::vector<string> insAttContainer = test1.InstanceAttributes();

			Assert::AreEqual(attContainer.size(), 11_z);
			Assert::AreEqual(classAttContainer.size(), 9_z);
			Assert::AreEqual(insAttContainer.size(), 2_z);

			ServiceMgr::Reset();
		}
		TEST_METHOD(InternalStorageTest)
		{
			AttributeBox test1;
			Datum& internalInt = test1["IntInternal"s];
			Assert::IsTrue(internalInt.IsType(Datum::Type::Int));
			Assert::AreEqual(internalInt.Size(), 0_z);
			internalInt.Push<int32_t>(1);
			Assert::AreEqual(internalInt.Size(), 1_z);

			AttributeBox test2(test1);
			Datum& internalInt2 = test2["IntInternal"s];
			// Address different
			Assert::IsFalse(&internalInt == &internalInt2);
			Assert::AreEqual(internalInt.Get<int32_t>(), internalInt2.Get<int32_t>());
			Assert::AreEqual(internalInt2.Get<int32_t>(), 1);
			Assert::IsTrue(internalInt2.IsType(Datum::Type::Int));
			Assert::AreEqual(internalInt2.Size(), 1_z);

			ServiceMgr::Reset();
		}
	};
}
	