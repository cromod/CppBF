#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

#include "expression.hpp"

class AbstractFactory
{
    private:
        char closure;
    public:
        AbstractFactory(char c);
        
        virtual AbstractExpressionPtr makeExpression() = 0;
        
        char getClosure();
};

typedef std::shared_ptr<AbstractFactory> AbstractFactoryPtr;

class LoopFactory: public AbstractFactory
{
    public:
        LoopFactory(char c);
        
        virtual AbstractExpressionPtr makeExpression();
};

#endif //FACTORY_HPP