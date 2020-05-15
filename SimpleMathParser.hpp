#include "SimpleMathParser.h"

using namespace smp;


//Special Functions -------------------------

double smp::getNumberFromLetter(char symb, double x_value)
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
}

bool smp::isLetter(char symb)
{
	if (symb >= 'a' && symb <= 'z' || symb >= 'A' && symb <= 'Z') 
		return true;

	return false;
}

bool smp::isDigit(char symb)
{
	if (symb >= '0' && symb <= '9')
		return true;

	return false;
}

bool smp::isServiceSymbol(char symb)
{
	if (symb == '(' || symb == ')' || symb == '+' || symb == '-' || symb == '*' || symb == '/' || symb == '^' || symb == '.' || symb == ':')
		return true;
	return false;
}

//If the function is called without arguments, all constants are set by default.
void smp::InitializeConstants(std::map<char, double>* consts, bool addConstants)
{
	Constants.clear();

	if (consts != nullptr)
	{
		Constants = *consts;
	}

	if (addConstants)
	{
		if(Constants.find('p') == Constants.end())//Default constants (No forced redifinitions)
			Constants['p'] = PI_VALUE;
		 
		if (Constants.find('e') == Constants.end())
			Constants['e'] = E_VALUE;
	}
}

void smp::setNewXAlias(char symb)
{
	x_alias = symb;
}




//Oper definintions-------------
Oper::Oper(std::string value)
{
	this->value = value;
	checkBracketsCorrect();
	prepareString();
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

void smp::Oper::prepareString()
{
	CutUnnecessary();

	for (int i = 0; i < value.size(); i++)
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
					if (value[i - 1] != '^' && value[i - 1] != '+' && value[i - 1] != '-' && value[i - 1] != 'x' && value[i - 1] != '/' && value[i - 1] != '(')
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

void smp::Oper::clearMemory()
{
	for (auto& var : sub_opers)
	{
		delete var;
	}

	sub_opers.clear();
}

void smp::Oper::checkBracketsCorrect()
{
	int opened_brackets = 0;
	int closed_brackets = 0;

	for (auto &var : value)
	{
		if (var == '(')
			opened_brackets++;
		else if (var == ')')
			closed_brackets++;
	}

	if (closed_brackets != opened_brackets)
		throw InvalidExpression("Brackets count is invalid. Expression isn`t correct!");
}

smp::Oper::~Oper()
{
	clearMemory();
}
//--------------------------------



//Expression defenitions----
Expression::Expression(std::string value) : Oper(value) { setExpression(value); }

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
	prepareString();
	clearMemory();
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

		sub_opers.push_back(new Multiplication_Oper(new_string));
	}
}
//------------------------



//Multiplication_Oper defenitions------------------------------------------
Multiplication_Oper::Multiplication_Oper(std::string value) : Oper(value) { setExpression(value); }

//TODO: Реализовать чтение строки value и рекурсивное создание новых Oper для умножения 
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
	clearMemory();
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

		while (i < value.size() && value[i] == '*' || value[i] == '/' || value[i] == ')')
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
						if (value[i + 1] == '^')
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
								sub_opers.push_back(new Expression(new_string));
								break;
							}

							else
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(new Power_Oper(new_string));
								break;
							}

						}
					}

					else if (opened_count == 0)
					{
						if (isComplexPowerMode == 0)
						{
							new_string.erase(0, 1);
							sub_opers.push_back(new Expression(new_string));
							break;
						}

						else
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(new Power_Oper(new_string));
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
						sub_opers.push_back(new Power_Oper(new_string));
						break;
					}
				}

				new_string.push_back(value[i]);
			}

			if (i > value.size() - 1)
			{
				if (isComplexPowerMode == 0)
					sub_opers.push_back(new Expression(new_string));
				else
					sub_opers.push_back(new Power_Oper(new_string));
			}

		}

		else if (!isLetter(value[i]))
		{
			int isComplexPowerMode = 0; //0 - Not defined, 1 - Complex, 2 - Not complex
			for (i; i < value.size(); i++, j++)
			{
				if (isComplexPowerMode == 0)
				{
					if (value[i] == '(' || isLetter(value[i]) || value[i] == '*' || value[i] == '/')
					{
						sub_opers.push_back(new Number(new_string));
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
						if (i < value.size() - 2)
						{
							if (value[i + 1] == '^')
							{
								if (value[i + 2] == '(')
									isComplexPowerMode = 1;
								else
									isComplexPowerMode = 2;
							}

							else
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(new Power_Oper(new_string));
								break;
							}
						}

						else
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(new Power_Oper(new_string));
							break;
						}
					}
				}

				if (isComplexPowerMode == 2)
				{
					if ((value[i] == '*' || value[i] == '/' || isLetter(value[i])) && i < value.size() - 1 && value[i + 1] != '^')
					{
						new_string.push_back(value[i]);
						sub_opers.push_back(new Power_Oper(new_string));
						break;
					}
				}
				new_string.push_back(value[i]);
			}


			if (i > value.size() - 1)
			{
				if(isComplexPowerMode == 0)
					sub_opers.push_back(new Number(new_string));
				else 
					sub_opers.push_back(new Power_Oper(new_string));
			}
		}

		else if (i < value.size() - 2 && value[i + 1] == '^')
		{
			int isComplexPowerMode; //0 - Not defined, 1 - Complex, 2 - Not complex

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
						if (i < value.size() - 2)
						{
							if (value[i + 1] == '^')
							{
								if (value[i + 2] == '(')
									isComplexPowerMode = 1;
								else
									isComplexPowerMode = 2;
							}

							else
							{
								new_string.push_back(value[i]);
								sub_opers.push_back(new Power_Oper(new_string));
								break;
							}
						}

						else
						{
							new_string.push_back(value[i]);
							sub_opers.push_back(new Power_Oper(new_string));
							break;
						}
					}
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
							sub_opers.push_back(new Power_Oper(new_string));
							break;
						}
					}
				}

				new_string.push_back(value[i]);
			}

			if (i > value.size() - 1)
				sub_opers.push_back(new Power_Oper(new_string));
		}

		else
		{
			new_string.push_back(value[i]);
			sub_opers.push_back(new Number(new_string));
			i++;
		}
	}
}
//------------------------




//Power_Oper defenitions------------------------------------------
Power_Oper::Power_Oper(std::string value) : Oper(value) { setExpression(value); }

//TODO: Реализовать чтение строки value и рекурсивное создание новых Oper для сложения 
double Power_Oper::getResult(double x)
{
	return pow(sub_opers.at(0)->getResult(x), sub_opers.at(1)->getResult(x));
}

void smp::Power_Oper::setExpression(std::string str)
{
	this->value = str;
	clearMemory();
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
			i = 1;
			for (i; i < value.size(); i++)
			{
				if (value[i] == ')')
					break;

				power_base.push_back(value[i]);
			}
			i += 2;

			sub_opers.push_back(new Expression(power_base));
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

			sub_opers.push_back(new Number(power_base));
		}
	}

	//Factor of power
	for (i; i < value.size(); i++)
	{
		power_factor.push_back(value[i]);
	}

	sub_opers.push_back(new Expression(power_factor));
}
//------------------------



//Number definitions------
Number::Number(std::string value) : Oper(value) {}

double Number::getResult(double x)
{
	if (value.size() != 0)
	{
		if (isLetter(value[0]))
			return getNumberFromLetter(value[0], x);
		else
		{
			try
			{
				return std::stod(value);
			}

			catch (std::invalid_argument&)
			{
				throw InvalidExpression("stof() couldn`t convert string to double. Expression isn`t correct!");
			}
		}
			
	}

	else
	{
		throw InvalidExpression("String was empty. Expression isn`t correct!");
	}
}

void smp::Number::setExpression(std::string str)
{
	this->value = str;
}
//-------------------------
