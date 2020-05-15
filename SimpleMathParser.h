#pragma once
#include <vector>
#include <string>
#include <map>
#include <vector>
#include <math.h>

namespace smp //Simple Math Parser
{
	//Constants
	const double PI_VALUE = 3.141592653589793;
	const double E_VALUE =  2.718281828459045;
	static char x_alias = 'x';

	static std::map<char, double> Constants;

	double getNumberFromLetter(char symb, double x_value);
	bool isLetter(char symb);
	bool isDigit(char symb);
	bool isServiceSymbol(char symb);
	void InitializeConstants(std::map<char, double> *consts = nullptr, bool addConstants = true);
	void setNewXAlias(char symb);


	class InvalidExpression : public std::exception
	{
	private:
		std::string m_error;

	public:
		InvalidExpression(std::string error) : m_error(error) {}

		const char* what() const noexcept { return m_error.c_str(); }
	};


	class Oper //Abstract class 
	{
	private:
		void CutUnnecessary();
		
	protected:
		std::vector<Oper*> sub_opers;
		std::string value;
		double x_value;

		Oper(std::string value = "");
		void prepareString();
		void clearMemory();
		void checkBracketsCorrect();

		virtual ~Oper();

	public:
		virtual double getResult(double x) = 0;
		virtual double getResult() { return 1; }
		virtual void setExpression(std::string str = "") = 0;
		virtual void updateSubOpers() = 0;

		std::string getExpression() { return this->value; }
		void setXValue(double x) { x_value = x; }
		double getXValue() { return x_value; }
	};


	class IActionsMap
	{
	protected:
		std::vector<char> actions;

		virtual ~IActionsMap() {};
	};



	class Expression : public Oper, public IActionsMap
	{
	private:
		void updateSubOpers() override;
	public:
		Expression(std::string value = "");

		double getResult(double x) override;
		double getResult() override { return getResult(x_value); }
		void setExpression(std::string str = "") override;

		~Expression() { int a; };
	};



	class Multiplication_Oper : public Oper, public IActionsMap
	{
	private:
		void updateSubOpers() override;
	public:
		Multiplication_Oper(std::string value);

		double getResult(double x) override;
		void setExpression(std::string str) override;

		~Multiplication_Oper() { int a; };
	};



	class Number : public Oper 
	{
	private:
		void updateSubOpers() override {}
	public:
		Number(std::string value);

		double getResult(double x) override;
		void setExpression(std::string str) override;

		~Number() { int a; };
	};



	class Power_Oper : public Oper
	{
	private:
		void updateSubOpers() override;
	public:
		Power_Oper(std::string value);

		double getResult(double x) override;
		void setExpression(std::string str) override;
	};




	//BONUS!
	//Additional class for generating ranges

	class RangeException : public std::exception
	{
	private:
		std::string m_error;

	public:
		RangeException(std::string error) : m_error(error) {}

		const char* what() const noexcept { return m_error.c_str(); }
	};



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
					result_vector.push_back(start_value + step_value*i);
				}
			}
		}

	};
}


