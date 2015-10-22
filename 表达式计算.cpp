#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Calculate{
private:
	string str;

	//���ַ���ת��Ϊdouble����  "123.321"->123.321
	double strToDouble(string str);
	//���� a (c) b   1 + 2
	double calculate(double a, double b, char c);
	//���ò�ͬ���ŵ����ȼ�
	int priority(char ch);
	//�ж�һ�������Ƿ����ջ�л��� ���м���
	void SymbolPush(char symbol);

	stack<double> numsStack;
	stack<char> symbolStack;
public:
	Calculate(string str);

	double getResult();
};


int main(){
	try{
		// 10��12.5��(1+1)��(25��5)+(11.22��2) = 55.61
		Calculate test("10*12.5*(1+1)/(25/5)+(11.22/2)#");
		cout << test.getResult() << endl;

		//��������Ϊ0�� TODO//�����ù��캯��
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
//���ַ���ת��Ϊdouble����  "123.321"->123.321
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
//���� a (c) b   1 + 2
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
//���ò�ͬ���ŵ����ȼ�
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
//�ж�һ�������Ƿ����ջ�л��� ���м���
void Calculate::SymbolPush(char symbol){

	//���ջ��Ϊ'(' ���ж�symbol�Ƿ�Ϊ')'�������Ƴ�'('  ����ѷ�������ջ��
	if (!symbolStack.empty() && symbolStack.top() == '('){
		if (symbol == ')')
			symbolStack.pop();
		else
			symbolStack.push(symbol);
		return;
	}

	//�������ջ��������Ԫ�����ϣ��ҵ�ǰ�Ĳ��������ȼ�С��ջ���ķ��ŵ����ȼ������Ƚ��Լ��㣬�����Ľ������ջ�� �ݹ鵱ǰ������������ѵ�ǰ����������ջ��
	if (numsStack.size() >= 2 &&
		priority(symbolStack.top()) >= priority(symbol)){
		double	a = numsStack.top();		numsStack.pop();
		double	b = numsStack.top();		numsStack.pop();
		char	ch = symbolStack.top();	symbolStack.pop();

		double temp = calculate(b, a, ch);
		numsStack.push(temp);
		//�ݹ鵱ǰ������
		SymbolPush(symbol);
	}
	else{
		symbolStack.push(symbol);
	}
}