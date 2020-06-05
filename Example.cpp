#include <iostream>
#include "SimpleMathParser.h" 

using namespace std;

//Example of program which uses Simple Math Parser (SMP)


//Prototypes of my own math functions which shall be added in parser (definitions of these you can find under main())
double gamma(double val); 
int N_factroial(int n);
double Factorial(double x); 


int main()
{
	smp::Expression myExp; //myExp instance
	
	try //Try to add own constants and functions
	{
		myExp.addConstant('g', 9.87);
		myExp.addConstant('p', 1.64);
		myExp.addFunction("fact", Factorial); //Adding factorial function
		myExp.addFunction("G", gamma); //Adding gamma function
	}

	catch (smp::IncorrectFunctionName& exp) //That is throwed if you add a function with incorrect name (only letters!)
	{
		cout << exp.what();
	}

	catch (smp::IncorrectConstantName& exp) //That is throwed if you add a constant with incorrect name (only letters!)
	{
		cout << exp.what();
	}

	while (true) //Main loop
	{
		try //Waiting for input of an expression and trying to calculate this
		{
			string str;
			cout << "Input expression: ";
			getline(cin, str);
			cout << "\n\n";
			myExp.setExpression(str);
			cout << "f(1) = " << myExp.getExpression() << " = " << myExp.getResult(1); //Just use 1 as default value of x
		}

		/*catch (smp::InvalidExpression& exp) //Base kind of any excecption (parent class for all exceptions in smp)
		{
			 cout << exp.what();
		}*/

		catch (smp::IncorrectSyntax& exp) //That`s gonna throw IncorrectSyntax: x^
		{
			cout << exp.what();
		}

		catch (smp::MathError& exp) //That throwed if you break math rules: (-4) ^ (1 / 4) or 1 / 0
		{
			cout << exp.what();
		}

		catch (smp::InvalidBracketsCount& exp) //That`s gonna throw InvalidBracketsCount: ((2 + 1) * x
		{
			cout << exp.what();
		}

		catch (smp::MathFunctionCrash& exp) //Common kind of function erros, can be throwed only with inheritor
		{
			cout << exp.what();
		}

		catch (smp::IncorrectArgument& exp) //That throwed when you use incorrect arguments with function: asin(10) or sqrt(-4)
		{
			cout << exp.what();
		}

		catch (smp::ConversionError& exp) //That`s gonna throw ConversionError: x.23 or x,
		{
			cout << exp.what();
		}

		cout << "\n------------\n\n";
	}

	return 0;
}


double gamma(double val) //Simple gamma function for example
{
	int const n = 999999; //Accurate value
	double result = 1;

	result /= val;
	for (int i = 1; i <= n; i++)
	{
		result *= i;
		result /= (val + i);
	}
	result *= pow(n, val);

	return result;
}

int N_factroial(int n)
{
	int result = 1;
	for (int i = 1; i <= n; i++)
	{
		result *= i;
	}

	return result;
}

double Factorial(double x) //Custom realization of factorial
{
	if (x < 0)
		throw smp::IncorrectArgument("Factorial argument can`t be less than zero!", "fact", to_string(x)); //We are throwing an exception

	double cel;
	if (modf(x, &cel) == 0)
		return N_factroial(x); //Factorial for natural number or zero

	return gamma(x + 1); //Factorial for float
}