/* Brainfuck interpreter using interpreter and composite patterns :) */

#include <exception>
#include <iostream>
#include <list>
#include <deque>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

struct Data
{
    deque<int> array;
    int ptr;
};

class AbstractExpression
{
    public:
        virtual void interpret(Data &data) = 0;
        virtual void add(AbstractExpression * exp) {}
        virtual bool isComposite() {return false;}
        virtual ~AbstractExpression() {}
};

class IncrementByte: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            data.array[data.ptr]++;
        }
};

class DecrementByte: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            data.array[data.ptr]--;
        }
};

class IncrementPtr: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            data.ptr++;
            if(data.array.size()==data.ptr) data.array.push_back(0);
        }
};

class DecrementPtr: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            data.ptr--;
            if(data.ptr<0) throw out_of_range("Negative value of pointer");
        }
};

class Output: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            cout<<char(data.array[data.ptr]);
        }
};

class Input: public AbstractExpression
{
    public:
        virtual void interpret(Data &data) {
            char input;
            cin>>input;
            data.array[data.ptr] = static_cast<char>(input);
        }
};

class CompositeExpression: AbstractExpression
{
    private:
        CompositeExpression() {}
    protected:
        map<char,AbstractExpression*> expMap;
        list<AbstractExpression*> expTree;
    public:
        CompositeExpression(string code): expTree() {
            expMap['+'] = new IncrementByte;
            expMap['-'] = new DecrementByte;
            expMap['>'] = new IncrementPtr;
            expMap['<'] = new DecrementPtr;
            expMap['.'] = new Output;
            expMap[','] = new Input;
            
            char chars[6] = {'+','-','>','<','.',','};
            int skip(0);
            
            for(int i=0; i<code.size(); i++) {
                if(skip) {
                    if(code[i] == '[') skip++;
                    if(code[i] == ']') skip--;
                    continue;
                }
                if(find(chars, chars+6, code[i]) != chars+6) {
                    this->add(expMap[code[i]]);
                }
                else if (code[i] == '[') {
                    this->add(new CompositeExpression(code.substr(i+1)));
                    skip = 1;
                }
                else if(code[i]==']') break;
            }
        }
        
        virtual ~CompositeExpression() {
            for(list<AbstractExpression*> ::iterator it=expTree.begin(); it!=expTree.end(); it++) {
                if((*it)->isComposite()) delete (*it);
            }
            for(map<char,AbstractExpression*>::iterator it=expMap.begin(); it!=expMap.end(); it++)
                delete (*it).second;
        }
        
        virtual bool isComposite() {return true;}
        
        virtual void add(AbstractExpression * exp) {expTree.push_back(exp);}
        
        virtual void interpret(Data &data)
        {
            for(list<AbstractExpression*>::iterator it=expTree.begin(); it!=expTree.end(); it++) {
                if((*it)->isComposite()) {
                    while(data.array[data.ptr])
                        (*it)->interpret(data);
                }
                else {
                    (*it)->interpret(data);
                }
            }
        }
};

int main()
{
    Data data; data.array.assign(1,0); data.ptr = 0;
    string code("++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
    //string code(",[.[-],]");
    CompositeExpression parser(code);
    parser.interpret(data);
}
