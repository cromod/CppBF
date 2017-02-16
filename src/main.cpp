// Brainfuck interpreter using interpreter, composite and factory patterns :)

#include <string>
#include "data.hpp"
#include "expression.hpp"
#include "parser.hpp"

using namespace std;

int main()
{
    Data data; data.array.assign(1,0); data.ptr = 0;
    string code("++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
    //string code(",[.[-],]");
    Parser parser;
    AbstractExpressionPtr syntaxTreePtr = parser.buildTree(code);
    syntaxTreePtr->interpret(data);
}
