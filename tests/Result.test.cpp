#include "../fp/Result.hpp"

#include <iostream>

const Result<int> parse(const int ok)
{
	if (ok == 1)
		return ok;
	else
		return Error("Error");
}

int main()
{
	const Result<int> result = parse(1);
	std::cout << "result.isOk: " << result.ok << std::endl;
	std::cout << "result.value: " << result.value << std::endl;
	std::cout << "result.error.message: " << result.error.message << std::endl;
	Result<int> result2 = parse(2);

	std::cout << "result2.isOk: " << result2.ok << std::endl;
	std::cout << "result2.value: " << result2.value << std::endl;
	std::cout << "result2.error.message: " << result2.error.message << std::endl;

	return 0;
}
