#include "SimpleMathParser.h"

using namespace smp;


//If the function is called without arguments, all constants are set by default.
void smp::ParserSettings::InitializeConstants(std::map<char, double>* consts, bool addConstants)
{
	Constants.clear();

	if (consts != nullptr)
	{
		Constants = *consts;
	}

	if (addConstants)
	{
		if (Constants.find('p') == Constants.end())//Default constants (No forced redifinitions)
			Constants['p'] = PI;

		if (Constants.find('e') == Constants.end())
			Constants['e'] = E;
	}
}

void smp::ParserSettings::InitializeFunctions(std::map<std::string, double(*)(double argument)> *funcs, bool addFunctions)
{
	Functions.clear();

	if (funcs != nullptr)
		this->Functions = *funcs;

	if (addFunctions) //Standart functions
	{
		Functions["arcctg"] = [](double argument) { return std::atan(1 / argument); };
		Functions["ctg"] = [](double argument) { return 1 / std::tan(argument); };
		Functions["cth"] = [](double argument) { return 1 / std::tanh(argument); };

		Functions["sin"] = std::sin;
		Functions["cos"] = std::cos;
		Functions["tan"] = std::tan;
		Functions["asin"] = std::asin;
		Functions["acos"] = std::acos;
		Functions["arctg"] = std::atan;

		Functions["sh"] = std::sinh;
		Functions["ch"] = std::cosh;
		Functions["th"] = std::tanh;

		Functions["sqrt"] = std::sqrt;
		Functions["lg"] = std::log10;
		Functions["ln"] = [](double argument) { return std::log(argument) / std::log(E); };
		Functions["abs"] = std::abs;
	}
}

double smp::ParserSettings::getNumberFromLetter(char symb, double x_value)
{
	if (symb != x_alias)
	{
		auto it = Constants.find(symb);
		if (it != Constants.end())
			return Constants.at(symb);
		else
			return 1;
	}

	else
		return x_value;
	return 0.0;
}

Function smp::ParserSettings::getFunctionFromString(std::string func_name)
{
	auto it = Functions.find(func_name);

	if (it == Functions.end())
		throw InvalidExpression(MathFunctionCrash, "Something went wrong with function. Expression isn`t correct!");

	return Functions[func_name];
}





//Oper definintions-------------
Oper::Oper(std::string value, std::shared_ptr<ParserSettings> ps)
{
	this->value = value;
	
	if (ps == nullptr)
	{
		std::shared_ptr<ParserSettings> ps_tmp(new ParserSettings);
		this->ps = ps_tmp;
	}
		
	else
		this->ps = ps;
}

void smp::Oper::setNewXAlias(char symb)
{
	ps->x_alias = symb;
}

bool smp::Oper::isLetter(char symb)
{
	if (symb >= 'a' && symb <= 'z' || symb >= 'A' && symb <= 'Z')
		return true;

	return false;
}

bool smp::Oper::isDigit(char symb)
{
	if (symb >= '0' && symb <= '9')
		return true;

	return false;
}

bool smp::Oper::isServiceSymbol(char symb)
{
	if (symb == '(' || symb == ')' || symb == '+' || symb == '-' || symb == '*' || symb == '/' || symb == '^' || symb == '.' || symb == ':')
		return true;
	return false;
}


void smp::Oper::CutUnnecessary() //Whitespaces, special symbols and unreadable symbols like '?'
{
	for (int i = 0; i < value.size(); i++)
	{
		if (!isLetter(value[i]) && !isDigit(value[i]) && !isServiceSymbol(value[i]))
		{
			value.erase(i, 1);
			i--;
		}
	}
}

void smp::Oper::FunctionsMarker()
{
	int pos;
	for (auto &var : ps->Functions)
	{
		pos = value.find(var.first, 0);
		std::string new_string = "({" + var.first + "}";

		while (pos != std::string::npos)
		{
			if (value[pos + var.first.size()] != '}')
			{
				value.replace(pos, var.first.length(), new_string);

				int j = pos + var.first.size() + 4;
				int opened_brackets = 1;

				while (j < value.size() && opened_brackets != 0)
				{
					if (value[j] == '(')
						opened_brackets++;
					else if (value[j] == ')')
						opened_brackets--;
					j++;
				}

				if (j < value.size() - 1)
					value.insert(j, 1, ')');
				else
					value.push_back(')');
			}

			pos = value.find(var.first, pos + 3);
		}
	}
}

void smp::Oper::prepareString()
{
	replaceIncorrectSymbols();
	CutUnnecessary();
	FunctionsMarker();

	bool isFunction = false;

	for (int i = 0; i < value.size(); i++)
	{
		if (value[i] == '{')
			isFunction = true;
		else if (value[i] == '}')
			isFunction = false;

		if (!isFunction)
		{
			if (i > 0)
			{
				if (isLetter(value[i]))
				{
					if (value[i - 1] != '*')
					{
						if (value[i - 1] != '+' && value[i - 1] != '-' && value[i - 1] != '^' && value[i - 1] != '/' && value[i - 1] != '(')
						{
							value.insert(i, 1, '*');
							i++;
						}
					}
				}

				if (value[i - 1] != '*')
				{
					if (value[i] == '(')
					{
						if (value[i - 1] != '^' && value[i - 1] != '+' && value[i - 1] != '-' && value[i - 1] != 'x' && value[i - 1] != '/' && value[i - 1] != '(' && value[i - 1] != '}')
						{
							value.insert(i, 1, '*');
							i++;
						}
					}
				}
			}

			if (i < value.size() - 1)
			{
				if (isLetter(value[i]))
				{
					if (value[i + 1] != '*')
					{
						if (value[i + 1] != '+' && value[i + 1] != '-' && value[i + 1] != '^' && value[i + 1] != '/' && value[i + 1] != ')')
						{
							value.insert(i + 1, 1, '*');
						}
					}
				}

				if (value[i] == ')')
				{
					if (value[i + 1] != '*')
					{
						if (value[i + 1] != '^' && value[i + 1] != '+' && value[i + 1] != '-' && value[i + 1] != 'x' && value[i + 1] != '/' && value[i + 1] != ')')
						{
							value.insert(i + 1, 1, '*');
						}
					}
				}
			}
		}
	}
}

void smp::Oper::clearSubOpers()
{
	sub_opers.clear();
}

void smp::Oper::checkBracketsCorrect()
{
	int opened_count = 0;

	for (auto &var : value)
	{
		if (var == '(')
			opened_count++;
		else if (var == ')')
			opened_count--;
	}

	if (opened_count != 0)
		throw InvalidExpression(InvalidBracketsCount, "Brackets count is invalid. Expression isn`t correct!");
}


void smp::Oper::replaceIncorrectSymbols()
{
	std::string to_change_syms[3]{ ":", ",", "pi" };

	for (auto var : to_change_syms)
	{
		int to_change = value.find(var); //Find all ':'
		while (to_change != std::string::npos)
		{
			if(var == ":")
				value[to_change] = '/';
			else if(var == ",")
				value[to_change] = '.';
			else if(var == "pi" && ps->Constants.find('p') != ps->Constants.end())
				value.replace(to_change, var.length(), "p");

			to_change = value.find(var, to_change + 1);
		}
	}

	
}

void smp::Oper::setConstants(std::map<char, double>* consts, bool addConstants)
{
	ps->InitializeConstants(consts, addConstants);
}

void smp::Oper::addConstant(char symb, double value)
{
	ps->Constants.emplace(symb, value);
}

void smp::Oper::setFunctions(std::map<std::string, double(*)(double argument)>* funcs, bool addFunctions)
{
	ps->InitializeFunctions(funcs, addFunctions);
}

void smp::Oper::addFunction(std::string name, double(*function)(double argument))
{
	ps->Functions[name] = function;
}

void smp::Oper::resetConstants(bool addDefault)
{
	if (addDefault)
		ps->InitializeConstants();
	else
		ps->Constants.clear();
}

void smp::Oper::resetFunctions(bool addDefault)
{
	if (addDefault)
		ps->InitializeFunctions();
	else
		ps->Constants.clear();
}

smp::Oper::~Oper()
{
	clearSubOpers();
}
//--------------------------------



//Expression defenitions----
Expression::Expression(std::string value, bool toBePrepared, std::shared_ptr<ParserSettings> ps) : Oper(value, ps)
{ 	
	checkBracketsCorrect();

	if (toBePrepared)
		prepareString();

	updateSubOpers();
}

double Expression::getResult(double x)
{
	double result = 0;
	int counter = 0;
	int k = 1;

	for (auto& var : sub_opers) 
	{
		if (actions[counter] == '+')
			k = 1;
		else
			k = -1;

		result += var->getResult(x) * k;
		counter++;
	}

	return result;
}

void smp::Expression::setExpression(std::string str)
{
	this->value = str;
	checkBracketsCorrect();
	prepareString();
	clearSubOpers();
	updateSubOpers();
}

void smp::Expression::updateSubOpers()
{
	actions.clear();

	int i = 0;
	if (value[i] != '-')
		actions.push_back('+');

	while (true)
	{
		while (value[i] == '-' || value[i] == '+')
		{
			actions.push_back(value[i]);
			i++;
		}

		if (i >= value.size())
			break;

		int j = 0;
		std::string new_string = "";
		bool isBracketOpened = false;
		int openedBrackets = 0;
		for (i; i < value.size(); i++, j++)
		{
			if ((value[i] == '+' || value[i] == '-') && !isBracketOpened)
				break;

			if (value[i] == '(')
			{
				isBracketOpened = true;
				openedBrackets++;
			}
			else if (value[i] == ')')
			{
				openedBrackets--;
				isBracketOpened = openedBrackets != 0;
			}

			new_string.push_back(value[i]);
		}

		sub_opers.push_back(std::shared_ptr<Oper>(new Multiplication_Oper(new_string, ps)));
	}
}
//------------------------



//Multiplication_Oper defenitions------------------------------------------
Multiplication_Oper::Multiplication_Oper(std::string value, std::shared_ptr<ParserSettings> ps) : Oper(value, ps) { setExpression(value); }

double Multiplication_Oper::getResult(double x)
{
	double result = 1;
	for (int i = 0; i < sub_opers.size(); i++)
	{
		if (actions[i] == '*')
			result *= sub_opers[i]->getResult(x);
		else
			result /= sub_opers[i]->getResult(x);
	}

	return result;
}

void smp::Multiplication_Oper::setExpression(std::string str)
{
	this->value = str;
	clearSubOpers();
	updateSubOpers();
}

void smp::Multiplication_Oper::updateSubOpers()
{
	actions.clear();
	actions.push_back('*');

	int i = 0;
	while (true)
	{
		int j = 0;
		std::string new_string = "";

		while (i < value.size() && value[i] == '*' || value[i] == '/' || value[i] == ')') //Filling the actions map
		{
			if (value[i] == '*' || value[i] == '/')
				actions.push_back(value[i]);

			i++;
		}


		if (i >= value.size())
			break;

		//An expression
		if (value[i] == '(')
		{
			int isComplexPowerMode = 0; //0 - Not defined, 1 - Complex, 2 - Not complex
			int opened_count = 0;
			for (i; i < value.size(); i++, j++)
			{
				if (value[i] == ')')
				{
					opened_count--;
					if (i < value.size() - 2)
					{
						if (value[i + 1] == '^' && opened_count == 0)
						{
							if (value[i + 2] == '(')
								isComplexPowerMode = 1;
							else
								isComplexPowerMode = 2;
						}

						else if (opened_count == 0)
						{
							if (isComplexPowerMode == 0)
							{
								new_string.erase(0, 1); 
								sub_opers.push_back(std::shared_ptr<Oper>(new Expression(new_string, false, ps)));
								break;
							}

							else
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
								break;
							}

						}
					}

					else if (opened_count == 0)
					{
						if (isComplexPowerMode == 0)
						{
							new_string.erase(0, 1);
							sub_opers.push_back(std::shared_ptr<Oper>(new Expression(new_string, false, ps)));
							break;
						}

						else
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
							break;
						}
					}
				}

				if (value[i] == '(')
				{
					opened_count++;
				}

				if (isComplexPowerMode == 2)
				{
					if ((value[i] == '*' || value[i] == '/' || isLetter(value[i])) && i < value.size() - 1 && value[i + 1] != '^')
					{
						sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
						break;
					}
				}

				new_string.push_back(value[i]);
			}

			if (i > value.size() - 1)
			{
				if (isComplexPowerMode == 0)
					sub_opers.push_back(std::shared_ptr<Oper>(new Expression(new_string, false, ps)));
				else
					sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
			}

		}

		//A function
		else if (value[i] == '{')
		{
			int opened_count = 0;
			for (i; i < value.size(); i++)
			{
				if (value[i] == '(')
					opened_count++;
				else if (value[i] == ')')
					opened_count--;

				new_string.push_back(value[i]);
			}

			sub_opers.push_back(std::shared_ptr<Oper>(new Function_Oper(new_string, ps)));
		}

		//A number
		else if (!isLetter(value[i]))
		{
			int isComplexPowerMode = 0; //0 - Not defined, 1 - Complex, 2 - Not complex
			int opened_count = 0;
			for (i; i < value.size(); i++, j++)
			{
				if (isComplexPowerMode == 0)
				{
					if (value[i] == '(' || isLetter(value[i]) || value[i] == '*' || value[i] == '/')
					{
						sub_opers.push_back(std::shared_ptr<Oper>(new Number(new_string, ps)));
						break;
					}
				}

				if (i < value.size() - 2)
				{
					if (value[i + 1] == '^')
					{
						if (value[i + 2] == '(')
							isComplexPowerMode = 1;
						else
							isComplexPowerMode = 2;
					}
				}

				if (isComplexPowerMode == 1)
				{
					if (value[i] == ')')
					{
						opened_count--;
						if (i < value.size() - 2)
						{
							if (value[i + 1] == '^' && opened_count == 0)
							{
								if (value[i + 2] == '(')
									isComplexPowerMode = 1;
								else
									isComplexPowerMode = 2;
							}

							else if(opened_count == 0)
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
								break;
							}
						}

						else if (opened_count == 0)
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
							break;
						}
					}
				}

				if (value[i] == '(')
				{
					opened_count++;
				}

				if (isComplexPowerMode == 2)
				{
					if ((value[i] == '*' || value[i] == '/' || isLetter(value[i])) && i < value.size() - 1 && value[i + 1] != '^')
					{
						new_string.push_back(value[i]);
						sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
						break;
					}
				}
				new_string.push_back(value[i]);
			}


			if (i > value.size() - 1)
			{
				if(isComplexPowerMode == 0)
					sub_opers.push_back(std::shared_ptr<Oper>(new Number(new_string, ps)));
				else 
					sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
			}
		}

		//x in n power
		else if (i < value.size() - 2 && value[i + 1] == '^')
		{
			int isComplexPowerMode; //0 - Not defined, 1 - Complex, 2 - Not complex
			int opened_count = 0;

			if (value[i + 2] == '(')
				isComplexPowerMode = 1;
			else
				isComplexPowerMode = 2;

			new_string.push_back(value[i]);
			i++;

			for (i; i < value.size(); i++, j++)
			{
				if (isComplexPowerMode == 1)
				{
					if (value[i] == ')')
					{
						opened_count--;
						if (i < value.size() - 2)
						{
							if (value[i + 1] == '^' && opened_count == 0)
							{
								if (value[i + 2] == '(')
									isComplexPowerMode = 1;
								else
									isComplexPowerMode = 2;
							}

							else if (opened_count == 0)
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
								break;
							}
						}

						else if (opened_count == 0)
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
							break;
						}
					}
				}

				if (value[i] == '(')
				{
					opened_count++;
				}

				if (isComplexPowerMode == 2)
				{
					if (value[i] == '^')
					{
						if (i < value.size() - 1 && value[i + 1] == '(')
							isComplexPowerMode = 1;
					}

					else
					{
						if ((value[i] == '*' || value[i] == '/' || isLetter(value[i])) && i < value.size() - 1 && value[i + 1] != '^')
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
							break;
						}
					}
				}

				new_string.push_back(value[i]);
			}

			if (i > value.size() - 1)
				sub_opers.push_back(std::shared_ptr<Oper>(new Power_Oper(new_string, ps)));
		}

		//Just x
		else
		{
			new_string.push_back(value[i]);
			sub_opers.push_back(std::shared_ptr<Oper>(new Number(new_string, ps)));
			i++;
		}
	}
}
//------------------------


//Power_Oper defenitions------------------------------------------
Power_Oper::Power_Oper(std::string value, std::shared_ptr<ParserSettings> ps) : Oper(value, ps) { setExpression(value); }

double Power_Oper::getResult(double x)
{
	return pow(sub_opers.at(0)->getResult(x), sub_opers.at(1)->getResult(x));
}

void smp::Power_Oper::setExpression(std::string str)
{
	this->value = str;
	clearSubOpers();
	updateSubOpers();
}

void smp::Power_Oper::updateSubOpers()
{
	int i = 0;
	std::string power_base = "";
	std::string power_factor = "";

	//Base of power
	{
		if (value[0] == '(')
		{
			int opened_count = 0;
			for (i; i < value.size(); i++)
			{
				if (value[i] == '(')
					opened_count++;
				else if (value[i] == ')')
					opened_count--;

				power_base.push_back(value[i]);

				if (opened_count == 0)
					break;
			}
			i += 2;

			sub_opers.push_back(std::shared_ptr<Oper>(new Expression(power_base, false, ps)));
		}

		else
		{
			for (i; i < value.size(); i++)
			{
				if (value[i] == '^')
					break;

				power_base.push_back(value[i]);
			}
			i++;

			sub_opers.push_back(std::shared_ptr<Oper>(new Number(power_base, ps)));
		}
	}

	//Factor of power
	for (i; i < value.size(); i++)
	{
		power_factor.push_back(value[i]);
	}

	sub_opers.push_back(std::shared_ptr<Oper>(new Expression(power_factor, false, ps)));
}
//------------------------



//Number definitions------
Number::Number(std::string value, std::shared_ptr<ParserSettings> ps) : Oper(value, ps) {}

double Number::getResult(double x)
{
	if (value.size() != 0)
	{
		if (isLetter(value[0]))
			return ps->getNumberFromLetter(value[0], x);
		else
		{
			try
			{
				return std::stod(value);
			}

			catch (std::invalid_argument&)
			{
				throw InvalidExpression(ConversionError, "stof() couldn`t convert string to double. Expression isn`t correct!");
			}
		}
			
	}

	else
	{
		throw InvalidExpression(IncorrectSyntax, "String was empty. Expression isn`t correct!");
	}
}

void smp::Number::setExpression(std::string str)
{
	this->value = str;
}
//-------------------------




//Function definitions
smp::Function_Oper::Function_Oper(std::string value, std::shared_ptr<ParserSettings> ps) : Oper(value, ps) { setExpression(value); }

void smp::Function_Oper::setExpression(std::string str)
{
	this->value = str;
	clearSubOpers();
	updateSubOpers();
}

double smp::Function_Oper::getResult(double x) //You can add your functions there (Check documentation)
{
	std::string func_string;
	for (auto & var : ps->Functions) //Definding the the type of function
	{
		if (value.find(var.first, 0) != std::string::npos)
		{
			func_string = var.first;
			break;
		}
			
	}

	return ps->getFunctionFromString(func_string)(sub_opers[0]->getResult(x));
}

void smp::Function_Oper::updateSubOpers()
{
	std::string argument;
	int i = 0;

	while (value[i] != '(' && i < value.size())
		i++;

	i++; //We don`t read '('

	for (i; i < value.size() - 1; i++)
		argument.push_back(value[i]);

	sub_opers.push_back(std::shared_ptr<Oper>(new Expression(argument, false, ps)));
}


