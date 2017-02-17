#include "parser.hpp"

using namespace std;

Parser::Parser() {
    terminalMap['+'] = AbstractExpressionPtr(new IncrementByte);
    terminalMap['-'] = AbstractExpressionPtr(new DecrementByte);
    terminalMap['>'] = AbstractExpressionPtr(new IncrementPtr);
    terminalMap['<'] = AbstractExpressionPtr(new DecrementPtr);
    terminalMap['.'] = AbstractExpressionPtr(new Output);
    terminalMap[','] = AbstractExpressionPtr(new Input);
    nonTerminalMap['['] = AbstractFactoryPtr(new LoopFactory(']'));
}

AbstractExpressionPtr Parser::buildTree(const std::string & code) {
    AbstractExpressionPtr syntaxTreePtr(new CompositeExpression);

    list<AbstractExpressionPtr> listNode(1, syntaxTreePtr);
    list<char> closure;
            
    for(char c : code) {
        if(terminalMap.find(c) != terminalMap.end()) {
            listNode.back()->add(terminalMap[c]);
        }
        else if(nonTerminalMap.find(c) != nonTerminalMap.end()) {
            AbstractExpressionPtr current = nonTerminalMap[c]->makeExpression();
            listNode.back()->add(current);
            listNode.push_back(current);
            closure.push_back(nonTerminalMap[c]->getClosure());
        }
        else if(c == closure.back()) {
            listNode.pop_back();
            closure.pop_back();
        }
    }
    
    return syntaxTreePtr;
}