#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Lex.h"

using std::string;

class Ident
{
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
    double dvalue;
public:
    Ident (){
        declare = false;
        assign = false;
    }
    Ident (const string n){
        name = n;
        declare = false;
        assign = false;
    }
    string get_name () const{ return name;}
    void put_name (const string n){
        name = n;
    }

    bool get_declare (){return declare;}

    void put_declare (){declare = true;}

    type_of_lex get_type (){return type;}

    void put_type (type_of_lex t ){type = t;}

    bool get_assign (){return assign;}

    void put_assign (){assign = true;}

    int get_value (){return value;}

    void put_value (int v){value = v;}

    double get_dvalue(){return dvalue;}

    void put_dvalue(double d){dvalue = d;}

    bool operator ==(const string&s) const{
        return name == s;
    }

};

std::vector <Ident> TID; // переделать на unordered map
//unordered map: Ident - int
int put (const string &  buf) {

     std::vector <Ident> :: iterator k;

     if ((k = find (TID.begin(), TID.end(), buf)) != TID.end()) return   k - TID.begin();

     TID.push_back (Ident (buf));  
     return   TID.size() - 1;
}