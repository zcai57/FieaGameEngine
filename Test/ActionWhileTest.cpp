#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Actions/ActionWhile.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"
#include "FieaGameEngine/Actions/ActionWhileIncrement.h"
#include "FieaGameEngine/Action.h"
#include "FieaGameEngine/ActionList.h"
#include "FieaGameEngine/GameObject.h"

using namespace Fiea::Engine::Actions;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	TEST_CLASS(ActionWhileTest)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(ActionWhileTest1)
		{
			ActionWhile* actionWhile = new ActionWhile;
			GameObject elf;
			elf.Append("SwordDmg"s).SetType(Datum::Type::Float);
			elf["SwordDmg"s].Push<float>(2.0f);
			elf.Append("BowDmg"s).SetType(Datum::Type::Float);
			elf["BowDmg"s].Push<float>(1.0f);
			elf.Append("Health"s).SetType(Datum::Type::Float);
			elf["Health"s].Push<float>(30.0f);
			elf.Adopt("ActionWhile"s, *actionWhile);

			ActionList* buffActions = new ActionList;

			ActionIncrement* buffSword = new ActionIncrement;
			buffSword->SetIncrement("SwordDmg"s, 2.0f);
			ActionIncrement* buffBow = new ActionIncrement;
			buffBow->SetIncrement("BowDmg"s, 3.0f);

			ActionIncrement* healthBuff = new ActionIncrement;
			healthBuff->SetIncrement("Health"s, 10.0f);

			buffActions->AddAction(*buffSword);
			buffActions->AddAction(*buffBow);

			actionWhile->SetCondition(2, 1, healthBuff, buffActions);
			actionWhile->Update();

			Assert::AreEqual(elf["SwordDmg"s].Get<float>(), 6.0f);
			Assert::AreEqual(elf["BowDmg"s].Get<float>(), 7.0f);
			Assert::AreEqual(elf["Health"s].Get<float>(), 40.0f);
			
			ServiceMgr::Reset();
		}
	};
}

