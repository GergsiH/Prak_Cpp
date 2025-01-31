#include <iostream>
#include "Parser.h"

int main(int argc, char* argv[]) {
    char* file = "./test1.txt";
    if(argc >= 2) file = argv[1];

    Parser P(file);
    try{
        P.analyze();
    }catch(Lex err){
        cout << "Unexpected lexeme: " << err << endl;
    }catch(char c){
        cout << "Unexpected char: " << c << endl;
    }
    return 0;
}