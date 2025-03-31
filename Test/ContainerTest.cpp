#include "CppUnitTest.h"
#include "Support/Foo.h"
#include "FieaGameEngine/Timer.h"
#include "Tests.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <functional>

#include <ostream>

namespace Fiea::Engine::Tests
{
	const size_t NUM_MEASURES = 10;
	// template meta-programming!
	// start with a template class that produces an always-false boolean
	template< typename C, typename = void >
	struct HasReserve : std::false_type {};

	// using SFINAE, resolve to struct containing a true value, if the reserve method exists!
	template< typename C >
	struct HasReserve<
		C, typename std::enable_if<
			std::is_same<
				decltype(std::declval<C>().reserve(std::declval<typename C::size_type>())),
				void
			>::value
		>::type
	> : std::true_type {};

	// using HasReserve, produce a default impl for things that don't have reserve
	template <typename C>
	std::enable_if_t<!HasReserve<C>::value> Reserve(C&, size_t) {}

	// and relying upon SFINAE again, we do the reserve!
	template <typename C>
	std::enable_if_t<HasReserve<C>::value> Reserve(C& c, size_t n)
	{
		c.reserve(n);
	}

	TEST_CLASS(ContainerList)
	{
		// General GetValue
		template <typename T>
		static T GetValue(size_t n) {
			return (T)(n);
		}
		// GetValue overloaded for string
		template <>
		static string GetValue(size_t n) {
			return std::to_string(n);
		}
		// GetValue overloaded for Foo
		template <>
		static Foo GetValue(size_t n)
		{
			return Foo((int32_t)n);
		}
		// Fill container
		template <template <typename T> typename C, typename E>
		static C<E> Fill(size_t n) {
			C<E> c;
			// Compute enough space to reserve 
			size_t r_space = n + 2 * NUM_MEASURES;
			Reserve<C<E>>(c, r_space);

			for (size_t i = 0; i < n; ++i) {
				c.insert(c.end(), GetValue<E>(i));
			}
			return c;
		}
		// Find template for vector and list
		template <template <typename E>typename T, typename E>
		static void Find(T<E>& c, size_t n) {
			auto it = find(c.begin(), c.end(), GetValue<E>(n));
		}
		// Push back template for all types
		template <template <typename E>typename T, typename E>
		static void PushBack(T<E>& c, size_t n) {
			c.insert(c.end(), GetValue<E>(n));
		}
		// Push front template for all types
		template <template <typename E>typename T, typename E>
		static void PushFront(T<E>& c, size_t n) {
			c.insert(c.begin(), GetValue<E>(n));
		}
		// Find template for set and unordered set
		template <template <typename E>typename T, typename E>
		static void SetFind(T<E>& c, size_t n) {
			auto it = c.find(GetValue<E>(n));
		}
		
	public:
		const size_t sizes[5] = { 1000, 10000, 100000, 1000000, 10000000 };
		const size_t numSizes = sizeof(sizes) / sizeof(sizes[0]);
		
		TEST_METHOD(vectorTest)
		{
			// fill container with N items
			std::function<void(std::vector<Foo>&, size_t n)> operations[] = {
				Find<std::vector, Foo>,
				PushBack<std::vector, Foo>,
				PushFront<std::vector, Foo>,
			};
			for (size_t i = 0; i < numSizes; ++i) {
				std::vector<Foo> fooVec;
				// start timer
				auto container = Fill<std::vector, Foo>(sizes[i]);
				for (int op = 0; op <= 2; ++op)
				{
					LOG("op #" << op << std::endl);
					Timer t;
					for (size_t j = sizes[i]; j < sizes[i] + NUM_MEASURES; ++j) {
						operations[op](container, j);
					}

					// get timer
					double ms = t.Elapsed();

					LOG(sizes[i] << "," << ms << std::endl);
				}
			}

		}
		// Test list<int32_t>
		TEST_METHOD(listTest) {
			std::function<void(std::list<int32_t>&, size_t n)> operations[] = {
					Find<std::list, int32_t>,
					PushBack<std::list, int32_t>,
					PushFront<std::list, int32_t>,
			};
			for (size_t i = 0; i < numSizes; ++i) {
				auto container = Fill<std::list, int32_t>(sizes[i]);
				for (int op = 0; op <= 2; ++op) {
					LOG("op #" << op << std::endl);
					Timer t;
					for (size_t j = sizes[i]; j < sizes[i] + NUM_MEASURES; ++j) {
						operations[op](container, j);
					}
					// get timer
					double ms = t.Elapsed();

					LOG(sizes[i] << "," << ms << std::endl);
				}
			}
		}
		// Test set<double>
		TEST_METHOD(setTest) {
			std::function<void(std::set<double>&, size_t n)> operations[] = {
				SetFind<std::set, double>,
				PushBack<std::set, double>,
				PushFront<std::set, double>,
			};
			for (size_t i = 0; i < numSizes; ++i) {
				auto container = Fill<std::set, double>(sizes[i]);
				for (int op = 0; op <= 2; ++op) {
					LOG("op #" << op << std::endl);
					Timer t;
					for (size_t j = sizes[i]; j < sizes[i] + NUM_MEASURES; ++j) {
						operations[op](container, j);
					}
				// get timer
					double ms = t.Elapsed();

					LOG(sizes[i] << "," << ms << std::endl);
				}
			}
		}
		// Test unordered_set<string>
		TEST_METHOD(unorderedTest) {
			std::function<void(std::unordered_set<string>&, size_t n)> operations[] = {
				SetFind<std::unordered_set, string>,
				PushBack<std::unordered_set, string>,
				PushFront<std::unordered_set, string>,
			};
			for (size_t i = 0; i < numSizes; ++i) {
				auto container = Fill<std::unordered_set, string>(sizes[i]);
				for (int op = 0; op <= 2; ++op) {
					LOG("op #" << op << std::endl);
					Timer t;
					for (size_t j = sizes[i]; j < sizes[i] + NUM_MEASURES; ++j) {
						// avoid collision by changing insert values
						if (op == 2) {
							operations[op](container, j + NUM_MEASURES);
						}
						else {
							operations[op](container, j);
						}
						
					}
					// get timer
					double ms = t.Elapsed();
					LOG(sizes[i] << "," << ms << std::endl);
				}
			}
		}
	};
}