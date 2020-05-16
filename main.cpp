#include "SimpleMathParser.h"

#include <iostream>
#include <map>

int main()
{
	std::map<char, double> mp; //Example of setting user`s constant
	mp.emplace('g', 9.8);

	smp::InitializeConstants(&mp); //Will be created 'g' and standart ones (can be canceled or redifined): 'e', 'p'
	smp::Expression exp; //Initialisation of Expression (can be done without start expression)
	std::string str_exp;

	exp.setXValue(2); //Set x value

	while (true)
	{
		std::cout << "Input your expression or function: ";
		std::getline(std::cin, str_exp);

		try
		{
			exp.setExpression(str_exp); //Check this: 4p(M / (2pRT))^(3/2) * v^2 * e^(-Mv^2/(2RT))

			std::cout << "\nf(" << exp.getXValue() << ") = " << exp.getExpression() << " = ";
			std::cout << exp.getResult() << std::endl; //getResult() used without arguments, so it will use x_value
		}

		catch (smp::InvalidExpression &exp)
		{
			std::cout << exp.what();
		}

		std::cout << "\n\n";
	}


	
	int d;
	std::cin >> d;

	return 0;
}