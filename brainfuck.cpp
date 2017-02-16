/* Brainfuck interpreter using interpreter and composite patterns :) */

#include <exception>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <memory>

using namespace std;

struct Data
{
    vector<int> array;
    int ptr;
};

class AbstractExpression
{
    public:
        virtual void add(shared_ptr<AbstractExpression>) {}
        virtual void interpret(Data &) = 0;
};

typedef shared_ptr<AbstractExpression> AbstractExpressionPtr;

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

class CompositeExpression: public AbstractExpression
{
    protected:
        list<AbstractExpressionPtr> expTree;
    public:
        virtual void add(AbstractExpressionPtr exp) {expTree.push_back(exp);}

        virtual void interpret(Data &data) {
            for(list<AbstractExpressionPtr>::iterator it=expTree.begin(); it!=expTree.end(); it++)
                (*it)->interpret(data);
        }
};

class Loop: public CompositeExpression
{
    public:
        virtual void interpret(Data &data) {
            while(data.array[data.ptr]) {
                for(list<AbstractExpressionPtr>::iterator it=expTree.begin(); it!=expTree.end(); it++)
                    (*it)->interpret(data);
            }
        }
};

class AbstractFactory
{
    private:
        char last;
    public:
        AbstractFactory(char c): last(c) {}
        virtual AbstractExpressionPtr makeExpression() = 0;
        char getClosure() {return last;}
};

typedef shared_ptr<AbstractFactory> AbstractFactoryPtr;

class LoopFactory: public AbstractFactory
{
    public:
        LoopFactory(char c): AbstractFactory(c) {}
        virtual AbstractExpressionPtr makeExpression() {
            return AbstractExpressionPtr(new Loop);
        }
};

class Parser
{
    private:
        map<char, AbstractExpressionPtr> terminalMap;
        map<char, AbstractFactoryPtr> nonTerminalMap;
    public:
        Parser() {
            terminalMap['+'] = AbstractExpressionPtr(new IncrementByte);
            terminalMap['-'] = AbstractExpressionPtr(new DecrementByte);
            terminalMap['>'] = AbstractExpressionPtr(new IncrementPtr);
            terminalMap['<'] = AbstractExpressionPtr(new DecrementPtr);
            terminalMap['.'] = AbstractExpressionPtr(new Output);
            terminalMap[','] = AbstractExpressionPtr(new Input);
            nonTerminalMap['['] = AbstractFactoryPtr(new LoopFactory(']'));
        }
        
        AbstractExpressionPtr buildTree(const string & code) {
            AbstractExpressionPtr syntaxTreePtr(new CompositeExpression);

            list<AbstractExpressionPtr> listNode(1, syntaxTreePtr);
            list<char> closure;
            
            for(int i=0; i<code.size(); i++) {
                if(terminalMap.find(code[i]) != terminalMap.end()) {
                    listNode.back()->add(terminalMap[code[i]]);
                }
                else if(nonTerminalMap.find(code[i]) != nonTerminalMap.end()) {
                    AbstractExpressionPtr current = nonTerminalMap[code[i]]->makeExpression();
                    listNode.back()->add(current);
                    listNode.push_back(current);
                    closure.push_back(nonTerminalMap[code[i]]->getClosure());
                }
                else if(code[i] == closure.back()) {
                    listNode.pop_back();
                    closure.pop_back();
                }
            }
            
            return syntaxTreePtr;
        }
};

int main()
{
    Data data; data.array.assign(1,0); data.ptr = 0;
    string code("++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
    //string code(",[.[-],]");
    Parser parser;
    AbstractExpressionPtr syntaxTreePtr = parser.buildTree(code);
    syntaxTreePtr->interpret(data);
}
