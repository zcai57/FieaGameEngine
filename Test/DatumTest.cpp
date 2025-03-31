#include "CppUnitTest.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/Types.h"

using namespace Fiea::Engine::Content;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	TEST_CLASS(DatumTest)
	{
	public:
		size_t defaultSize = 5_z;
		TEST_MEMCHECK;
		TEST_METHOD(DatumConstructorTest)
		{
			Datum dt;
			Assert::IsTrue(dt.Size() == 0);
			dt.Clear();
		}
		TEST_METHOD(CopyAssignTest)
		{
			// Copy test
			{
				Datum dt{ 1 };
				Datum dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<int32_t>() == 1);
			}
			{
				Datum dt{ 1.0f };
				Datum dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<float>() == 1.0f);
			}
			{
				Datum dt{ string("1") };
				Datum dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<string>() == string("1"));
			}
			{
				Datum dt{ 1 };
				Datum dt1;
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<int32_t>() == 1);
			}
			{
				Datum dt{ 1.0f };
				Datum dt1;
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<float>() == 1.0f);
			}
			{
				Datum dt{ string("1") };
				Datum dt1;
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<string>() == string("1"));
			}
			{
				Datum dt{ Vec4Type(1.0f, 1.0f, 1.0f, 1.0f) };
				Datum dt1;
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<Vec4Type>() == Vec4Type(1.0f, 1.0f, 1.0f, 1.0f));
				Assert::IsTrue(dt1.IsType(Datum::Type::Vec4));
			}
			{
				Mat4x4Type m1(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				Datum dt{ m1 };
				Datum dt1;
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1);
				Assert::IsTrue(dt1.Get<Mat4x4Type>() == m1);
				Assert::IsTrue(dt1.IsType(Datum::Type::Mat4x4));
			}
		}
		TEST_METHOD(InitializerListTest)
		{
			Datum dt { 1, 2, 3 };
			Assert::IsTrue(dt.Size() == 3);
			Assert::IsTrue(dt.Capacity() == defaultSize - 3);
			Assert::IsTrue(dt.Get<int32_t>() == 1);

			Datum dt1 { 1.0f, 2.0f, 3.0f };
			Assert::IsTrue(dt1.Size() == 3);
			Assert::IsTrue(dt1.Capacity() == defaultSize - 3);
			Assert::IsTrue(dt1.Get<float>() == 1.0f);

			Datum dt2{ string("Hello"), string("World")};
			Assert::IsTrue(dt2.Size() == 2);
			Assert::IsTrue(dt2.Capacity() == defaultSize - 2);
			Assert::IsTrue(dt2.Get<string>(1) == string("World"));

			// Init list for vec, mat tested in above test

			Scope s;
			s.Append(string("Datum1"));
			Scope* sptr = &s;
			Datum dt3{ sptr };
			Assert::IsTrue(dt3.Size() == 1);
			Assert::IsTrue(dt3.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt3.GetPointer<Scope*>(0) == sptr);

			
			RTTI* r = new Foo(5);
			Datum dt4{ r };
			Assert::IsTrue(dt4.Size() == 1);
			Assert::IsTrue(dt4.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt4.GetPointer<RTTI*>(0) == r);
			delete r;
		}
		TEST_METHOD(DatumSetTypeTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			Assert::IsTrue(dt.Capacity() == defaultSize);
			Assert::IsTrue(dt.Size() == 0);
			
			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			Assert::IsTrue(dt1.Capacity() == defaultSize);
			Assert::IsTrue(dt1.Size() == 0);

			Datum dt2;
			dt2.SetType(Datum::Type::String);
			Assert::IsTrue(dt2.Capacity() == defaultSize);
			Assert::IsTrue(dt2.Size() == 0);

			Datum dt3;
			dt3.SetType(Datum::Type::Pointer);
			Assert::IsTrue(dt3.Capacity() == defaultSize);
			Assert::IsTrue(dt3.Size() == 0);

			Datum dt4;
			dt4.SetType(Datum::Type::Table);
			Assert::IsTrue(dt4.Capacity() == defaultSize);
			Assert::IsTrue(dt4.Size() == 0);

			Datum dt5;
			dt5.SetType(Datum::Type::Vec4);
			Assert::AreEqual(dt5.Capacity(), defaultSize);
			Assert::AreEqual(dt5.Size(), 0_z);

			Datum dt6;
			dt6.SetType(Datum::Type::Mat4x4);
			Assert::AreEqual(dt6.Capacity(), defaultSize);
			Assert::AreEqual(dt6.Size(), 0_z);
		}
		TEST_METHOD(DatumPushPopTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			// Push Type int
			dt.Push(1);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt.Size() == 1);
			dt.Pop();
			Assert::IsTrue(dt.Capacity() == defaultSize);
			Assert::IsTrue(dt.Size() == 0);
			Assert::IsTrue(dt.Get<int32_t>() == 1);

			//Push type string
			Datum dt1;
			dt1.SetType(Datum::Type::String);
			dt1.Push<string>("Hello");
			Assert::IsTrue(dt1.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt1.Size() == 1);
			Assert::IsTrue(dt1.Get<string>(0) == "Hello");
			dt1.Pop();

			// Push pop for float
			Datum dt2;
			dt2.SetType(Datum::Type::Float);
			dt2.Push<float>(1.0f);
			Assert::IsTrue(dt2.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt2.Size() == 1);
			Assert::IsTrue(dt2.Get<float>() == 1.0f);
			dt2.Pop();

			// Push pop for Vec Mat
			{
				Datum dt3;
				dt3.SetType(Datum::Type::Vec4);
				Vec4Type v(1.0f, 1.0f, 1.0f, 1.0f);
				dt3.Push<Vec4Type>(v);
				Assert::IsTrue(dt3.Capacity() == defaultSize - 1);
				Assert::IsTrue(dt3.Size() == 1);
				Assert::IsTrue(dt3.Get<Vec4Type>() == v);
				dt3.Pop();
				Assert::AreEqual(dt3.Size(), 0_z);

				for (size_t i = 0; i <= defaultSize; ++i)
				{
					dt3.Push<Vec4Type>(v);
				}
				Assert::IsTrue(dt3.Capacity() == 2 * defaultSize - dt3.Size());
				Assert::IsTrue(dt3.Size() == defaultSize + 1);
				Assert::IsTrue(dt3.Get<Vec4Type>(defaultSize) == v);

			}

			{
				Datum dt3;
				dt3.SetType(Datum::Type::Mat4x4);
				Mat4x4Type m1(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				dt3.Push<Mat4x4Type>(m1);
				Assert::IsTrue(dt3.Capacity() == defaultSize - 1);
				Assert::IsTrue(dt3.Size() == 1);
				Assert::IsTrue(dt3.Get<Mat4x4Type>() == m1);
				dt3.Pop();
				Assert::AreEqual(dt3.Size(), 0_z);

				for (size_t i = 0; i <= defaultSize; ++i)
				{
					dt3.Push<Mat4x4Type>(m1);
				}
				Assert::IsTrue(dt3.Capacity() == 2 * defaultSize - dt3.Size());
				Assert::IsTrue(dt3.Size() == defaultSize + 1);
				Assert::IsTrue(dt3.Get<Mat4x4Type>(defaultSize) == m1);
			}

			//Test push past Capacity will resize
			for (size_t i = 0; i <= defaultSize; ++i)
			{
				dt.Push(1);
			}
			Assert::IsTrue(dt.Capacity() == 2 * defaultSize - dt.Size());
			Assert::IsTrue(dt.Size() == defaultSize + 1);
			Assert::IsTrue(dt.Get<int32_t>() == 1);

			// Test push past Capcity for string
			for (size_t i = 0; i <= defaultSize; ++i)
			{
				dt1.Push<string>(string("hello"));
			}
			Assert::IsTrue(dt1.Capacity() == 2 * defaultSize - dt.Size());
			Assert::IsTrue(dt1.Size() == defaultSize + 1);
			Assert::IsTrue(dt1.Get<string>(defaultSize) == string("hello"));

			// Test push past Capacity for float
			for (size_t i = 0; i <= defaultSize; ++i)
			{
				dt2.Push(1.0f);
			}
			Assert::IsTrue(dt2.Capacity() == 2 * defaultSize - dt.Size());
			Assert::IsTrue(dt2.Size() == defaultSize + 1);
			Assert::IsTrue(dt2.Get<float>(defaultSize) == 1.0f);


			//Push type RTTI*
			RTTI* r = new Foo(5);
			Datum dt3;
			dt3.SetType(Datum::Type::Pointer);
			dt3.PushPointer<RTTI*>(r);
			Assert::IsTrue(dt3.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt3.Size() == 1);
			Assert::IsTrue(dt3.GetPointer<RTTI*>(0) == r);
			dt3.Pop();
			Assert::IsTrue(dt3.Size() == 0);
			delete r;

			RTTI** rTest = reinterpret_cast<RTTI**>(malloc((defaultSize + 1) * sizeof(RTTI*)));
			//Test push past Capacity will resize
			for (size_t i = 0; i <= defaultSize; ++i)
			{
				dt3.PushPointer(rTest[i]);
			}
			Assert::IsTrue(dt3.Capacity() == 2 * defaultSize - dt.Size());
			Assert::IsTrue(dt3.Size() == defaultSize + 1);
			Assert::IsTrue(dt3.GetPointer<RTTI*>() == rTest[0]);
			free(rTest);

			//Push type Scope*
			Scope* s = new Scope;
			Datum dt4;
			dt4.SetType(Datum::Type::Table);
			dt4.PushPointer<Scope*>(s);
			Assert::IsTrue(dt4.Capacity() == defaultSize - 1);
			Assert::IsTrue(dt4.Size() == 1);
			Assert::IsTrue(dt4.GetPointer<Scope*>(0) == s);
			dt4.Pop();
			Assert::IsTrue(dt4.Size() == 0);
			delete s;

			Scope** sTest = reinterpret_cast<Scope**>(malloc((defaultSize + 1) * sizeof(Scope*)));
			for (size_t i = 0; i <= defaultSize; ++i)
			{
				dt4.PushPointer(sTest[i]);
			}
			Assert::IsTrue(dt4.Capacity() == 2 * defaultSize - dt.Size());
			Assert::IsTrue(dt4.Size() == defaultSize + 1);
			Assert::IsTrue(dt4.GetPointer<Scope*>() == sTest[0]);
			free(sTest);

		}
		TEST_METHOD(DatumGetSetTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			dt.Push(1);
			dt.Push(3);
			int32_t test1 = dt.Get<int32_t>(0);
			Assert::IsTrue(test1 == 1);
			Assert::IsTrue(dt.Get<int32_t>(1) == 3);
			dt.Set<int32_t>(0, 0);
			Assert::IsTrue(dt.Get<int32_t>(0) == 0);
			Assert::IsTrue(dt.Get<int32_t>(1) == 3);
			dt.Clear();

			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			dt1.Push(1.0f);
			dt1.Set(2.0f);
			Assert::AreEqual(dt1.Get<float>(), 2.0f);

			Datum dt2;
			dt2.SetType(Datum::Type::String);
			dt2.Push(string("HELLO"));
			dt2.Set(string("OLLEH"));
			Assert::AreEqual(dt2.Get<string>(), string("OLLEH"));

			// Test string const Get
			const Datum dt3("hello");
			Assert::AreEqual(dt3.Get<string>(), string("hello"));

			{
				Datum dt4;
				dt4.SetType(Datum::Type::Vec4);
				Vec4Type v(1.0f, 1.0f, 1.0f, 1.0f);
				Vec4Type v1(2.0f, 1.0f, 1.0f, 1.0f);
				dt4.Push<Vec4Type>(v);
				dt4.Set<Vec4Type>(v1);
				Assert::IsTrue(dt4.Get<Vec4Type>() == v1);
				Assert::IsFalse(dt4.Get<Vec4Type>() == v);
			}

			{
				Datum dt4;
				dt4.SetType(Datum::Type::Mat4x4);
				Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				Mat4x4Type m1(2.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				dt4.Push<Mat4x4Type>(m);
				dt4.Set<Mat4x4Type>(m1);
				Assert::IsTrue(dt4.Get<Mat4x4Type>() == m1);
				Assert::IsFalse(dt4.Get<Mat4x4Type>() == m);
			}
			
			// Test set for RTTI*
			Datum dt4;
			dt4.SetType(Datum::Type::Pointer);
			RTTI* r = new Foo(1);
			dt4.PushPointer(r);
			RTTI* r1 = new Foo(2);
			dt4.SetPointer(r1);
			Assert::IsTrue(dt4.GetPointer<RTTI*>() == r1);
			Assert::IsTrue(dt4.GetPointer<RTTI*>() != r);
			delete r;
			delete r1;

			// Test set for Scope*
			Datum dt5;
			dt5.SetType(Datum::Type::Table);
			Scope* s = new Scope();
			dt5.PushPointer(s);
			Scope* s1 = new Scope();
			dt5.SetPointer(s1);
			Assert::IsTrue(dt5.GetPointer<Scope*>() == s1);
			Assert::IsTrue(dt5.GetPointer<Scope*>() != s);
			delete s;
			delete s1;
		}
		TEST_METHOD(IsTypeTest)
		{
			Datum dt;
			Assert::IsTrue(dt.IsType(Datum::Type::Unknown));
			Assert::IsFalse(dt.IsType(Datum::Type::Int));
			dt.SetType(Datum::Type::Float);
			Assert::IsTrue(dt.IsType(Datum::Type::Float));
			Assert::IsFalse (dt.IsType(Datum::Type::String));
			dt.SetType(Datum::Type::Vec4);
			Assert::IsFalse(dt.IsType(Datum::Type::Mat4x4));
			Assert::IsTrue(dt.IsType(Datum::Type::Vec4));
			dt.SetType(Datum::Type::Mat4x4);
			Assert::IsTrue(dt.IsType(Datum::Type::Mat4x4));
			Assert::IsFalse(dt.IsType(Datum::Type::Vec4));
			dt.Clear();
		}
		TEST_METHOD(CopyMoveConstructorTest)
		{
			// Move int
			Datum dt1;
			dt1.SetType(Datum::Type::Int);
			dt1.Push(1);
			Datum dt = std::move(dt1);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<int32_t>(0) == 1);

			// Move float
			Datum dt2;
			dt2.SetType(Datum::Type::Float);
			dt2.Push(1.0f);
			Datum dt3 = std::move(dt2);
			Assert::IsTrue(dt3.Size() == 1_z);
			Assert::IsTrue(dt3.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt3.Get<float>() == 1.0f);

			// Move string
			Datum dt4;
			dt4.SetType(Datum::Type::String);
			dt4.Push(string("hello"));
			Datum dt5 = std::move(dt4);
			Assert::IsTrue(dt5.Size() == 1_z);
			Assert::IsTrue(dt5.Capacity() == defaultSize - 1_z);
			Assert::AreEqual(dt5.Get<string>(), string("hello"));

			// Move copy for Vec, Mat
			{
				Datum dt;
				dt.SetType(Datum::Type::Vec4);
				Vec4Type v( 1.0f, 1.0f, 1.0f, 1.0f );
				dt.Push<Vec4Type>(v);
				Datum dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Vec4Type>() == v);
			}

			{
				Datum dt;
				dt.SetType(Datum::Type::Mat4x4);
				Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				dt.Push<Mat4x4Type>(m);
				Datum dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Mat4x4Type>() == m);
			}

			// Copy constructor move RTTI*
			{
				Datum dt;
				dt.SetType(Datum::Type::Pointer);
				RTTI* r = new Foo(5);
				dt.PushPointer(r);
				Datum dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.GetPointer<RTTI*>() == r);
				delete r;
			}
			// Copy constructor move Scope*
			{
				Datum dt;
				dt.SetType(Datum::Type::Table);
				Scope* s = new Scope(5);
				dt.PushPointer(s);
				Datum dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.GetPointer<Scope*>() == s);
				delete s;
			}

			// Single data copy
			Datum dt6(1);
			Assert::IsTrue(dt6.Size() == 1_z);
			Assert::IsTrue(dt6.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt6.Get<int32_t>() == 1);

			Datum dt7(1.0f);
			Assert::IsTrue(dt7.Size() == 1_z);
			Assert::IsTrue(dt7.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt7.Get<float>() == 1.0f);

			Datum dt8("hello");
			Assert::IsTrue(dt8.Size() == 1_z);
			Assert::IsTrue(dt8.Capacity() == defaultSize - 1_z);
			Assert::AreEqual(dt8.Get<string>(), string("hello"));

			// Single Data copy for Vec, Mat
			{
				Vec4Type v(1.0f, 1.0f, 1.0f, 1.0f);
				Datum dt(v);
				Assert::IsTrue(dt.Size() == 1_z);
				Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt.Get<Vec4Type>() == v);
			}
			{
				Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				Datum dt(m);
				Assert::IsTrue(dt.Size() == 1_z);
				Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt.Get<Mat4x4Type>() == m);
			}

			RTTI* r= new Foo(1);
			Datum dt9(r);
			Assert::IsTrue(dt9.Size() == 1_z);
			Assert::IsTrue(dt9.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt9.GetPointer<RTTI*>() == r);
			delete r;

			Scope* s = new Scope(5);
			Datum dt10(s);
			Assert::IsTrue(dt10.Size() == 1_z);
			Assert::IsTrue(dt10.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt10.GetPointer<Scope*>() == s);
			delete s;
		}
		TEST_METHOD(AssignmentOperatorTest)
		{
			// Move semantic float
			Datum dt;
			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			dt1.Push(1.0f);
			dt = std::move(dt1);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<float>(0) == 1.0f);

			// Move semantic string
			Datum dt2;
			dt2.SetType(Datum::Type::String);
			dt2.Push(string("hello"));
			dt = std::move(dt2);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<string>() == string("hello"));

			// Move semantic int32_t
			Datum dt3;
			dt3.SetType(Datum::Type::Int);
			dt3.Push(1);
			dt = std::move(dt3);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<int32_t>() == 1);

			//Move Semantic Vec Mat
			{
				Datum dt;
				Datum dt1;
				dt.SetType(Datum::Type::Vec4);
				Vec4Type v(1.0f, 1.0f, 1.0f, 1.0f);
				dt.Push<Vec4Type>(v);
				dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Vec4Type>() == v);
			}

			{
				Datum dt;
				Datum dt1;
				dt.SetType(Datum::Type::Mat4x4);
				Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				dt.Push<Mat4x4Type>(m);
				dt1 = std::move(dt);
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Mat4x4Type>() == m);
			}
			
			// Move Semantic RTTI*
			Datum dt4;
			dt4.SetType(Datum::Type::Pointer);
			RTTI* r1 = new Foo(0);
			dt4.PushPointer(r1);
			dt = std::move(dt4);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.GetPointer<RTTI*>() == r1);
			delete r1;

			// Move Semantic RTTI*
			Datum dt5;
			dt5.SetType(Datum::Type::Table);
			Scope* s1 = new Scope(0);
			dt5.PushPointer(s1);
			dt = std::move(dt5);
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.GetPointer<Scope*>() == s1);
			delete s1;

			// Assignment with int32_t
			dt = 5;
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<int32_t>(0) == 5);

			// Assignment with float
			dt = 1.0f;
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<float>() == 1.0f);

			// Assignment with string
			dt = string("hello");
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.Get<string>(0) == "hello");

			// Assignment with Vec Mat
			{
				Datum dt;
				Datum dt1;
				dt.SetType(Datum::Type::Vec4);
				Vec4Type v(1.0f, 1.0f, 1.0f, 1.0f);
				dt.Push<Vec4Type>(v);
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Vec4Type>() == v);
			}

			{
				Datum dt;
				Datum dt1;
				dt.SetType(Datum::Type::Mat4x4);
				Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f, 1.0f);
				dt.Push<Mat4x4Type>(m);
				dt1 = dt;
				Assert::IsTrue(dt1.Size() == 1_z);
				Assert::IsTrue(dt1.Capacity() == defaultSize - 1_z);
				Assert::IsTrue(dt1.Get<Mat4x4Type>() == m);
			}

			// Assignment with RTTI*
			RTTI* r = new Foo(5);
			dt = r;
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.GetPointer<RTTI*>(0) == r);
			delete r;

			// Assignment with Scope*
			Scope* s = new Scope(5);
			dt = s;
			Assert::IsTrue(dt.Size() == 1_z);
			Assert::IsTrue(dt.Capacity() == defaultSize - 1_z);
			Assert::IsTrue(dt.GetPointer<Scope*>(0) == s);
			delete s;
		}
		TEST_METHOD(SetFromStringTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			dt.SetFromString<int32_t>(string("1"), 0);
			Assert::IsTrue(dt.Get<int32_t>(0) == (int32_t)1);

			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			dt1.SetFromString<float>(string("1.0"), 0);
			Assert::IsTrue(dt1.Get<float>(0) == 1.0f);

			Datum dt2;
			dt2.SetType(Datum::Type::Vec4);
			dt2.SetFromString<Vec4Type>(string("(1.0, 1.0, 1.0, 1.0)"), 0);
			Assert::IsTrue(dt2.Get<Vec4Type>(0) == Vec4Type(1.0, 1.0f, 1.0f, 1.0f));

			Datum dt3;
			dt3.SetType(Datum::Type::Mat4x4);
			Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f);
			dt3.SetFromString<Mat4x4Type>(string("((1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0))"), 0);
			Assert::IsTrue(dt3.Get<Mat4x4Type>() == m);

			dt.Clear();
			dt1.Clear();
		}
		TEST_METHOD(PushFromStringTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			dt.PushFromString<int32_t>(string("1"));
			Assert::IsTrue(dt.Get<int32_t>(0) == (int32_t)1);

			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			dt1.PushFromString<float>(string("1.0"));
			Assert::IsTrue(dt1.Get<float>(0) == 1.0f);

			Datum dt2;
			dt2.SetType(Datum::Type::Vec4);
			dt2.PushFromString<Vec4Type>(string("(1.0, 1.0, 1.0, 1.0)"));
			Assert::IsTrue(dt2.Get<Vec4Type>(0) == Vec4Type(1.0, 1.0f, 1.0f, 1.0f));

			Datum dt3;
			dt3.SetType(Datum::Type::Mat4x4);
			Mat4x4Type m(1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f);
			dt3.PushFromString<Mat4x4Type>(string("((1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0), (1.0, 1.0, 1.0, 1.0))"));
			Assert::IsTrue(dt3.Get<Mat4x4Type>() == m);
		}
		TEST_METHOD(GetToStringTest)
		{
			Datum dt;
			dt.SetType(Datum::Type::Int);
			dt.Push(1);
			string test1 = dt.GetToString<int32_t>(0);
			Assert::IsTrue(string("1").compare(test1) == 0);

			Datum dt1;
			dt1.SetType(Datum::Type::Float);
			dt1.Push(1.0f);
			string test2 = dt1.GetToString<float>(0);
			Assert::IsTrue(string("1.000000").compare(test2) == 0);

			Datum dt2;
			dt2.SetType(Datum::Type::Vec4);
			dt2.Push(Vec4Type(1.0, 1.0, 1.0, 1.0));
			string test3 = dt2.GetToString<Vec4Type>(0);
			Assert::IsTrue(string("vec4(1.000000, 1.000000, 1.000000, 1.000000)").compare(test3) == 0);

			Datum dt3;
			dt3.SetType(Datum::Type::Mat4x4);
			dt3.Push(Mat4x4Type(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0));
			string m = "mat4x4((1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000))";
			string test4 = dt3.GetToString<Mat4x4Type>(0);
			Assert::IsTrue(m.compare(test4) == 0);
		}
		TEST_METHOD(ExternalMemTest)
		{
			// int32_T
			int32_t i = 7;
			Datum d;
			d.SetStorage(&i);
			Assert::AreEqual(7, d.Get<int32_t>(0));
			i = 6;
			Assert::AreEqual(6, d.Get<int32_t>(0));
			d.Set(5);
			Assert::AreEqual(5, i);

			// int32_t Arrays
			int32_t values[3] = { 1, 2, 3 };
			d.SetStorage(values, 3);
			d.Set(5, 1);
			Assert::AreEqual(values[1], 5);
			values[0] = 6;
			Assert::AreEqual(d.Get<int32_t>(0), 6);
			d.Pop();
			d.Push(4);
			d.Clear();
			Assert::IsTrue(d.Size() == 3);
			Assert::IsTrue(d.Size() == d.Capacity());

			// float 
			float j = 1.0f;
			d.SetStorage<float>(&j);
			d.Set(2.0f);
			Assert::AreEqual(j, 2.0f);
			j = 3.0f;
			Assert::AreEqual(d.Get<float>(), 3.0f);

			// float Array
			float valueF[3] = { 1.0f, 2.0f, 3.0f };
			d.SetStorage(valueF, 3);
			d.Set(5.0f, 1);
			Assert::AreEqual(valueF[1], 5.0f);
			valueF[2] = 4.0f;
			Assert::AreEqual(d.Get<float>(2), 4.0f);
			d.Pop();
			d.Push(4.0f);
			d.Clear();
			Assert::IsTrue(d.Size() == 3);
			Assert::IsTrue(d.Size() == d.Capacity());

			// String
			string k = "hello";
			d.SetStorage<string>(&k);
			d.Set(string("hell"));
			Assert::AreEqual(k, string("hell"));
			k = "ello";
			Assert::AreEqual(d.Get<string>(), string("ello"));

			// String Array
			string valueS[3] = { "cat", "dog", "bird" };
			d.SetStorage<string>(valueS, 3);
			d.Set(string("god"), 1);
			Assert::AreEqual(valueS[1], string("god"));
			valueS[2] = "drib";
			Assert::AreEqual(d.Get<string>(2), string("drib"));
			// Push pop should do nothing
			d.Pop();
			d.Push(string("world"));
			d.Clear();
			Assert::IsTrue(d.Size() == 3);
			Assert::IsTrue(d.Size() == d.Capacity());

			// RTTI
			/*RTTI* valueR1 = new Foo(5);
			d.SetStoragePtr<RTTI>(&(valueR1), 1);
			RTTI* valueR2 = new Foo(6);
			Assert::IsTrue(d.GetPointer<RTTI*>() == valueR1);

			d.SetPointer<RTTI*>(valueR2, 0);
			Assert::IsTrue(valueR2 == d.GetPointer<RTTI*>());
			d.Clear();*/
			// RTTI[]
	/*		Foo* Foo1 = new Foo(5);
			Foo* Foo2 = new Foo(6);
			RTTI* valueArr1[] = {Foo1, Foo2};
			d.SetStoragePtr<RTTI>(&(valueArr1), 2);*/

		/*	delete valueR1;
			delete valueR2;*/
		}
		TEST_METHOD(EqualityOperatorTest)
		{
			{
				Datum test1;
				Datum test2;
				test1.SetType(Datum::Type::Int);
				test2.SetType(Datum::Type::Int);
				test1.Push<int32_t>(1);
				test2.Push<int32_t>(1);
				Assert::IsTrue(test1 == test2);
				Assert::IsFalse(test1 != test2);


				test1.Push<int32_t>(2);
				Assert::IsFalse(test1 == test2);
				Assert::IsTrue(test1 != test2);
			}

			{
				Datum test1;
				Datum test2;
				test1.SetType(Datum::Type::Float);
				test2.SetType(Datum::Type::Float);
				test1.Push<float>(1.0f);
				test2.Push<float>(1.0f);
				Assert::IsTrue(test1 == test2);
				Assert::IsFalse(test1 != test2);

				test1.Push<float>(2.0f);
				Assert::IsFalse(test1 == test2);
				Assert::IsTrue(test1 != test2);
			}
			{
				Datum test1;
				Datum test2;
				test1.SetType(Datum::Type::String);
				test2.SetType(Datum::Type::String);
				test1.Push<string>(string("s"));
				test2.Push<string>(string("s"));
				Assert::IsTrue(test1 == test2);
				Assert::IsFalse(test1 != test2);

				test1.Push<string>(string("w"));
				Assert::IsFalse(test1 == test2);
				Assert::IsTrue(test1 != test2);
			}
			{ 
				Datum test1;
				Datum test2;
				test1.SetType(Datum::Type::Pointer);
				test2.SetType(Datum::Type::Pointer);
				RTTI* r = new Foo(0);
				test1.PushPointer<RTTI*>(r);
				test2.PushPointer<RTTI*>(r);
				Assert::IsTrue(test1 == test2);
				Assert::IsFalse(test1 != test2);

				RTTI* r1 = new Foo(1);
				test1.PushPointer<RTTI*>(r1);
				Assert::IsFalse(test1 == test2);
				Assert::IsTrue(test1 != test2);
				delete r;
				delete r1;
			}

			{
				Datum test1;
				Datum test2;
				test1.SetType(Datum::Type::Table);
				test2.SetType(Datum::Type::Table);
				Scope* s = new Scope(0);
				test1.PushPointer<Scope*>(s);
				test2.PushPointer<Scope*>(s);
				Assert::IsTrue(test1 == test2);
				Assert::IsFalse(test1 != test2);
				
				Scope* s1 = new Scope(0);
				test1.PushPointer<Scope*>(s1);
				Assert::IsFalse(test1 == test2);
				Assert::IsTrue(test1 != test2);
				delete s;
				delete s1;
			}
		}
#ifdef FIEA_DEBUG
		/*TEST_METHOD(RemoveAtTest)
		{
			Scope* s = new Scope(5);
			Datum test1(s);
			Scope* s1 = new Scope(4);
			test1.PushPointer<Scope*>(s1);
			Scope* s2 = new Scope(3);
			test1.PushPointer<Scope*>(s2);

			Assert::IsTrue(test1.GetPointer<Scope*>(1) == s1);
		}*/
#endif // FIEA_DEBUG
	};
}
