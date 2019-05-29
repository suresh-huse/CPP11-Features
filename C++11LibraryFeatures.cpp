
//==============================================================
// C++11 Library Features
//==============================================================


//==============================================================
// 1. std::move
//==============================================================

/*
//  std::move indicates that the object passed to it may be moved, or in other words,
//	moved from one object to another without a copy.
//	The object passed in should not be used after the move in certain situations.

//  A definition of std::move(performing a move is nothing more than casting to an rvalue) :

template <typename T>
typename remove_reference<T>::type&& move(T&& arg) {
return static_cast<typename remove_reference<T>::type&&>(arg);
}

// Transferring std::unique_ptrs:

std::unique_ptr<int> p1{ new int{ 0 } };
//	std::unique_ptr<int> p2 = p1; // error -- cannot copy unique pointers
std::unique_ptr<int> p3 = std::move(p1); // move `p1` into `p3`
// now unsafe to dereference object held by `p1`
*/


//==============================================================
// 2. std::forward
//==============================================================

/*
//	Returns the arguments passed to it as - is, either as an lvalue or rvalue references,
//	and includes cv - qualification.Useful for generic code that need a reference(either lvalue or rvalue) when appropriate,
//	e.g factories.Used in conjunction with forwarding references.

//	A definition of std::forward:
template <typename T>
T&& forward(typename remove_reference<T>::type& arg) {
return static_cast<T&&>(arg);
};

//	An example of a function wrapper which just forwards other A objects to a new A object's copy or move constructor:

struct AF
{
AF() = default;
AF(const AF& o) { std::cout << "copied" << std::endl; }
AF(AF&& o) { std::cout << "moved" << std::endl; }
};

template <typename T>
AF wrapper(T&& arg) {
return AF{ std::forward<T>(arg) };
};

//wrapper(AF{}); // moved
AF a;
//wrapper(a); // copied
//wrapper(std::move(a)); // moved
*/


//==============================================================
//	3. std::thread
//==============================================================
/*
//	The std::thread library provides a standard way to control threads, such as spawning and killing them.In the example below,
//  multiple threads are spawned to do different calculations and then the program waits for all of them to finish.

void foo(bool clause) {}

class MyThread
{
std::vector<std::thread> threadsVector;
//threadsVector.emplace_back([]() {
// Lambda function that will be invoked
//});
//threadsVector.emplace_back(foo, true);  // thread will run foo(true)
//for (auto& thread : threadsVector) {
//thread.join(); // Wait for threads to finish
//}
};
*/


//==============================================================
//	3. std::to_string
//==============================================================

/*
class MyString
{
//	Converts a numeric argument to a std::string.
// std::to_string(1.2); // == "1.2"
// std::to_string(123); // == "123"
}
*/


//==============================================================
//	4. Type traits
//==============================================================

/*
//	Type traits defines a compile - time template - based interface to query or modify the properties of types.
static_assert(std::is_integral<int>::value);
static_assert(std::is_same<int, int>::value);
static_assert(std::is_same<std::conditional<true, int, double>::type, int>::value);
*/

//==============================================================
//	5. Smart pointers
//==============================================================

/*
//	C++11 introduces new smart(er) pointers: std::unique_ptr, std::shared_ptr, std::weak_ptr.
//  std::auto_ptr now becomes deprecated and then eventually removed in C++17.
//	std::unique_ptr is a non - copyable, movable smart pointer that properly manages arrays and STL containers.
//	Note : Prefer using the std::make_X helper functions as opposed to using constructors.See the sections for std::make_unique and std::make_shared.

std::unique_ptr<Foo> p1{ new Foo{} };  // `p1` owns `Foo`
if (p1) {
p1->bar();
}

{
std::unique_ptr<Foo> p2{ std::move(p1) };  // Now `p2` owns `Foo`
f(*p2);

p1 = std::move(p2);  // Ownership returns to `p1` -- `p2` gets destroyed
}

if (p1) {
p1->bar();
}

//  Foo instance is destroyed when `p1` goes out of scope
//  A std::shared_ptr is a smart pointer that manages a resource that is shared across multiple owners.
//  A shared pointer holds a control block which has a few components such as the managed object and a reference counter.
//  All control block access is thread - safe, however, manipulating the managed object itself is not thread - safe.

void foo(std::shared_ptr<T> t) {
// Do something with `t`...
}

void bar(std::shared_ptr<T> t) {
// Do something with `t`...
}

void baz(std::shared_ptr<T> t) {
// Do something with `t`...
}

std::shared_ptr<T> p1{ new T{} };
// Perhaps these take place in another threads?
foo(p1);
bar(p1);
baz(p1);

*/


//==============================================================
//	6. std::chrono
//==============================================================
/*
// The chrono library contains a set of utility functions and types that deal with durations, clocks,
// and time points.One use case of this library is benchmarking code :

std::chrono::time_point<std::chrono::steady_clock> start, end;
start = std::chrono::steady_clock::now();
// Some computations...
end = std::chrono::steady_clock::now();

std::chrono::duration<double> elapsed_seconds = end - start;
double t = elapsed_seconds.count(); // t number of seconds, represented as a `double`
*/

//==============================================================
//	7. Tuples
//==============================================================

/*
//	Tuples are a fixed - size collection of heterogeneous values.
//  Access the elements of a std::tuple by unpacking using std::tie, or using std::get.

// `playerProfile` has type `std::tuple<int, const char*, const char*>`.

auto playerProfile = std::make_tuple(51, "Frans Nielsen", "NYI");
std::get<0>(playerProfile); // 51
std::get<1>(playerProfile); // "Frans Nielsen"
std::get<2>(playerProfile); // "NYI"
*/


//==============================================================
//	8. std::tie
//==============================================================

/*
//	Creates a tuple of lvalue references.Useful for unpacking std::pair and std::tuple objects.
//  Use std::ignore as a placeholder for ignored values.In C++17, structured bindings should be used instead.

// With tuples...
std::string playerName;
std::tie(std::ignore, playerName, std::ignore) = std::make_tuple(91, "John Tavares", "NYI");

// With pairs...
std::string yes, no;
std::tie(yes, no) = std::make_pair("yes", "no");
*/


//==============================================================
//	9. std::array
//==============================================================

/*
//	std::array is a container built on top of a C - style array.Supports common container operations such as sorting.

std::array<int, 3> a = { 2, 1, 3 };
std::sort(a.begin(), a.end()); // a == { 1, 2, 3 }
for (int& x : a) x *= 2; // a == { 2, 4, 6 }

*/


//==============================================================
//	10. Unordered containers
//==============================================================
//
    
/*
//	These containers maintain average constant - time complexity for search, insert, and remove operations.
//  In order to achieve constant - time complexity, sacrifices order for speed by hashing elements into buckets.
//  There are four unordered containers :

	unordered_set
	unordered_multiset
	unordered_map
	unordered_multimap

*/


//==============================================================
//	11. std::make_shared
//==============================================================

/*
//	std::make_shared is the recommended way to create instances of std::shared_ptrs due to the following reasons :

//  Avoid having to use the new operator.
//  Prevents code repetition when specifying the underlying type the pointer shall hold.
//  It provides exception - safety.Suppose we were calling a function foo like so :

foo(std::shared_ptr<T>{new T{}}, function_that_throws(), std::shared_ptr<T>{new T{}});

//  The compiler is free to call new T{}, then function_that_throws(), and so on...
//  Since we have allocated data on the heap in the first construction of a T, we have introduced a leak here.
//  With std::make_shared, we are given exception - safety:

foo(std::make_shared<T>(), function_that_throws(), std::make_shared<T>());

//  Prevents having to do two allocations.When calling std::shared_ptr{ new T{} }, we have to allocate memory for T,
//  then in the shared pointer we have to allocate memory for the control block within the pointer.
//  See the section on smart pointers for more information on std::unique_ptr and std::shared_ptr.

*/


//==============================================================
//	11. Memory model
//==============================================================

/*
//  C++11 introduces a memory model for C++, which means library support for threading and atomic operations.
//  Some of these operations include(but aren't limited to) atomic loads/stores,
//  compare-and-swap, atomic flags, promises, futures, locks, and condition variables.
//  See the sections on : std::thread

std::async
std::async runs the given function either asynchronously or lazily - evaluated, then returns a std::future which holds the result of that function call.

//The first parameter is the policy which can be :

std::launch::async | std::launch::deferred It is up to the implementation whether to perform asynchronous execution or lazy evaluation.
std::launch::async Run the callable object on a new thread.
std::launch::deferred Perform lazy evaluation on the current thread.
int foo()
{
// Do something here, then return the result.
return 1000;
}

auto handle = std::async(std::launch::async, foo);  // create an async task
auto result = handle.get();  // wait for the result
*/