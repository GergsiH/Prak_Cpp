#pragma once
#include "INTsem.cpp"
#include <iostream>
#include <vector>
#include <stack>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::stack;
using std::string;

template <class T, class T_EL>
void from_poliz ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Executer{
    public:
    void execute(vector<Lexeme>& poliz){
        Lexeme pc_el, o1, o2;
        stack <Lexeme> args;
        stack <Lexeme> wr_stack;
        int i, index = 0, size = poliz.size();
        int a = 0;
        double d = 0;
        string s = "";
        string l;
        while (index < size){
            pc_el = poliz[index];
            switch(pc_el.getType()){
                case LEX_NUM: case LEX_RNUM: case LEX_STRINGT: case POLIZ_ADDRESS: case POLIZ_LABEL:
                    args.push(pc_el);
                    break;
                case LEX_ID:
                    i = pc_el.getValue();
                    if (!ListOfIdent[i].getAssign()) throw "POLIZ: INDEFINITE IDENTIFIER";
                    else{
                        if (ListOfIdent[i].getType() == LEX_INT || ListOfIdent[i].getType() == LEX_NUM){
                            args.push(Lexeme(LEX_NUM, ListOfIdent[i].getValue()));
                            break;
                        }
                        else if (ListOfIdent[i].getType() == LEX_STRING || ListOfIdent[i].getType() == LEX_STRINGT){
                            args.push(Lexeme(LEX_STRING, 0, 0, ListOfIdent[i].getString()));
                            break;
                        }
                        else if (ListOfIdent[i].getType() == LEX_REAL || ListOfIdent[i].getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, ListOfIdent[i].getDValue()));
                            break;
                        }
                        else throw "POLIZ: IMPOSSIBLE IDENTIFIER TYPE";
                        /* args.push(Lexeme(ListOfIdent[i].getType(), ListOfIdent[i].getValue(), ListOfIdent[i].get_svalue()));
                        break; */
                    }
                case POLIZ_GO:
                    from_poliz(args, o1);
                    index = o1.getValue() - 1;
                    break;
                case POLIZ_FGO:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o2.getType() != LEX_NUM) throw "INVALID !F CONDITION";
                    else{
                        if(o2.getValue() == 0) index = o1.getValue() - 1;
                        break;
                    }
                case LEX_NOT:
                    from_poliz(args, o1);
                    if (o1.getValue()){
                        o1.putType(LEX_NUM);
                        o1.putValue(0);
                    }
                    else {
                        o1.putType(LEX_NUM);
                        o1.putValue(1);
                    }
                    args.push(o1);
                    break;
                case LEX_OR:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    o1.putValue(o2.getValue() || o1.getValue());
                    if (o1.getValue()) {
                        o1.putType(LEX_NUM);
                        o1.putValue(1);
                    }
                    else {
                        o1.putType(LEX_NUM);
                        o1.putValue(0);
                    }
                    args.push(o1);
                    break;
                case LEX_AND:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    o1.putValue(o2.getValue() && o1.getValue());
                    if (o1.getValue()) {
                        o1.putType(LEX_NUM);
                        o1.putValue(1);
                    }
                    else {
                        o1.putType(LEX_NUM);
                        o1.putValue(0);
                    }
                    args.push(o1);
                    break;
                case LEX_PLUS: 
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM ){
                        if(o2.getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() + o1.getValue()));
                        }else{
                            args.push(Lexeme(LEX_NUM, o2.getValue() + o1.getValue()));
                        }
                    }else if (o2.getType() == LEX_RNUM){
                        args.push(Lexeme(LEX_RNUM, 0, o1.getDValue() + o2.getDValue()));
                    }
                    else if (o1.getType() == LEX_STRINGT)
                        args.push(Lexeme(LEX_STRINGT, 0, 0, o1.getString() + o1.getString()));
                    break;
                case LEX_MUL:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM ){
                        if(o2.getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() * o1.getValue()));
                        }else{
                            args.push(Lexeme(LEX_NUM, o2.getValue() * o1.getValue()));
                        }
                    }else if (o2.getType() == LEX_RNUM){
                        args.push(Lexeme(LEX_RNUM, 0, o1.getDValue() * o2.getDValue()));
                    }
                    break;
                case LEX_MINUS:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM ){
                        if(o2.getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() - o1.getValue()));
                        }else{
                            args.push(Lexeme(LEX_NUM, o2.getValue() - o1.getValue()));
                        }
                    }else if (o2.getType() == LEX_RNUM){
                        args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() - o1.getDValue()));
                    }
                    break;
                case LEX_DIV:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if(o1.getValue())
                        if (o1.getType() == LEX_NUM ){
                            if(o2.getType() == LEX_RNUM){
                                args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() / o1.getValue()));
                            }else{
                                args.push(Lexeme(LEX_NUM, o2.getValue() / o1.getValue()));
                            }
                        }else if (o2.getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() / o1.getDValue()));
                        }
                    else throw "POLIZ: DIVISION BY ZERO";
                    break;
                case LEX_EQ:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() == o1.getValue());
                        else o1.putValue(o2.getValue() == o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() == o1.getDValue());
                        else o1.putValue(o2.getValue() == o1.getDValue());
                    }
                    else if (o1.getType() == LEX_STRING) {
                        o1.putValue(o1.getString() == o2.getString());
                    }

                    o1.putType(LEX_NUM);
                    args.push(o1);
                    break;
                case LEX_NEQ:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() != o1.getValue());
                        else o1.putValue(o2.getValue() != o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() != o1.getDValue());
                        else o1.putValue(o2.getValue() != o1.getDValue());
                    }
                    else if (o1.getType() == LEX_STRING) {
                        o1.putValue(o1.getString() != o2.getString());
                    }
                    o1.putType(LEX_NUM);
                    args.push(o1);
                    break;
                case LEX_LSS:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() < o1.getValue());
                        else o1.putValue(o2.getValue() < o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() < o1.getDValue());
                        else o1.putValue(o2.getValue() < o1.getDValue());
                    }
                    else if (o1.getType() == LEX_STRING) {
                        o1.putValue(o1.getString() < o2.getString());
                    }
                    o1.putType(LEX_NUM); 
                    args.push(o1);
                    break;
                case LEX_GTR:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() > o1.getValue());
                        else o1.putValue(o2.getValue() > o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() > o1.getDValue());
                        else o1.putValue(o2.getValue() > o1.getDValue());
                    }
                    else if (o1.getType() == LEX_STRING) {
                        o1.putValue(o1.getString() > o2.getString());
                    }
                    o1.putType(LEX_NUM);
                    args.push(o1);
                    break;
                case LEX_LTSEQ:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() <= o1.getValue());
                        else o1.putValue(o2.getValue() <= o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() <= o1.getDValue());
                        else o1.putValue(o2.getValue() <= o1.getDValue());
                    }
                    o1.putType(LEX_NUM);
                    args.push(o1);;
                    break;
                case LEX_GTSEQ:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    if (o1.getType() == LEX_NUM) {
                        if(o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() >= o1.getValue());
                        else o1.putValue(o2.getValue() >= o1.getValue());
                    }
                    else if (o1.getType() == LEX_RNUM) {
                        if (o2.getType() == LEX_REAL) o1.putValue(o2.getDValue() >= o1.getDValue());
                        else o1.putValue(o2.getValue() >= o1.getDValue());
                    }
                    o1.putType(LEX_NUM);
                    args.push(o1);
                    break;
                case LEX_ASSIGN:
                    from_poliz(args, o1);
                    from_poliz(args, o2);
                    i = o2.getValue();
                    if(ListOfIdent[i].getType() == LEX_INT) {
                        if(o1.getType() == LEX_RNUM)
                            ListOfIdent[i].putValue(floor(o1.getDValue()));
                        else ListOfIdent[i].putValue(o1.getValue());
                    }
                    else if(ListOfIdent[i].getType() == LEX_REAL){
                        if(o1.getType() == LEX_NUM)
                            ListOfIdent[i].putDValue(o1.getValue());
                        else ListOfIdent[i].putDValue(o1.getDValue());
                    } 
                    else if(ListOfIdent[i].getType() == LEX_STRING) ListOfIdent[i].putString(o1.getString());
                    ListOfIdent[i].putAssign();
                    args.push(o1);
                    break;
                case LEX_WRITE:
                    while(!args.empty()){
                        from_poliz(args, o1);
                        wr_stack.push(o1);
                    }
                    while(!wr_stack.empty()){
                        from_poliz(wr_stack, o1);
                        if (o1.getType() == LEX_NUM) cout << o1.getValue();
                        else if(o1.getType() == LEX_RNUM) cout << o1.getDValue();
                        else if (o1.getType() == LEX_STRINGT) cout << o1.getString();
                        cout << " ";
                    }
                    cout << endl;
                    break;
                case LEX_READ:
                    from_poliz(args, o1);
                    i = o1.getValue();
                    if (ListOfIdent[i].getType() == LEX_INT){
                        cout << "Input int value for " << ListOfIdent[i].getName() << " : ";
                        cin >> a;
                    }
                    else if (ListOfIdent[i].getType() == LEX_REAL){
                        cout << "Input real value for " << ListOfIdent[i].getName() << " : ";
                        cin >> d;
                    }
                    else if (ListOfIdent[i].getType() == LEX_STRING){
                        cout << "Input string value for " << ListOfIdent[i].getName() << " : ";
                        cin >> s;
                    }
                    ListOfIdent[i].putValue(a);
                    ListOfIdent[i].putDValue(d);
                    ListOfIdent[i].putString(s);
                    ListOfIdent[i].putAssign();
                    a=0;
                    d=0;
                    s="";
                    break;
                case LEX_SEMICOLON:
                    args.pop();
                    break;
                default:
                    throw "POLIZ: UNEXPECTED ELEMENT";
            }
            ++index;
        };
        // cout << "Execution finished!!!" << endl;
    }
};