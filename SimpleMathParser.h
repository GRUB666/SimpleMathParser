#pragma once
#include <vector>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <memory>


//Created by grub_666 (Alexander Furmanov)
//Check README file on github: https://github.com/GRUB666/SimpleMathParser
//My contact mail: aleksandrfurmanoa@gmail.com


constexpr auto PI = 3.141592653589793; //Just constants;
constexpr auto E = 2.718281828459045;

namespace smp //Simple Math Parser namespace
{
	//Exception-classes----------------------------
	enum  ExceptionType //Views of exceptions
	{
		IncorrectSyntax,
		InvalidBracketsCount,
		MathFunctionCrash,
		ConversionError
	};

	class InvalidExpression : public std::exception //Main exception class
	{
	private:
		std::string m_error;
		ExceptionType type;

	public:
		InvalidExpression(ExceptionType tp, std::string error) : m_error(error) 
		{
			type = tp;
		}

		ExceptionType getExceptionType() { return type; }

		const char* what() const noexcept { return m_error.c_str(); }
	};

	class RangeException : public std::exception
	{
	private:
		std::string m_error;

	public:
		RangeException(std::string error) : m_error(error) {}

		const char* what() const noexcept { return m_error.c_str(); }
	};




	using Function = double(*)(double argument);

	struct ParserSettings
	{
		std::map<char, double> Constants;
		std::map<std::string, double(*)(double argument)> Functions;
		char x_alias = 'x';


		ParserSettings()
		{
			InitializeConstants();
			InitializeFunctions();
		}

		void InitializeConstants(std::map<char, double> *consts = nullptr, bool addConstants = true);
		void InitializeFunctions(std::map<std::string, double(*)(double argument)> *funcs = nullptr, bool addFunctions = true);

		double getNumberFromLetter(char symb, double x_value);
		Function getFunctionFromString(std::string func_name);
	};


	class Oper //Base abstract class
	{
	private:
		void CutUnnecessary();
		void FunctionsMarker();
		
	protected:
		std::vector<std::shared_ptr<Oper>> sub_opers;
		std::string value;
		std::shared_ptr<ParserSettings> ps;
		double x_value;

		Oper(std::string value = "", std::shared_ptr<ParserSettings> ps = nullptr);
		void prepareString();
		void clearSubOpers();
		void checkBracketsCorrect();
		void replaceIncorrectSymbols();
		bool isLetter(char symb);
		bool isDigit(char symb);
		bool isServiceSymbol(char symb);

		virtual ~Oper();

	public:
		virtual double getResult(double x) = 0;
		virtual double getResult() { return 1; }
		virtual void setExpression(std::string str = "") = 0;
		virtual void updateSubOpers() = 0;

		std::string getExpression() { return this->value; }
		void setXValue(double x) { x_value = x; }
		double getXValue() { return x_value; }
		void setConstants(std::map<char, double> *consts = nullptr, bool addConstants = true);
		void addConstant(char symb, double value);
		void resetConstants(bool addDefault = true);

		void setFunctions(std::map<std::string, double(*)(double argument)> *funcs = nullptr, bool addFunctions = true);
		void addFunction(std::string name, double(*function)(double argument));
		void resetFunctions(bool addDefault = true);
	    void setNewXAlias(char symb);
	};


	class IActionsMap //Map of mathematical actions (multipling, dividing e.t.c)
	{
	protected:
		std::vector<char> actions;

		virtual ~IActionsMap() {};
	};


	//Main class. To be used in external space
	class Expression : public Oper, public IActionsMap
	{
	private:
		void updateSubOpers() override;
	public:
		Expression(std::string value = "", bool toBePrepared = true, std::shared_ptr<ParserSettings> ps = nullptr);

		double getResult(double x) override;
		double getResult() override { return getResult(x_value); }
		void setExpression(std::string str = "") override;
	};



	class Multiplication_Oper : public Oper, public IActionsMap
	{
	private:
		void updateSubOpers() override;
	public:
		Multiplication_Oper(std::string value, std::shared_ptr<ParserSettings> ps);

		double getResult(double x) override;
		void setExpression(std::string str) override;

		~Multiplication_Oper() { };
	};



	class Number : public Oper 
	{
	private:
		void updateSubOpers() override {}
	public:
		Number(std::string value, std::shared_ptr<ParserSettings> ps);

		double getResult(double x) override;
		void setExpression(std::string str) override;

		~Number() { };
	};



	class Power_Oper : public Oper
	{
	private:
		void updateSubOpers() override;
	public:
		Power_Oper(std::string value, std::shared_ptr<ParserSettings> ps);

		double getResult(double x) override;
		void setExpression(std::string str) override;
	};


	class Function_Oper : public Oper
	{
	private:
		void updateSubOpers() override;
	public:
		Function_Oper(std::string value, std::shared_ptr<ParserSettings> ps);

		double getResult(double x) override;
		void setExpression(std::string str) override;
	};




	//BONUS!
	//Additional class for generating ranges
	class RangeGenerator
	{
	private:
		float step_value;
		float min, max;
		Expression expression;
		std::vector<double> result_vector;

	public:
		RangeGenerator(double minimum = -5, double maximum = 5, double step = 1)
		{
			setLimits(minimum, maximum);
			setStep(step);
		}

		void setExpression(Expression& exp)
		{
			expression = exp;
		}

		void setLimits(double minimum, double maximum)
		{
			if (minimum > maximum)
				throw RangeException("Uncorrect limits of range! Minimum > Maximum!");
			else
			{
				min = minimum;
				max = maximum;
			}
		}
		void setStep(double step) //Step must be more than 0
		{ 
			if (step <= 0)
				throw RangeException("Uncorrect step value! Step must be more than 0!");
			else
				step_value = step;
		}  

		double getMinimum() { return min; }
		double getMaximum() { return max; }
		double getStep() { return step_value; }
		
		std::vector<double>& getResultVector()
		{
			calculateVector();
			return result_vector;
		}

	private:
		void calculateVector()
		{
			result_vector.clear();
			if (min <= max)
			{
				double start_value = min;
				int i = 0;

				while (start_value <= max)
				{
					result_vector.push_back(this->expression.getResult(start_value));
					start_value += step_value;
				}
			}
		}
	};

}


