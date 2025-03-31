#include "CppUnitTest.h"
#include "Tests.h"
#include "FieaGameEngine/Types.h"
#include "Heap.h"
using namespace Fiea::Engine::Memory;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	TEST_CLASS(HeapTests)
	{
	public:
		TEST_MEMCHECK;
		TEST_METHOD(DefaultContructorTest)
		{
			Heap* test = Heap::CreateHeap("test", 100_z);
			size_t used = test->Used();
			size_t available = test->Available();
			size_t overhead = test->Overhead();
			Assert::IsTrue(used == 0_z);
			Assert::IsTrue(available == 104_z);
			Assert::IsTrue(overhead == 0_z);
			Assert::IsTrue(used + available + overhead == 104_z);
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(DestructorTest)
		{
			Heap* test = Heap::CreateHeap("test", 100_z);	
			Heap::DestroyHeap(test);
			// Test memcheck will auto check memory leak!
		}
		TEST_METHOD(GetNameTest)
		{
			Heap* test = Heap::CreateHeap("test", 100_z);
			Assert::IsTrue(test->GetName() == "test");
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(AllocWithoutSplitTest)
		{
			Heap* test = Heap::CreateHeap("test", 100_z);
			void* alloc0 = test->Alloc(200_z);
			Assert::IsTrue(alloc0 == nullptr);
			// Alloc without splitting
			void* alloc1 = test->Alloc(72_z);
			Assert::IsTrue(alloc1 == (char*)test + sizeof(Heap));
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(AllocWithSplitTest)
		{
			size_t HeaderSize = 24_z;
			Heap* test = Heap::CreateHeap("test", 100_z);
			// Alloc with splitting
			void* alloc1 = test->Alloc(30_z);
			// Heap: test - 80 byte - alloc1 - 24byte - header(free, 24byte) 
			Assert::IsTrue(alloc1 == (char*)test + sizeof(Heap));
			Assert::IsTrue(test->Used() == 32_z);
			Assert::IsTrue(test->Available() == 48_z);
			Assert::IsTrue(test->Overhead() == HeaderSize);
			void* alloc2 = test->Alloc(32_z);
			// Heap: alloc1 - 32byte - header(notfree, 24byte) - alloc2
			Assert::IsTrue(alloc2 == (char*)alloc1 + HeaderSize + 32_z);
			Assert::IsTrue(test->Used() == 80_z);
			Assert::IsTrue(test->Available() == 0_z);
			Assert::IsTrue(test->Overhead() == HeaderSize);
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(FreeTest)
		{
			size_t HeaderSize = 24_z;
			Heap* test = Heap::CreateHeap("test", 112_z);
			// alloc 32
			void* alloc1 = test->Alloc(30_z);
			// alloc 48
			void* alloc2 = test->Alloc(32_z);
			test->Free(alloc1);
			Assert::IsTrue(test->Used() == 56_z);
			Assert::IsTrue(test->Available() == 32_z);
			Assert::IsTrue(test->Overhead() == HeaderSize);
			test->Free(alloc2);
			Assert::IsTrue(test->Used() == 0_z);
			Assert::IsTrue(test->Available() == 112_z - HeaderSize);
			Assert::IsTrue(test->Overhead() == HeaderSize);
			// Allocate to trigger coalesce
			void* alloc3 = test->Alloc(40_z);
			Assert::IsTrue(alloc3 == (char*)test + sizeof(Heap));
			Assert::IsTrue(test->Used() == 40_z);
			Assert::IsTrue(test->Available() == 48_z);
			Assert::IsTrue(test->Overhead() == HeaderSize);
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(FreeAllocateTest)
		{
			size_t headerSize = 24_z;
			Heap* test = Heap::CreateHeap("test", 200_z);
			void* alloc1 = test->Alloc(30_z);
			void* alloc2 = test->Alloc(60_z);
			void* alloc3 = test->Alloc(30_z);
			test->Free(alloc1);
			test->Free(alloc3);
			// Though heap have enough space for 64 byte, space is not continuous
			// should fail when alloc
			void* alloc4 = test->Alloc(60_z);
			Assert::IsTrue(alloc4 == nullptr);
			test->Free(alloc2);
			Assert::IsTrue(test->Available() == 200_z - 2 * headerSize);
			Assert::IsTrue(test->Overhead() == 2 * headerSize);
			// Assert can alloc 60 bytes now
			void* alloc5 = test->Alloc(60_z);
			Assert::IsTrue(alloc5 != nullptr);
			Heap::DestroyHeap(test);
		}
		TEST_METHOD(ContainerTest)
		{
			Heap* test = Heap::CreateHeap("test", 200_z);
			// Check left bound
			Assert::IsTrue(test->Contains((char*)test));
			Assert::IsFalse(test->Contains((char*)test) - 1_z);
			// Check right bound
			Assert::IsTrue(test->Contains((char*)test + 200_z));
			Assert::IsFalse(test->Contains((char*)test + 201_z));
			Heap::DestroyHeap(test);
		}
	};
}