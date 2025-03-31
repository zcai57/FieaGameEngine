#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"
#include "FieaGameEngine/GameObject.h"

using namespace Fiea::Engine::Actions;
using namespace Fiea::Engine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	TEST_CLASS(ActionIncrementTest)
	{
		TEST_MEMCHECK;
	public:
		TEST_METHOD(UpdateTest)
		{
			GameObject Worrior;
			
			GameObject* Sword = new GameObject;
			Worrior.AddChild(*Sword);
			
			Datum& swordDmg = Sword->Append("SwordDmg"s);
			swordDmg.Push<float>(1.0f);
			Assert::IsTrue(swordDmg.IsType(Datum::Type::Float));
			Assert::AreEqual(swordDmg.Get<float>(), 1.0f);

			ActionIncrement* swordDmgBuff = new ActionIncrement;
			swordDmgBuff->SetIncrement("SwordDmg"s, 2.0f);
			//swordDmgBuff->Key = "SwordDmg"s;
			//swordDmgBuff->Amount = 2.0f;
			Scope* scope = swordDmgBuff;
			Sword->Adopt("SwordDmgBuff"s, *scope);

            bool dmgBuffed = swordDmgBuff->Update();
            Assert::IsTrue(dmgBuffed);
            Assert::AreEqual(swordDmg.Get<float>(), 3.0f);
			ServiceMgr::Reset();
		}
	};
}
