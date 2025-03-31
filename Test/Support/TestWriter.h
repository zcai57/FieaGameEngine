#pragma once

#include "FieaGameEngine/ContentService.h"
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Test::Support
{
    class TestWriter : public ParseWriter
    {
        RTTI_DECLARATIONS(TestWriter, ParseWriter);
        /// These are the endpoints for "writing"
        //    RecordInt    // No params, just keep track of how many integer values are encountered at each depth
        //    RecordFloat  // No params, just keep track of how many float values are encountered at each depth
        //    RecordString // No params, just keep track of how many string values are encountered at each depth
        //    // These are the endpoints for reading the results, after deserialization
        //    CountInts    // Input: depth, Output: number of integers encountered at that depth
        //    CountFloats  // Input: depth, Output: number of floats encountered at that depth
        //    CountString  // Input: depth, Output: number of strings encountered at that depth
    public:
        TestWriter();
        ~TestWriter();
        void RecordInt();
        void RecordFloat();
        void RecordString();
        size_t CountInts(size_t depth);
        size_t CountFloats(size_t depth);
        size_t CountString(size_t depth);
    private:
        std::vector<int32_t> intCounter;
        std::vector<int32_t> floatCounter;
        std::vector<int32_t> stringCounter;
    };

    class IntParserHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
    class FloatParserHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
    class StringParserHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
    class ObjectParseHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
}


