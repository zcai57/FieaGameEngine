#include "StringManager.h"
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine::Test::Support
{
    FIEA_SERVICE_BIND1(StringManager, INameGiver);
    /**
     * Slap "Name: " in front of the char*
     * \param value
     * \return 
     */
    string StringManager::GiveName(const char* value) const
    {
        string name = "Name: " + string(value);
        return name;
    }
}

