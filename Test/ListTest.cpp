#include "CppUnitTest.h"
#include "Tests.h"
#include <list>
#include <string>
#include "FieaGameEngine/List.h"
#include "FieaGameEngine/Types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
	using StringList = List<string>;
	//using StringList = std::list<std::string>;

	TEST_CLASS(ListTest)
	{
	public:
		TEST_MEMCHECK;
		// List();
		TEST_METHOD(DefaultConstructorTest)
		{
			StringList s1;
			Assert::IsTrue(s1.empty());

			StringList s2 = { "1" };
			Assert::IsTrue(s2.front() == "1");
		}
		// List(const List&); 
		TEST_METHOD(CopyConstructorTest)
		{
			StringList s1 = { "1", "2" };
			StringList s2(s1);
			Assert::IsTrue(s1 == s2);
			Assert::IsTrue(s1.front() == s2.front());

			 //Changeing s2 does not affect s1
			s2.back() = ("3");
			Assert::IsFalse(s1 == s2);
			Assert::IsTrue(s1.size() == s2.size());
			
			s2.push_back("2");
			Assert::IsFalse(s1 == s2);
		}
		//~List();
		TEST_METHOD(DestructorTest)
		{
			{
				StringList s1 = { "1" };
			}
			StringList s1 = { "1", "2" };
			Assert::IsTrue(s1.back() == "2");
		}
		//List& operator=(const List&);
		TEST_METHOD(ReplaceOperatorTest)
		{
			// Case 1: Simple assignment
			StringList s1 = { "1" };
			StringList s2 = { "2" };
			s2 = s1;
			Assert::IsTrue(s1 == s2);
			Assert::IsTrue(s2.front() == "1");

			// Case 2: Self-assignment
			s1 = s1;
			Assert::IsTrue(s1.front() == "1");

			// Case 3: Assign Empty list
			StringList empty;
			s1 = empty;
			Assert::IsTrue(s1.empty());
		}
		// List::iterator begin();
		TEST_METHOD(BeginIteratorTest)
		{
			StringList s1 = { "1", "2"}; 
			StringList::iterator s1_iter = s1.begin();
			Assert::IsTrue(*s1_iter == "1");
			s1_iter++;
			Assert::IsTrue(*s1_iter == "2");
			s1_iter++;
			Assert::IsTrue(s1_iter == s1.end());
		}
		// List::iterator end();
		TEST_METHOD(EndIteratorTest)
		{
			StringList s1 = { "1", "2" };
			StringList::iterator s1_iter = s1.end();
			s1_iter--;
			Assert::IsTrue(*s1_iter == "2");
			s1_iter--;
			Assert::IsTrue(*s1_iter == "1");
		}
		// void push_back(const value_type& value);
		TEST_METHOD(PushbackTest)
		{
			// Start from empty list
			StringList s1;
			s1.push_back("2");
			Assert::IsTrue(s1.back() == "2");
			// check list is still complete
			s1.push_back("1");
			Assert::IsTrue(s1.back() == "1");
			Assert::IsTrue(s1.front() == "2");
			Assert::AreEqual(s1.size(), (size_t)2);
		}
		// void pop_back();
		TEST_METHOD(PopbackTest)
		{
			StringList s1 = { "1", "2"};
			s1.pop_back();
			Assert::IsTrue(s1.back() == "1");
			s1.pop_back();
			Assert::AreEqual(s1.size(), (size_t)0);
			Assert::IsTrue(s1.empty());
		}
		// void push_front(const value_type& value);
		TEST_METHOD(PushFrontTest)
		{
			StringList s1;
			s1.push_front("1");
			Assert::IsTrue(s1.front() == "1");
			Assert::IsTrue(s1.back() == "1");
			Assert::IsTrue(s1.size() == 1_z);
			s1.push_front("2");
			Assert::IsTrue(s1.front() == "2");
			Assert::IsTrue(s1.back() == "1");
			Assert::IsTrue(s1.size() == (size_t)2);
		}
		// void pop_front()
		TEST_METHOD(PopfrontTest)
		{
			StringList s1 = { "1", "2" };
			s1.pop_front();
			Assert::IsTrue(s1.front() == "2");
			Assert::IsTrue(s1.size() == (size_t)1);

			s1.pop_front();
			Assert::IsTrue(s1.begin() == s1.end());
			Assert::IsTrue(s1.empty());
		}
		// value_type& back();
		TEST_METHOD(BackTest)
		{
			StringList s1 = { "2", "1" };
			string& s1ref = s1.back();
			s1ref = "2";
			Assert::IsTrue(s1.back() == "2");
			// Changing s1back will also change its reference
			s1.back() = "1";
			Assert::IsTrue(s1ref == "1");
			// Check s1ref after original value lost
			s1.pop_back();
			Assert::IsTrue(s1ref != "1");
		}
		// const value_type& back() const;
		TEST_METHOD(ConstBackTest)
		{
			const StringList s1 = { "2", "1" };
			const string& s1ref = s1.back();
			Assert::IsTrue(s1.back() == "1");
			// Changing s1back will also change its reference
			//s1.back() = "1";
			//Assert::IsTrue(s1ref == "1");
			// Check s1ref after original value lost
			//s1.pop_back();
			//Assert::IsTrue(s1ref != "1");
		}
		// value_type& front();
		TEST_METHOD(FrontTest) {
			StringList s1 = { "2", "1" };
			string& s1ref = s1.front();
			s1ref = "1";
			Assert::IsTrue(s1.front() == "1");
			// Changing s1front will also change its reference
			s1.front() = "3";
			Assert::IsTrue(s1ref == "3");
			// Pop s1ref value will leave it dangling
			s1.pop_front();
			Assert::IsTrue(s1ref != "3");
		}
		// const value_type& front() const;
		TEST_METHOD(ConstFrontTest) {
			const StringList s1 = { "2", "1" };
			const string& s1ref = s1.front();
			Assert::IsTrue(s1.front() == "2");
			// Changing s1front will also change its reference
			//s1.front() = "3";
			//Assert::IsTrue(s1ref == "3");
			// Pop s1ref value will leave it dangling
			//s1.pop_front();
			//Assert::IsTrue(s1ref != "3");
		}

		//size_type size() const;
		TEST_METHOD(SizeTest) {
			StringList s1 = { "1" };
			StringList s2 = { "2" };
			Assert::IsTrue(s1.size() == s2.size());
			// Push s2 to reach size 2
			s2.push_back({ "3" });
			Assert::IsTrue(s1.size() != s2.size());
			Assert::IsTrue(s2.size() == (size_t)2);
			// Pop s2 to reach size 1
			s2.pop_back();
			Assert::IsTrue(s2.size() == (size_t)1);
			// Pop s1 to reach size 0
			s1.pop_back();
			Assert::IsTrue(s1.size() == (size_t)0);
		}
		// bool empty() const;
		TEST_METHOD(EmptyTest) {
			StringList s1 = { "1" };
			s1.pop_front();
			Assert::IsTrue(s1.size() == (size_t)0);
			Assert::IsTrue(s1.empty());
			// Push s1 to make s1 not empty
			s1.push_front("1");
			Assert::IsFalse(s1.empty());
		}
		//reference operator*();
		TEST_METHOD(ReferenceOperatorTest)
		{
			StringList s1 = { "1", "2"};
			StringList::iterator s1_iter = s1.begin();
			*s1_iter = "3";
			Assert::IsTrue(*s1_iter == "3");
			// Iterator reference can be incremented
			s1_iter++;
			Assert::IsTrue(*s1_iter == "2");
			// s1 begin will not affected by s1_iter
			Assert::IsTrue(*s1.begin() == "3");
		}
		//const reference operator*() const;
		TEST_METHOD(ConstReferenceOperatorTest)
		{
			StringList s1 = { "1", "2" };
			const StringList::iterator s1_iter = s1.begin();
			// cannot change *s1_iter value
			Assert::IsTrue(*s1_iter == "1");
			//const Iterator reference can still be incremented
			//s1_iter++;
			//Assert::IsTrue(*s1_iter == "2");
			// s1 begin will not affected by s1_iter
			//Assert::IsTrue(*s1.begin() == "1");
		}
		//pointer operator->();
		TEST_METHOD(PointerOperatorTest)
		{
			StringList s1 = { "1" };
			StringList::iterator s1_iter = s1.begin();
			s1_iter->clear();
			Assert::IsTrue(s1.front() == "");
			Assert::IsTrue(*s1_iter == "");
			Assert::IsTrue(s1_iter->empty());
		}
		//const pointer operator->() const;
		TEST_METHOD(ConstPointerOperatorTest)
		{
			StringList s1 = { "1" };
			const StringList::iterator s1_iter = s1.begin();
			// cannot run s1_iter->clear since s1_iter is const;
			Assert::IsTrue(s1.front() == "1");
			// can still use functions that dont change value
			Assert::IsFalse(s1_iter->empty());
		}
		//iterator& operator++();
		//iterator operator++(int);
		TEST_METHOD(RefPlusplusOperatorTest)
		{
			StringList s1 = { "1", "2", "3"};
			StringList::iterator s1_iter1 = s1.begin();
			StringList::iterator& s1_iter2 = s1_iter1;
			// iter2 should update properly and also change iter1
			s1_iter2++;
			Assert::IsTrue(*s1_iter2 == "2");
			Assert::IsTrue(*s1_iter1 == "2");		
			// Change iter1 will change its reference
			StringList::iterator s1_iter3 = s1_iter1++;
			Assert::IsTrue(*s1_iter3 == "2");
			Assert::IsTrue(*s1_iter2 == "3");
			Assert::IsTrue(*s1_iter1 == "3");
			++s1_iter1;
			Assert::IsTrue(s1_iter1 == s1.end());
			Assert::IsTrue(s1_iter2 == s1.end());
		}
		//iterator& operator--();
		//iterator operator--();
		TEST_METHOD(RefMinusMinusOperatorTest)
		{
			StringList s1 = { "1", "2", "3"};
			StringList::iterator s1_iter1 = s1.end();
			StringList::iterator& s1_iter2 = s1_iter1;
			Assert::IsTrue(s1_iter2 == s1.end());
			// iter2 should update properly and also change iter1
			--s1_iter2;
			Assert::IsTrue(*s1_iter2 == "3");
			Assert::IsTrue(*s1_iter1 == "3");
			// Change iter1 will change its reference
			StringList::iterator s1_iter3 = s1_iter1--;
			Assert::IsTrue(*s1_iter3 == "3");
			Assert::IsTrue(*s1_iter1 == "2");
			Assert::IsTrue(*s1_iter2 == "2");
			s1_iter3 = --s1_iter1;
			Assert::IsTrue(*s1_iter3 == "1");
			Assert::IsTrue(*s1_iter1 == "1");
		}
		// bool operator==(const iterator&, const iterator&);
		TEST_METHOD(DoubleEqualTest)
		{
			StringList s1 = { "1" };
			StringList::iterator s1_iter1 = s1.begin();
			StringList::iterator s1_iter2 = s1.end();
			s1_iter2--;
			Assert::IsTrue(s1_iter1 == s1_iter2);
			// Add element and check equality between begin and end fail
			s1.push_back("2");
			Assert::IsFalse(s1.begin() == s1.end());
		}
		// bool operator!=(const iterator&, const iterator&);
		TEST_METHOD(NotEqualTest)
		{
			StringList s1 = { "1" };
			StringList::iterator s1_iter1 = s1.begin();
			StringList::iterator s1_iter2 = s1.end();
			Assert::IsTrue(s1_iter1 != s1_iter2);
			// Begin will update and check unequal with iter1 and 2
			s1.push_front("2");
			Assert::IsTrue(s1.begin() != s1_iter1);
			Assert::IsTrue(s1.begin() != s1_iter2);
		}
	};
}
