// File: src/main.cpp

#include "ServerStructs.hpp"
#include <iostream>
// Other necessary includes...

int main()
{
	// Example usage
	HttpRequest request{"GET", "/index.html"};
	HttpResponse response = createResponse(request);

	std::cout << "Response Status: " << response.status << std::endl;
	std::cout << "Response Body: " << response.body << std::endl;

	// Server initialization and execution logic...
	return 0;
}
