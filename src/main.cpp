// Brainfuck interpreter using interpreter, composite and factory patterns :)

#include <sstream>
#include <fstream>

#include "parser.hpp"

using namespace std;

int main(int argc, char** argv)
{
    ifstream inFile;
    inFile.open(argv[1]);
    stringstream strStream;
    strStream << inFile.rdbuf();
    string code = strStream.str();

    Data data; data.array.assign(1,0); data.ptr = 0;
    Parser parser;
    AbstractExpressionPtr syntaxTreePtr = parser.buildTree(code);
    syntaxTreePtr->interpret(data);
    
    return EXIT_SUCCESS;
}
