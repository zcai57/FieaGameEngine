#include "FieaGameEngine/GameObject.h"
#include "CppUnitTest.h"
#include <chrono>
#include <thread>
#include "Tests.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"
#include "Support/testAction.h"



using namespace Fiea::Engine::Actions;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
	using namespace Support;
	using namespace std::string_literals;
	TEST_CLASS(GameObjectTest)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(CtorTest)
		{
			GameObject a;
			std::vector<string> classAttributes = a.ClassAttributes();
			std::vector<string> instanceAttributes = a.InstanceAttributes();
			Assert::AreEqual(classAttributes.size(), 5_z);
			Assert::AreEqual(classAttributes.at(0_z).data(), "Position"s.c_str());
			Assert::AreEqual(classAttributes.at(1_z).data(), "Rotation"s.c_str());
			Assert::AreEqual(classAttributes.at(2_z).data(), "Scales"s.c_str());
			Assert::AreEqual(classAttributes.at(3_z).data(), "Children"s.c_str());
			ServiceMgr::Reset();
		}
		TEST_METHOD(AddChild)
		{
			GameObject a;
			Assert::AreEqual(a.ToString(), "GameObject"s);

			GameObject* b = new GameObject();
			a.AddChild(*b);
			Assert::AreEqual(a.Find("Children")->Size(), 1_z);
			Assert::IsTrue(a["Children"s].GetPointer<Scope*>() == b);


			a.RemoveChild(0_z);
			Assert::AreEqual(a["Children"s].Size(), 0_z);

			GameObject* c = new GameObject();
			a.AddChild(*c);
			Assert::AreEqual(a["Children"s].Size(), 1_z);
			Assert::IsTrue(a["Children"s].GetPointer<Scope*>() == c);
			ServiceMgr::Reset();
		}

		/*void AddSizeToContainer(GameObject* object)
		{

		}*/

		TEST_METHOD(IteratingTest)
		{
			GameObject a;
			GameObject* b = new GameObject();
			GameObject* c = new GameObject();
			a.AddChild(*b);
			a.AddChild(*c);
			Assert::IsTrue(a.GetChild(0) == b);
			Assert::IsTrue(a.GetChild(1) == c);
			Assert::AreEqual(a.GetChildCount(), 2_z);

			Assert::IsTrue(a.GetChild(0)->GetChildCount() == 0_z);
			Assert::IsTrue(a.GetChild(1)->GetChildCount() == 0_z);

			a.ForEachChild([](GameObject* child) {
				GameObject* c = new GameObject();
				child->AddChild(*c);
			});
			Assert::IsTrue(a.GetChild(0)->GetChildCount() == 1_z);
			Assert::IsTrue(a.GetChild(1)->GetChildCount() == 1_z);
			ServiceMgr::Reset();
		}
		TEST_METHOD(CopyTest)
		{
			GameObject a;
			GameObject b(a);
			Assert::AreEqual(a.Size(), b.Size());

			GameObject c;
			c = a;
			Assert::AreEqual(c.Size(), a.Size());
			
			ServiceMgr::Reset();
		}
		TEST_METHOD(MoveTest)
		{
			GameObject* a = new GameObject;
			GameObject* b = new GameObject;

			GameObject c = std::move(*a);
			Assert::AreNotEqual(c.Size(), a->Size());

			GameObject d;
			d = std::move(*b);
			Assert::AreNotEqual(d.Size(), b->Size());
			delete a;
			delete b;
			ServiceMgr::Reset();
		}
		TEST_METHOD(TestActionUpdate)
		{
			GameObject Worrior;
			Worrior.Append("SwordDmg"s).SetType(Datum::Type::Float);
			Worrior["SwordDmg"s].Push<float>(1.0f);
			testAction* test1 = new testAction;
			
			ActionIncrement* swordBuff = new ActionIncrement;
			swordBuff->SetIncrement("SwordDmg"s, 2.0f);

			Worrior.AddAction(*test1);
			Worrior.AddAction(*swordBuff);
			test1->Duration = 1;
			
			Worrior.Update();
			Assert::AreEqual(Worrior["SwordDmg"s].Get<float>(), 3.0f);
			Assert::AreEqual(Worrior.GetActionCount(), 1_z);
			std::this_thread::sleep_for(std::chrono::seconds(2));
			// Update after 1 second
			Worrior.Update();
			Assert::AreEqual(Worrior.GetActionCount(), 0_z);

			ServiceMgr::Reset();
		}
	};
}
