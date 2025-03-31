#include "FieaGameEngine/ContentService.h"
namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(ParseWriter);

    /// <summary>
    /// Increment depth
    /// </summary>
    void ParseWriter::IncrementDepth()
    {
        ++_NestingDepth;
    }
    /// <summary>
    /// Decrement depth
    /// </summary>
    void ParseWriter::DecrementDepth()
    {
        --_NestingDepth;
    }
    /// <summary>
    /// Return current depth
    /// </summary>
    /// <returns></returns>
    size_t ParseWriter::Depth()
    {
        return _NestingDepth;
    }
}
