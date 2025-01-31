#pragma once

#include <string>
#include <cctype>
#include <iostream>
#include <vector>
#include "Lex.h"
#include "Ident.h"

using std::string;
using std::ostream;

class Scanner {
    FILE * fp;
    char   c;
    int look ( const string buf, const char ** list ) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc () { c = fgetc (fp); }

public:
    static const char * TW [], * TD [];

    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) ) 
            throw  "Сan’t open file" ;
    }

    Lex get_lex ();

    ~Scanner(){fclose(fp);}
};

 
const char * Scanner::TW [] = { "", "program", "int", "string", "real", "read", "write", "if", "else", "while", "for", "continue", "and", "or", "not", NULL};
 
const char * Scanner::TD [] = { "@", "=", "==", "!=", "*", "/", "+", "-", "%", ">", ">=", "<", "<=", ";", ",", ".", "(", ")", "{", "}", NULL };
 
Lex Scanner::get_lex () {
    enum state { H, IDENT, NUMB, RNUMB, COM, COM2, ALE, NEQ, SLASH, QUOTE };
    int d, j;
    int st = 10;
    double r; //дробная часть
    string  buf;
    state   CS = H;
    do {
        gc ();
        switch (CS) {
            case H:
                if (feof(fp)) {
                    return Lex(LEX_FIN);
                }
                if ( c ==' ' || c == '\n' || c== '\r' || c == '\t' ); 
                else if (isalpha (c)) {
                    buf.push_back (c);
                    CS  = IDENT;
                }
                else if (isdigit (c)) {
                    d   = c - '0';
                    CS  = NUMB;
                } 
                else if (c == '!') {
                    buf.push_back (c);
                    CS  = NEQ;
                }
                else if (c == '=' || c == '<' || c == '>') { 
                    buf.push_back (c);
                    CS  = ALE; 
                }
                else if (c == '/') {
                    buf.push_back (c);
                    CS  = SLASH;
                }
                else if (c == '"'){
                    CS = QUOTE;
                }
                else if (c == '@')
                    return Lex (LEX_FIN);

                else {
                    buf.push_back (c);
                    if ((j = look (buf, TD))){
                        return Lex ((type_of_lex)(j + (int) LEX_FIN), j);
                    }
                    else
                        throw c;
                }
                break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c); 
                }
                else {
                    ungetc (c, fp);
                    if ((j = look (buf, TW))) return Lex ((type_of_lex) j, j);

                    else {
                        j = put(buf);
                        return Lex (LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit (c)) {
                    d = d * 10 + ( c - '0' );
                }else if(c == '.'){
                    r = d;
                    CS = RNUMB;
                }
                else {
                    ungetc (c, fp);
                    return Lex(LEX_NUM, d);
                }
                break;
            case RNUMB:
                if (isdigit (c)) {
                    r = r + (double)(c - '0') / st;
                    st*=10;
                }
                else {
                    ungetc (c, fp);
                    return Lex(LEX_NUM, 0, r);
                }
                break;
            case SLASH:
                if (c == '*') {
                    buf.pop_back();
                    CS = COM;
                } else if(feof(fp)){
                    throw '@';
                }else {
                    ungetc (c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            case COM:
                if (c == '*' ) {
                    CS  = COM2;
                }
                else if (feof(fp) )
                     throw '@';
                break;
            case COM2:
                if (c == '/'){
                    CS = H;
                }else if(feof(fp)){
                    throw '@';
                }else if(c != '*'){ // если *, то ничего не делаем, всё ещё в этом состоянии
                    CS = COM;
                }
                break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) ( j + (int) LEX_FIN ), j);
                }
                else {
                    ungetc (c, fp);
                    j = look (buf, TD);
                    return Lex((type_of_lex) ( j + (int) LEX_FIN ), j);
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j = look ( buf, TD );
                    return Lex (LEX_NEQ, j);
                }
                else
                    throw '!';
                break;
            case QUOTE: 
                if (c == '"') {
                    std::string res = "";
                    for (size_t i = 0; i < buf.size(); ++i) {
                        res += buf[i];
                    }
                    return Lex(LEX_STR_CONST, 0, 0, res);
                } else if (feof(fp)) {
                    throw '@';
                }
                buf.push_back(c);
                break;
    }
  } while (true);
}
 
ostream & operator<< ( ostream &s, Lex l ) {
    string t;
    if ( l.get_type() <= LEX_NOT )
        t = Scanner::TW[l.get_type()];
    else if ( l.get_type() >= LEX_FIN && l.get_type() <= LEX_RBR )
        t = Scanner::TD[ l.get_type() - LEX_FIN ];
    else if ( l.get_type() == LEX_NUM ){
        t = "NUMB";
        if(l.get_double_value() != 0) s << '(' << t << ',' << l.get_double_value() << ");" << std::endl;
        else s << '(' << t << ',' << l.get_value() << ");" << std::endl;
        return s;
    }
    else if ( l.get_type() == LEX_ID )
        t = TID[l.get_value()].get_name();
    else if ( l.get_type() == LEX_STR_CONST){
        t = "STRING CONST";
        s << '(' << t << ',' << l.get_string() << ");" << std::endl;
        return s;
    }
    
    else
        throw l;
    s << '(' << t << ',' << l.get_value() << ");" << std::endl;
    return s;
}