#include "TestWriter.h"
namespace Fiea::Engine::Test::Support
{
    RTTI_DEFINITIONS(TestWriter);
    TestWriter::TestWriter()
    {
        _NestingDepth = 0;
    }
    TestWriter::~TestWriter()
    {
    }
    void TestWriter::RecordInt()
    {
        if (_NestingDepth >= intCounter.size()) {
            intCounter.resize(_NestingDepth + 1);
            intCounter[_NestingDepth] = 1;
        }
        else {
            intCounter[_NestingDepth] += 1;
        }
    }
    void TestWriter::RecordFloat()
    {
        if (_NestingDepth >= floatCounter.size()) {
            floatCounter.resize(_NestingDepth + 1);
            floatCounter[_NestingDepth] = 1;
        }
        else {
            floatCounter[_NestingDepth] += 1;
        }
    }
    void TestWriter::RecordString()
    {
        if (_NestingDepth >= stringCounter.size()) {
            stringCounter.resize(_NestingDepth + 1);
            stringCounter[_NestingDepth] = 1;
        }
        else {
            stringCounter[_NestingDepth] += 1;
        }
    }
    size_t TestWriter::CountInts(size_t depth)
    {
        if (depth >= intCounter.size())
        {
            return 0;
        }
        return intCounter[depth];
    }
    size_t TestWriter::CountFloats(size_t depth)
    {
        if (depth >= floatCounter.size())
        {
            return 0;
        }
        return floatCounter[depth];
    }
    size_t TestWriter::CountString(size_t depth)
    {
        if (depth >= stringCounter.size())
        {
            return 0;
        }
        return stringCounter[depth];
    }
    bool IntParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isInt())
        {
            TestWriter* tWriter = writer.As<TestWriter>();
            tWriter->RecordInt();
            return true;
        }
        return false;
    }
    bool IntParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return true;
    }
    bool FloatParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isDouble())
        {
            TestWriter* tWriter = writer.As<TestWriter>();
            tWriter->RecordFloat();
            return true;
        }
        return false;
    }
    bool FloatParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return true;
    }
    bool StringParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isString())
        {
            TestWriter* tWriter = writer.As<TestWriter>();
            tWriter->RecordString();
            return true;
        }
        return false;
    }
    bool StringParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return true;
    }
    bool ObjectParseHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isObject()) return true;
        return false;
    }
    bool ObjectParseHandler::ExitKey(ParseWriter&, Json::String, Json::Value)
    {
        return true;
    }
}
