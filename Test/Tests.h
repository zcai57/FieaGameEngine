#pragma once

#include <excpt.h>
#include <CppUnitTest.h>
#include "FieaGameEngine\Types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// make it easier to create size_t and ptrdiff_t literals
constexpr std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
constexpr std::ptrdiff_t operator "" _sz(unsigned long long n) { return ptrdiff_t(n); }

namespace Fiea::Engine::Tests
{
    template<typename _FUNCTOR> static uint32_t TestSEH(_FUNCTOR functor)
    {
        __try
        {
            functor();
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return GetExceptionCode();
        }
        return 0;
    }

    template<uint32_t _CODE, typename _FUNCTOR> static void ExpectSEH(_FUNCTOR functor, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
    {
        FIEA_ASSERT_HIDE();

        if (_CODE != TestSEH(functor))
        {
            Assert::Fail(message, pLineInfo);
        }
    }
}

#define LOG(_stream)                            \
    {                                           \
        std::ostringstream stream;                 \
        stream << _stream;                         \
        Logger::WriteMessage(stream.str().c_str());\
    }

#ifdef FIEA_DEBUG
#define TEST_MEMCHECK                                                                           \
    public:                                                                                     \
        TEST_METHOD_INITIALIZE(Initialize)                                                      \
        {                                                                                       \
            _CrtMemCheckpoint(&_startMemState);                                                 \
        }                                                                                       \
                                                                                                \
        TEST_METHOD_CLEANUP(Cleanup)                                                            \
        {                                                                                       \
            _CrtMemState endMemState, diffMemState;                                             \
            _CrtMemCheckpoint(&endMemState);                                                    \
            if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))                \
            {                                                                                   \
                _CrtMemDumpStatistics(&diffMemState);                                           \
                Assert::Fail(L"Memory Leaks!");                                                 \
            }                                                                                   \
        }                                                                                       \
    private:                                                                                    \
        inline static _CrtMemState _startMemState
#else
#define TEST_MEMCHECK
#endif