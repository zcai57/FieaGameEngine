#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Types.h"
#include "Support/ScopedFoo.h"
#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/FactoryService.h"
#include "../src/Content/ScopeWriter.h"
#include "FieaGameEngine/GameObject.h"

using namespace Fiea::Engine::Content;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	using namespace Support;
	TEST_CLASS(ScopeWriterClass)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(ParseTest)
		{
			ScopeWriter writer;
			Scope root;
			IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
			IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();

			configService->AddHandler(std::make_unique<IntParserHandler>());
			configService->AddHandler(std::make_unique<FloatParserHandler>());
			configService->AddHandler(std::make_unique<StringParserHandler>());
			configService->AddHandler(std::make_unique<ObjectParserHandler>());

			IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopeFactory>())));

			string jsonStr = R"({ "val1":1, "val2":2.01, "<Scope>val3":{ "val11":29 }, "val4": "Hello"})"s;
			writer.BeginWrite(&root);
			bool success = parserService->ParseString(jsonStr, writer);
			Assert::IsTrue(success);
			Assert::AreEqual(root.Size(), 4_z);
			// Check int, float, string type
			Assert::AreEqual(root.Find("val1")->Get<int32_t>(0), 1);
			Assert::AreEqual(root.Find("val2")->Get<float>(), 2.01f);
			Assert::AreEqual(root.Find("val4")->Get<string>(), "Hello"s);
			// Check nested child
			Assert::IsNotNull(root.Find("val3"));
			Scope* rootChild = root["val3"].GetPointer<Scope*>();
			Assert::IsNotNull(rootChild->Find("val11"));
			Assert::AreEqual(rootChild->Find("val11")->Get<int32_t>(0), 29);
			// Test array
			string jsonStr1 = R"({"val1":[2,3,4]})"s;
			bool success1 = parserService->ParseString(jsonStr1, writer);
			Assert::IsTrue(success1);
			Assert::AreEqual(root.Find("val1")->Get<int32_t>(0), 1);
			Assert::AreEqual(root.Find("val1")->Get<int32_t>(1), 2);
			Assert::AreEqual(root.Find("val1")->Get<int32_t>(2), 3);
			Assert::AreEqual(root.Find("val1")->Get<int32_t>(3), 4);

			// End write to write to another scope
			Scope root2;
			writer.EndWrite();
			writer.BeginWrite(&root2);
			string jsonStr2 = R"({"val5":[2.01,3.01,4.01]})"s;
			bool success2 = parserService->ParseString(jsonStr2, writer);
			Assert::IsTrue(success2);
			Assert::AreEqual(root.Size(), 4_z);
			Assert::AreEqual(root2.Size(), 1_z);
			Assert::AreEqual(root2.Find("val5")->Get<float>(0), 2.01f);
			Assert::AreEqual(root2.Find("val5")->Get<float>(1), 3.01f);
			Assert::AreEqual(root2.Find("val5")->Get<float>(2), 4.01f);

			ServiceMgr::Reset();
			//delete rootChild;
		}
		TEST_METHOD(ParserTypeTest)
		{
			ScopeWriter writer;
			Scope root;
			IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
			IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();
			size_t id1 = Scope::TypeIdClass();
			size_t id2 = ScopedFoo::TypeIdClass();

			configService->AddHandler(std::make_unique<IntParserHandler>());
			configService->AddHandler(std::make_unique<FloatParserHandler>());
			configService->AddHandler(std::make_unique<StringParserHandler>());
			configService->AddHandler(std::make_unique<ObjectParserHandler>());

			IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopeFactory>())));
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopedFooFactory>())));


			string jsonStr = R"({ "<Scope>val1": { "val2": 2.01 }, "<ScopedFoo>val3":{ "val11":29 }})"s;

			writer.BeginWrite(&root);
			bool success = parserService->ParseString(jsonStr, writer);
			Assert::IsTrue(success);
			Scope* val1 = root["val1"s].GetPointer<Scope*>();
			Scope* val2 = root["val3"s].GetPointer<Scope*>();
			Assert::AreEqual(val1->TypeIdInstance(), Scope::TypeIdClass());
			Assert::AreNotEqual(val1->TypeIdInstance(), ScopedFoo::TypeIdClass());

			Assert::AreEqual(val2->TypeIdInstance(), ScopedFoo::TypeIdClass());
			Assert::AreNotEqual(val2->TypeIdInstance(), Scope::TypeIdClass());

			Assert::AreEqual(val1->Find("val2"s)->Get<float>(), 2.01f);
			Assert::AreEqual(val2->Find("val11"s)->Get<int32_t>(), 29);

			ServiceMgr::Reset();
		/*	delete val1;
			delete val2;*/
		}
		TEST_METHOD(GameObjectParseTest)
		{
			ScopeWriter writer;
			GameObject root;
			IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
			IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();
			size_t id1 = Scope::TypeIdClass();
			size_t id2 = ScopedFoo::TypeIdClass();

			configService->AddHandler(std::make_unique<IntParserHandler>());
			configService->AddHandler(std::make_unique<FloatParserHandler>());
			configService->AddHandler(std::make_unique<StringParserHandler>());
			configService->AddHandler(std::make_unique<ObjectParserHandler>());

			IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopeFactory>())));
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ScopedFooFactory>())));
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<GameObjectFactory>())));
			fs->Register(std::unique_ptr<IAbstractFactory<Scope>>(std::move(std::make_unique<ActionFactory>())));
			string jsonStr = R"({ "<GameObject>val1": { "val2": 2.01 }, "<Action>val4": { "val12": 3}})"s;
			writer.BeginWrite(&root);

			bool success = parserService->ParseString(jsonStr, writer);
			Assert::IsTrue(success);
			Scope* val1 = root["val1"s].GetPointer<Scope*>();
			Assert::IsTrue(val1->TypeIdInstance() == GameObject::TypeIdClass());

			Assert::AreEqual(reinterpret_cast<GameObject*>(&root)->GetActionCount(), 1_z);
			//Assert::AreEqual(reinterpret_cast<GameObject*>(&root)->GetChildCount(), 1_z);

			ServiceMgr::Reset();
		}
	};
}