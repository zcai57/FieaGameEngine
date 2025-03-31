#include "CppUnitTest.h"
#include "Tests.h"
#include "Heap.h"
#include "Support/Foo.h"
#include "Tracker.h"
#include "MemoryService.cpp"
#include "FieaGameEngine/ServiceMgr.h"
#include "FieaGameEngine/MemoryService.h"

using namespace Fiea::Engine::Memory;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	
	TEST_CLASS(MemoryServiceTest)
	{
#ifdef FIEA_MEMORY_DEBUG

		TEST_MEMCHECK;
	public:
		size_t HeapAlignment = 8_z;
		size_t HeapHeader = 24_z;
		size_t AlignSize(size_t size) {
			return size + HeapAlignment - size % (HeapAlignment);
		}
		//TEST_MEMCHECK;
		TEST_METHOD(CreateHeapTest)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Assert::IsNotNull(hp);
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
		TEST_METHOD(DestroyHeapTest)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			Heap* hp = memService->CreateHeap("heap1", 200_z);
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
//#ifndef FIEA_MEMORY_DEBUG
		TEST_METHOD(GetAllocatorWithRef)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();
			// Get heap with ref and test alloc
			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Allocator alloc1 = allocService->GetAllocator(hp, "alloc1");
			void* a1 = alloc1.Alloc(100_z, __FILE__, __LINE__);
			Assert::IsNotNull(a1);
			alloc1.Free(a1);
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}

		TEST_METHOD(GetAllocatorWithName)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();
			// Get heap with ref and test alloc
			// Get heap with name and test alloc
			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Allocator alloc1 = allocService->GetAllocator("heap1", "alloc1");
			void* a1 = alloc1.Alloc(100_z, __FILE__, __LINE__);
			Assert::IsNotNull(a1);
			Assert::IsTrue(hp->Used() == AlignSize(100_z));
			alloc1.Free(a1);
			Allocator alloc2 = allocService->GetAllocator("heap2", "alloc2");
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
		TEST_METHOD(FNewDeleteMacroTest)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();

			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Allocator alloc1 = allocService->GetAllocator("heap1", "alloc1");

			Foo* testFoo = FNEW(alloc1, Foo(3), __FILE__, __LINE__);
			Assert::IsTrue(hp->Used() == AlignSize(sizeof(Foo*)));

			int32_t* testInt = FNEW(alloc1, int32_t, __FILE__, __LINE__);
			Assert::IsTrue(hp->Used() == AlignSize(sizeof(Foo*)) + AlignSize(sizeof(int32_t)));
			
			FDELETE(alloc1, testFoo);
			Assert::IsTrue(hp->Used() == AlignSize(sizeof(int32_t)));
			
			FDELETE(alloc1, testInt);
			Assert::IsTrue(hp->Used() == 0_z);

			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
		TEST_METHOD(FAllocFreeTest)
		{
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();

			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Allocator alloc1 = allocService->GetAllocator("heap1", "alloc1");

			void* adr1 = FALLOC(alloc1, 100_z, __FILE__, __LINE__);
			Assert::IsTrue(hp->Used() == AlignSize(100_z));

			void* adr2 = FALLOC(alloc1, 50_z, __FILE__, __LINE__);
			Assert::IsTrue(hp->Used() == 200_z - HeapHeader);

			FFREE(alloc1, adr1);
			Assert::IsTrue(hp->Used() == 200_z - HeapHeader - AlignSize(100_z));

			FFREE(alloc1, adr2);
			Assert::IsTrue(hp->Used() == 0_z);
			
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
//#else
		// Testing Update FNEW FDELETE, used updated new, delete and updated alloc.
		TEST_METHOD(DebugAllocFreeWithReport)
		{
			size_t ostreamSize = 72_z;
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();
			Heap* hp = memService->CreateHeap("heap1", 200_z);
			Allocator alloc1 = allocService->GetAllocator("heap1", "alloc1");
			// New A test Int
			int32_t* testInt = FNEW(alloc1, int32_t, __FILE__, __LINE__);
			std::ostringstream output;
			memService->DumpAllocations(output, hp);
			std::string result = output.str();
			// Check address in Log
			LOG(testInt<< ": " << result << "\n");
			Assert::IsTrue(result.size() == ostreamSize);
			output.str("");

			Assert::IsTrue(hp->Used() == AlignSize(sizeof(int32_t)));
			Assert::IsTrue(memService->CountAllocations(hp) == 1_z);
			
			// New second int
			int32_t* testInt1 = FNEW(alloc1, int32_t, __FILE__, __LINE__);
			memService->DumpAllocations(output, hp);
			std::string result2 = output.str();
			Assert::IsTrue(result2.size() == ostreamSize * 2);
			LOG(result2 << "\n");
			output.str("");
			Assert::IsTrue(memService->CountAllocations(hp) == 2_z);

			FDELETE(alloc1, testInt);
			Assert::IsTrue(memService->CountAllocations(hp) == 1_z);

			memService->DumpAllocations(output, hp);
			std::string result3 = output.str();
			LOG(result3 << "\n");
			output.str("");
			Assert::IsTrue(result3.size() == ostreamSize);

			// Test dump allocation with null hp
			memService->DumpAllocations(output, nullptr);
			memService->CountAllocations(nullptr);

			FDELETE(alloc1, testInt1);

			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}

		TEST_METHOD(GlobalNewDeleteTest)
		{
			// This test will crush as of right now
			Assert::IsTrue(true);
			IMemoryService* memService = ServiceMgr::ProvideInterface<IMemoryService>();
			IAllocatorProvider* allocService = ServiceMgr::ProvideInterface<IAllocatorProvider>();
			Heap* hp = memService->CreateHeap("heap1", 1000_z);
			Allocator alloc1 = allocService->GetAllocator("heap1", "alloc1");
			
			allocService->PushDefaultAllocator(alloc1, __FILE__, __LINE__);
			// Allocate using custom heap and allocator
			int32_t* testInt0 = new int32_t(100);
			Assert::IsTrue(hp->Used() == 8_z);
			// Delete testInt0 with destroy_at and global delete
			std::destroy_at(testInt0);
			delete testInt0;

			allocService->PopDefaultAllocator(alloc1);
			// Allocate without using allocator
			int32_t* testInt1 = new int32_t(100);
			Assert::IsTrue(hp->Used() == 0_z);

			delete testInt1;
			memService->DestroyHeap(hp);
			ServiceMgr::Reset();
		}
#endif // !FIEA_MEMORY_DEBUG	
	};
}