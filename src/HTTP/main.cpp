#include "cgi.hpp"
#include <fstream>
#include <sstream>

using namespace std;

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage: " << av[0] << " <filepath>" << std::endl;
		return (1);
	}
	string	filepath = av[1];
	ifstream	file(filepath);
	if (file)
	{
		stringstream	buf;
		buf << file.rdbuf();
		string	content = buf.str();
		cout << content << endl;

		// CGI
		
	}
	file.close();
	return (0);
}
