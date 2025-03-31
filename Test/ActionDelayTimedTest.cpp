#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Action.h"
#include "FieaGameEngine/Actions/ActionIncrement.h"
#include "FieaGameEngine/Actions/DelayAction.h"
#include "FieaGameEngine/Actions/TimedAction.h"
#include "FieaGameEngine/Actions/ActionWhile.h"
#include "FieaGameEngine/GameObject.h"

using namespace Fiea::Engine::Actions;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	TEST_CLASS(ActionDelayTimedTest)
	{
	public:

		TEST_METHOD(StressTest)
		{
			GameObject Mage;
			GameObject* Health = new GameObject;
			GameObject* Mana = new GameObject;
			Mage.AddChild(*Health);
			Mage.AddChild(*Mana);

			Datum& MaxHealth = Health->Append("MaxHealth"s);
			MaxHealth.Push<float>(40.0f);

			Datum& currHealth = Health->Append("CurrentHealth"s);
			currHealth.Push<float>(20.0f);

			Datum& currMana = Mana->Append("CurrentMana"s);
			currMana.Push<float>(80.0f);

			ActionIncrement* DamageReceived = new ActionIncrement;
			DamageReceived->SetIncrement("CurrentHealth"s, -8.0f);
			
			DelayAction* GetDamage = new DelayAction;
			GetDamage->AddAction(*DamageReceived);
			GetDamage->SetUp(5000);

			ActionIncrement* HealReceived = new ActionIncrement;
			HealReceived->SetIncrement("CurrentHealth"s, 1.0f);
			
			DelayAction* DoHeal = new DelayAction;
			DoHeal->AddAction(*HealReceived);
			DoHeal->SetUp(400);

			ActionIncrement* ManaUsed = new ActionIncrement;
			ManaUsed->SetIncrement("CurrentMana"s, -8.0f);

			DelayAction* UseMana = new DelayAction;
			UseMana->AddAction(*ManaUsed);
			UseMana->SetUp(8000);
			
			TimedAction* healEffect = new TimedAction;
			healEffect->AddAction(*DoHeal);
			healEffect->SetUp(3000);

			ActionIncrement* empty = new ActionIncrement;
			ManaUsed->SetIncrement("CurrentMana"s, 0.0f);
			DelayAction* healSpell = new DelayAction;
			healSpell->AddAction(*empty);
			healSpell->SetUp(5000);

			DelayAction* healCombine = new DelayAction;
			healCombine->AddAction(*healEffect);
			healCombine->AddAction(*healSpell);
			healCombine->SetUp(0);

			/*ActionList* battleEncounter = new ActionList;
			Mage.Adopt("battleEncounter"s, *battleEncounter);

			battleEncounter->AddAction(*GetDamage);
			battleEncounter->AddAction(*healSpell);*/
			Health->AddAction(*GetDamage);
			//Health->AddAction(*healCombine);
			Health->AddAction(*healEffect);
			//Health->AddAction(*healSpell);
			Mana->AddAction(*UseMana);
			
			auto startTime = std::chrono::system_clock::now();
			while(currHealth.Get<float>() >= 0.0f)
			{
				/*battleEncounter->Update();*/
				Mage.Update();
				auto now = std::chrono::system_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
				if (elapsed > std::chrono::milliseconds(500))
				{
					LOG("Health:" << currHealth.Get<float>() << std::endl);
					LOG("Mana:" << currMana.Get<float>() << std::endl);
					startTime = std::chrono::system_clock::now();
				}
			}
			ServiceMgr::Reset();
		}
	};
}