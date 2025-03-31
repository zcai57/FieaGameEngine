#include "testAction.h"
#include "FieaGameEngine/GameObject.h"
#include <chrono>
#include <thread>
namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(testAction);
    using namespace std::string_literals;
    Action* testAction::Clone() const
    {
        return new testAction(*this);
    }
    std::string testAction::ToString() const
    {
        return "testAction"s;
    }

    bool testAction::Equals(const RTTI* rhs) const
    {
        return Scope::Equals(rhs);
    }

    bool testAction::Update()
    {
        FIEA_ASSERT(this->GetParent() != nullptr);
        FIEA_ASSERT(this->GetParent()->TypeIdInstance() == GameObject::TypeIdClass());
        GameObject* parent = reinterpret_cast<GameObject*>(this->GetParent());
        string key = string("testString");
        if (parent->Find(key) != nullptr)
        {
            return true;
        }
        parent->Append(key).SetType(Datum::Type::String);
        // Wait for 1 seconds
        /*std::this_thread::sleep_for(std::chrono::seconds(Duration));*/
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        if (elapsed > std::chrono::seconds(Duration))
        {
            parent->RemoveDatum(key);
            return true;
        }

        parent->RemoveDatum(key);
        return false;
    }

    bool testAction::Init()
    {
        if (!ActionInited)
        {
            startTime = std::chrono::system_clock::now();
            ActionInited = true;
        }
        
        return true;
    }

    bool testAction::CleanUp()
    {
        return true;
    }

    testAction::ClassDef testAction::ClassDefinitions()
    {
        ClassDef ExternalData;
        ExternalData.Signatures.push_back({ offsetof(testAction, Key), Datum::Type::String, 1, "Key"s });
        ExternalData.Signatures.push_back({ offsetof(testAction, Value), Datum::Type::String, 1, "Value"s });
        ExternalData.Signatures.push_back({ offsetof(testAction, Duration), Datum::Type::Int, 1, "Duration"s });
        return ClassDef();
    }
}


