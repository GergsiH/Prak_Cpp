#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <stack>



int NNN = 1;

enum typelex
{
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT, LEX_STRING, LEX_REAL,
    LEX_TRUE, LEX_FALSE,
    LEX_IF, LEX_ELSE, LEX_FOR, LEX_WHILE,
    LEX_READ, LEX_WRITE, LEX_BREAK,
    LEX_NOT, LEX_OR, LEX_AND, 
    LEX_FIN,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, 
    LEX_QUOTE, LEX_EQ, LEX_LSS, LEX_GTR,
    LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_LTSEQ, LEX_NEQ, LEX_GTSEQ,
    LEX_LBRACE, LEX_RBRACE, LEX_LPAREN, LEX_RPAREN,
    LEX_ID, LEX_NUM, LEX_RNUM, LEX_BOOL, LEX_STRINGT,
    LEX_LABEL,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO,
};

class Lexeme 
{
    private:
        typelex type;
        int value;
        double dvalue;
        std::string str;
    public:
        Lexeme(typelex t = LEX_NULL, int v = 0, double d = 0.0, std::string s = "")
        {
            type = t;
            value = v;
            str = s;
            dvalue = d;
        }
        typelex getType()
        {
            return type;
        }
        int getValue()
        {
            return value;
        }
        double getDValue()
        {
            return dvalue;
        }
        std::string getString()
        {
            return str;
        }
        void putType(typelex t)
        {
            type = t;
        }
        void putValue(int v)
        {
            value = v;
        }
        void putDValue(double d)
        {
            dvalue = d;
        }
        void putString(std::string s)
        {
            str = s;
        }
        friend std::ostream& operator<< (std::ostream &s, Lexeme l);
};
 

class Identifier
{
    std::string name;
    bool declare, assign;
    typelex type;
    int value;
    double dvalue;
    std::string str;
    public:
        Identifier() 
        {
            declare = false;
            assign = false;
        }
        Identifier(const std::string n)
        {
            name = n;
            declare = false;
            assign = false;
        }
        std::string getName()
        {
            return name;
        }
        bool getDeclare() 
        {
            return declare;
        }
        typelex  getType()
        {
            return type;
        }
        bool getAssign()
        {
            return assign;
        }
        int getValue()
        {
            return value;
        }
        double getDValue()
        {
            return dvalue;
        }
        std::string getString()
        {
            return str;
        }
        void putType(typelex  t)
        {
            type = t;
        }
        void putDeclare()
        {
            declare = true;
        }
        void putAssign()
        {
            assign = true;
        }
        void putValue(int v)
        {
            value = v;
        }
        void putDValue(double d)
        {
            dvalue = d;
        }
        void putString(std::string s)
        {
            str = s;
        }
        bool operator==(const std::string& s) const;
};

bool Identifier::operator==(const std::string& s) const 
{
    return name == s;
}

std::vector<Identifier> ListOfIdent;

int put (const std::string & buf) 
{
    std::vector<Identifier>::iterator k;
    if ((k = find(ListOfIdent.begin(), ListOfIdent.end(), buf)) != ListOfIdent.end())
        return k - ListOfIdent.begin();
    ListOfIdent.push_back (Identifier(buf));
    return ListOfIdent.size() - 1;
}

class Scanner
{
    private:
        FILE *f;
        char c;
        int in (const std::string& buf, const char ** list) 
        {
            int i = 0;
            while (list[i]) 
            {
                if (buf == list[i])
                    return i;
                ++i;
            }
            return -1;
        }
        void gc () 
        {
            c = fgetc(f);
        }
    public:
        Scanner(const char* new_file)
        {
            f = fopen(new_file, "r");
        };
        Lexeme getLex();
        static const char* words[], *symbols[];
};

const char* Scanner::words [] = {"", "program", "int", "string", "real", "true", "false","if", "else", "for", "while", "read", "write", "break", "not", "or", "and", NULL};
const char* Scanner::symbols [] = { ";", ",", ":", "=", "\"","==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{","}","(", ")", NULL };

Lexeme Scanner::getLex()
{
    enum state {H, ID, NUMB, RNUMB, CMP, EQ, NEQ, QUOTE, COM, SLASH, COM2};
    int d, j;
    int st = 10;
    double r;
    std::string  buf;
    state current = H;
    while(true)
    {
        gc();
        switch (current) 
        {
            case H:
                    if ( c == '\n'){
                        NNN++;
                    }
                    else if (c == ' ' || c== '\n' || c == '\r' || c == '\t');
                    else if (isalpha(c)) 
                    {
                        buf.push_back (c);
                        current = ID;
                    }
                    else if (isdigit(c)) 
                    {
                        d = c - '0';
                        current = NUMB;
                    } 
                    else if (c == '<' || c == '>') 
                    { 
                        buf.push_back(c);
                        current = CMP; 
                    }
                    else if (c == '!')
                    {
                        buf.push_back(c);
                        current = NEQ; 
                    }
                    else if (c == '=')
                    {
                        buf.push_back(c);
                        current = EQ; 
                    }
                    else if (c == '\"') 
                    {
                        current = QUOTE;
                    }
                    else if (c == EOF)
                    {
                        return Lexeme(LEX_FIN);
                    }
                    else if (c == '/')
                    {
                        buf.push_back(c);
                        current = SLASH;
                    }
                    else 
                    {
                        buf.push_back (c);
                        if ((j = in(buf, symbols)) >= 0)
                        {
                            return Lexeme ((typelex)(j + (int) LEX_FIN + 1), j);
                        }
                        else
                            throw c;
                    }
                    break;
            case ID:
                if (isalpha (c) || isdigit (c))
                {
                    buf.push_back (c); 
                }
                else 
                {
                    ungetc (c, f);
                    if ((j = in(buf, words)) >= 0) 
                    {
                        return Lexeme ((typelex) j, j);
                    }
                    else 
                    {
                        j = put(buf);
                        return Lexeme(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit (c)) 
                {
                    d = d * 10 + (c - '0');
                }
                else if (c == '.')
                {
                    r = d;
                    current = RNUMB;
                }
                else 
                {
                    ungetc (c, f);
                    return Lexeme(LEX_NUM, d);
                }
                break;
            case RNUMB:
                if (isdigit (c)) 
                {
                    r = r + (double)(c - '0') / st;
                    st*=10;
                }
                else 
                {
                    ungetc (c, f);
                    return Lexeme(LEX_RNUM, 0, r);
                }
                break;
            case EQ:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in (buf, symbols);
                    return Lexeme ((typelex)LEX_EQ, j);
                }
                else 
                {
                    ungetc (c, f);
                    j = in (buf, symbols);
                    return Lexeme((typelex)LEX_ASSIGN, j);
                }
                break;
            case NEQ:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in ( buf, symbols);
                    return Lexeme (LEX_NEQ, j);
                }
                else
                    throw '!';
                break;
            case CMP:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in(buf, symbols);
                    return Lexeme((typelex) (j + (int)LEX_FIN + 1), j);
                }
                else
                {
                    ungetc (c, f);
                    j = in (buf, symbols);
                    return Lexeme((typelex) (j + (int)LEX_FIN + 1), j);
                }
                break;
            case QUOTE:
                if (c == '\"') 
                {
                    return Lexeme(LEX_STRINGT, 0, 0, buf);
                }
                buf.push_back(c); 
                continue;  
            case SLASH:
                if (c == '*')
                {
                    buf.pop_back();
                    current = COM;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                else
                {
                    ungetc (c, f);
                    j = in(buf, symbols);
                    return Lexeme((typelex)(j + (int)LEX_FIN), j);
                }
                break;
            case COM:
                if (c == '*' ) 
                {
                    current  = COM2;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                break;
            case COM2:
                if (c == '/')
                {
                    current = H;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                else if(c != '*')
                {
                    current = COM;
                }
                break;           
        }
    }
}

std::ostream & operator<< ( std::ostream &s, Lexeme l ) 
{
    std::string t;
    if (l.getType() <= LEX_AND)
        t = Scanner::words[l.getType()];
    else if (l.getType() > LEX_FIN && l.getType() <= LEX_RPAREN)
        t = Scanner::symbols[l.getType() - LEX_FIN - 1];
    else if (l.getType() == LEX_FIN)
    {
        s<<"FIN"<< std::endl;
        return s;
    }
    else if (l.getType() == LEX_STRINGT)
    {
        t = "STRING";
        s << '(' << t << ',' << l.getString() << ");"<< std::endl;
        return s;
    }
    else if (l.getType() == LEX_NUM)
    {
        t = "NUM";
        s << '(' << t << ',' << l.getValue() << ");"<< std::endl;
        return s;
    }
    else if (l.getType() == LEX_RNUM)
    {
        t = "RNUM";
        s << '(' << t << ',' << l.getDValue() << ");"<< std::endl;
        return s;
    }
    else if (l.getType() == LEX_ID)
        t = ListOfIdent[l.getValue()].getName();
    else if (l.getType() == POLIZ_LABEL){
        t = "Label";
    }
    else if (l.getType() == POLIZ_ADDRESS){
        t = "Addr";
    }
    else if (l.getType()== POLIZ_GO){
        t ="!";
    }
    else if (l.getType() == POLIZ_FGO){
        t = "!F";
    }
    else{
        throw "~";
    }
    s << '(' << t << ',' << l.getValue() << ");"<< std::endl;
    return s;
}

using std::stack;

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) 
{
    if (st.top() == LEX_ID) st.pop();
    i = st.top(); 
    st.pop();
}


template <class T, class T_EL>
void from_pol(T& st, T_EL& i) {
    i = st.top(); st.pop();
}

class Parser
{
    Lexeme curr_lex;
    typelex curr_type;
    typelex ctype;
    int curr_val;
    float curr_rval;
    std::string  curr_strval;
    Scanner scan;

    stack <int> st_int;
    stack <typelex> st_lex;
    stack <int> labels_for_con;
    stack <int> labels_for_break;


    void PROG();
    void DEF1();
    void DEF();
    void PEREM();
    void OPERS();
    void OPER();
    void E();
    void E_OR();
    void E_AND();
    void E_CMP();
    void E_PM();
    void E_MD();
    void T();

    void dec (typelex type);
    void check_id ();
    void check_lab();
    void enumerate_labs();
    void check_op ();
    void check_not ();
    void check_pm (Lexeme op);
    void eq_type ();
    void eq_bool ();
    void check_id_in_read ();

    void gl()
    {
        curr_lex = scan.getLex();
        curr_type = curr_lex.getType();
        curr_val = curr_lex.getValue();     
        std::cout<< curr_lex<< std::endl;   
    }

public:
    Parser (const char *program): scan (program) { }
    void  analyze();
    std::vector <Lexeme> poliz;
};

void Parser::analyze() 
{
    gl();
    PROG();
    if (curr_type != LEX_FIN)
        throw curr_lex;
     int num = 0;
     /*std::cout << std::endl << "POLIZ" << std::endl;
     for (Lexeme l : poliz){
         std::cout << num<< " " << l;
         num++;
     }
     std::cout << "Correct program" << std::endl;*/
}

void Parser::PROG() 
{
    if (curr_type == LEX_PROGRAM) 
    {
        gl();
    }
    else throw curr_lex;

    if(curr_type != LEX_LBRACE)
        throw curr_lex;
    DEF1(); 
    OPERS(); 
    if(curr_type == LEX_RBRACE)
    {
        gl();
    }
    else
    {
        throw curr_lex;    
    }
        
}

void Parser::DEF1() 
{
    while (true) 
    {
        gl();
        if (curr_type == LEX_INT || curr_type == LEX_STRING || curr_type == LEX_REAL) 
        {
            ctype = curr_type;
            DEF();
        }
        else 
            return;
        if (curr_type != LEX_SEMICOLON) 
            throw curr_lex;
    }
}

void Parser::DEF() 
{
    gl();
    PEREM(); // чтение лексемы после переменной
    gl();
    while (curr_type == LEX_COMMA) 
    {
        gl();
        PEREM();
        if (curr_type == LEX_SEMICOLON) 
            break;
        else if (curr_type == LEX_COMMA)
            continue;
        gl();
    }
}


void Parser::PEREM() 
{
    if (curr_type != LEX_ID)
    {
        throw curr_lex;
    }
    st_int.push(curr_val);
    dec(ctype);
    int index = curr_val;
    gl();
    if (curr_type != LEX_ASSIGN)
        return;
    gl();
    if (curr_type == LEX_MINUS || curr_type == LEX_PLUS) 
    {
        gl();
        if (curr_type != LEX_NUM && curr_type != LEX_RNUM) 
            throw curr_lex;
        if (curr_type == LEX_NUM) {
            int val = (curr_type == LEX_PLUS ? curr_val : -curr_val);
            ListOfIdent[index].putValue(val);
        }
        if (curr_type == LEX_RNUM) {
            ListOfIdent[index].putDValue(curr_lex.getDValue());
        }
        return;
    }
    if (curr_type != LEX_STRINGT && curr_type != LEX_NUM && curr_type != LEX_RNUM) 
        throw curr_lex;
    else if (curr_type == LEX_STRINGT) {
        ListOfIdent[index].putString(curr_lex.getString());
    }
    else if (curr_type == LEX_NUM) {
        ListOfIdent[index].putValue(curr_lex.getValue());
    }
    else if (curr_type == LEX_RNUM) {
        ListOfIdent[index].putDValue(curr_lex.getDValue());
    }
}

void Parser::OPERS() 
{
    while (curr_type != LEX_RBRACE) 
    {
        OPER();
    }
}

void Parser::OPER() 
{
    int pl0, pl1, pl2, pl3;
    if (curr_type == LEX_LBRACE) 
    {
        gl();
        OPERS();
        if (curr_type != LEX_RBRACE) 
            throw curr_lex;
        else gl();
    }

    else if (curr_type == LEX_IF)
    {
        gl();
        if (curr_type == LEX_LPAREN) 
            gl();
        else 
            throw curr_lex;
        E();
        if (curr_type == LEX_RPAREN) 
        {
            gl();
        } 
        else 
            throw curr_lex;
        eq_bool();
        pl2 = poliz.size();
        poliz.push_back(Lexeme());
        poliz.push_back(Lexeme(POLIZ_FGO));
        OPER();
        pl3 = poliz.size();
        poliz.push_back(Lexeme());

        poliz.push_back(Lexeme(POLIZ_GO));
        poliz[pl2] = Lexeme(POLIZ_LABEL, poliz.size());
        
        if (curr_type == LEX_ELSE) 
        {
            gl();
            OPER();
            poliz[pl3] = Lexeme(POLIZ_LABEL, poliz.size());
        }
        else
            throw curr_lex;
    } 

    else if (curr_type == LEX_BREAK) { // break
        labels_for_break.push(poliz.size());
        poliz.push_back(Lexeme());
        poliz.push_back(Lexeme(POLIZ_GO));
        gl();
        if (curr_type != LEX_SEMICOLON){
             throw curr_lex;
        }else
             gl();
    }

    else if (curr_type == LEX_WHILE) 
    {
        pl0 = poliz.size();
        gl();
        if (curr_type == LEX_LPAREN) 
            gl();
        else 
            throw curr_lex;
        E();
        if (curr_type == LEX_RPAREN) 
            gl(); 
        else 
            throw curr_lex;
        eq_bool();
        pl1 = poliz.size();
        poliz.push_back(Lexeme());
        poliz.push_back(Lexeme(POLIZ_FGO));
        OPER();
        poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
        poliz.push_back(Lexeme(POLIZ_GO));
        poliz[pl1] = Lexeme(POLIZ_LABEL, poliz.size());
        if (!labels_for_break.empty()) {
            poliz[labels_for_break.top()] = Lexeme(POLIZ_LABEL, poliz.size());
            labels_for_break.pop();
        }
        
    } 
    else if (curr_type == LEX_FOR) {
        gl();
        if (curr_type != LEX_LPAREN)
            throw curr_lex;
        else{

            gl();

            if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRINGT) //проверка на выражение, которого может не быть
                E(); 
            if(curr_type != LEX_SEMICOLON)
                throw curr_lex;
            else{
                pl0 = poliz.size(); //зпоминаем конец 1го выражения
                gl();

                if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRINGT) //проверка на выражение, которого может не быть
                    E();
            }
            if(curr_type != LEX_SEMICOLON)
                throw curr_lex;
            else{
                poliz.push_back(Lexeme());
                poliz.push_back(Lexeme(POLIZ_FGO));
                poliz.push_back(Lexeme());
                poliz.push_back(Lexeme(POLIZ_GO));
                pl1 = poliz.size();
                labels_for_con.push(poliz.size());

                gl();

                if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRINGT) //проверка на выражение, которого может не быть
                    E();
            }

            if(curr_type != LEX_RPAREN)
                throw curr_lex;
            else{
                poliz.push_back(Lexeme(POLIZ_LABEL, pl0));
                poliz.push_back(Lexeme(POLIZ_GO));
                poliz[pl1 - 2] = Lexeme(POLIZ_LABEL, poliz.size());
                gl();
                OPER();
                poliz.push_back(Lexeme(POLIZ_LABEL, pl1));
                poliz.push_back(Lexeme(POLIZ_GO));
                poliz[pl1 - 4] = Lexeme(POLIZ_LABEL, poliz.size());

                
            }

        }
        if (!labels_for_break.empty()) {
            poliz[labels_for_break.top()] = Lexeme(POLIZ_LABEL, poliz.size());
            labels_for_break.pop();
        }
    }
    else if (curr_type == LEX_READ) 
    {
        gl();
        if (curr_type == LEX_LPAREN) 
        {
            gl();
            if (curr_type == LEX_ID) 
            {
                check_id_in_read();
                poliz.push_back(Lexeme(POLIZ_ADDRESS, curr_val));
                gl();
            }
            else
                throw curr_lex;
            if (curr_type == LEX_RPAREN) 
            {
                gl();
                poliz.push_back(Lexeme(LEX_READ));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;

        if (curr_type == LEX_SEMICOLON) 
            gl();
        else throw curr_lex;
    }

    else if (curr_type == LEX_WRITE) 
    {
        gl();
        if (curr_type == LEX_LPAREN) 
        {
            typelex t;
            gl();
            E();
            from_st(st_lex, t);
            while (curr_type == LEX_COMMA) 
            {
                gl();
                E();
                from_st(st_lex, t);
            }
            if (curr_type == LEX_RPAREN)
            {
                gl();
                poliz.push_back(Lexeme(LEX_WRITE));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;

        if (curr_type == LEX_SEMICOLON)
            gl();
        else 
            throw curr_lex;
    }

    else if (curr_type == LEX_ID) 
    {
        check_id();
        poliz.push_back(Lexeme(POLIZ_ADDRESS, curr_val));
        gl();
        if (curr_type == LEX_ASSIGN) 
        {
            gl();
            E();
            eq_type();
            poliz.push_back(Lexeme(LEX_ASSIGN));
            poliz.push_back(Lexeme(LEX_SEMICOLON));
        }
        else if (curr_type == LEX_COLON) 
        {
            gl();
            OPER();
            return;
        }
        else
            throw curr_lex;
        gl();
    } 

    else if(curr_type == LEX_SEMICOLON)
    {
        gl();  
    }   
    else 
    {
        E();
        if (curr_type != LEX_SEMICOLON)
            throw curr_lex;
        else 
        {
            gl();
            //poliz.push_back(Lexeme(LEX_SEMICOLON));
        }  
    }
}


void Parser::E() {

    E_OR();
    while (curr_type == LEX_ASSIGN)
    {
        int pl1 = poliz.size() - 1;
        if(st_lex.top() == LEX_ID) poliz[pl1] = Lexeme(POLIZ_ADDRESS, poliz[pl1].getValue());
        gl();
        E();
        eq_type();
        poliz.push_back(Lexeme(LEX_ASSIGN));
    }
}

void Parser::E_OR() 
{
    E_AND();
    while (curr_type == LEX_OR) 
    {
        st_lex.push(curr_type);
        gl();
        E_AND();
        check_op();
    }
}

void Parser::E_AND() 
{
    E_CMP();
    while (curr_type == LEX_AND) 
    {
        st_lex.push(curr_type);
        gl();
        E_CMP();
        check_op();
    }
}

void Parser::E_CMP() 
{
    E_PM();
    while (curr_type == LEX_EQ || curr_type == LEX_LSS || curr_type == LEX_GTR || curr_type == LEX_LTSEQ || curr_type == LEX_GTSEQ || curr_type == LEX_NEQ)
    {
        st_lex.push(curr_type);
        gl();
        E_PM();
        check_op();
    }
}

void Parser::E_PM() 
{
    E_MD();
    while (curr_type == LEX_PLUS || curr_type == LEX_MINUS)
    {
        st_lex.push(curr_type);
        gl();
        E_MD();
        check_op();
    }
}

void Parser::E_MD() 
{
    T();
    while (curr_type == LEX_MUL || curr_type == LEX_DIV)
    {
        st_lex.push(curr_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T() 
{
    if (curr_type == LEX_ID) 
    {
        check_id();
        poliz.push_back(Lexeme(LEX_ID, curr_val));
        gl();
    }
    else if (curr_type == LEX_NUM )
    {  
        st_lex.push(LEX_INT);          
        poliz.push_back(curr_lex);
        gl();
    }
    else if(curr_type == LEX_STRINGT)
    {
        st_lex.push(LEX_STRING);          
        poliz.push_back(curr_lex);
        gl();
    }
    else if (curr_type == LEX_RNUM)
    {
        st_lex.push(LEX_REAL);          
        poliz.push_back(curr_lex);
        gl();
    }
    else if (curr_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lexeme(LEX_TRUE, 1));
        gl();
    }
    else if (curr_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lexeme(LEX_FALSE, 0));
        gl();
    }
    else if (curr_type == LEX_NOT) 
    {
        gl();
        T();
        check_not();
    }
    else if (curr_type == LEX_PLUS || curr_type == LEX_MINUS) 
    {
        int pl = poliz.size();
        poliz.push_back(Lexeme(LEX_INT, 0));
        gl();
        T();
        typelex t;
        from_st(st_lex, t);
        if (t != LEX_INT && t != LEX_REAL) 
        {
            std::cout<< "Unar operation"<< std::endl;
        }
        if (t == LEX_REAL) poliz[pl] = Lexeme(LEX_REAL, 0);
        if (t == LEX_REAL) poliz[pl] = Lexeme(LEX_REAL, 0);
        st_lex.push(t);
        poliz.push_back(Lexeme(LEX_MINUS));

    }
    else if (curr_type == LEX_LPAREN)
    {
        gl();
        E();
        if (curr_type == LEX_RPAREN)
            gl();
        else
            throw curr_lex;
    }
    else
    {
        throw curr_lex;
    }
        
}

void Parser::dec (typelex type)
{
    int i;
    while ( !st_int.empty () ) 
    {
        from_st (st_int, i);
        if (ListOfIdent[i].getDeclare() ) 
        {
            std::cout<< "Declared twice"<< ListOfIdent[i].getName()<< std::endl;
            throw '~';
        }
        else 
        {
            ListOfIdent[i].putDeclare();
            ListOfIdent[i].putType(type);
        }
    }
}

void Parser::check_id () 
{
    if (ListOfIdent[curr_val].getDeclare())
    {
        st_lex.push (ListOfIdent[curr_val].getType());
    }
    else 
        {
            std::cout << "Not declared " <<  ListOfIdent[curr_val].getName()<< std::endl;
            throw '~'; 
        }
}
 
void Parser::check_op() {
    typelex t1, t2, op;

    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);

    if (op == LEX_PLUS) 
    {
        if (t1 == t2 && t1 == LEX_STRING) 
            st_lex.push(LEX_STRING);
        else if (t1 == t2 && t1 == LEX_INT) 
            st_lex.push(LEX_INT);
        else if (t1 == t2 && t1 == LEX_REAL) 
            st_lex.push(LEX_REAL);
        else if ((t1 == LEX_REAL && t2 == LEX_INT) || (t1 == LEX_INT && t2 == LEX_REAL)) 
            st_lex.push(LEX_REAL);
        else 
        {
            std::cout<<"Error in +"<<std::endl;
            throw '~';
        }
    }
    else if (op == LEX_MUL || op == LEX_DIV || op == LEX_MINUS) 
    {
        if (t1 == t2 && t1 == LEX_INT) 
            st_lex.push(LEX_INT);
        else if (t1 == t2 && t1 == LEX_REAL) 
            st_lex.push(LEX_REAL);
        else if ((t1 == LEX_REAL && t2 == LEX_INT) || (t1 == LEX_INT && t2 == LEX_REAL)) 
            st_lex.push(LEX_REAL);

    }
    else if (op == LEX_GTR || op == LEX_LSS || op == LEX_EQ || op == LEX_NEQ) 
    {
        if (t1 == t2 && t1 == LEX_STRING) 
            st_lex.push(LEX_BOOL);
        else if (t1 == t2 && t1 == LEX_INT) 
            st_lex.push(LEX_BOOL);
        else if (t1 == t2 && t1 == LEX_REAL) 
            st_lex.push(LEX_BOOL);
        else if ((t1 == LEX_REAL && t2 == LEX_INT) || (t1 == LEX_INT && t2 == LEX_REAL)) 
            st_lex.push(LEX_BOOL);
        else 
        {
            std::cout<<"Error in one of > < == !="<<std::endl;
            throw '~';
        }
    }
    else if (op == LEX_GTSEQ || op == LEX_LTSEQ) 
    {
        if (t1 == t2 && t1 == LEX_INT) 
            st_lex.push(LEX_BOOL);
        else if (t1 == t2 && t2 == LEX_REAL) 
            st_lex.push(LEX_BOOL);
        else if ((t1 == LEX_REAL && t2 == LEX_INT) || (t1 == LEX_INT && t2 == LEX_REAL)) 
            st_lex.push(LEX_BOOL);
        else 
        {
            std::cout<<"Error in one of >= <="<<std::endl;
            throw '~';
        }
    }
    else if (op == LEX_AND || op == LEX_OR) 
    {
        if (t1 == LEX_BOOL && t2 == LEX_BOOL) 
            st_lex.push(LEX_BOOL);
        else 
        {
            std::cout<<"Error in one of AND OR"<<std::endl;
            throw '~';
        } 
    }
    else 
    {
        std::cout<<"Wrong operands"<<std::endl;
        throw '~';
    } 
    poliz.push_back(Lexeme(op));
}
 
void Parser::check_not () {
    if (st_lex.top() !=  LEX_BOOL)
        {
            std::cout << "not boolean type in not" << std::endl;
            throw '~';  
        }
    else  
        poliz.push_back (Lexeme (LEX_NOT));
}

void Parser::check_pm ( Lexeme op) {
    if (st_lex.top() != LEX_INT)
        {
            std::cout << "not int type in unary +-" <<  std::endl;
            throw '~';  
        }
    else  
        poliz.push_back ( op );
}
 
void Parser::eq_type () {
    typelex t;
    from_st ( st_lex, t );
    if (st_lex.top () != t)
        {
            std::cout << "wrong types in assign" << std::endl;
            throw '~';  
        }
    st_lex.pop();
}
 
void Parser::eq_bool () {
    if ( st_lex.top () !=  LEX_BOOL )
        {
            std::cout << "not boolean expression" << std::endl;
            throw '~';  
        }
    st_lex.pop ();
  }
 
void Parser::check_id_in_read () {
    if ( !ListOfIdent[curr_val].getDeclare() )
        {
            std::cout << "not declared" << std::endl;
            throw '~';  
        };
}

template <class T, class T_EL>
void from_poliz ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Executer{
    public:
    void execute(std::vector<Lexeme>& poliz){
        Lexeme pc_el, o1, o2;
        stack <Lexeme> args;
        stack <Lexeme> wr_stack;
        int i, index = 0, size = poliz.size();
        int a = 0;
        double d = 0;
        std::string s = "";
        std::string l;
        while (index < size){
            pc_el = poliz[index];
            /*std::cout<<pc_el<<' '<<std::endl;*/
            switch(pc_el.getType()){
                case LEX_NUM: case LEX_RNUM: case LEX_STRINGT: case POLIZ_ADDRESS: case POLIZ_LABEL:
                    args.push(pc_el);
                    break;
                case LEX_ID:
                    i = pc_el.getValue();
                    
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
                    if(o1.getValue()) { 
                        if (o1.getType() == LEX_NUM ){
                            if(o2.getType() == LEX_RNUM){
                                args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() / o1.getValue()));
                            }else{
                                args.push(Lexeme(LEX_NUM, o2.getValue() / o1.getValue()));
                            }
                        }else if (o2.getType() == LEX_RNUM){
                            args.push(Lexeme(LEX_RNUM, 0, o2.getDValue() / o1.getDValue()));
                        }
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
                        if (o1.getType() == LEX_NUM) std::cout << o1.getValue();
                        else if(o1.getType() == LEX_RNUM) std::cout << o1.getDValue();
                        else if (o1.getType() == LEX_STRINGT) std::cout << o1.getString();
                        std::cout << " ";
                    }
                    std::cout << std::endl;
                    break;
                case LEX_READ:
                    from_poliz(args, o1);
                    i = o1.getValue();
                    if (ListOfIdent[i].getType() == LEX_INT){
                        std::cout << "Input int value for " << ListOfIdent[i].getName() << " : ";
                        std::cin >> a;
                    }
                    else if (ListOfIdent[i].getType() == LEX_REAL){
                        std::cout << "Input real value for " << ListOfIdent[i].getName() << " : ";
                        std::cin >> d;
                    }
                    else if (ListOfIdent[i].getType() == LEX_STRING){
                        std::cout << "Input std::string value for " << ListOfIdent[i].getName() << " : ";
                        std::cin >> s;
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
        // std::cout << "Execution finished!!!" << std::endl;
    }
};


class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

int main(int argc, char ** argv )
{
     /*try
     {
         Parser p("Test2.txt");
         p.analyze();
     }
     catch(char c)
     {
          std::std::cout
           << NNN << ": Error with symbol " << c << std::endl;
          return 1;
     }
     catch(Lexeme l)
     {
         std::std::cout
          << NNN << ": Error with lexeme " << l << std::endl;
         return 1;
     }*/
 

    const char * s = "Test2.txt";
    if(argc > 1)s = argv[1];
    try {
        Interpretator I (s);
        I.interpretation ();
        return 0;
    }
    catch ( char c ) {
        std::cout << "unexpected symbol " << c << std::endl;
        return 1;
    }
    catch ( Lexeme l ) {
        std::cout
         << "unexpected lexeme" << l << std::endl;
        return 1;
    }
    catch ( const char *source ) {
        std::cout << source << std::endl;
        return 1;
    }
}


