#include "FieaGameEngine/Content/Parser.h"
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ContentService.h"
#include <iostream>
#include <fstream>

namespace Fiea::Engine::Content
{
    /// <summary>
    /// Inputs: a sequential collection of IParseHandler, Copy handlers to local Handlers
    /// </summary>
    /// <param name="handlers"></param>
    Parser::Parser(std::vector<std::unique_ptr<IParseHandler>>& handlers) : _HandlerContainer(handlers)
    {
        /*_HandlerContainer = handlers;
        _HandlerContainer.reserve(handlers.size());*/
       /* for (const auto& handler : handlers)
        {
            _HandlerContainer.push_back(handler);
        }*/
    }
    /// <summary>
    /// Deserialize a string using RecursiveParse function
    /// </summary>
    /// <param name="text"></param>
    /// <param name="writer"></param>
    /// <returns></returns>
    bool Parser::DeserializeString(std::string text, ParseWriter& writer)
    {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errors;
        std::istringstream iss(text);
        if (!Json::parseFromStream(builder, iss, &root, &errors))
        {
            FIEA_ERROR("ParseFromStream failed on type filename");
            return false;
        }
        if (RecursiveParse(root, writer)) return true;
        return false;
    }

    /// <summary>
    /// Deserialize a stream using RecursiveParse function
    /// </summary>
    /// <param name="stream"></param>
    /// <param name="writer"></param>
    /// <returns></returns>
    bool Parser::DeserializeStream(std::istream& stream, ParseWriter& writer)
    {
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errors;
        if (!Json::parseFromStream(builder, stream, &root, &errors))
        {
            FIEA_ERROR("ParseFromStream failed on type filename");
            return false;
        }
        if (RecursiveParse(root, writer)) return true;
        return false;
    }

    /// <summary>
    /// Deserialize a file using RecursiveParse function
    /// </summary>
    /// <param name="filename"></param>
    /// <param name="writer"></param>
    /// <returns></returns>
    bool Parser::DeserializeFile(string filename, ParseWriter& writer)
    {
        std::ifstream fileStream(filename);
        if (!fileStream.is_open())
        {
            FIEA_ERROR("File open failed");
            return false;
        }
        Json::Value root;
        Json::CharReaderBuilder builder;
        std::string errors;
        if (!Json::parseFromStream(builder, fileStream, &root, &errors))
        {
            FIEA_ERROR("ParseFromStream failed on type filename");
            return false;
        }
        if (RecursiveParse(root, writer)) return true;
        return false;
    }

    /// <summary>
    /// Private helper, try each handler to handle json value
    /// </summary>
    /// <param name="key"></param>
    /// <param name="writer"></param>
    /// <param name="element"></param>
    /// <returns></returns>
    bool Parser::ParseField(Json::String key, ParseWriter& writer, Json::Value element)
    {
        // Try handle for all handlers
        for (auto& _handler : _HandlerContainer)
        {
            if (_handler->EnterKey(writer, key, element))
            {
                return true;
                continue;
            }
        }
        return false;
    }
    
    /// <summary>
    /// Private helper for deserializing array, call parseField on each array element
    /// </summary>
    /// <param name="key"></param>
    /// <param name="writer"></param>
    /// <param name="element"></param>
    /// <returns></returns>
    bool Parser::ParseArray(Json::String key, ParseWriter& writer, Json::Value element)
    {
        FIEA_ASSERT(element.isArray());
        bool handleSuccess = false;
        for (Json::ArrayIndex i = 0; i < element.size(); ++i)
        {
            handleSuccess = ParseField(key, writer, element[i]);
            if (!handleSuccess) return false;
        }
        return handleSuccess;
    }

    /// <summary>
    /// Private helper for parsing object, handle self -> recursive parse data inside -> exit handler
    /// </summary>
    /// <param name="key"></param>
    /// <param name="writer"></param>
    /// <param name="element"></param>
    /// <returns></returns>
    bool Parser::ParseObject(Json::String key, ParseWriter& writer, Json::Value element)
    {
        bool handleSuccess = false;

        writer.IncrementDepth();
        for (auto& _handler : _HandlerContainer)
        {
            if (_handler->EnterKey(writer, key, element))
            {
                handleSuccess =  true;
                break;
            }
        }
        // If cant find Object handler, return false
        if (!handleSuccess) return false;

        // Recursive Parse for data within
        handleSuccess = RecursiveParse(element, writer);
        writer.DecrementDepth();
        if (!handleSuccess) return false;
        // Try to exit key
        for (auto& _handler : _HandlerContainer)
        {
            if (_handler->ExitKey(writer, key, element))
            {
                handleSuccess = true;
                break;
            }
        }
        return handleSuccess;
    }

    /// <summary>
    /// Private helper for parsing, iterate each json value and determine each's value type, then call
    /// different type's parse helper
    /// </summary>
    /// <param name="root"></param>
    /// <param name="writer"></param>
    /// <returns></returns>
    bool Parser::RecursiveParse(const Json::Value& root, ParseWriter& writer)
    {
        // Iterate over each on this level, recusive call if find scope
        for (const auto& key : root.getMemberNames())
        {
            const Json::Value& element = root[key];
            bool handleSuccess = false;
            if (element.isObject())
            {
                handleSuccess = ParseObject(key, writer, element);
            }
            else if (element.isArray())
            {
                handleSuccess = ParseArray(key, writer, element);
            }
            else {
                // Try handle for all handlers
                handleSuccess = ParseField(key, writer, element);
            }
            // Check if the element is handled successively
            if (!handleSuccess) return false;
        }
        return true;
    }

}
