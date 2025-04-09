#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

map<string, int> Memory;

class Token {
public:
    enum Type {PLUS, MINUS, MUL,DIV, NUM, ERR, PD, PI, END, ID, ASSIGN, PC, PRINT, PLUS1};
    Type type;
    string text;
    Token(Type);
    Token(Type, char c);
    Token(Type, const string& source, int first, int last);
};

class Scanner {
private:
    string input;
    int first, current;
public:
    Scanner(const char* in_s);
    Token* nextToken();
    ~Scanner();

};

enum BinaryOp { PLUS, MINUS,MUL,DIV };

class Exp {
public:
    virtual void print() = 0;
    virtual int eval() = 0;
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
};


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    void print();
    int eval();
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    void print();
    int eval();
    ~NumberExp();
};

class IdExp: public Exp {
public:
    string id;
    IdExp(string _id);
    void print();
    int eval();
    ~IdExp();
};

class Stmt {
public:
    virtual ~Stmt() = 0;
    virtual void execute() = 0;
};

class PrintStmt : public Stmt {
public:
    Exp* exp;
    PrintStmt(Exp* e);
    void execute();
    ~PrintStmt();
};

class AssignStmt : public Stmt {
public:
    string id;
    Exp* exp;
    AssignStmt(string _id, Exp* e);
    void execute();
    ~AssignStmt();
};

class Program {
private:
    vector<Stmt*> stmtList;
public:
    Program(vector<Stmt*> list);
    void execute();
    ~Program();
};

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFactor();
    Stmt* parseStmt();
    Program* parseProgram();
    bool tokenToOp(Token* tk, BinaryOp& op);
public:
    Parser(Scanner* scanner);
    Exp* parse();
    Stmt* parse2();
    Program* parse3();
};


Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = c; }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first,last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    if (tok.text.empty()){
        return outs << tok.type;
    }
    else
        return outs << "TOK" << "(" << tok.text << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

// SCANNER //

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    while (input[current]==' ' || input[current] == '\n' || input[current] == '\t') current++;
    if (input[current] == '\0') return new Token(Token::END);
    char c  = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (isdigit(input[current]))
            current++;

        token = new Token(Token::NUM,input,first,current-first);
    } else if (isalpha(c)){
        current++;
        while(isalpha(input[current]) || isdigit(input[current]))
            current++;

        string text = input.substr(first,current-first);
        if(text == "print"){
            token = new Token(Token::PRINT);
        }
        else token = new Token(Token::ID,input,first,current-first);
    } else if (strchr("+-*/()=;'\n'", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS,c); break;
            case '-': token = new Token(Token::MINUS,c); break;
            case '*': token = new Token(Token::MUL,c); break;
            case '/': token = new Token(Token::DIV,c); break;
            case '(': token = new Token(Token::PI,c); break;
            case ')': token = new Token(Token::PD,c); break;
            case '=': token = new Token(Token::ASSIGN,c); break;
            case ';': token = new Token(Token::PC,c); break;
            default: cout << "No deberia llegar aca" << endl;
        }
        current++;
    } else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

Scanner::~Scanner() { }

// PARSER //

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp =current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Parse error, unrecognised character: " << current->text << endl;
            exit(0);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = current = NULL;
    return;
};

Exp* Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        cout << "Error en scanner - caracter invalido" << endl;
        exit(0);
    }
    Exp* exp = parseExpression();
    if (current) delete current;
    return exp;
}

Stmt* Parser::parse2() {
    current = scanner->nextToken();
    if (check(Token::ERR)){
        cout<< "Error en scanner - formula invalida" << endl;
        exit(0);
    }
    Stmt* stmt = parseStmt();
    if(current) delete current;
    return stmt;
}

Program* Parser::parse3() {
    current = scanner -> nextToken();
    if(check(Token::ERR)){
        cout<< "Error en escanner - programa inválido" << endl;
        exit(0);
    }
    Program* program = parseProgram();
    if(current) delete current;
    return program;
}

Exp* Parser::parseExpression() {

    Exp* left = parseTerm();

    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }

    return left;
}

Exp* Parser::parseTerm() {

    Exp* left = parseFactor();

    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL;
        }
        else if (previous->type == Token::DIV){
            op = DIV;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::PI)){
        e = parseExpression();
        if (!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    else if (match(Token::ID)){
        return new IdExp(previous->text);
    }
    cout << "Error: se esperaba un número o una variable." << endl;
    exit(0);
}

Stmt* Parser::parseStmt() {
    if (match(Token::PRINT)){
        if(!match(Token::PI)){
            cout<<"Falta parentesis izquierdo después del print" << endl;
            exit(0);
        }
        Exp* e = parseExpression();
        if(!match(Token::PD)){
            cout<<"Falta parentesis derecho después del print" << endl;
            exit(0);
        }
        return new PrintStmt(e);
    }
    else if(match (Token::ID)){
        string id_value = previous -> text;
        if(!match(Token::ASSIGN)){
            cout<<"Falta símbolo de asignación" << endl;
            exit(0);
        }
        Exp* e = parseExpression();
        return new AssignStmt(id_value, e);
    }
    cout<<" Se esperaba un statement" << endl;
    exit(0);
}

Program* Parser::parseProgram(){
    Stmt* stmt = parseStmt();
    vector<Stmt*> list;
    list.push_back(stmt);
    while(match(Token::PC)){
        stmt = parseStmt();
        list.push_back(stmt);
    }
    return new Program(list);
}

char Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    switch(op) {
        case PLUS: c = '+'; break;
        case MINUS: c = '-'; break;
        case MUL: c = '*'; break;
        case DIV: c = '/'; break;
        default: c = '$';
    }
    return c;
}

// AST //


BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
IdExp::IdExp(string _id):id(_id) {}
PrintStmt::PrintStmt(Exp* e): exp(e) {}
AssignStmt::AssignStmt(string _id, Exp* e): id(_id), exp(e) {}
Program::Program(vector<Stmt*> list): stmtList(list) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
IdExp::~IdExp() { }
PrintStmt::~PrintStmt() { delete exp; }
AssignStmt::~AssignStmt() { delete exp; }
Stmt::~Stmt() {}
Program::~Program() { for(auto x:stmtList) delete x; } // quizas esta mal ono

void BinaryExp::print() {
    left->print();
    char c = binopToChar(this->op);;
    cout << ' ' << c << ' ';
    right->print();
}

void NumberExp::print() {
    cout << value;
}

void IdExp::print(){
    cout << id;
}

int BinaryExp::eval() {
    int result;
    int v1=left->eval();
    int v2=right->eval();
    switch(this->op) {
        case PLUS: result = v1+v2; break;
        case MINUS: result = v1-v2; break;
        case MUL: result = v1*v2; break;
        case DIV: result = v1/v2; break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int NumberExp::eval() {
    return value;
}

int IdExp::eval() {
    return Memory[id];
}

void AssignStmt::execute(){
    Memory[id] = exp->eval();
}

void PrintStmt::execute(){
    cout << exp->eval() <<endl;
}

void Program::execute(){
    for(auto stmt:stmtList){
        stmt->execute();
    }
}

void test_scanner(Scanner * scanner) {
    Token* current;
    current = scanner->nextToken();
    while (current->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - caracter invalido: " << current->text << endl;
            break;
        } else
            cout << current << endl;
        current = scanner->nextToken();
    }
    exit(1);

}

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }

    ifstream file(argv[1]);
    string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    const char* content_ptr = content.c_str();

    Scanner scanner(content_ptr);

    //test_scanner(&scanner);

    Parser parser(&scanner);

    Program* program = parser.parse3();
    program->execute();
    delete program;
}