#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Calculate{
private:
	string str;

	//将字符串转换为double类型  "123.321"->123.321
	double strToDouble(string str);
	//返回 a (c) b   1 + 2
	double calculate(double a, double b, char c);
	//设置不同符号的优先级
	int priority(char ch);
	//判断一个符号是否放入栈中还是 进行计算
	void SymbolPush(char symbol);

	stack<double> numsStack;
	stack<char> symbolStack;
public:
	Calculate(string str);

	double getResult();
};


int main(){
	try{
		// 10×12.5×(1+1)÷(25÷5)+(11.22÷2) = 55.61
		Calculate test("10*12.5*(1+1)/(25/5)+(11.22/2)#");
		cout << test.getResult() << endl;

		//除数不能为0； TODO//不该让构造函数
		Calculate test2("10*12.5*(1+1)/0#");

	}
	catch (string *err){
		cout << *err;
	}

}





Calculate::Calculate(string str){
	this->str = str;
	for (int i = 0, j = -1; i < str.length();){
		if (str[i]<'0' || str[i]>'9'){
			SymbolPush(str[i]);
			j = i;
			i++;
		}
		else{
			while (str[i] >= '0' && str[i] <= '9' || str[i] == '.') i++;
			numsStack.push(strToDouble(string(str.begin() + (j + 1), str.begin() + i)));
			j = i;
		}
	}
}
double Calculate::getResult(){
	return numsStack.top();
}
//将字符串转换为double类型  "123.321"->123.321
double Calculate::strToDouble(string str){
	double integer = 0, decimal = 0;
	int i = 0;
	for (; i < str.size() && str[i] != '.'; i++){
		integer *= 10;
		integer += str[i] - '0';
	}
	int j = i++;
	for (; i < str.size(); i++){
		decimal += (double)(str[i] - '0') / pow(10, i - j);
	}
	return integer + decimal;
}
//返回 a (c) b   1 + 2
double Calculate::calculate(double a, double b, char c){
	switch (c){
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '/':{
				 if (b == 0.0)throw new string("divisor can not be zero");
				 return a / b;
	}
	case '*':
		return a*b;

	}
}
//设置不同符号的优先级
int Calculate::priority(char ch){

	switch (ch){
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '(':
		return 3;
	case ')':
		return 0;
	case '#':
		return -1;
	}
}
//判断一个符号是否放入栈中还是 进行计算
void Calculate::SymbolPush(char symbol){

	//如果栈顶为'(' 则判断symbol是否为')'，是则推出'('  否则把符号推入栈中
	if (!symbolStack.empty() && symbolStack.top() == '('){
		if (symbol == ')')
			symbolStack.pop();
		else
			symbolStack.push(symbol);
		return;
	}

	//如果数字栈中有两个元素以上，且当前的操作符优先级小于栈顶的符号的优先级，则先进性计算，计算后的结果推入栈中 递归当前操作符。否则把当前操作符推入栈中
	if (numsStack.size() >= 2 &&
		priority(symbolStack.top()) >= priority(symbol)){
		double	a = numsStack.top();		numsStack.pop();
		double	b = numsStack.top();		numsStack.pop();
		char	ch = symbolStack.top();	symbolStack.pop();

		double temp = calculate(b, a, ch);
		numsStack.push(temp);
		//递归当前操作符
		SymbolPush(symbol);
	}
	else{
		symbolStack.push(symbol);
	}
}