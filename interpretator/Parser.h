#pragma once

#include <iostream>

#include "Scanner.h"
using std::cout;
using std::endl;
template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}
 
class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    Scanner      scan;

    void P();
    void D1();
    void D();
    void O();
    void O1();
    void E();
    void E_OR();
    void E_AND();
    void E_R();
    void E_AS();
    void E_MD();
    void F();

    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();

        cout << curr_lex;
    }
public:
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};
 
void Parser::analyze () {
    gl ();
    P ();
    if (c_type != LEX_FIN)
        throw curr_lex;

    cout << endl << "The End (Successfull)" << endl;
}
 
void Parser::P () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else throw curr_lex;

    if(c_type == LEX_LBR){
        gl();
    }else throw curr_lex;

    D (); 
    
    O (); // Проверка на закрывающую скобку здесь. Если ее не будет, то не выйдем оттуда

    if(c_type != LEX_RBR) throw curr_lex;

    gl(); //и надеемся, что напоремся на LEX_FIN - проверка в analyze
    
}
 
void Parser::D () {
    while(c_type == LEX_INT || c_type == LEX_REAL || c_type == LEX_STRING){
        D1(); //описание
        if(c_type == LEX_SEMICOLON) gl();
        else
            throw curr_lex;
    }
}
 
void Parser::D1 () {
    //можно не проверять на тип(тип ли это или нет), т.к. это уже сделано в D()
    type_of_lex t = c_type;
    gl(); // по идее, читаем LEX_ID

    if(c_type != LEX_ID) throw curr_lex;
    else{
        gl();
        if(c_type == LEX_EQ){ // Если <идентификатор>=<константа>
            gl();
            if(t == LEX_STRING){
                if(c_type != LEX_STR_CONST) throw curr_lex; //проверку типа можно и так
                //здесь инициализация переменной
            }else{
                if(c_type == LEX_PLUS || c_type == LEX_MINUS){
                    //соответствующие действия
                    gl();
                }
                if(c_type != LEX_NUM)
                    throw curr_lex;
                else{
                    //что-то делаем c числом (обработку дробной/недробной части добавила в лексический анализатор) понимание, дробное число или нет, можно получить из d
                } 
            }
            
            gl();
        }

        while ( c_type == LEX_COMMA ){
            gl();
            if ( c_type != LEX_ID )
                throw curr_lex;
            else {
                gl ();

                if(c_type == LEX_EQ){ // Если <идентификатор>=<константа>
                    gl();
                    if(t == LEX_STRING){
                        if(c_type != LEX_STR_CONST) throw curr_lex; //проверку типа можно и так
                        //здесь инициализация переменной
                    }else{
                        if(c_type == LEX_PLUS || c_type == LEX_MINUS){ //если указали знак
                            //соответствующие действия
                            gl();
                        }
                        if(c_type != LEX_NUM)
                            throw curr_lex;
                        else{
                            //что-то делаем c числом (обработку дробной/недробной части добавила в лексический анализатор) понимание, дробное число или нет, можно получить из d
                        } 
                    }
                    
                    gl();
                }
                //иначе соответствующие действия, но пока просто скипаем
            }
        }
        //здесь будет функция определения, для которой нужен будет t
    }
}
 
void Parser::O() { //можно объединить в целом
    while(c_type != LEX_RBR){
        O1();
    }
}
 
void Parser::O1 () {
 
    if ( c_type == LEX_IF ) {
        gl ();
        if (c_type != LEX_LPAR)
            throw curr_lex;
        else{
            gl();
            E (); 
            if (c_type == LEX_RPAR) {
                gl();
                O1();
                if (c_type == LEX_ELSE) {
                    gl();
                    O1();
                } else {
                    //тут НЕ БУДЕТ соответствующая обработка, если нету else, так как в моем варианте такоо быть не может
                    throw curr_lex;
                }
            } else
                throw curr_lex;
        }
    }//end if
    else if ( c_type == LEX_WHILE ) {
        gl ();
        if (c_type != LEX_LPAR) {
            throw curr_lex;
        }else{
            gl();
            E();
            if (c_type != LEX_RPAR)
                throw curr_lex;
            else{
                gl();
                O1();
            }
        }
    }//end while
    else if (c_type == LEX_CONTINUE) {
        gl();

        if (c_type != LEX_SEMICOLON){
            throw curr_lex;
        }else 
            gl();

    }//end continue;
    else if (c_type == LEX_FOR) {

        gl();
        if (c_type != LEX_LPAR)
            throw curr_lex;
        else{
            gl();

            if(c_type == LEX_ID || c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_NOT || c_type == LEX_NUM || c_type == LEX_STR_CONST) //проверка на выражение, которого может не быть
                E();//gl()?

            if(c_type != LEX_SEMICOLON)
                throw curr_lex;
            else{
                gl();

                if(c_type == LEX_ID || c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_NOT || c_type == LEX_NUM || c_type == LEX_STR_CONST) //проверка на выражение, которого может не быть
                    E();
            }
            if(c_type != LEX_SEMICOLON)
                throw curr_lex;
            else{

                gl();

                if(c_type == LEX_ID || c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_NOT || c_type == LEX_NUM || c_type == LEX_STR_CONST) //проверка на выражение, которого может не быть
                    E();
            }

            if(c_type != LEX_RPAR)
                throw curr_lex;
            else{
                gl();
                O1();
            }
        }
    }
    else if ( c_type == LEX_READ ) {
        gl ();
        if ( c_type != LEX_LPAR )
            throw curr_lex;
        else{
            gl ();

            if (c_type != LEX_ID)
                throw curr_lex;
            else
                gl();

            if( c_type != LEX_RPAR )
                throw curr_lex;
            else
                gl ();

            if(c_type != LEX_SEMICOLON)
                throw curr_lex;
            else 
                gl();
        }
    }//end read
    else if ( c_type == LEX_WRITE ) {
        gl ();
        if ( c_type == LEX_LPAR ) {

            do {
                gl();
                E();
            }while ( c_type == LEX_COMMA );
            if ( c_type == LEX_RPAR )
                gl ();
            else
                throw curr_lex;
            
            if(c_type != LEX_SEMICOLON)
                throw curr_lex;
            else
                gl();   
        }
        else
            throw curr_lex;
    }//end write
    else if (c_type == LEX_ID || c_type == LEX_NUM  || c_type == LEX_STR_CONST) {   //|| c_type == LEX_PLUS || c_type == LEX_MINUS ???
        E();
        if(c_type != LEX_SEMICOLON) throw curr_lex;
        else gl();
    }// Оператор - выражение
    else if(c_type == LEX_LBR){ //составной оператор
        gl();
        O();
        if(c_type != LEX_RBR) 
            throw curr_lex;
        else gl();
    }else if(c_type == LEX_SEMICOLON){ //Пустой оператор?
        gl();
    }else
        throw curr_lex;
}
 
void Parser::E () { // норм или не норм?
    E_OR ();
    while ( c_type == LEX_EQ ) {
        gl (); 
        E_OR (); 
        //здесь ещё проверка операндов
    }
}
 
void Parser::E_OR () {
    E_AND ();
    while ( c_type == LEX_OR) {
        gl ();
        E_AND ();
        //здесь ещё проверка операндов
    }
}
 
void Parser::E_AND () {
    E_R ();
    while ( c_type == LEX_AND) {
        gl ();
        E_R ();
        //здесь ещё проверка операндов
    }
}

void Parser::E_R () {
    E_AS ();
    while ( c_type == LEX_GTS || c_type == LEX_GTSEQ || c_type == LEX_LTS || c_type == LEX_LTSEQ || c_type == LEX_NEQ || c_type == LEX_DEQ) {
        gl ();
        E_AS ();
        //здесь ещё проверка операндов
    }
}
void Parser::E_AS () {
    E_MD ();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS) {
        gl ();
        E_MD ();
        //здесь ещё проверка операндов
    }
}

void Parser::E_MD () {
    F ();
    while (c_type == LEX_STAR || c_type == LEX_SLASH) {
        gl ();
        F ();
        //здесь ещё проверка операндов
    }
}
 
void Parser::F () {
    if ( c_type == LEX_ID ) {
        gl ();
    }
    else if ( c_type == LEX_NUM ) {
        gl ();
    }
    else if(c_type == LEX_STR_CONST){
        gl();
    }
    else if ( c_type == LEX_NOT || c_type == LEX_PLUS  || c_type == LEX_MINUS) {
        gl (); 
        // if(c_type == LEX_PLUS  || c_type == LEX_MINUS){
        // gl();
        //     if(c_type == LEX_ID || c_type == LEX_NUM) {
        //         gl();
        //     }else   
        //         throw curr_lex;
        // }

        F (); 
    }
    else if ( c_type == LEX_LPAR ) {
        gl (); 
        E ();
        if ( c_type == LEX_RPAR)
            gl ();
        else 
            throw curr_lex;
    // }else if(c_type == LEX_SEMICOLON){
    //     return;
    // }
    }else 
        throw curr_lex;
}
 
