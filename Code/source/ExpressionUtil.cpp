#include "ExpressionUtil.h"

std::regex terms("(\\w+)");
std::regex validExpression("^(?:[\\(\\s]*(\\w+)[\\s\\)]*)(?:\\s*[+\\*\\-%\\/]\\s*(?:[\\(\\s]*(\\w+)[\\s\\)]*))*$");

/* Converts an Infix Expression into a Postfix Expression.
 * Preconditon: The Infix Expression must be a valid infixExpression. Perform a check by calling
 *			    verifyInfixExpression prior to calling this function.
 * Output: The Postfix Expression.
 */
std::string ExpressionUtil::convertInfixToPrefix(std::string expression) {
	expressionPreprocess(expression);

	std::stack<std::string> operatorStack;
	std::stack<std::string> operandStack;

	//Split by spaces to create substrings.
	char spaceDelimiter = ' ';
	std::vector<std::string> expressionVector = StringUtil::split(expression, spaceDelimiter);

	for (size_t i = 0; i < expressionVector.size(); i++) {
		//Push opening bracket to operator stack
		if (expressionVector[i] == "(") {
			operatorStack.push(expressionVector[i]);
		}

		//If Closing bracket, pop until we get back opening bracket.
		else if (expressionVector[i] == ")") {

			while (!operatorStack.empty() && operatorStack.top() != "(") {

				std::string operand1 = operandStack.top();
				operandStack.pop();

				std::string operand2 = operandStack.top();
				operandStack.pop();

				std::string operator1 = operatorStack.top();
				operatorStack.pop();

				//form sub-expression
				std::string subexpre = operator1 + " " + operand2 + " " + operand1;
				operandStack.push(subexpre);
			}

			//Opening bracket popped.
			operatorStack.pop();
		}

		//If operand push operand Stack
		else if (!isOperator(expressionVector[i].c_str()[0])) {
			operandStack.push(expressionVector[i]);
		}

		//If operator push to operator stack only after performing higher priority operator.
		else {
			while (!operatorStack.empty() && getPriority(expressionVector[i]) <= getPriority(operatorStack.top())) {
				std::string operand1 = operandStack.top();
				operandStack.pop();

				std::string operand2 = operandStack.top();
				operandStack.pop();

				std::string operator1 = operatorStack.top();
				operatorStack.pop();

				//form sub-expression
				std::string subexpre = operator1 + " " + operand2 + " " + operand1;
				operandStack.push(subexpre);
			}
			operatorStack.push(expressionVector[i]);
		}
	}

	while (!operatorStack.empty()) {
		std::string operand1 = operandStack.top();
		operandStack.pop();

		std::string operand2 = operandStack.top();
		operandStack.pop();

		std::string operator1 = operatorStack.top();
		operatorStack.pop();

		//form sub-expression

		std::string subexpre = operator1 + " " + operand2 + " " + operand1;
		operandStack.push(subexpre);
	}
			
	return " " + operandStack.top() + " ";
}

/* This function performs white space cleaning of an infix expression.
 * Precondition: Any infix expression
 * Output: The infix expression will now have no white space except a single space inbetween
 *         every operator.
 */
void ExpressionUtil::expressionPreprocess(std::string &expression)
{
	//Remove every single whitespace
	expression.erase(remove_if(expression.begin(), expression.end(), isspace), expression.end());

	std::vector<std::string> operators{ "+", "-", "*", "/", "%" , "(", ")" };

	//Add a space at every occurence of an operator.
	for (std::string op : operators) {
		size_t pos = 0;
		while (true) {
			pos = expression.find(op, pos);

			//Finishing condition
			if (pos == std::string::npos) {
				break;
			}

			//Don't add if at the start.
			if (pos != 0 && expression[pos] != '(') {
				expression.insert(pos, " ");
				pos++;
			}

			//Account for the actual operator
			pos++;

			//Don't add if at the end.
			if (pos >= expression.size()) {
				break;
			}

			if (expression[pos-1] != ')') {
				expression.insert(pos, " ");
				pos++;
			}
		}
	}
}

/* Returns the variables that can be found in the given infix Expression.
 * Preconditon: The Infix Expression must be a valid infixExpression. Perform a check by calling
 *			    verifyInfixExpression prior to calling this function.
 * Output: The list of Variables found in the expression.
 */
std::vector<std::string> ExpressionUtil::getVariables(std::string infixExpression) {
	std::smatch result;
	std::vector<std::string> variableVector;

	while (std::regex_search(infixExpression, result, terms)) {
		bool isValidName = LexicalToken::verifyName(result[0]);
		if (isValidName) {
			variableVector.push_back(result[0]);
		}
		infixExpression = result.suffix().str();
	}

	return variableVector;
}

/* Returns the constants that can be found in the given infix Expression.
 * Preconditon: The Infix Expression must be a valid infixExpression. Perform a check by calling
 *			    verifyInfixExpression prior to calling this function.
 * Output: The list of Constants found in the expression.
 */
std::vector<std::string> ExpressionUtil::getConstants(std::string infixExpression) {
	std::smatch result;
	std::vector<std::string> constantVector;

	while (regex_search(infixExpression, result, terms)) {
		bool isValidInteger = LexicalToken::verifyInteger(result[0]);
		if (isValidInteger) {
			constantVector.push_back(result[0]);
		}
		infixExpression = result.suffix().str();
	}

	return constantVector;
}

/* Takes in the expression as an argument and returns true if valid.
 * it also checks that all constants and variables inside are valid.
 * Perform this step before converting to InfixExpression or
 * extracting variables and constants.
 */
bool ExpressionUtil::verifyInfixExpression(std::string infixExpression) {
	if (regex_match(infixExpression, validExpression)) {
		if (checkParenthesis(infixExpression)) {
			std::smatch result;

			while (std::regex_search(infixExpression, result, terms)) {
				bool isValidName = LexicalToken::verifyName(result[0]);
				bool isValidInteger = LexicalToken::verifyInteger(result[0]);
				if (!isValidName && !isValidInteger) {
					return false;
				}
				infixExpression = result.suffix().str();
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

/* Checks if there is matching parenthesis. Returns true as long as for every
 * Left bracket, there is a matching right bracket further down the string.
 */
bool ExpressionUtil::checkParenthesis(std::string infixExpression) {
	std::stack<char> bracketStack;

	for (size_t i = 0; i < infixExpression.length(); i++) {
		if (infixExpression.at(i) == '(') {
			bracketStack.push('(');
		}
		else if (infixExpression.at(i) == ')') {
			if (bracketStack.empty()) {
				return false;
			}
			else {
				bracketStack.pop();
			}
		}
	}

	if (bracketStack.empty()) {
		return true;
	}
	else {
		return false;
	}
}

// Get priority of the operator. If is not a operator return 0.
int ExpressionUtil::getPriority(std::string p)
{
	char c = p.c_str()[0];
	if (c == '-' || c == '+')
		return 1;
	else if (c == '*' || c == '/' || c == '%')
		return 2;
	return 0;
}

/* Checks if the character is an operator. Returns true if it is not an alphabet or digit.
 * Otherwise returns false;
 */
bool ExpressionUtil::isOperator(char c)
{
	return (!isalpha(c) && !isdigit(c));
}
