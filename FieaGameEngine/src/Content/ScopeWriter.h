#pragma once
#include "FieaGameEngine/ContentService.h"
namespace Fiea::Engine::Content {
    class ScopeWriter : public ParseWriter
    {
    public:
        ScopeWriter();
        ~ScopeWriter() = default;
        void BeginWrite(Scope*);
        void EndWrite();
        Scope* GetCurrentScope();
        void writeInt(Json::String, Json::Value);
        void writeFloat(Json::String, Json::Value);
        void writeString(Json::String, Json::Value);
    private:
        Scope* _CurrentTarget;
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
    class ObjectParserHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
    /*class GameObjChidrenHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };*/
    class GameObjActionHandler : public IParseHandler
    {
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) override;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) override;
    };
}


