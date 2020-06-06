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
	smp::Expression giperbola_exp("1 / x"); //NEW!!! We are about to add this function in myExp like we do same with Factorial and gamma 
	
	try //Try to add own constants and functions (also we try to change alias of variable)
	{
		myExp.addConstant('g', 9.87); //Adding new constants
		myExp.addConstant('h', 1.64);

		myExp.addFunction("fact", Factorial); //Adding factorial function
		myExp.addFunction("G", gamma); //Adding gamma function

		myExp.addFunction("giperbola", giperbola_exp); //Adding giperbola expression! 
		//Also there is the third default parameter "save_origin_parser_setting" it`s false as default. 
		//That means all functions and constants of myExp will be written in giperbola_exp (common namespace)

		myExp.deleteFunction("giperbola"); //Example of deleting function and constant
		myExp.deleteConstant('h', true); //There are two parameters: "name" and "isThrow", as defeault isThrow = false
		//If isTrow = true, you would get exception when trying to delete function or constant that doesn`t exist


		myExp.setNewXAlias('y'); //Also we can change alias of variable in expression (Default is 'x')
		myExp.setNewXAlias('a');
		myExp.setNewXAlias('z'); //Now the variable in myExp is z
		myExp.setNewXAlias('x'); //And now this is a default value
	}

	catch (smp::IncorrectFunctionName& exp) //That is throwed if you add a function with incorrect name (only letters!)
	{
		cout << exp.what();
	}

	catch (smp::IncorrectConstantName& exp) //That is throwed if you add a constant with incorrect name (only letters!)
	{
		cout << exp.what();
	}

	catch (smp::IncorrectXAliasName& exp) //That is throwed if you change alias of x not correctly (only letters!)
	{
		cout << exp.what();
	}

	myExp.setXValue(4); //We can set default variable value

	while (true) //Main loop
	{
		try //Waiting for input of an expression and trying to calculate this
		{
			string str;
			cout << "Input expression: "; //Now parser can understand 16 standart math functions (like cos) and 2 own ones (Factorial, Gamma, Giperbola was deleted)
			getline(cin, str); //Also parser can understand 2 standart constants and 2 own ones
			cout << "\n\n";
			myExp.setExpression(str);
			cout << "f(" + std::to_string(myExp.getXValue()) + ") = " << myExp.getExpression() << " = " << myExp.getResult(); //Just use a default value of variable
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

		catch (smp::RecursionException& exp) //That throwed if your expression uses other expression that has reference to first one
		{
			cout << exp.what(); //That checks in addFunction()
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