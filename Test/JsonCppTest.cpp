#include "Tests.h"
#include "json/json.h"

#include <iostream>

namespace Fiea::Engine::Tests
{
    using namespace std::string_literals;

    bool DeserializeJson(std::istream& jsonStream, Json::Value& root)
    {
        try {
            jsonStream >> root;
        } catch (const Json::RuntimeError& e) {
            // parse error
            LOG("Parse error: " << e.what() << std::endl);
            return false;
        }
        return true;
    }

    TEST_CLASS(JsonCppTest)
    {
        TEST_MEMCHECK;

    public:
        TEST_METHOD(BasicTest)
        {
            std::istringstream simple(R"({
                "int":1,
                "float":2.0,
                "str":"abc"
            })"s);

            Json::Value root;

            // deserialize some json
            bool success = DeserializeJson(simple, root);
            Assert::IsTrue(success);
            Assert::IsTrue(root.isObject());
            Assert::AreEqual(3u, root.size());

            // testing isMember methods
            Assert::IsTrue(root.isMember("int"));
            Assert::IsTrue(root.isMember("float"));
            Assert::IsTrue(root.isMember("str"));
            Assert::IsFalse(root.isMember("other"));

            // testing operator[] for objects
            Assert::IsTrue(root["int"].isInt());
            Assert::IsTrue(root["float"].isDouble());
            Assert::IsTrue(root["str"].isString());

            // testing as*
            Assert::AreEqual(1, root["int"].asInt());
            Assert::AreEqual(2.0f, root["float"].asFloat());
            Assert::AreEqual("abc"s, root["str"].asString());

            // testing as<>
            Assert::AreEqual(1, root["int"].as<int32_t>());
            Assert::AreEqual(2.0f, root["float"].as<float>());
            Assert::AreEqual("abc"s, root["str"].as<string>());

            // testing iterator syntax and streamed output
            LOG("Json fields" << std::endl);
            for (auto it = root.begin(); it != root.end(); ++it)
            {
                const string& key = it.name();
                const Json::Value& value = *it;
                LOG("  " << key << ": " << value << std::endl);
            }
        }

        TEST_METHOD(NestedTest)
        {
            std::istringstream nested(R"({
                "int":1,
                "child":{
                    "int":2
                }
            })"s);

            Json::Value root;

            // deserialize
            bool success = DeserializeJson(nested, root);
            Assert::IsTrue(success);
            Assert::AreEqual(2u, root.size());

            // validate int field
            Assert::IsTrue(root["int"].isInt());
            Assert::AreEqual(1, root["int"].as<int32_t>());

            // validate and acquire child object
            Assert::IsTrue(root["child"].isObject());
            Json::Value& child = root["child"];
            Assert::AreEqual(1u, child.size());

            // check values inside child object
            Assert::IsTrue(child["int"].isInt());
            Assert::AreEqual(2, child["int"].as<int32_t>());
        }

        TEST_METHOD(ArrayTest)
        {
            std::istringstream array(R"({
                "ints":[
                    1,
                    2,
                    3
                ]
            })"s);

            Json::Value root;

            // deserialize
            bool success = DeserializeJson(array, root);
            Assert::IsTrue(success);
            Assert::AreEqual(1u, root.size());

            // confirm structure
            Assert::IsTrue(root["ints"].isArray());
            Json::Value& vals = root["ints"];
            Assert::AreEqual(3u, vals.size());

            // use operator[] for arrays
            Assert::AreEqual(1, vals[0].as<int32_t>());
            Assert::AreEqual(2, vals[1].as<int32_t>());
            Assert::AreEqual(3, vals[2].as<int32_t>());

            // use iterator syntax (with ranged-for)
            int i = 1;
            for (auto val : vals)
            {
                Assert::AreEqual(i++, val.as<int32_t>());
            }

            // demonstrate "accidental" growth
            Json::Value& extra = vals[3];
            Assert::IsTrue(extra.isNull());
            Assert::AreEqual(4u, vals.size());
        }

        TEST_METHOD(BrokenTest)
        {
            Json::Value root;

            std::istringstream missingQuotes(R"({int:5})"s);
            Assert::IsFalse(DeserializeJson(missingQuotes, root));

            std::istringstream mismatchBraces(R"({"int":5)"s);
            Assert::IsFalse(DeserializeJson(mismatchBraces, root));

            std::istringstream hangingComma(R"({
                "int":1,
            })"s);
            Assert::IsFalse(DeserializeJson(hangingComma, root));

            // this isn't technically broken, but may not produce the expected output
            // note: it is compliant w/ the json standard, though! see discussion at
            //   https://stackoverflow.com/questions/21832701/does-json-syntax-allow-duplicate-keys-in-an-object
            std::istringstream dupeKey(R"({
                "int":1,
                "int":2
            })"s);
            Assert::IsTrue(DeserializeJson(dupeKey, root));
            Assert::AreEqual(1u, root.size());
            Assert::AreEqual(2, root["int"].as<int32_t>());

            std::istringstream arrayConfusion(R"({"ints":["i":1, 5)"s);
            Assert::IsFalse(DeserializeJson(mismatchBraces, root));
        }

        TEST_METHOD(NonEmptyRoot)
        {
            std::istringstream additional(R"({
                "int":1
            })"s);

            Json::Value root;
            root["stuff"] = "stuff";
            Assert::AreEqual(1u, root.size());

            // deserialize, verify that prior contents are cleared
            bool success = DeserializeJson(additional, root);
            Assert::IsTrue(success);
            Assert::AreEqual(1u, root.size());
            Assert::IsFalse(root.isMember("stuff"));
            Assert::IsTrue(root.isMember("int"));

            // clear EOF, seek the beginning of the stream, and deserialize AGAIN
            additional.clear();
            additional.seekg(0);
            success = DeserializeJson(additional, root);
            Assert::IsTrue(success);
            Assert::AreEqual(1u, root.size());
            Assert::IsFalse(root.isMember("stuff"));
            Assert::IsTrue(root.isMember("int"));
        }
    };
}