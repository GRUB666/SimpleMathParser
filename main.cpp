#include "SimpleMathParser.h"

#include <iostream>
#include <map>


double MyFunction(double argument) //Our additional function with name "abs"
{
	return std::abs(argument);
}


int main()
{
	smp::Expression exp; //Initialisation of Expression (with initialization of default constants)
	std::string str_exp;

	exp.setXValue(2); //Set x default value
	exp.addConstant('g', 9.8); //We can add any constants

	exp.addFunction("abs", MyFunction); //Season hit! We can add own functions!

	while (true)
	{
		std::cout << "Input your expression or function: ";
		std::getline(std::cin, str_exp);

		try //abs(sin(x) + cos(x)) //
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

	return 0;
}