#include "factory.hpp"

// AbstractFactory
AbstractFactory::AbstractFactory(char c): closure(c) {}

char AbstractFactory::getClosure() {
    return closure;
}

// LoopFactory
LoopFactory::LoopFactory(char c): AbstractFactory(c) {}

AbstractExpressionPtr LoopFactory::makeExpression() {
    return AbstractExpressionPtr(new Loop);
}