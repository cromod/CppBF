#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

#include "expression.hpp"

class AbstractFactory
{
    private:
        char closure;
    public:
        AbstractFactory(char c): closure(c) {}
        
        virtual AbstractExpressionPtr makeExpression() = 0;
        
        char getClosure() {return closure;}
};

typedef std::shared_ptr<AbstractFactory> AbstractFactoryPtr;

template <class Expression>
class Factory: public AbstractFactory
{
    public:
        Factory(char c): AbstractFactory(c) {}
        
        virtual AbstractExpressionPtr makeExpression() {
            return AbstractExpressionPtr(new Expression);
        }
};

#endif //FACTORY_HPP