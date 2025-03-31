#include "ScopeWriter.h"
#include <regex>
#include "FieaGameEngine/FactoryService.h"
#include "FieaGameEngine/GameObject.h"
#include "FieaGameEngine/Action.h"

namespace Fiea::Engine::Content
{
    /// <summary>
    /// ScopeWriter constructor
    /// </summary>
    Fiea::Engine::Content::ScopeWriter::ScopeWriter() : _CurrentTarget(nullptr)
    {
    }

    /// <summary>
    /// Function that make writer write to a designated scope
    /// </summary>
    /// <param name="scope"></param>
    void ScopeWriter::BeginWrite(Scope* scope)
    {
        // Begin Write with scope as current target
        _CurrentTarget = scope;
    }

    /// <summary>
    /// Stop writing to current scope 
    /// </summary>
    void ScopeWriter::EndWrite()
    {
        _CurrentTarget = nullptr;
    }

    /// <summary>
    /// Return current target Scope
    /// </summary>
    /// <returns></returns>
    Scope* ScopeWriter::GetCurrentScope()
    {
        return _CurrentTarget;
    }

    /// <summary>
    /// Write function for when Json value is int32_t
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    void ScopeWriter::writeInt(Json::String key, Json::Value value)
    {
        FIEA_ASSERT(value.isInt());
        FIEA_ASSERT(_CurrentTarget != nullptr);
        Datum& dt = _CurrentTarget->Append(key);
        int32_t intValue = static_cast<int32_t>(value.asInt());
        dt.Push<int32_t>(intValue);
    }

    /// <summary>
    /// Write function for when Json value is float
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    void ScopeWriter::writeFloat(Json::String key, Json::Value value)
    {
        FIEA_ASSERT(value.isDouble());
        FIEA_ASSERT(_CurrentTarget != nullptr);
        Datum& dt = _CurrentTarget->Append(key);
        float floatValue = (float)(value.asDouble());
        dt.Push<float>(floatValue);
    }

    /// <summary>
    /// Write function for when Json value is string
    /// </summary>
    /// <param name="key"></param>
    /// <param name="value"></param>
    void ScopeWriter::writeString(Json::String key, Json::Value value)
    {
        FIEA_ASSERT(value.isString());
        FIEA_ASSERT(_CurrentTarget != nullptr);
        Datum& dt = _CurrentTarget->Append(key);
        string stringValue = value.asString();
        dt.Push<string>(stringValue);
    }

    /// <summary>
    /// Handler function that calls writeInt
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool IntParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isInt())
        {
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            sWriter->writeInt(key, value);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Handlefunction for writeInt
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool IntParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return 0;
    }

    /// <summary>
    /// Handler function for write float
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool FloatParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isDouble())
        {
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            sWriter->writeFloat(key, value);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Handler function for float type
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool FloatParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return 0;
    }

    /// <summary>
    /// Handler function that calls writeString
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool StringParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isString())
        {
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            sWriter->writeString(key, value);
            return true;
        }
        return false;
    }

    /// <summary>
    /// Handler function for type String
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool StringParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        return 0;
    }

    /// <summary>
    /// Handler function for object type. Create nested scope children, set current target to children
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool ObjectParserHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isObject())
        {
            std::regex pattern("^<([^>]+)>(.+)$");
            std::smatch matches;
            string type;
            Scope* temp = nullptr;
            if (std::regex_match(key, matches, pattern)) {
                type = matches[1].str();
                key = matches[2].str();
                IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
                //if (type == string("Action")) return false;
                temp = fs->Create<Scope>(type);
            }
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            Scope* sc = sWriter->GetCurrentScope();
            // Add scope if dont exist
            if (sc->Find(key) == nullptr)
            {
                if (temp == nullptr) return false;
             
                // If <GameObject> or <Scope>, adopt as Scope child
                if (type == string("Action"))
                {
                    Action* tempAction = reinterpret_cast<Action*>(temp);
                    reinterpret_cast<GameObject*>(sc)->AddAction(*tempAction);
                    sWriter->BeginWrite(tempAction);
                }
                else {
                    sc->Adopt(key, *temp);
                    sWriter->BeginWrite(sc->Find(key)->GetPointer<Scope*>(0));
                }
               
            }
           
            return true;
        }
        return false;
    }

    /// <summary>
    /// Handler function for object type. Finished writing to child, find and set current target to parent
    /// </summary>
    /// <param name="writer"></param>
    /// <param name="key"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    bool ObjectParserHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isObject())
        {
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            sWriter->BeginWrite(sWriter->GetCurrentScope()->GetParent());
        }
        return true;
    }
   /* bool GameObjChidrenHandler::EnterKey(ParseWriter&, Json::String, Json::Value)
    {
        return false;
    }
    bool GameObjChidrenHandler::ExitKey(ParseWriter&, Json::String, Json::Value)
    {
        return false;
    }*/
    bool GameObjActionHandler::EnterKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isObject())
        {
            std::regex pattern("^<([^>]+)>(.+)$");
            std::smatch matches;
            string type;
            Scope* temp = nullptr;
            if (std::regex_match(key, matches, pattern)) {
                type = matches[1].str();
                key = matches[2].str();
                IFactoryService* fs = ServiceMgr::ProvideInterface<IFactoryService>();
                if (type != string("Action")) return false;
                temp = fs->Create<Scope>(type);
            }
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            Scope* sc = sWriter->GetCurrentScope();
            if (sc->Find(key) == nullptr)
            {
                if (temp == nullptr) return false;
                //Add action
                Action* tempAction = reinterpret_cast<Action*>(temp);
                reinterpret_cast<GameObject*>(sc)->AddAction(*tempAction);
                sWriter->BeginWrite(tempAction);
            }
            return true;
        }
        return false;
    }
    bool GameObjActionHandler::ExitKey(ParseWriter& writer, Json::String key, Json::Value value)
    {
        if (value.isObject())
        {
            ScopeWriter* sWriter = writer.As<ScopeWriter>();
            sWriter->BeginWrite(sWriter->GetCurrentScope()->GetParent());
        }
        return true;
    }
}
