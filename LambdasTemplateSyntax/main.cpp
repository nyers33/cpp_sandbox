#include <iostream>
#include <string>
#include <sstream>

#include <functional>
#include <ranges>
#include <vector>

// template syntax for lambdas to allow lambdas to be more flexible and generic
// ~ how regular function templates work

// lambdas could be generic with auto parameters, but there were limitations
// - couldn't constrain types
// - couldn't have multiple overloads or SFINAE-style control (Substitution Failure Is Not An Error)
// - no way to explicitly specify template parameters

int main()
{
	// simple generic lambda
	{
		// lambda accepts any type T that supports operator+
		// <typename T> is the template parameter list
		auto add = []<typename T>(T a, T b) {
			return a + b;
		};

		// int
		std::cout << "3 + 4 = " << add(3, 4) << std::endl;

		// double
		std::cout << "3.5 + 2.1 = " << add(3.5, 2.1) << std::endl;
	}

	// using "requires" specifies the requirements on template arguments
	{
		// uses concepts to restrict the lambda to integral types only
		// prevents misuse at compile-time with cleaner error messages
		auto multiply = []<typename T>(T a, T b) requires std::integral<T> {
			return a * b;
		};

		// ok
		std::cout << "5 * 4 = " << multiply(5, 4) << std::endl;

		// compile error: not integral
		// std::cout << "3.5 * 2.1 = " << multiply(3.5, 2.1);
	}

	// multiple template parameters + overload
	{
		// explicit type specification for both source and destination types
		// fully generic and type - safe, using template syntax inside the lambda
		auto converter = []<typename From, typename To>(From input) -> To {
			return static_cast<To>(input);
		};

		int x = converter<double, int>(3.14159);
		std::string s = converter<const char*, std::string>("Hello!");
		
		std::cout << "Converted int: " << x << std::endl;
		std::cout << "Converted string: " << s << std::endl;
	}

	// filtering and transforming a range with a templated lambda
	{
		std::vector<int> nums = { 1, 2, 3, 4, 5, 6 };

		// template lambda to filter even numbers
		auto is_even = []<typename T>(T x) {
			return x % 2 == 0;
		};

		// template lambda to square any type
		auto square = []<typename T>(T x) {
			return x * x;
		};

		for (auto val : nums | std::views::filter(is_even) | std::views::transform(square)) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}

	// generic math operation wrapped in std::function
	{
		std::function<int(int, int)> operation;

		// template lambda for math ops
		auto add = []<typename T>(T a, T b) -> T {
			return a + b;
		};

		// wrap it with a specific type
		// std::function needs a fixed type — bind the template lambda with a specific one
		operation = add<int>;

		std::cout << operation(5, 7) << std::endl;
	}

	// concepts (compile-time type contracts) in lambdas
	{
		auto print_if_integral = []<typename T>(T value) requires std::integral<T> {
			std::cout << "Integral: " << value << std::endl;
		};

		// ok
		print_if_integral(42);

		// compile error
		// print_if_integral(3.14);
	}

	return 0;
}
