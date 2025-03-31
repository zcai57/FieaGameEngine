#include "CppUnitTest.h"
#include "Tests.h"
#include "Support/TestWriter.h"
#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/Content/Parser.h"
#include <fstream>

using namespace Fiea::Engine::Content;
using namespace Fiea::Engine::Test::Support;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using namespace std::string_literals;
	TEST_CLASS(ParserTest)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(JsonParseTest)
		{
			// Parse String Test
			{
				TestWriter myWriter;
				std::vector<std::unique_ptr<IParseHandler>> myHandlers;
				myHandlers.push_back(std::make_unique<IntParserHandler>());
				myHandlers.push_back(std::make_unique<FloatParserHandler>());
				myHandlers.push_back(std::make_unique<StringParserHandler>());
				myHandlers.push_back(std::make_unique<ObjectParseHandler>());

				Parser parser(myHandlers);
				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				bool success = parser.DeserializeString(jsonStr, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));

				// Put in 1 int at level 2
				string jsonStr1 = R"({"value4": {"val12": { "value111": 31}}})"s;
				bool success1 = parser.DeserializeString(jsonStr1, myWriter);
				Assert::IsTrue(success1);
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(1_z, myWriter.CountInts(2));

				// Put in 1 float and 1 string at level 1
				string jsonStr2 = R"({"value5": {"float": 2.01, "string": "Hello"}})"s;
				bool success2 = parser.DeserializeString(jsonStr2, myWriter);
				Assert::IsTrue(success2);
				Assert::AreEqual(1_z, myWriter.CountFloats(1));
				Assert::AreEqual(1_z, myWriter.CountString(1));

			}
			// Parse istream test
			{
				TestWriter myWriter;
				std::vector<std::unique_ptr<IParseHandler>> myHandlers;
				myHandlers.push_back(std::make_unique<IntParserHandler>());
				myHandlers.push_back(std::make_unique<FloatParserHandler>());
				myHandlers.push_back(std::make_unique<StringParserHandler>());
				myHandlers.push_back(std::make_unique<ObjectParseHandler>());

				Parser parser(myHandlers);
				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				std::istringstream iss(jsonStr);
				bool success = parser.DeserializeStream(iss, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));
			}
			// Parse from file test
			{
				TestWriter myWriter;

				std::vector<std::unique_ptr<IParseHandler>> myHandlers;
				myHandlers.push_back(std::make_unique<IntParserHandler>());
				myHandlers.push_back(std::make_unique<FloatParserHandler>());
				myHandlers.push_back(std::make_unique<StringParserHandler>());
				myHandlers.push_back(std::make_unique<ObjectParseHandler>());

				Parser parser(myHandlers);
				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				std::string filename = "C:/Dev/Prog2/FieaGameEngine/Test/ParserTest.json";
				std::ofstream outFile(filename);
				outFile << jsonStr; 
				outFile.close();
				bool success = parser.DeserializeFile(filename, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));
			}
		}
		TEST_METHOD(ContentServiceJsonTest)
		{
			{
				TestWriter myWriter;

				IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
				IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();

				configService->AddHandler(std::make_unique<IntParserHandler>());
				configService->AddHandler(std::make_unique<FloatParserHandler>());
				configService->AddHandler(std::make_unique<StringParserHandler>());
				configService->AddHandler(std::make_unique<ObjectParseHandler>());

				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				bool success = parserService->ParseString(jsonStr, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));

				// Put in 1 int at level 2
				string jsonStr1 = R"({"value4": {"val12": { "value111": 31}}})"s;
				bool success1 = parserService->ParseString(jsonStr1, myWriter);
				Assert::IsTrue(success1);
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(1_z, myWriter.CountInts(2));

				// Put in 1 float and 1 string at level 1
				string jsonStr2 = R"({"value5": {"float": 2.01, "string": "Hello"}})"s;
				bool success2 = parserService->ParseString(jsonStr2, myWriter);
				Assert::IsTrue(success2);
				Assert::AreEqual(1_z, myWriter.CountFloats(1));
				Assert::AreEqual(1_z, myWriter.CountString(1));

				// Test handler erase and insert with Priority
				string jsonStr3 = R"({"value5": {"float": 2.0, "string": "Hello"}})"s;
				bool success3 = parserService->ParseString(jsonStr3, myWriter);
				Assert::IsTrue(success3);
				Assert::AreEqual(2_z, myWriter.CountInts(1));

				configService->RemoveHandler(1);
				// Add high priority float parser
				configService->AddHandler(std::make_unique<FloatParserHandler>(), 0);
				bool success4 = parserService->ParseString(jsonStr3, myWriter);
				Assert::IsTrue(success4);
				Assert::AreEqual(2_z, myWriter.CountInts(1));
				Assert::AreEqual(2_z, myWriter.CountFloats(1));
				// Remove high priority floa parser
				configService->RemoveHandler(0);
			}

			// Parse istream test
			{
				TestWriter myWriter;
				IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
				IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();

				configService->AddHandler(std::make_unique<FloatParserHandler>());

				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				std::istringstream iss(jsonStr);
				bool success = parserService->ParseStream(iss, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));
			}
			// Parse from file test
			{
				TestWriter myWriter;

				IJsonConfig* configService = ServiceMgr::ProvideInterface<IJsonConfig>();
				IJsonParser* parserService = ServiceMgr::ProvideInterface<IJsonParser>();

				string jsonStr = R"({ "val1":1, "val2":2, "val3":{ "val11":29 }})"s;
				std::string filename = "C:/Dev/Prog2/FieaGameEngine/Test/ParserTest.json";
				std::ofstream outFile(filename);
				outFile << jsonStr;
				outFile.close();
				bool success = parserService->ParseFile(filename, myWriter);
				Assert::IsTrue(success);
				Assert::AreEqual(2_z, myWriter.CountInts(0));
				Assert::AreEqual(1_z, myWriter.CountInts(1));
				Assert::AreEqual(0_z, myWriter.CountInts(2));
				
			}
			ServiceMgr::Reset();
		}
	};
}
