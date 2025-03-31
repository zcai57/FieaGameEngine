#pragma once
#include "RTTI.h"
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ServiceMgr.h"
#include "Content/Attributed.h"
#include "Content/Datum.h"
#include "Content/Scope.h"
#include "json/json.h"

namespace Fiea::Engine::Content
{
    //ParseWriter - abstract base class
    //    No current need to support copy or move
    //    Derive from RTTI(we'll need to do some explicit downcasting from our handlers)
    //        This is a base class that we can extend depending on the purpose of the json file being parsed
    //        IncrementDepth // increment a “nesting depth” counter. Called from Parser, as needed, when entering a nested json object
    //        DecrementDepth // decrement a “nesting depth” counter. Called from Parser, as needed, when leaving a nested json object
    //        Depth          // return the current nesting depth
    //        Hint : What qualifier should the destructor have ?
    //        Hint : No pure virtual methods... how to make sure the class is abstract ?
    class ParseWriter : public RTTI
    {
        RTTI_DECLARATIONS(ParseWriter, RTTI);
    public:
        // increment a “nesting depth” counter. Called from Parser, as needed, when entering a nested json object
        void IncrementDepth();
        // DecrementDepth
        void DecrementDepth();
        // return the current nesting depth
        size_t Depth();
    protected:
        ParseWriter() = default;
        size_t _NestingDepth;        
    };


    /// <summary>
    /// IPareseHandler interface
    /// </summary>
    struct IParseHandler
    {
        // initialization code, when beginning to process a new json file, if any. consider empty "default" implementation?
        virtual void Begin() {};
        // cleanup code, when beginning to process a new json file, if any. consider empty "default" implementation?
        virtual void End() {};
        //Inputs: a ParseWriter, a string for the Json key, a Json::Value associated with that key
        // Outputs: a value to let the Parser know whether the key was processed by this handler
        // Note: you may add/change params, if needed... 
        virtual bool EnterKey(ParseWriter&, Json::String, Json::Value) = 0;
        virtual bool ExitKey(ParseWriter&, Json::String, Json::Value) = 0;
    };
    ///ITypeRegistrar - endpoint(s) required to register a ClassDefinition by Type Id(RTTI)
    struct ITypeRegistrar
    {
        FIEA_SERVICE_INTERFACE;
        virtual void RegisterClassDef(const Attributed::ClassDef, RTTI::IdType id) = 0;
    };

    ///ITypeProvider - endpoint(s) required to retrieve a ClassDefinition by Type Id(RTTI)
    struct ITypeProvider
    {
        FIEA_SERVICE_INTERFACE;
        virtual const Attributed::ClassDef* ProvideClassDef(RTTI::IdType id) = 0;
    };

    /// <summary>
    /// IJsonConfig 
    /// </summary>
    struct IJsonConfig
    {
        FIEA_SERVICE_INTERFACE;
        //AddHandler    // register a handler to be considered during parsing
        //      // consider whether calling code can be relied upon to add handlers in the necessary processing order,
        //      // or whether some form of prioritization makes sense?
        //    RemoveHandler // remove a registered handler
        virtual size_t AddHandler(std::unique_ptr<IParseHandler>, size_t priority = 1) = 0;
        virtual void RemoveHandler(size_t) = 0;
    };

    /// <summary>
    /// IJsonParser
    /// </summary>
    struct IJsonParser
    {
        FIEA_SERVICE_INTERFACE;
        virtual bool ParseString(string, ParseWriter&) = 0;
        virtual bool ParseStream(std::istream&, ParseWriter&) = 0;
        virtual bool ParseFile(string, ParseWriter&) = 0;
    };

    class ContentService : public ITypeRegistrar, public ITypeProvider, public IJsonConfig, public IJsonParser
    {
    public:
        virtual void RegisterClassDef(const Attributed::ClassDef, RTTI::IdType id) override;
        virtual const Attributed::ClassDef* ProvideClassDef(RTTI::IdType id) override;
        virtual size_t AddHandler(std::unique_ptr<IParseHandler>, size_t priority = 1) override;
        virtual void RemoveHandler(size_t) override;
        virtual bool ParseString(string, ParseWriter&) override;
        virtual bool ParseStream(std::istream&, ParseWriter&) override;
        virtual bool ParseFile(string, ParseWriter&) override;

    private:
        static bool _Registration();
        static inline bool _registered = _Registration();
        std::unordered_map<RTTI::IdType, const Attributed::ClassDef> _cachedClassDef;
        std::vector<std::unique_ptr<IParseHandler>> _HandlerContainer;
    };
}

