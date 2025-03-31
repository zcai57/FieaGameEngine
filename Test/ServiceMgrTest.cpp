#include "CppUnitTest.h"
#include "Tests.h"
#include "Support/FooManager.h"
#include "Support/StringManager.h"
#include "FieaGameEngine/ClockService.h"
#include "FieaGameEngine/ServiceMgr.h"

using namespace Fiea::Engine::Clock;
using namespace Fiea::Engine::Test::Support;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	TEST_CLASS(ServiceProviderTest)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(FooMgrTest)
		{
			ServiceProvider fooMakerProvider = GetFooMakerProvider();
			ServiceProvider fooTakerProvider = GetFooTakerProvider();
			void* ptr = fooMakerProvider.Create();
			IFooMaker* fooMaker = reinterpret_cast<IFooMaker*>(fooMakerProvider.Provide(ptr));
			fooMaker->Make("hello", 29);
			IFooTaker* fooTaker = reinterpret_cast<IFooTaker*>(fooTakerProvider.Provide(ptr));
			int32_t testInt = fooTaker->Take("hello");
			Assert::IsTrue(testInt == 29);
			fooMakerProvider.Destroy(ptr);
			ServiceMgr::Reset();
			//ServiceMgr::Destroy();
		}
		TEST_METHOD(ServiceMgrTest)
		{
			//ServiceMgr::Create();
			IFooMaker* fooMaker = ServiceMgr::ProvideInterface<IFooMaker>();
			IFooTaker* fooTaker = ServiceMgr::ProvideInterface<IFooTaker>();
			fooMaker->Make("hello", 29);
			int32_t testInt = fooTaker->Take("hello");
			Assert::IsTrue(testInt == 29);
			ServiceMgr::Reset();
			//ServiceMgr::Destroy();
		}

		TEST_METHOD(MacroStringMgrTest)
		{
			INameGiver* nameGiver = ServiceMgr::ProvideInterface<INameGiver>();
			Assert::IsTrue("Name: Hans" == nameGiver->GiveName("Hans"));
			ServiceMgr::Reset();
		}

		TEST_METHOD(ClockServiceTest)
		{
			ITimeProvider* timeProvider = ServiceMgr::ProvideInterface<ITimeProvider>();
			ITimeUpdater* timeUpdater = ServiceMgr::ProvideInterface<ITimeUpdater>();
			timeProvider->Time();
			timeUpdater->Update();
			timeUpdater->Update(100);
			Assert::IsTrue((uint32_t)100 == timeUpdater->Update(200));
			ServiceMgr::Reset();
		}
	};
}