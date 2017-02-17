#include <exception>
#include <iostream>

#include "expression.hpp"

using namespace std;

// AbstractExpression
void AbstractExpression::add(shared_ptr<AbstractExpression>) {}

// IncrementByte
void IncrementByte::interpret(Data &data) const {
    data.array[data.ptr]++;
}

// DecrementByte
void DecrementByte::interpret(Data &data) const {
    data.array[data.ptr]--;
}

// IncrementPtr
void IncrementPtr::interpret(Data &data) const {
    data.ptr++;
    if(data.array.size()==data.ptr) data.array.push_back(0);
}

// DecrementPtr
void DecrementPtr::interpret(Data &data) const {
    data.ptr--;
    if(data.ptr<0) throw out_of_range("Negative value of pointer");
}

// Output
void Output::interpret(Data &data) const {
    cout<<char(data.array[data.ptr]);
}

// Input
void Input::interpret(Data &data) const {
    char input;
    cin>>input;
    data.array[data.ptr] = static_cast<char>(input);
}

// CompositeExpression
void CompositeExpression::add(AbstractExpressionPtr exp) {
    expTree.push_back(exp);
}

void CompositeExpression::interpret(Data &data) const {
    for(const AbstractExpressionPtr & expPtr : expTree)
        expPtr->interpret(data);
}

// Loop
void Loop::interpret(Data &data) const {
    while(data.array[data.ptr]) {
        for(const AbstractExpressionPtr & expPtr : expTree)
            expPtr->interpret(data);
    }
}