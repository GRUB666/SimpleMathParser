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


/*Structure:
1. Definitions of default constants (PI and E)
2. strcut ParserSettings
3. Abstact class Oper
4. Interface IActionsMap
5. Class Expression (To use)
6. Class Multiplication_Oper (Service class)
7. Class Number_Oper (Service class)
8. Class Power_Oper (Service class)
9. Class Function_Oper (Service class)
10. Exception classes
11. Bonus rangeClass (To use)
*/

namespace smp //Simple Math Parser namespace
{
	constexpr auto PI = 3.141592653589793; //Just constants;
	constexpr auto E = 2.718281828459045;

	struct FunctionWrapper;
	class Expression;

	using Function = double(*)(double argument);


	struct MapFunctionCompare
	{
		bool operator()(const std::string &lhs, const std::string &rhs) const;
	};

	typedef std::map<std::string, FunctionWrapper, MapFunctionCompare> FunctionsMap;
	typedef std::map<char, double> ConstantsMap;

	struct ParserSettings
	{
		ConstantsMap Constants;
		FunctionsMap Functions;
		char x_alias = 'x';


		ParserSettings();

		void InitializeConstants(std::map<char, double> *consts = nullptr, bool addConstants = true);
		void InitializeFunctions(FunctionsMap *funcs = nullptr, bool addFunctions = true);

		double getFunctionValue(std::string name, double argument);
		double getNumberFromLetter(char symb, double x_value);
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
		std::string origin_string;

		Oper(std::string value = "", std::shared_ptr<ParserSettings> ps = nullptr);
		void prepareString();
		void clearSubOpers();
		void checkBracketsCorrect();
		void replaceIncorrectSymbols();
		static bool isLetter(char symb);
		static bool isDigit(char symb);
		static bool isServiceSymbol(char symb);

		virtual ~Oper();
		virtual void updateSubOpers() = 0;

	public:
		virtual double getResult(double x) = 0;
		virtual double getResult() { return 1; }
		virtual void setExpression(std::string str = "") = 0;
		
		std::string getExpression() const { return this->value; } //returns pasred string
		std::string getOriginalExpression() const { return this->origin_string; } //returns inputed string
		void setXValue(double x) { x_value = x; } //set current default x value
		double getXValue() const { return x_value; } //returns current default x value
		void setConstants(ConstantsMap *consts = nullptr, bool addConstants = true); //set Constants with ConstantsMap
		ConstantsMap getConstants() const; //Returns ConstantsMap
		void addConstant(char symb, double value); //Adds a current constant
		void deleteConstant(char name, bool isThrow = false); //Delete a current constants
		void resetConstants(bool addDefault = true); //Reset all constants (if true - it adds default constants)

		void setFunctions(FunctionsMap *funcs = nullptr, bool addFunctions = true); //Set functions with functions Map
		FunctionsMap getFunctions() const; //Returns FunctionsMap
		static void checkFunctionNameCorrectness(std::string func_name); //Check if function name is not correct
		void addFunction(std::string name, Function function); //Adds a current function (with function pointer)
		void addFunction(std::string name, Expression &exp); //Add function with Expression instance
		void addFunction(std::string name, std::string function_str); //Add function with string of expression
		void deleteFunction(std::string name, bool isThrow = false); //It throws exception if isThrow = true and there is no target function
		void resetFunctions(bool addDefault = true); //Reset functions (default or not)
		void setParserSettings(ParserSettings& se); 
	    void setNewXAlias(char symb); //Set alias of variable (as default - x)
		char getXAlias() const; //Returns current variable alias
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
		friend FunctionWrapper;

		explicit Expression(std::string value = "", bool toBePrepared = true, std::shared_ptr<ParserSettings> ps = nullptr);
		Expression(Expression& exp);

		double getResult(double x) override;
		double getResult() override { return getResult(x_value); }
		void setExpression(std::string str = "") override;

		void operator=(std::string exp);

		std::string operator+(const Oper& obj) const;
		std::string operator-(const Oper& obj) const;
		std::string operator*(const Oper& obj) const;
		std::string operator/(const Oper& obj) const;
		std::string operator^(const Oper& obj) const;
	};

	struct FunctionWrapper
	{
	private:
		Function function_ptr;
		std::shared_ptr<Expression> exp_ptr;

	public:

		FunctionWrapper(Function func) : function_ptr(func), exp_ptr(nullptr) {}

		FunctionWrapper() {}

		FunctionWrapper(std::shared_ptr<Expression> ptr) : exp_ptr(ptr) {}

		std::string getFunctionNotation();

		FunctionWrapper(const FunctionWrapper& copy);

		double getCalculatedValue(double argument);

		bool isExpressionObject() { return exp_ptr != nullptr; }

		std::shared_ptr<ParserSettings> getParserSettingsFromExpressionObject();

		Expression* getExpPtr() { return exp_ptr.get(); }
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




	//Exception-classes----------------------------
	enum  ExceptionType //Views of exceptions
	{
		E_UnexpectedError,
		E_IncorrectSyntax,
		E_InvalidBracketsCount,
		E_MathFunctionCrash,
		E_ConversionError,
		E_IncorrectArgument,
		E_MathError,
		E_NameError,
	};

	class InvalidExpression : public std::exception //Main exception class
	{
	protected:
		std::string m_error;
		ExceptionType type;

	public:
		InvalidExpression(std::string error, ExceptionType tp = E_UnexpectedError) : m_error(error), type(tp) {}

		ExceptionType getExceptionType() { return type; }

		const char* what() const noexcept { return m_error.c_str(); }
	};

	class IncorrectSyntax : public InvalidExpression
	{
	public:
		IncorrectSyntax(std::string error) : InvalidExpression(error, E_IncorrectSyntax) {}
	};

	class MathError : public InvalidExpression
	{
	public:
		MathError(std::string error) : InvalidExpression(error, E_MathError) {}
	};

	class InvalidBracketsCount : public InvalidExpression
	{
	public:
		InvalidBracketsCount(std::string error) : InvalidExpression(error, E_InvalidBracketsCount) {}
	};

	class ConversionError : public InvalidExpression
	{
	private:
		std::string invalid_value;
	public:
		ConversionError(std::string error, std::string invalid_value) : InvalidExpression(error, E_ConversionError), invalid_value(invalid_value) {}

		std::string getInvalidValue() { return invalid_value; }
	};
	
	class MathFunctionCrash : public InvalidExpression
	{
	private:
		std::string target_function;
	public:
		MathFunctionCrash(std::string error, std::string targ_func) : InvalidExpression(error, E_MathFunctionCrash), target_function(targ_func) {}

		std::string getTargetFunction() { return target_function; }
	};

	class RecursionException : public MathFunctionCrash
	{
	private:
		Oper* exp1;
		Expression* exp2;
	public:
		RecursionException(std::string error, std::string targ_func, Oper* exp1, Expression* exp2) : MathFunctionCrash(error, targ_func), exp1(exp1), exp2(exp2) {}
		Oper* getExp1() { return exp1; }
		Expression* getExp2() { return exp2; }
	};

	class IncorrectArgument : public MathFunctionCrash
	{
	private:
		std::string invalid_argument;
	public:
		IncorrectArgument(std::string error, std::string targ_func, std::string arg) : MathFunctionCrash(error, targ_func), invalid_argument(arg) {}

		std::string getIncorrectArgument() { return invalid_argument; }
	};
	
	class NameErrorException : public InvalidExpression
	{
	private:
		std::string invalid_character;
	public:
		NameErrorException(std::string error, std::string letter) : InvalidExpression(error, E_NameError), invalid_character(letter) {}

		std::string getInvalidCharacter() { return invalid_character; }
	};

	class IncorrectFunctionName : public NameErrorException
	{
	public:
		IncorrectFunctionName(std::string error, std::string letter) : NameErrorException(error, letter) {}
	};

	class IncorrectConstantName : public NameErrorException
	{
	public:
		IncorrectConstantName(std::string error, std::string letter) : NameErrorException(error, letter) {}
	};

	class IncorrectXAliasName : public NameErrorException
	{
	public:
		IncorrectXAliasName(std::string error, std::string letter) : NameErrorException(error, letter) {}
	};

	class RangeException : public std::exception
	{
	private:
		std::string m_error;

	public:
		RangeException(std::string error) : m_error(error) {}

		const char* what() const noexcept { return m_error.c_str(); }
	};

	//--------------------------------------


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