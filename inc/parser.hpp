#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <string>

#include "factory.hpp"

class Parser: public AbstractFactory
{
    private:
        std::map<char, AbstractExpressionPtr> terminalMap;
        std::map<char, AbstractFactoryPtr> nonTerminalMap;
        std::string code;
    public:
        Parser(const std::string & c);
        
        virtual AbstractExpressionPtr makeExpression();
};

#endif //PARSER_HPP