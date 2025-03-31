#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/GameObject.h"
#include "FieaGameEngine/ActionList.h"
#include "FieaGameEngine/Action.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"

using namespace Fiea::Engine::Actions;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	TEST_CLASS(ActionListTest)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(ActionIncrementTest)
		{
			GameObject elf;
			elf.Append("SwordDmg"s).SetType(Datum::Type::Float);
			elf["SwordDmg"s].Push<float>(2.0f);
			elf.Append("BowDmg"s).SetType(Datum::Type::Float);
			elf["BowDmg"s].Push<float>(1.0f);

			ActionList buffActions;
			elf.Adopt("debuffAction"s, buffActions);
			
			ActionIncrement* buffSword = new ActionIncrement;
			buffSword->SetIncrement("SwordDmg"s, 2.0f);

			ActionIncrement* buffBow = new ActionIncrement;
			buffBow->SetIncrement("BowDmg"s, 3.0f);

			buffActions.AddAction(*buffSword);
			buffActions.AddAction(*buffBow);

			buffActions.Update();
			Assert::AreEqual(elf["SwordDmg"s].Get<float>(), 4.0f);
			Assert::AreEqual(elf["BowDmg"s].Get<float>(), 4.0f);

			ServiceMgr::Reset();
		}
	};
}
