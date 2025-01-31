#pragma once

#include <iostream>
#include <string>

enum type_of_lex {
    //TW
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT,
    LEX_STRING,
    LEX_REAL,
    LEX_READ,
    LEX_WRITE,
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_FOR,
    LEX_CONTINUE,
    LEX_AND, 
    LEX_OR,
    LEX_NOT,
    //TD
    LEX_FIN,

    LEX_EQ,
    LEX_DEQ, //double =
    LEX_NEQ,
    LEX_STAR,
    LEX_SLASH,
    LEX_PLUS,
    LEX_MINUS,
    LEX_PERC,
    LEX_GTS, //greater than sign
    LEX_GTSEQ, // >=
    LEX_LTS,
    LEX_LTSEQ,
    LEX_SEMICOLON, 
    LEX_COMMA,
    LEX_DOT,
    LEX_LPAR, //(
    LEX_RPAR,
    LEX_LBR, //{ (left bracket)
    LEX_RBR,
    //остальное
    LEX_NUM,
    LEX_ID,
    LEX_STR_CONST,
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    double d_lex;
    std::string s_lex;
public:
    Lex (type_of_lex t = LEX_NULL, int v = 0, double d = 0, std::string s = " ") {
        t_lex = t;
        v_lex = v;
        d_lex = d;
        s_lex = s;
    }
    type_of_lex get_type ( ) const {return t_lex;}
    int get_value ( ) const {return v_lex;}
    double get_double_value() const{return d_lex;}
    std::string get_string() const {return s_lex;}

    void put_type(type_of_lex t) {t_lex = t;}

    void put_value(int v) {v_lex = v;}

    void put_double_value(double d) {d_lex = d;}

    void put_string(std::string s) {s_lex = s;}
};