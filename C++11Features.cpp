#include "stdafx.h"
#include <vector>
#include<map>
#include <iostream>
#include <functional>
#include <memory>
#include <string> 
#include <algorithm>
#include <array>

using namespace std;


template <typename T>
class Buffer
{
	std::string          _name;
	size_t               _size;
	std::unique_ptr<T[]> _buffer;

public:
	//  default constructor
	Buffer() :
		_size(16),
		_buffer(new T[16])
	{}

	//  constructor
	Buffer(const std::string& name, size_t size) :
		_name(name),
		_size(size),
		_buffer(new T[size])
	{}

	//  copy constructor
	Buffer(const Buffer& copy) :
		_name(copy._name),
		_size(copy._size),
		_buffer(new T[copy._size])
	{
		T* source = copy._buffer.get();
		T* dest = _buffer.get();
		std::copy(source, source + copy._size, dest);
	}

	//  copy assignment operator
	Buffer& operator=(const Buffer& copy)
	{
		if (this != &copy)
		{
			_name = copy._name;

			if (_size != copy._size)
			{
				_buffer = nullptr;
				_size = copy._size;
				_buffer = _size > 0 > new T[_size] : nullptr;
			}

			T* source = copy._buffer.get();
			T* dest = _buffer.get();
			std::copy(source, source + copy._size, dest);
		}

		return *this;
	}

	//  move constructor
	Buffer(Buffer&& temp) :
		_name(std::move(temp._name)),
		_size(temp._size),
		_buffer(std::move(temp._buffer))
	{
		temp._buffer = nullptr;
		temp._size = 0;
	}

	//  move assignment operator
	Buffer& operator=(Buffer&& temp)
	{
		//static_assert(this != &temp); // assert if this is not a temporary

		_buffer = nullptr;
		_size = temp._size;
		_buffer = std::move(temp._buffer);

		_name = std::move(temp._name);

		temp._buffer = nullptr;
		temp._size = 0;

		return *this;
	}
};

template <typename T>
Buffer<T> getBuffer(const std::string& name)
{
	Buffer<T> b(name, 128);
	return b;
}


// Since C++14 or later:
void f(int && t) {
	//  ...
	cout << " normal f() - " << t << endl;
	t = 5;
}

// Since C++11 or later:
template <typename T>
void f(T&& t) {
	//  ...
	cout << " template f() - " << t << endl;
	t = 5;
}

template <typename... T>
struct arity {
	constexpr static int value = sizeof...(T);
};

int sum(const std::initializer_list<int>& list) {
	int total = 0;
	for (auto& e : list) {
		total += e;
	}

	return total;
}


template <typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
	return x + y;
}

template <typename T>
using Vec = std::vector<T>;

void foo1(int) {};
void foo1(char*) {};

constexpr int square(int x) {
	return x * x;
}

int square2(int x) {
	return x * x;
}

long long operator "" _celsius(unsigned long long tempCelsius) {
	return std::llround(tempCelsius * 1.8 + 32);
}

long operator "" _Square(unsigned long long num) {
	return (num * num);
}

int operator "" _int(const char* str, std::size_t) {
	return std::stoi(str);
}

namespace Program {
	namespace Version1 {
		int getVersion() { return 1; }
		bool isFirstVersion() { return true; }
	}
	inline namespace Version2 {
		int getVersion() { return 2; }
	}
}

struct AM
{
	std::string s;
	AM() : s("test") {}
	AM(const AM& o) : s(o.s) {}
	AM(AM&& o) : s(std::move(o.s)) {}
	AM& operator=(AM&& o) {
		s = std::move(o.s);
		return *this;
	}
};

AM fmove(AM am)
{
	return am;
};

int main()
{
	//============================================================
	//  C++11 Language Features
	//============================================================

	//============================================================
	//  1. Smart Pointers
	//============================================================
	std::unique_ptr<int> p1(new int(42));
	std::unique_ptr<int> p2 = std::move(p1); // transfer ownership


											 //============================================================										 
											 //   2. Move semantics
											 //============================================================
											 //	Move semantics is mostly about performance optimization : the ability to move an object without the expensive overhead of copying.
											 //  The difference between a copy and a move is that a copy leaves the source unchanged, 
											 //  and a move will leave the source either unchanged or radically different -- depending on what the source is.
											 //  For plain old data, a move is the same as a copy.

											 //	To move an object means to transfer ownership of some resource it manages to another object.
											 //  You could think of this as changing pointers held by the source object to be moved, or now held, by the destination object; 
											 //  the resource remains in its location in memory.
											 //  Such an inexpensive transfer of resources is extremely useful when the source is an rvalue, 
											 //  where the potentially dangerous side - effect of changing the source after the move is redundant ,
											 //  since the source is a temporary object that won't be accessible later.

											 //	Moves also make it possible to transfer objects such as std::unique_ptrs, 
											 //  smart pointers that are designed to hold a pointer to a unique object, from one scope to another.
											 //	See the sections on : rvalue references, defining move special member functions, std::move, std::forward, forwarding references.

											 //	Rvalue references
											 //	C++11 introduces a new reference termed the rvalue reference.An rvalue reference to A, 
											 //  which is a non - template type parameter(such as int, or a user - defined type), 
											 //  is created with the syntax A&&.Rvalue references only bind to rvalues.

											 //	Type deduction with lvalues and rvalues :
	Buffer<int> b1;
	Buffer<int> b2("buf2", 64);
	Buffer<int> b3 = b2;
	Buffer<int> b4 = getBuffer<int>("buf4");
	b1 = getBuffer<int>("buf5");

	//  Special member functions for move semantics
	//	The copy constructor and copy assignment operator are called when copies are made, 
	//  and with C++11's introduction of move semantics, 
	//  there is now a move constructor and move assignment operator for moves.

	AM am1 = fmove(AM{}); // move-constructed from rvalue temporary
	AM am2 = std::move(am1); // move-constructed using std::move
	AM am3 = AM{};
	am2 = std::move(am3); // move-assignment using std::move
	am1 = fmove(AM{}); // move-assignment from rvalue temporary


					   //============================================================
					   //	3. Forwarding references
					   //============================================================
					   //	Also known(unofficially) as universal references.
					   //  A forwarding reference is created with the syntax T&& where T is a template type parameter, 
					   //  or using auto&&.This enables two major features : move semantics; 
					   //  and perfect forwarding, the ability to pass arguments that are either lvalues or rvalues.
					   //	Forwarding references allow a reference to bind to either an lvalue or rvalue depending on the type.
					   //  Forwarding references follow the rules of reference collapsing :

					   //	T& & becomes T&
					   //	T& && becomes T&
					   //	T&& & becomes T&
					   //	T&& && becomes T&&
					   //	auto type deduction with lvalues and rvalues:

	int x = 0;
	f(0); // deduces as f(int&&)
	cout << " f(0) - " << x << endl;
	f(x); // deduces as f(int&)
	cout << " f(x) - " << x << endl;

	int& y = x;
	f(y); // deduces as f(int& &&) => f(int&)
	cout << " f(y) - " << y << endl;

	int&& z = 0;
	f(z); // deduces as f(int&& &&) => f(int&&)
	cout << " f(z) - " << z << endl;


	//============================================================
	//  4. Variadic templates
	//============================================================
	//  The ... syntax creates a parameter pack or expands one.
	//  A template parameter pack is a template parameter that accepts 
	//  zero or more template arguments(non - types, types, or templates).
	//  A template with at least one parameter pack is called a variadic template.

	static_assert(arity<>::value == 0);
	static_assert(arity<char, short, int>::value == 3);


	//============================================================
	//   5. auto 
	//============================================================
	//  auto - typed variables are deduced by the compiler according to the type of their initializer.

	auto a = 3.14; // double
	auto b = 1; // int
	auto& c = b; // int&
	auto d = { 0 }; // std::initializer_list<int>
	auto&& e = 1; // int&&
	auto&& f = b; // int&
	auto g = new auto(123); // int*
	const auto h = 1; // const int
	auto i = 1, j = 2, k = 3; // int, int, int
							  //	auto l = 1, m = true, n = 1.61; // error -- `l` deduced to be int, `m` is bool
							  //	auto o; // error -- `o` requires initializer
							  //	Extremely useful for readability, especially for complicated types :

	std::vector<int> v = {};
	std::vector<int>::const_iterator cit1 = v.cbegin();
	//  vs.
	auto cit2 = v.cbegin();
	//	Functions can also deduce the return type using auto.
	//	In C++11, a return type must be specified either explicitly, or using decltype like so :

	add(1, 2); // == 3
	add(1, 2.0); // == 3.0
	add(1.5, 1.5); // == 3.0

				   //	The trailing return type in the above example is the declared type(see section on decltype) of the expression x + y.
				   //	For example, if x is an integer and y is a double, decltype(x + y) is a double.Therefore, 
				   //	the above function will deduce the type depending on what type the expression x + y yields.
				   //	Notice that the trailing return type has access to its parameters, and this when appropriate.


				   //============================================================
				   // 6. Initializer lists
				   //============================================================
				   //   A lightweight array - like container of elements created using a "braced list" syntax.
				   //   For example, { 1, 2, 3 } creates a sequences of integers, 
				   //   that has type std::initializer_list<int>.Useful as a replacement to passing a vector of objects to a function.

	auto list = { 1, 2, 3 };
	sum(list); // == 6
	sum({ 1, 2, 3 }); // == 6
	sum({}); // == 0


			 //============================================================
			 //  7. Static assertions
			 //============================================================
			 //	Assertions that are evaluated at compile - time.

	constexpr int x1 = 0;
	constexpr int y1 = 0;
	static_assert(x1 == y1, "x1 != y1");


	//============================================================
	//	8. Lambda expressions
	//============================================================
	//	A lambda is an unnamed function object capable of capturing variables in scope.It features : a capture list; 
	//   an optional set of parameters with an optional trailing return type; and a body.Examples of capture lists :
	//	[] - captures nothing.
	//	[=] - capture local objects(local variables, parameters) in scope by value.
	//	[&] - capture local objects(local variables, parameters) in scope by reference.
	//	[this] - capture this pointer by value.
	//	[a, &b] - capture objects a by value, b by reference.

	std::function<int(int)> lfib = [&lfib](int n) {return n < 2 ? 1 : lfib(n - 1) + lfib(n - 2); };
	cout << " Fib - " << lfib(5) << endl;

	auto Add = [](int n, int m) {return (n + m); };
	cout << " Addition - " << Add(5, 7) << endl;

	int xLambda = 1;

	auto getX = [=] { return xLambda; };
	getX(); // == 1

	auto addX = [=](int y) { return xLambda + y; };
	addX(1); // == 2

	auto getXRef = [&]() -> int& { return xLambda; };
	getXRef(); // int& to `x`
			   //  By default, value - captures cannot be modified inside the lambda because the compiler - generated method is marked as const.
			   //  The mutable keyword allows modifying captured variables.
			   //  The keyword is placed after the parameter - list(which must be present even if it is empty).

	xLambda = 1;
	auto f1 = [&xLambda] { xLambda = 2; }; // OK: x is a reference and modifies the original

										   //auto f2 = [x] { x = 2; }; // ERROR: the lambda can only perform const-operations on the captured value
										   // vs.
	auto f3 = [xLambda]() mutable { xLambda = 2; }; // OK: the lambda can perform any operations on the captured value


													//============================================================
													//  9. decltype
													//============================================================
													//	decltype is an operator which returns the declared type of an expression passed to it.
													//   cv - qualifiers and references are maintained if they are part of the expression.

	int aa = 1; // `a` is declared as type `int`
	decltype(aa) bb = aa; // `decltype(a)` is `int`
	const int& cc = aa; // `c` is declared as type `const int&`
	decltype(cc) dd = aa; // `decltype(c)` is `const int&`
	decltype(123) ee = 123; // `decltype(123)` is `int`
	int&& ff = 1; // `f` is declared as type `int&&`
	decltype(ff) gg = 1; // `decltype(f) is `int&&`
	decltype((aa)) hh = gg; // `decltype((a))` is int&

	add(1, 2.0); // `decltype(x + y)` => `decltype(3.0)` => `double`


				 //============================================================
				 //	10. Template aliases
				 //============================================================
				 //	Semantically similar to using a typedef however, 
				 //  template aliases with using are easier to read and are compatible with templates.

	Vec<int> vec; // std::vector<int>
	using String = std::string;
	String s{ "foo" };


	//============================================================
	//	11. nullptr
	//============================================================
	//	C++11 introduces a new null pointer type designed to replace C's NULL macro. 
	//	nullptr itself is of type std::nullptr_t and can be implicitly converted into pointer types, and unlike NULL, 
	//	not convertible to integral types except bool.

	//foo1(NULL); // error -- ambiguous
	foo1(nullptr); // calls foo(char*)


				   //============================================================
				   //   12. Strongly - typed enums
				   //============================================================
				   //	Type - safe enums that solve a variety of problems with C - style enums including : implicit conversions, 
				   //  inability to specify the underlying type, scope pollution.
				   //  Specifying underlying type as `unsigned int`
	enum class Color : unsigned int { Red = 0xff0000, Green = 0xff00, Blue = 0xff };
	//  `Red`/`Green` in `Alert` don't conflict with `Color`
	enum class Alert : bool { Red, Green };
	Color col = Color::Red;


	//============================================================
	//   13. constexpr
	//============================================================
	//	Constant expressions are expressions evaluated by the compiler at compile - time.
	//  Only non - complex computations can be carried out in a constant expression.
	//	Use the constexpr specifier to indicate the variable, function, etc.is a constant expression.
	int aa1 = square(2);	// mov DWORD PTR [rbp-4], 4
	int bb1 = square2(2);	// mov edi, 2
							//  call square2(int)
							//  mov DWORD PTR [rbp-8], eax

							//	constexpr values are those that the compiler can evaluate at compile - time:
	const int xc = 123;
	//constexpr const int& yc = xc; // error -- constexpr variable `y` must be initialized by a constant expression

	//	Constant expressions with classes :
	struct Complex {
		constexpr Complex(double r, double i) : re(r), im(i) { }
		constexpr double real() { return re; }
		constexpr double imag() { return im; }

	private:
		double re;
		double im;
	};

	constexpr Complex I(0, 1);


	//============================================================
	//   14. Delegating constructors
	//============================================================
	//	Constructors can now call other constructors in the same class using an initializer list.
	struct Foo
	{
		int foo;
		Foo(int foo) : foo(foo) {}
		Foo() : Foo(0) {}
	};

	Foo foo;
	foo.foo; // == 0


			 //============================================================
			 //   15. User - defined literals
			 //============================================================
			 //	User - defined literals allow you to extend the language and add your own syntax.To create a literal,
			 //  define a T operator "" X(...) { ... } function that returns a type T, with a name X.
			 //  Note that the name of this function defines the name of the literal.
			 //  Any literal names not starting with an underscore are reserved and won't be invoked. 
			 //	There are rules on what parameters a user-defined literal function should accept, according to what type the literal is called on.

			 //	Converting Celsius to Fahrenheit :

			 //  `unsigned long long` parameter required for integer literal.
			 //long long operator "" _celsius(unsigned long long tempCelsius) {
			 //	return std::llround(tempCelsius * 1.8 + 32);
			 //}

	24_celsius; // == 75
	long sq = 5_Square; // 25
	sq = 6_Square; // 36
				   //String to integer conversion :
				   //  `const char*` and `std::size_t` required as parameters.
				   //int operator "" _int(const char* str, std::size_t) {
				   //	return std::stoi(str);
				   //}

	"123"_int; // == 123, with type `int`


			   //============================================================
			   //   16. Explicit virtual overrides
			   //============================================================
			   //	Specifies that a virtual function overrides another virtual function.
			   //  If the virtual function does not override a parent's virtual function,
			   //	throws a compiler error.

	struct A {
		virtual void foo() {};
		void bar() {};
	};

	struct B : A {
		void foo() override {}; // correct -- B::foo overrides A::foo
								//void bar() override; // error -- A::bar is not virtual
								//void baz() override; // error -- B::baz does not override A::baz
	};


	//============================================================
	//   17. Final specifier
	//============================================================
	//	Specifies that a virtual function cannot be overridden in a derived class or that a class cannot be inherited from.

	struct AA {
		virtual void foo() {};
	};

	struct BB : AA {
		virtual void foo() final {};
	};

	struct CC : BB {
		//virtual void foo() {}; // error -- declaration of 'foo' overrides a 'final' function
	};
	//	Class cannot be inherited from.

	struct AAA final {
	};
	//  struct BBB : AAA {}; // error -- base 'A' is marked 'final'


	//============================================================
	//   18. Explicit conversion functions
	//============================================================
	//	Conversion functions can now be made explicit using the explicit specifier.

	struct AE
	{
		operator bool() const { return true; }
	};

	struct BE {
		explicit operator bool() const { return true; }
	};

	AE ae;
	if (ae); // OK calls A::operator bool()
	bool bae = ae; // OK copy-initialization selects A::operator bool()

	BE be;
	if (be); // OK calls B::operator bool()
			 //bool bbe = be; // error copy-initialization does not consider B::operator bool()


			 //============================================================
			 //   19. Default functions
			 //============================================================
			 //	A more elegant, efficient way to provide a default implementation of a function, such as a constructor.

	struct AD
	{
		AD() = default;
		AD(int x) : x(x) {}
		int x{ 1 };
	};
	AD ad; // a.x == 1
	AD ad2{ 123 }; // a.x == 123

				   //With inheritance :
	struct BD
	{
		BD() : x(1) {}
		int x;
	};

	struct CD : BD
	{
		//  Calls BD::BD
		CD() = default;
	};
	CD cd; // cd.x == 1


		   //============================================================
		   //   20. Deleted functions
		   //============================================================
		   //	A more elegant, efficient way to provide a deleted implementation of a function.
		   //	Useful for preventing copies on objects.

	class AX
	{
		int x;
	public:
		AX(int x) : x(x) {};
		AX(const AX&) = delete;
		AX& operator=(const AX&) = delete;
	};

	AX ax{ 123 };
	//AX ay = ax; // error -- call to deleted copy constructor
	//ay = ax; // error -- operator= deleted


	//============================================================
	//   21. Range - based for loops
	//============================================================
	//	Syntactic sugar for iterating over a container's elements.

	std::array<int, 5> arr{ 1, 2, 3, 4, 5 };
	for (int& x : arr) x *= 2;
	//  a == { 2, 4, 6, 8, 10 }

	//	Note the difference when using int as opposed to int&:
	std::array<int, 5> arr2{ 1, 2, 3, 4, 5 };
	for (int x : arr2) x *= 2;
	//  a == { 1, 2, 3, 4, 5 }



	//============================================================
	//   22. Converting constructors
	//============================================================
	//	Converting constructors will convert values of braced list syntax into constructor arguments.

	struct AC1
	{
		AC1(int) {}
		AC1(int, int) {}
		AC1(int, int, int) {}
	};

	AC1 ac1{ 0, 0 }; // calls AC1::AC1(int, int)
	AC1 bc1(0, 0); // calls AC1::AC1(int, int)
	AC1 cc1 = { 0, 0 }; // calls AC1::AC1(int, int)
	AC1 dc1{ 0, 0, 0 }; // calls AC1::AC1(int, int, int)
						//	Note that the braced list syntax does not allow narrowing :

	struct AC2 {
		AC2(int) {}
	};

	AC2 ac2(1.1); // OK
				  //  AC2 bc2{ 1.1 }; // Error narrowing conversion from double to int
				  //  Note that if a constructor accepts a std::initializer_list, it will be called instead :

	struct AC3 {
		AC3(int) {}
		AC3(int, int) {}
		AC3(int, int, int) {}
		AC3(std::initializer_list<int>) {}
	};

	AC3 ac3{ 0, 0 }; // calls AC3::AC3(std::initializer_list<int>)
	AC3 bc3(0, 0); // calls AC3::AC3(int, int)
	AC3 cc3 = { 0, 0 }; // calls AC3::AC3(std::initializer_list<int>)
	AC3 dc3{ 0, 0, 0 }; // calls AC3::AC3(std::initializer_list<int>)


						//============================================================
						//   23. Explicit conversion functions
						//============================================================
						//	Conversion functions can now be made explicit using the explicit specifier.

	struct AEC
	{
		operator bool() const { return true; }
	};

	struct BEC {
		explicit operator bool() const { return true; }
	};

	AEC aec;
	if (aec); // OK calls AEC::operator bool()
	bool bac = aec; // OK copy-initialization selects A::operator bool()

	BEC bec;
	if (bec); // OK calls BEC::operator bool()
			  //  bool bb = bec; // error copy-initialization does not consider B::operator bool()


			  //============================================================
			  //   24. Inline namespaces
			  //============================================================
			  //	All members of an inline namespace are treated as if they were part of its parent namespace, 
			  //  allowing specialization of functions and easing the process of versioning.
			  //  This is a transitive property, if A contains B, which in turn contains C and both B and C are inline namespaces, 
			  //  C's members can be used as if they were on A.

	int version{ Program::getVersion() };              // Uses getVersion() from Version2
	int oldVersion{ Program::Version1::getVersion() }; // Uses getVersion() from Version1
													   //  bool firstVersion{ Program::isFirstVersion() };    // Does not compile when Version2 is added


													   //============================================================
													   //   25. Non - static data member initializers
													   //============================================================
													   //	Allows non - static data members to be initialized where they are declared,
													   //  potentially cleaning up constructors of default initializations.

													   //  Default initialization prior to C++11
	class Human1 {
		Human1() : age(0) {}
	private:
		unsigned age;
	};

	//  Default initialization on C++11
	class Human2 {
	private:
		unsigned age{ 0 };
	};


	//============================================================
	//   26. Right angle Brackets
	//============================================================
	//	C++11 is now able to infer when a series of right angle brackets is used as an operator or 
	//  as a closing statement of typedef, without having to add whitespace.

	typedef std::map<int, std::map <int, std::map <int, int> > > cpp98LongTypedef;
	typedef std::map<int, std::map <int, std::map <int, int>>>   cpp11LongTypedef;

	//============================================================ End ==============================================
	return 0;
}


