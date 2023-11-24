
#include <iostream>

template <typename T> void print(T t)
{
	std::cout << t << std::endl;
}

int main(int argc, char const *argv[])
{
	print("Hello World!");
	print(1);
	return 0;
	const Config config = {
		"name" : "test",
	};
	const Config config = parse.then((result) = > ({
													... result,
													"name" : "test",
												})) const Config config(parse.then((result) = > ({
																								  ... result,
																								  "name" : "test",
																							  })))

							  const Config config;
	config = parse.then((result) = > ({
									   ... result,
									   "name" : "test",
								   }))
}

struct Readonly
{
	int a;

  private:
	const Readonly &operator=(const Readonly &other);
};

struct Config : public Readonly
{
  public:
	Config(const Config &config);
	Chat	chat;
	int		a;
	long	pp;
};