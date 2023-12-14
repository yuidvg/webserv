#include "HTTPParser.hpp"

HTTPPARSER_H::HTTPPARSER_H() {}

HTTPPARSER_H::HTTPPARSER_H(const HTTPPARSER_H &src)
{
	*this = src;
}

HTTPPARSER_H	&HTTPPARSER_H::operator=(const HTTPPARSER_H &src)
{
	if (this != &src)
	{
		// copy
	}
	return (*this);
}

HTTPPARSER_H::~HTTPPARSER_H() {}

