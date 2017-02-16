#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <string>

#include "factory.hpp"

class Parser
{
    private:
        std::map<char, AbstractExpressionPtr> terminalMap;
        std::map<char, AbstractFactoryPtr> nonTerminalMap;
    public:
        Parser();
        
        AbstractExpressionPtr buildTree(const std::string & code);
};

#endif //PARSER_HPP