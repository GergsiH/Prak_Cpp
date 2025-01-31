
#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <algorithm>

enum typelex
{
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT, LEX_STRING, LEX_REAL,
    LEX_IF, LEX_ELSE, LEX_WHILE, LEX_FOR,
    LEX_READ, LEX_WRITE, LEX_BREAK,
    LEX_NOT, LEX_OR, LEX_AND, 
    LEX_FIN,

    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, 
    LEX_QUOTE, LEX_EQ, LEX_LSS, LEX_GTR,
    LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_LTSEQ, LEX_NEQ, LEX_GTSEQ,
    LEX_LBRACE, LEX_RBRACE, LEX_LPAREN, LEX_RPAREN,

    LEX_ID, LEX_NUM, LEX_RNUM,
};


int NNN = 1;



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
        static const char* TW[], *TD[];
};

const char* Scanner::TW[] = {"", "program", "int", "string", "real", "if", "else", "while","for", "read", "write", "break", "not", "or", "and", NULL};
const char* Scanner::TD[] = { ";", ",", ":", "=", "\"","==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{","}","(", ")", NULL };

Lexeme Scanner::getLex()
{
    enum state {H, ID, NUMB, RNUMB, CMP, EQ, NEQ, QUOTE, COM, SLASH, COM2};
    int d, j;
    int st = 10;
    double r;
    std::string  buf;
    state CS = H;
    while(true)
    {
        gc();
        switch (CS) 
        {
            case H:
                    if ( c == '\n'){
                        NNN++;
                    }
                    else if (c == ' '  || c == '\r' || c == '\t');
                    else if (isalpha(c)) 
                    {
                        buf.push_back (c);
                        CS = ID;
                    }
                    else if (isdigit(c)) 
                    {
                        d = c - '0';
                        CS = NUMB;
                    } 
                    else if (c == '<' || c == '>') 
                    { 
                        buf.push_back(c);
                        CS = CMP; 
                    }
                    else if (c == '!')
                    {
                        buf.push_back(c);
                        CS = NEQ; 
                    }
                    else if (c == '=')
                    {
                        buf.push_back(c);
                        CS = EQ; 
                    }
                    else if (c == '\"') 
                    {
                        CS = QUOTE;
                    }
                    else if (c == EOF)
                    {
                        return Lexeme(LEX_FIN);
                    }
                    else if (c == '/')
                    {
                        buf.push_back(c);
                        CS = SLASH;
                    }
                    else 
                    {
                        buf.push_back (c);
                        if ((j = in(buf, TD)) >= 0)
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
                    if ((j = in(buf, TW)) >= 0) 
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
                if (isdigit (c)) {
                    d = d * 10 + ( c - '0' );
                }else if(c == '.'){
                    r = d;
                    CS = RNUMB;
                }
                else {
                    ungetc (c, f);
                    return Lexeme(LEX_NUM, d);
                }
                break;
            case RNUMB:
                if (isdigit (c)) {
                    r = r + (double)(c - '0') / st;
                    st*=10;
                }
                else {
                    ungetc (c, f);
                    return Lexeme(LEX_NUM, 0, r);
                }
                break;
            case EQ:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in (buf, TD);
                    return Lexeme ((typelex)LEX_EQ, j);
                }
                else 
                {
                    ungetc (c, f);
                    j = in (buf, TD);
                    return Lexeme((typelex)LEX_ASSIGN, j);
                }
                break;
            case NEQ:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in ( buf, TD);
                    return Lexeme (LEX_NEQ, j);
                }
                else
                    throw '!';
                break;
            case CMP:
                if (c == '=') 
                {
                    buf.push_back(c);
                    j = in(buf, TD);
                    return Lexeme((typelex) (j + (int)LEX_FIN + 1), j);
                }
                else
                {
                    ungetc (c, f);
                    j = in (buf, TD);
                    return Lexeme((typelex) (j + (int)LEX_FIN + 1), j);
                }
                break;
            case QUOTE:
                if (c == '\"') 
                {
                    return Lexeme(LEX_STRING, 0, 0, buf);
                }
                buf.push_back(c); 
                continue;  
            case SLASH:
                if (c == '*')
                {
                    buf.pop_back();
                    CS = COM;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                else
                {
                    ungetc (c, f);
                    j = in(buf, TD);
                    return Lexeme((typelex)(j + (int)LEX_FIN), j);
                }
                break;
            case COM:
                if (c == '*' ) 
                {
                    CS  = COM2;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                break;
            case COM2:
                if (c == '/')
                {
                    CS = H;
                }
                else if(feof(f))
                {
                    throw '!';
                }
                else if(c != '*')
                {
                    CS = COM;
                }
                break;           
        }
    }
}

std::ostream & operator<< ( std::ostream &s, Lexeme l ) 
{
    std::string t;
    if (l.getType() <= LEX_OR)
        t = Scanner::TW[l.getType()];
    else if (l.getType() > LEX_FIN && l.getType() <= LEX_RPAREN)
        t = Scanner::TD[l.getType() - LEX_FIN - 1];
    else if (l.getType() == LEX_FIN)
    {
        s<<"FIN"<< std::endl;
        return s;
    }
    else if (l.getType() == LEX_STRING)
    {
        t = "STRING";
        s << '(' << t << ',' << l.getString() << ");" << std::endl;
        return s;
    }
    else if (l.getType() == LEX_NUM)
    {
        t = "NUM";
        s << '(' << t << ',' << l.getValue() << ");" << std::endl;
        return s;
    }
    else if (l.getType() == LEX_RNUM)
    {
        t = "RNUM";
        s << '(' << t << ',' << l.getDValue() << ");" << std::endl;
        return s;
    }
    else if (l.getType() == LEX_ID)
        t = ListOfIdent[l.getValue()].getName();
    else
        throw l;
    s << '(' << t << ',' << l.getValue() << ");" << std::endl;
    return s;
}

class Parser
{
    Lexeme curr_lex;
    typelex curr_type;
    int curr_val;
    Scanner scan;

    void PROG();
    void D1();
    void D();
    void P();
    void OS();
    void O();
    void E();
    void E_OR();
    void E_AND();
    void E_CMP();
    void E_PM();
    void E_MD();
    void T();

    void  gl ()
    {
        //std::cout<<"    "<<std::endl;
        curr_lex = scan.getLex ();
        curr_type = curr_lex.getType ();
        curr_val = curr_lex.getValue ();

        //std::cout << curr_lex;
        
    }

public:
    Parser (const char *program): scan (program) { }
    void  analyze();
};

void Parser::analyze() 
{
    gl();
    PROG();
    if (curr_type != LEX_FIN)
        throw curr_lex;

    std::cout << "Good job, man!" << std::endl;
}

void Parser::PROG() 
{
    if (curr_type == LEX_PROGRAM ) 
    {
        gl();
    }
    else throw curr_lex;

    if(curr_type != LEX_LBRACE)
        throw curr_lex;
    D1(); 
    OS(); 
    if(curr_type == LEX_RBRACE)
    {
        gl();
    }
    else
        throw curr_lex;    
}

void Parser::D1() 
{
    while (true) 
    {
        gl();
        if (curr_type == LEX_INT || curr_type == LEX_STRING || curr_type == LEX_REAL) 
        {
            D();
        }
        else 
            return;
        if (curr_type != LEX_SEMICOLON) 
            throw curr_lex;
    }
}

void Parser::D() 
{
    gl();
    P(); 
    gl();
    while (curr_type == LEX_COMMA) 
    {
        gl();
        P();
        if (curr_type == LEX_SEMICOLON) 
            break;
        else if (curr_type == LEX_COMMA)
            continue;
        gl();
    }
}


void Parser::P() 
{
    if (curr_type != LEX_ID)
    {
        throw curr_lex;
    }
        
    gl();
    if (curr_type != LEX_ASSIGN)
        return;
    gl();
    if (curr_type == LEX_MINUS || curr_type == LEX_PLUS) 
    {
        gl();
        if (curr_type != LEX_NUM && curr_type != LEX_RNUM) 
            throw curr_lex;
        return;
    }
    if (curr_type != LEX_STRING && curr_type != LEX_NUM && curr_type != LEX_RNUM) 
        throw curr_lex;
}

void Parser::OS() 
{
    while (curr_type != LEX_RBRACE) 
    {
        O();
    }
}

void Parser::O() 
{
    if (curr_type == LEX_LBRACE) 
    {
        gl();
        OS();
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
            gl(); 
        else 
            throw curr_lex;
        O();
        
        if (curr_type == LEX_ELSE) 
        {
            gl();
            O();
        }
        else
            throw curr_lex;

    } 

    else if (curr_type == LEX_WHILE) 
    {
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
       
        O();
        
    } 
    else if (curr_type == LEX_READ) 
    {
        gl();
        if (curr_type == LEX_LPAREN) 
        {
            gl();
            if (curr_type == LEX_ID) 
                gl();
            else
                throw curr_lex;
            if (curr_type == LEX_RPAREN) 
                gl();
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
            gl();
            E();
            while (curr_type == LEX_COMMA) 
            {
                gl();
                E();
            }
            if (curr_type == LEX_RPAREN) 
                gl();
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
        gl();
        if (curr_type == LEX_ASSIGN) 
        {
            gl();
            E();
        }
        else if (curr_type == LEX_COLON) 
        {
            gl();
            O();
            return;
        }
        else
            throw curr_lex;
        gl();
    }
    else if (curr_type == LEX_BREAK) { // break
        gl();
        if (curr_type != LEX_SEMICOLON){
             throw curr_lex;
        }else
             gl();
    }
     else if (curr_type == LEX_FOR) {

        gl();
        if (curr_type != LEX_LPAREN)
            throw curr_lex;
        else{
            gl();

            if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRING) //проверка на выражение, которого может не быть
                E();

            if(curr_type != LEX_SEMICOLON)
                throw curr_lex;
            else{
                gl();

                if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRING) //проверка на выражение, которого может не быть
                    E();
            }
            if(curr_type != LEX_SEMICOLON)
                throw curr_lex;
            else{

                gl();

                if(curr_type == LEX_ID || curr_type == LEX_PLUS || curr_type == LEX_MINUS || curr_type == LEX_NOT || curr_type == LEX_NUM || curr_type == LEX_STRING) //проверка на выражение, которого может не быть
                    E();
            }

            if(curr_type != LEX_RPAREN)
                throw curr_lex;
            else{
                gl();
                O();
            }
        }
    }
    else if(curr_type == LEX_SEMICOLON) 
        gl();
    else if (curr_type == LEX_ID || curr_type == LEX_NUM || curr_type == LEX_STRING)
    {
        E();
        if (curr_type != LEX_SEMICOLON)
            throw curr_lex;
        else 
            gl();
    }
    else
        OS();
}


void Parser::E() {
    E_OR();
    while (curr_type == LEX_ASSIGN)
    {
        gl();
        E_OR();
    }
}

void Parser::E_OR() 
{
    E_AND();
    while (curr_type == LEX_OR) 
    {
        gl();
        E_AND();
    }
}

void Parser::E_AND() 
{
    E_CMP();
    while (curr_type == LEX_AND) 
    {
        gl();
        E_CMP();
    }
}

void Parser::E_CMP() 
{
    E_PM();
    while (curr_type == LEX_EQ || curr_type == LEX_LSS || curr_type == LEX_GTR || curr_type == LEX_LTSEQ || curr_type == LEX_GTSEQ || curr_type == LEX_NEQ)
    {
        gl();
        E_PM();
    }
}

void Parser::E_PM() 
{
    E_MD();
    while (curr_type == LEX_PLUS || curr_type == LEX_MINUS)
    {
        gl();
        E_MD();
    }
}

void Parser::E_MD() 
{
    T();
    while (curr_type == LEX_MUL || curr_type == LEX_DIV)
    {
        gl();
        T();
    }
}

void Parser::T() 
{
    if (curr_type == LEX_ID) 
        gl();
    else if (curr_type == LEX_NUM || curr_type == LEX_STRING || curr_type == LEX_RNUM)
        gl();
    else if (curr_type == LEX_NOT || curr_type == LEX_PLUS || curr_type == LEX_MINUS) 
    {
        gl();
        T();
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

int main()
{
    try
    {
        Parser p("Test2.txt");
        p.analyze();
    }
    catch(char c)
    {
        std::cout << NNN << " : "<<"Error with symbol" << c << std::endl;
        return 1;
    }
    catch(Lexeme l)
    {
        std::cout <<  "Error with lexeme" << l << std::endl;
        return 1;
    }
    
    // Scanner scanner("2.txt");
    // while(true)
    // {
    //     try
    //     {
    //         Lexeme l(scanner.getLex());
    //         std::cout << l << std::endl;
    //         if(l.getType() == LEX_FIN)
    //             break;
    //     }
    //     catch(char c)
    //     {
    //         std::cout << "Error with symbol" << c << std::endl;
    //         return 1;
    //     }
    //     catch ( Lexeme l ) 
    //     {
    //         std::cout << "Error with lexeme" << l << std::endl;
    //         return 1;
    //     }
    // }
}

