/* Brainfuck interpreter using interpreter and composite patterns :) */

#include <exception>
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
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
        AbstractExpression() {}
        virtual ~AbstractExpression() {}
        virtual void add(shared_ptr<AbstractExpression>) {}
        virtual bool isComposite() {return false;}
        virtual void interpret(Data &) = 0;
        virtual void parse(const string &) {}
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

typedef shared_ptr<AbstractExpression> AbstractExpressionPtr;

class CompositeExpression: public AbstractExpression
{
    private:
        list<AbstractExpressionPtr> expTree;
    public:
        CompositeExpression(): expTree() {}

        virtual ~CompositeExpression() {}

        virtual bool isComposite() {return true;}

        virtual void add(AbstractExpressionPtr exp) {expTree.push_back(exp);}

        virtual void interpret(Data &data) {
            for(list<AbstractExpressionPtr>::iterator it=expTree.begin(); it!=expTree.end(); it++) {
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

class Parser
{
    private:
        map<char, AbstractExpressionPtr> expMap;
        list<char> chars;
    public:
        Parser() {
            chars.push_back('+'); expMap[chars.back()] = AbstractExpressionPtr(new IncrementByte);
            chars.push_back('-'); expMap[chars.back()] = AbstractExpressionPtr(new DecrementByte);
            chars.push_back('>'); expMap[chars.back()] = AbstractExpressionPtr(new IncrementPtr);
            chars.push_back('<'); expMap[chars.back()] = AbstractExpressionPtr(new DecrementPtr);
            chars.push_back('.'); expMap[chars.back()] = AbstractExpressionPtr(new Output);
            chars.push_back(','); expMap[chars.back()] = AbstractExpressionPtr(new Input);
        }

        AbstractExpressionPtr buildTree(const string & code) {
            AbstractExpressionPtr syntaxTreePtr(new CompositeExpression);
            int skip(0);
            for(int i=0; i<code.size(); i++) {
                if(skip) {
                    if(code[i] == '[') skip++;
                    if(code[i] == ']') skip--;
                    continue;
                }
                if(find(chars.begin(), chars.end(), code[i]) != chars.end()) {
                    syntaxTreePtr->add(expMap[code[i]]);
                }
                else if (code[i] == '[') {
                    AbstractExpressionPtr exp = buildTree(code.substr(i+1));
                    syntaxTreePtr->add(exp);
                    skip = 1;
                }
                else if(code[i]==']') break;
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
