#include <iostream>
#include <fstream>
#include <string>
#include "ArgumentManager.h"
using namespace std;

template <class T>
struct Node{
    T data;
    Node<T>* next;
};

template <class T>
class Stack{
    private:
        Node<T>* top;

    public:
        Stack(){ top = 0; }
        bool isEmpty();
        bool isFull();
        void push(T x);
        T pop();
        bool isBalanced(string exp);
        bool isOperator(T x);
        bool checkOperator(T x);
        bool checkClosingParan(T x);
        bool checkDigit(T x);
        bool checkOpeningParan(T x);
        bool isGarbage(T x);
        bool checkErrors(string exp);
        string removeSpaces(string exp);
        string addZeroes(string exp);
        int pre(T x);
        string InToPost(string exp);
        long long int Eval(string postfix);
};

template <class T>
bool Stack<T>::isEmpty(){
    if(top == 0)
        return 1;
    return 0;
}

template <class T>
bool Stack<T>::isFull(){
    Node<T>* t = new Node<T>;
    if(t == 0)
        return 1;
    return 0;
}

template <class T>
void Stack<T>::push(T x){
    Node<T> *t;
    if(!isFull()){
        t = new Node<T>;
        t->data = x;
        t->next = top;
        top = t;
    }
}

template <class T>
T Stack<T>::pop(){
    T x;
    Node<T>* p;
    if(!isEmpty()){
        p = top;
        top = top->next;
        x = p->data;
        delete p;
    }
    return x;
}

template <class T>
bool Stack<T>::isBalanced(string exp){
    for(int i=0; i<exp.length(); i++){
        if(exp[i] == '(')
            push(exp[i]);
        else if(exp[i] == ')'){
            if(isEmpty())
                return 0; 
            pop();
        }
    }
    if(isEmpty())
        return 1;
    else{
        while(!isEmpty())
            pop();
        return 0;
    }
}

template <class T>
bool Stack<T>::isOperator(T x){
    if(x == '+' || x == '-' || x == '*' || x == '/')
        return 1;
    return 0;
}

template <class T>
bool Stack<T>::checkOperator(T x){
    if(isOperator(x) || x == ')')   //if operator follows operator or a closing paran
        return 1;   //true -> error
    return 0;
}

template <class T>
bool Stack<T>::checkClosingParan(T x){
    if(x == '(' || isdigit(x))
		return 1;
	return 0;
}

template <class T>
bool Stack<T>::checkDigit(T x){
    if(x == '(')
		return 1;
	return 0;
}

template <class T>
bool Stack<T>::checkOpeningParan(T x){
    if(x == '/' || x == '*' || x == ')')
		return 1;
	return 0;
}

template <class T>
bool Stack<T>::isGarbage(T x) {
	if(x != '(' && x != ')' && !isOperator(x) && !isdigit(x))
		return 1;
	return 0;
}

template <class T>
bool Stack<T>::checkErrors(string exp){   //check if expression is valid
    exp = removeSpaces(exp);
    for(int i=0; i<exp.length()-1; i++){
        if(isGarbage(exp[exp.length() - 1]) || isOperator(exp[exp.length() - 1]))
            return 0;  //false -> error; exp is invalid
        if(exp[0] == '*' || exp[0] == '/')
            return 0;
        if(isOperator(exp[i]) && checkOperator(exp[i+1]))
            return 0;
        if(exp[i] == ')' && checkClosingParan(exp[i + 1]))
			return 0;
		if(isdigit(exp[i]) && checkDigit(exp[i + 1]))
			return 0;
		if(exp[i] == '(' && checkOpeningParan(exp[i + 1]))
			return 0;
		if(isGarbage(exp[i]))
			return 0;
    }
    return 1;  //true -> no error; exp is valid
}

template <class T>
string Stack<T>::removeSpaces(string exp){
    while(exp.find(" ") != -1)
        exp.erase(exp.find(" "),1);
    return exp;
}


template <class T>
string Stack<T>::addZeroes(string exp){
    int x;
    if(isOperator(exp[0])){
        exp.insert(0,"0");
        x = 2;
    }
    else
        x = 1;

    for(int i=x; i<exp.length(); i++){
        if(isOperator(exp[i]) && exp[i-1] == '('){
            exp.insert(i,"0");
            i++;
        }
    }
    return exp;
}

template <class T>
int Stack<T>::pre(T x){
    if(x == '+' || x == '-')
        return 1;
    else if(x == '*' || x == '/')
        return 2;
    return 0;
}

template <class T>
string Stack<T>::InToPost(string exp){
    string postfix = "";
    for(int i=0; i<exp.length(); i++){
        if(isdigit(exp[i]))
            postfix += exp[i];
        else if(exp[i] == '('){
            postfix += ' '; 
            push(exp[i]);
        }
        else if(exp[i] == ')'){
            postfix += ' ';
            while(!isEmpty() && top->data != '(')
                postfix = postfix + pop();
            if(top->data == '(')
                pop();
        }
        else{   //must be an operator
            postfix += ' ';
            while(!isEmpty() && pre(exp[i]) <= pre(top->data))
                postfix = postfix + pop();
            push(exp[i]);
        }
    }
    postfix += ' ';
    while(!isEmpty())
        postfix = postfix + pop();
    return postfix;
}

template <class T>
long long int Stack<T>::Eval(string postfix){
    long long int x1, x2, r;
    for (int i=0; i<postfix.length(); i++){
        if(isdigit(postfix[i])){
            string num = "";
            while(i<postfix.length() && isdigit(postfix[i])){
                num += postfix[i];
                i++;
            }
            push(stoll(num));
        }
        else if(isOperator(postfix[i])){
            x2 = pop();
            x1 = pop();

            switch(postfix[i])
            {
                case '+': r = x1 + x2; break;
                case '-': r = x1 - x2; break;
                case '*': r = x1 * x2; break;
                case '/': r = x1 / x2; break;
            }
            push(r);
        }
    }
    return top->data;
}

int main(int argc, char* argv[]) {
	ArgumentManager am(argc,argv);
    ifstream i(am.get("input"));
    ofstream o(am.get("output"));
    string temp;    //stores original expression
    Stack<char> st1;
    Stack<long long int> st2;
    bool isEmpty = 1;  //to check if file is empty
    while(!i.eof()){
        getline(i, temp);
        if(temp.find("\r") != -1)
            temp.erase(temp.find("\r"),2);
        if(temp != ""){
            isEmpty = 0;
            if(st1.isBalanced(temp) && st1.checkErrors(temp))
                o << temp << "=" << st2.Eval(st1.InToPost(st1.addZeroes(st1.removeSpaces((temp))))) << endl;
            else    //expression is invalid
                o << "error" << endl;
        }
    }
    if(isEmpty)    //empty file
        o << "error" << endl;

    i.close();
    o.close();
}