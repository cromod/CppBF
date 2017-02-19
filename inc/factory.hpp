#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

#include "expression.hpp"

class AbstractFactory
{
    public:
        virtual AbstractExpressionPtr makeExpression() = 0;
        virtual char getClosure() {}
};

typedef std::shared_ptr<AbstractFactory> AbstractFactoryPtr;

template <class Expression>
class Factory: public AbstractFactory
{
    private:
        char closure;
    public:
        Factory(char c): closure(c) {}
        
        virtual AbstractExpressionPtr makeExpression() {
            return AbstractExpressionPtr(new Expression);
        }
        
        virtual char getClosure() {return closure;}
};

#endif //FACTORY_HPP