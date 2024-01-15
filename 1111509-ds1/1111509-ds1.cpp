#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include<iomanip>
using namespace std;

class Token
{
public:
	string type; // number, operator, left parenthesis, right parenthesis
	string value;
	Token(string type, string value)
	{
		this->type = type;
		this->value = value;
	}
};

class vecToken
{
public:
	string type;
	vector<double> number;
	string oper;

	vecToken(string type, vector<double>& number)
	{
		this->type = type;
		this->number = number;
	}
	vecToken(string type, string oper)
	{
		this->type = type;
		this->oper = oper;
	}

	vecToken operator+(vecToken& other)
	{
		vector<double> result;
		int size = max(this->number.size(), other.number.size());
		int thisSize = this->number.size();
		int otherSize = other.number.size();
		for (int i = 0; i < size; i++)
		{
			result.push_back(this->number[i % thisSize] + other.number[i % otherSize]);
		}
		return vecToken("number", result);
	}

	vecToken operator-(vecToken& other)
	{
		vector<double> result;
		int size = max(this->number.size(), other.number.size());
		int thisSize = this->number.size();
		int otherSize = other.number.size();
		for (int i = 0; i < size; i++)
		{
			result.push_back(this->number[i % thisSize] - other.number[i % otherSize]);
		}
		return vecToken("number", result);
	}

	vecToken operator*(vecToken& other)
	{
		vector<double> result;
		int size = max(this->number.size(), other.number.size());
		int thisSize = this->number.size();
		int otherSize = other.number.size();
		for (int i = 0; i < size; i++)
		{
			result.push_back(this->number[i % thisSize] * other.number[i % otherSize]);
		}
		return vecToken("number", result);
	}

	vecToken operator/(vecToken& other)
	{
		vector<double> result;
		int size = max(this->number.size(), other.number.size());
		int thisSize = this->number.size();
		int otherSize = other.number.size();
		for (int i = 0; i < size; i++)
		{
			result.push_back(this->number[i % thisSize] / other.number[i % otherSize]);
		}
		return vecToken("number", result);
	}


};

int priority(const string ope) {
	if (ope == "+" || ope == "-")
		return 1;

	if (ope == "*" || ope == "/")
		return 2;

	if (ope == "^")
		return 3;
	
	return 0;
}

vector<Token> tokenlize(string expression)
{
	// Tokenize the expression
	vector<Token> tokens;
	bool isBinary = false;
	int index = 0;
	while (index < expression.length())
	{
		if (expression[index] == '(')
			tokens.push_back(Token("left parenthesis", "(")), isBinary = false;
		else if (expression[index] == ')')
			tokens.push_back(Token("right parenthesis", ")")), isBinary = true;
		else if (isdigit(expression[index]))
		{
			string number;
			while (isdigit(expression[index]) || expression[index] == '.')
			{
				number += expression[index];
				index++;
			}
			tokens.push_back(Token("number", number));
			index--;
			isBinary = true;
		}
		else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' ||
			expression[index] == '/' || expression[index] == '^')
		{
			if (expression[index] == '-' && isBinary == false)
				tokens.push_back(Token("negative", "-u")), isBinary = false;
			else
				tokens.push_back(Token("operator", string(1, expression[index]))), isBinary = false;

		}
		index++;
	}

	/*for (Token t : tokens) {
		cout << t.type << " " << t.value << endl;
	}*/

	return tokens;

}


vector<Token> infixToPostfix(vector<Token> tokens)
{
	vector<Token> postfix;
	vector<Token> stack;
	for (Token t : tokens)
	{
		if (t.type == "number")
			postfix.push_back(t);
		else if (t.type == "left parenthesis")
			stack.push_back(t);
		else if (t.type == "right parenthesis")
		{
			while (!stack.empty() && stack.back().type != "left parenthesis")
			{
				postfix.push_back(stack.back());
				stack.pop_back();
			}

			if (!stack.empty() && stack.back().type == "left parenthesis")
				stack.pop_back();

		}
		else if (t.type == "negative")
			stack.push_back(t);
		else if (t.type == "operator")
		{
			while (!stack.empty() && priority(stack.back().value) >= priority(t.value))
			{
				postfix.push_back(stack.back());
				stack.pop_back();
			}

			stack.push_back(t);

		}

	}
	while (!stack.empty())
	{
		postfix.push_back(stack.back());
		stack.pop_back();
	}

	/*for (Token t : postfix)
		cout << t.type << " " << t.value << endl;*/

	return postfix;
}

double calculatePostfix(vector<Token> postfix)
{
	vector<double> stack;

	for (Token t : postfix)
	{
		if (t.type == "number")
			stack.push_back(stod(t.value));
		else if (t.type == "negative")
		{
			double num = stack.back();
			stack.pop_back();
			stack.push_back(-num);
		}
		else if (t.type == "operator")
		{
			double num2 = stack.back();
			stack.pop_back();
			double num1 = stack.back();
			stack.pop_back();

			if (t.value == "+")
				stack.push_back(num1 + num2);
			else if (t.value == "-")
				stack.push_back(num1 - num2);
			else if (t.value == "*")
				stack.push_back(num1 * num2);
			else if (t.value == "/")
				stack.push_back(num1 / num2);
			else if (t.value == "^")
				stack.push_back(pow(num1, num2));
		}
	}
	//cout << "ans" << stack.back() << endl;

	return stack.back();
}

vector<vecToken> vecinfixToPostfix(vector<vecToken> vectoken)
{
	vector<vecToken> postfix;
	vector<vecToken> stack;
	for (vecToken t : vectoken)
	{
		if (t.type == "number")
			postfix.push_back(t);
		else if (t.type == "operator")
		{
			while (!stack.empty() && priority(stack.back().oper) >= priority(t.oper))
			{
				postfix.push_back(stack.back());
				stack.pop_back();
			}

			stack.push_back(t);

		}
	}

	while (!stack.empty()) {
		postfix.push_back(stack.back());
		stack.pop_back();
	}
	
	return postfix;
}

vector<double> veccalculatePostfix(vector<vecToken> postfix)
{
	vector<vecToken> stack;

	for (vecToken t : postfix)
	{
		if (t.type == "number")
			stack.push_back(t);
		else if (t.type == "operator")
		{
			vecToken num2 = stack.back();
			stack.pop_back();
			vecToken num1 = stack.back();
			stack.pop_back();

			if (t.oper == "+")
				stack.push_back(num1 + num2);
			else if (t.oper == "-")
				stack.push_back(num1 - num2);
			else if (t.oper == "*")
				stack.push_back(num1 * num2);
			else if (t.oper == "/")
				stack.push_back(num1 / num2);
		}
	}

	if (stack.size() != 1) {
		cout << "error" << endl;
		exit(0);
	}
	return stack.back().number;
}

double eval(string expression)
{

	vector<Token> Token = tokenlize(expression);
	Token = infixToPostfix(Token);
	return calculatePostfix(Token);

}

// 9*8,(3+1)^3,9/2
vector<double> calVector(string vec)
{
	// split by ','
	vector<double> answerVector;
	stringstream ss(vec);
	string element;
	while (getline(ss, element, ',')) {
		answerVector.push_back(eval(element));
	}
	return answerVector;
}

// example: [9*8,(3+1)^3,9/2]+[2+3,5-3]
vector<double> calculate(string input)
{
	vector<vecToken>extract;
	string vec;
	int index = 0;
	while (index < input.length()) {
		if (input[index] == '[') {
			vec = "";
			index++;
			while (input[index] != ']') {
				vec += input[index];
				index++;
			}
			vector<double> number = calVector(vec);
			extract.push_back(vecToken("number", number));
		}
		else if (input[index] == '+' || input[index] == '-' || input[index] == '*' || input[index] == '/') {
			extract.push_back(vecToken("operator", string(1, input[index])));
		}
		index++;
	}

	return veccalculatePostfix(vecinfixToPostfix(extract));

}

int main(int argc, char* argv[])
{

	ifstream inFile(argv[1]);
	string input;
	cout << setprecision(10);

	while (getline(inFile, input)) {
		string i_input;
		for (int i = 0; i < input.size(); i++) {
			if (input[i] != ' ' && input[i] != '\n' && input[i] != '\t')
				i_input += input[i];
		}

		vector<double> answerVector = calculate(i_input);

		cout << "[";
		for (int i = 0; i < answerVector.size(); i++) {
			cout << round(answerVector[i] * 100.0 + 0.00000001) / 100.0;
			// cout << answerVector[i];
			if (i != answerVector.size() - 1)
				cout << ",";
		}
		cout << "]" << endl;
	}

	inFile.close();
	return 0;
}