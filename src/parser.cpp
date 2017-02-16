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