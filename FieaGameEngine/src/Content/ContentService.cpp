#include "FieaGameEngine/ContentService.h"
#include "FieaGameEngine/ServiceMgr.h"
#include "FieaGameEngine/Content/Parser.h"

namespace Fiea::Engine::Content
{
    class Parser;
   
    FIEA_SERVICE_BIND4(ContentService, ITypeRegistrar, ITypeProvider, IJsonParser, IJsonConfig);
    /// <summary>
    /// Register ClassDef into Service's map
    /// </summary>
    /// <param name="id"></param>
    void ContentService::RegisterClassDef(const Attributed::ClassDef def, RTTI::IdType id) {
        // If not found ClassDef, insert one
        if (_cachedClassDef.find(id) == _cachedClassDef.end())
        {
            _cachedClassDef.insert({id, def});
        }
    }

    /// <summary>
    /// Getter for ClassDef
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    const Attributed::ClassDef* ContentService::ProvideClassDef(RTTI::IdType id)
    {
        if (_cachedClassDef.find(id) != _cachedClassDef.end())
        {
            return &(_cachedClassDef.at(id));
        }
        return nullptr;
    }

    size_t ContentService::AddHandler(std::unique_ptr<IParseHandler> handler, size_t priority)
    {
        if (priority == (size_t)1)
        {
            _HandlerContainer.push_back(std::move(handler));
        }
        else {
            // if priority is 0, high priorty, insert from front
            _HandlerContainer.insert(_HandlerContainer.begin(), std::move(handler));
        }
        return _HandlerContainer.size() - 1;
    }

    void ContentService::RemoveHandler(size_t index)
    {
        _HandlerContainer.erase(_HandlerContainer.begin() + index);
    }

    bool ContentService::ParseString(string text, ParseWriter& writer)
    {
        Parser parser(_HandlerContainer);
        return parser.DeserializeString(text, writer);
        //return false;
    }

    bool ContentService::ParseStream(std::istream& stream, ParseWriter& writer)
    {
        Parser parser(_HandlerContainer);
        return parser.DeserializeStream(stream, writer);
        //return false;
    }

    bool ContentService::ParseFile(string filename, ParseWriter& writer)
    {
        Parser parser(_HandlerContainer);
        return parser.DeserializeFile(filename, writer);
        //return false;
    }
    
}
