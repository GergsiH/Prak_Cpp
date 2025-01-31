#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class ERROR{
    int row;
    std::string str;
public:
    ERROR(int r, std::string s) {
        row = r;
        str = s;
    }
    int get_row() {return row;}
    std::string& get_error() {return str;}
};

enum type_of_lex {
    lex_null,
    lex_prog, lex_int, lex_string, lex_if, lex_else, lex_while, lex_read, lex_write, lex_not, lex_and, 
    lex_or,lex_bool, lex_true, lex_false, lex_real,

    lex_multi, lex_div, lex_mod, lex_plus, lex_minus, lex_less, lex_great, lex_le, lex_ge, 
    lex_equal2, lex_neq, lex_comma, lex_semicolon, lex_colon, lex_rbracket, lex_lbracket, lex_lbrace, 
    lex_rbrace, lex_equal1,
    
    lex_final,
    lex_id,
    lex_num,
    lex_str
};

enum state {
    H, IDENT, NUMB, STR, LGE, NEQ, PREDCOM1, COM, PREDCOM2, DIV,
};

class Ident {
    std::string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident() {
        declare = false;
        assign = false;
    }
    Ident(const char * n) {
        name = n;
        declare = false;
        assign = false;
    }
    bool operator == (const std::string& s) {return s == name;}
    std::string get_name() const {return name;}
    bool get_declare() const {return declare;}
    void set_declare() {declare = true;}
    type_of_lex get_type() const {return type;}
    bool get_assign() const {return assign;}
    void set_assign() {assign = true;}
    int get_value() const {return value;}
    void set_value(const int v) {value = v;}
};

std::vector <Ident> TID;

int put(const std::string& buf) {
    std::vector <Ident> :: iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end()) {
        return k - TID.begin();
    };
    TID.push_back(Ident(buf.c_str()));
    return TID.size() - 1;
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    std::string str;
public:
    Lex(type_of_lex t = lex_null, int v = 0, const std::string s = "") {
        t_lex = t;
        v_lex = v;
        str = s;
    }
    type_of_lex get_type() const {return t_lex;}
    int get_value() const {return v_lex;}
    friend std::ostream& operator << (std::ostream& s, Lex l) {
        if (l.str != "") s << '(' << l.t_lex << ", " << l.v_lex << ", " << l.str << ')' << std::endl;
        else s << '(' << l.t_lex << ", " << l.v_lex << ')' << std::endl;
        return s;
    }
};

class Scanner {
    FILE * f;
    int c;
    int found(const std::string& s, const char** list) {
        int i = 1;
        while (list[i]) {
            if (s == list[i]) return i;
            i++;
        }
        return 0;
    }
    int row;
    void gc() {c = fgetc(f);}
public:
    static const char* TW[];
    static const char* TD[];
    Scanner(const char * program) {
        f = fopen(program, "r");
        row = 1;
    }
    Lex get_lex() {
        state CS = H;
        int d, j;
        std::string buf;
        do {
            gc();
            switch(CS) {
                case H: {
                    if (c == ' ' or c == '\r' or c == '\t') {}
                    else if (c == '\n') {
                        row += 1;
                    }
                    else if ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')) {
                        CS = IDENT;
                        buf.push_back(c);
                    }
                    else if (c >= '0' and c <= '9') {
                        d = c - '0';
                        CS = NUMB;
                    }
                    else if (c == '"') {
                        CS = STR;
                        buf.push_back(c);
                    }
                    else if (c == '<' or c == '>' or c == '=') {
                        CS = LGE;
                        buf.push_back(c);
                    }
                    else if (c == '!') {
                        CS = NEQ;
                        buf.push_back(c);
                    }
                    else if (c == -1) {
                        return Lex(lex_final);
                    }
                    else if (c == '/') {
                        CS = PREDCOM1;
                    }
                    else {
                        CS = DIV;
                        buf.push_back(c);
                    }
                    break;
                };
                case IDENT: {
                    if ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or (c >= '0' and c <= '9')) {
                        buf.push_back(c);
                    }
                    else if (c == '"') {
                        throw ERROR(row, std::string("IDENT"));
                        //throw "IDENT";
                    }
                    else {
                        ungetc(c, f);
                        if ((j = found(buf, TW))) {
                            return Lex((type_of_lex) j, j);
                        }
                        else {
                            j = put(buf);
                            return Lex(lex_id, j);
                        }
                    };
                    break;
                }
                case NUMB: {
                    if (c >= '0' and c <= '9') {
                        d = d*10 + (c - '0');
                    }
                    else if (c == '"' or (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')) {
                        throw ERROR(row, std::string("NUMB"));
                        //throw "NUMB";
                    }
                    else {
                        ungetc(c, f);
                        return Lex(lex_num, d);
                    };
                    break;
                }
                case STR: {
                    if (c == -1) throw ERROR(row, std::string("STR1"));
                    else if (c != '"') buf.push_back(c);
                    else {
                        buf.push_back(c);
                        gc();
                        if (c == '"' or (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or (c >= '0' and c <= '9')) {
                            throw ERROR(row, std::string("STR2"));
                            //throw "STR2";
                        }
                        ungetc(c, f);
                        return Lex(lex_str, 0, buf);
                    };
                    break;
                }
                case LGE: {
                    if (c == '=') {
                        buf.push_back(c);
                        j = found(buf, TD);
                        return Lex((type_of_lex) (j - 8 + (int) lex_le), j);
                    }
                    else {
                        ungetc(c, f);
                        j = found(buf, TD);
                        return Lex((type_of_lex) (j - 6 + (int) lex_less), j);
                    };
                    break;
                }
                case NEQ: {
                    if (c == '=') {
                        return Lex(lex_neq, 11);
                    }
                    else {
                        throw ERROR(row, std::string("NEQ"));
                        //throw "NEQ";
                    }
                    break;
                }
                case PREDCOM1: {
                    if (c == '*') {
                        CS = COM;
                    }
                    else {
                        ungetc(c, f);
                        return Lex(lex_div, 2);
                    }
                    break;
                }
                case COM: {
                    if (c == '*') CS = PREDCOM2;
                    else if (c == -1) {
                        throw ERROR(row, std::string("COM"));
                        //throw "COM";
                    }
                    break;
                }
                case PREDCOM2: {
                    if (c == '/'){
                        CS = H;
                    }
                    else if(c != '*'){
                        CS = COM;
                    }
                    break;
                }
                case DIV: {
                    ungetc(c, f);
                    if ((j = found(buf, TD))) {
                        return Lex((type_of_lex) (j + (int) lex_real), j);
                    }
                    else {
                        throw ERROR(row, std::string("DIV"));
                        //throw "DIV";
                    }
                    break;
                }
            }
        } while (true);
    }
};

const char* Scanner::TW[] = {"", "program\0", "int\0", "string\0", "if\0", "else\0", "while\0", "read\0", "write\0", "not\0", "and\0", "or\0", "boolean\0", "true\0", "false\0", "real\0", NULL};
const char* Scanner::TD[] = {"", "*\0", "/\0", "%\0", "+\0", "-\0", "<\0", ">\0", "<=\0", ">=\0", "==\0", "!=\0", ",\0", ";\0", ":\0", "(\0", ")\0", "{\0", "}\0", "=\0", NULL};


template <class t, class  t_elem>
void from_st ( t & stack, t_elem & i ) {
    i = stack.top(); 
    stack.pop();
}





int main() {
    Scanner s("Test2.txt");
    try {
        Lex l = s.get_lex();
        while (l.get_type() != lex_final) {
            std::cout << l;
            l = s.get_lex();
        }
    }
    catch(ERROR& err) {
        std::cout << err.get_row() << " строка - лексическая ошибка: ";
        std::string s = err.get_error();
        if (s == "IDENT") {
            std::cout << "Неправильное имя идентификатора" << std::endl;
        }
        else if (s == "NUMB") {
            std::cout << "Неправильно написано число" << std::endl;
        }
        else if (s == "STR1") {
            std::cout << "Строка не имеет конца" << std::endl;
        }
        else if (s == "STR2") {
            std::cout << "За строкой не может идти ничего, кроме пробела и разделителей" << std::endl;
        }
        else if (s == "NEQ") {
            std::cout << "Не может стоять просто !, должно быть !=" << std::endl;
        }
        else if (s == "COM") {
            std::cout << "Комментарий не имеет конца" << std::endl;
        }
        else if (s == "DIV") {
            std::cout << "Такого разделителя не существует или использован некорректный символ" << std::endl;
        }
    };/***/
};