#pragma once
#include "FieaGameEngine/ContentService.h"
#include "Parser.h"
namespace Fiea::Engine::Content
{
    class Parser final
    {
    public:
        // Inputs: a sequential collection of IParseHandler
        Parser(std::vector<std::unique_ptr<IParseHandler>>&);
        // Inputs: a string of json text, and a ParseWriter
        bool DeserializeString(std::string text, ParseWriter& writer);
        // Inputs: a std::istream to read json from, and a ParseWriter
        bool DeserializeStream(std::istream&, ParseWriter& writer);
        // Inputs: a string containing a path to a file containing json text, and a ParseWriter
        bool DeserializeFile(string, ParseWriter& writer);
    private:
        bool ParseField(Json::String key, ParseWriter& writer, Json::Value element);
        //void ParseObject();
        bool ParseArray(Json::String key, ParseWriter& writer, Json::Value element);
        bool ParseObject(Json::String key, ParseWriter& writer, Json::Value element);
        bool RecursiveParse(const Json::Value&, ParseWriter& writer);
        std::vector<std::unique_ptr<IParseHandler>> &_HandlerContainer;
    };

}


