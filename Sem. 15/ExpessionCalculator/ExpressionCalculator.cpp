#include "ExpressionCalculator.h"

void ExpressionCalculator::copyFrom(const ExpressionCalculator& other)
{
	expr = other.expr->clone();
}

void ExpressionCalculator::free()
{
	delete expr;
}

bool isOperator(char ch)
{
	return ch == AND || ch == OR || ch == IFF || ch == IMPL || ch == XOR || ch == NEG;
}

ExpressionCalculator::BooleanExpr* parse(const MyString& str)
{

	if (str.getSize() == 1)
	{
		return new ExpressionCalculator::Variable(str[0]);
	}

	MyString strWithoutBrackets = str.subString(1, str.getSize() - 2); //We remove the brackets.

	
	int countBrackets = 0;
	for (size_t i = 0; i < strWithoutBrackets.getSize(); i++) //we start searching for the operation.
	{														 // then we split to two expressions
		if (strWithoutBrackets[i] == '(')
			countBrackets++;
		else if (strWithoutBrackets[i] == ')')
			countBrackets--;
		else if (isOperator(strWithoutBrackets[i]) && countBrackets == 0)
		{
			if (strWithoutBrackets[i] == NEG)
				return new ExpressionCalculator::UnaryOperation('!', parse(strWithoutBrackets.subString(i + 1, strWithoutBrackets.getSize() - 1)));
			else
				return new ExpressionCalculator::BinaryOperation(parse(strWithoutBrackets.subString(0, i - 1)), strWithoutBrackets[i], parse(strWithoutBrackets.subString(i+1, strWithoutBrackets.getSize() - 1)));
		}

	}
}

ExpressionCalculator::ExpressionCalculator(const MyString& str)
{
	expr = parse(str);

}
ExpressionCalculator::ExpressionCalculator(const ExpressionCalculator& other)
{
	copyFrom(other);
}
ExpressionCalculator& ExpressionCalculator::operator=(const ExpressionCalculator& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
ExpressionCalculator::~ExpressionCalculator()
{
	free();
}

bool ExpressionCalculator::isTautology() const
{
	return checkAll(expr, true);
}
bool ExpressionCalculator::isContradiction() const
{
	return checkAll(expr, false);
}