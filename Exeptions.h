namespace PlanetSystem{
	namespace Exeptions{
#pragma once
#include <exception>

		using namespace std;
		class NoActiveScene : public exception
		{
			virtual const char* what() const throw()
			{
				return "No Active Scene";
			}
		} myex;
	}
}
