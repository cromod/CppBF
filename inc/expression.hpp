#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <list>

#include "data.hpp"

class AbstractExpression
{
    public:
        virtual void add(std::shared_ptr<AbstractExpression>);
        virtual void interpret(Data &) = 0;
};

typedef std::shared_ptr<AbstractExpression> AbstractExpressionPtr;

class IncrementByte: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class DecrementByte: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class IncrementPtr: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class DecrementPtr: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class Output: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class Input: public AbstractExpression
{
    public:
        virtual void interpret(Data &data);
};

class CompositeExpression: public AbstractExpression
{
    protected:
        std::list<AbstractExpressionPtr> expTree;
    public:
        virtual void add(AbstractExpressionPtr exp);

        virtual void interpret(Data &data);
};

class Loop: public CompositeExpression
{
    public:
        virtual void interpret(Data &data);
};

#endif //EXPRESSION_HPP