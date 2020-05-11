#pragma once
#include <vector>
#include <string>
#include <map>
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
	void InitializeConstants(std::map<char, double> *consts = nullptr);
	void setNewXAlias(char symb);


	class Oper //Abstract class 
	{
	private:
		void CutWhiteSpases();
		
	protected:
		std::vector<Oper*> sub_opers;
		std::string value;

		Oper(std::string value);
		void prepareString();

		~Oper();

	public:
		virtual double getValue(double x = 0) = 0;
		virtual void setExpression(std::string str) = 0;
		virtual void updateSubOpers() = 0;
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
		Expression(std::string value);

		double getValue(double x = 0) override;
		void setExpression(std::string str) override;
	};



	class Multiplication_Oper : public Oper, public IActionsMap
	{
	private:
		void updateSubOpers() override;
	public:
		Multiplication_Oper(std::string value);

		double getValue(double x = 0) override;
		void setExpression(std::string str) override;
	};



	class Number : public Oper 
	{
	private:
		void updateSubOpers() override {}
	public:
		Number(std::string value);

		double getValue(double x = 0) override;
		void setExpression(std::string str) override;
	};



	class Power_Oper : public Oper
	{
	private:
		void updateSubOpers() override;
	public:
		Power_Oper(std::string value);

		double getValue(double x = 0) override;
		void setExpression(std::string str) override;
	};
}
