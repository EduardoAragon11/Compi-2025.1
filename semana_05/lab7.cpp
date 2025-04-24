#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <list>
#include <unordered_map>
#include <cmath>
using namespace std;
unordered_map<string,int> memoria;


class Token {
public:
    enum Type {PLUS, MINUS, MUL,DIV, NUM, ERR, PD, PI,
            END,PC,ID,PRINT,ASSIGN, SQRT, POW, FACT, BOOL};
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

enum BinaryOp { PLUS, MINUS,MUL,DIV, POW };

class ASTVisitor;

class Exp {
public:
    virtual ~Exp() = 0;
    static char binopToChar(BinaryOp op);
    virtual int accept(ASTVisitor* v)=0;
};

class SQRTExp: public Exp{
public:
    Exp* exp;
    SQRTExp(Exp* exp);
    int accept(ASTVisitor* v);
    ~SQRTExp();
};

SQRTExp::~SQRTExp(){}


SQRTExp::SQRTExp(Exp* exp) : exp(exp) {}

class FACTExp: public Exp{
public:
    Exp* exp;
    FACTExp(Exp* exp);
    int accept(ASTVisitor* v);
    ~FACTExp();
};

FACTExp::~FACTExp(){}
FACTExp::FACTExp(Exp* exp) : exp(exp) {}


class BinaryExp : public Exp {
public:
    Exp *left, *right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(ASTVisitor* v);
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(ASTVisitor* v);
    ~NumberExp();
};


class IdExp : public Exp {
public:
    string id;
    IdExp(string id);
    int accept(ASTVisitor* v);
    ~IdExp();
};

IdExp::IdExp(string ide) {
    id=ide;
}
IdExp::~IdExp() {}


class Stm {
public:
    virtual int accept(ASTVisitor* v) = 0;
    virtual ~Stm() = 0;
};
Stm::~Stm() {}



class AssignStatement : public Stm {
public:
    string id;
    Exp* rhs;
    AssignStatement(string id, Exp* e);
    int accept(ASTVisitor* v);
    ~AssignStatement();
};
AssignStatement::~AssignStatement() {delete rhs;}
AssignStatement::AssignStatement(string ide, Exp *e) {
    id = ide;
    rhs = e;
}

class PrintStatement : public Stm {
public:
    Exp* e;
    PrintStatement(Exp* e);
    int accept(ASTVisitor* v);
    ~PrintStatement();
};
PrintStatement::~PrintStatement() {delete e;}
PrintStatement::PrintStatement(Exp *ee) {
    e=ee;
}

class Program {
public:
    list<Stm*> slist;
    Program();
    void add(Stm* s);
    int accept(ASTVisitor* v);
    ~Program();
};
void Program::add(Stm *s) {
    slist.push_back(s);
}
Program::~Program() {
    for(auto it:slist) {
        delete it;
    }
}
Program::Program() {}

// AST VISITOR
class ASTVisitor {
public:
    virtual int visit(BinaryExp* e) = 0;
    virtual int visit(NumberExp* e) = 0;
    virtual int visit(IdExp* e) = 0;
    virtual int visit(PrintStatement* e) = 0;
    virtual int visit(AssignStatement* e) = 0;
    virtual int visit(Program* e) = 0;
    virtual int visit(SQRTExp* e) = 0;
    virtual int visit(FACTExp* e)=0;
};

int BinaryExp::accept(ASTVisitor* v) {
    return v->visit(this);
}
int NumberExp::accept(ASTVisitor* v) {
    return v->visit(this);
}
int IdExp::accept(ASTVisitor* v) {
    return v->visit(this);
}
int PrintStatement::accept(ASTVisitor* v) {
    return v->visit(this);
}
int AssignStatement::accept(ASTVisitor* v) {
    return v->visit(this);
}
int Program::accept(ASTVisitor* v) {
    return v->visit(this);
}

int SQRTExp::accept(ASTVisitor* v) {
    return v->visit(this);
}

int FACTExp::accept(ASTVisitor* v){
    return v->visit(this);
}

class ASTEvaluator : public ASTVisitor{
public:
    int eval(Program*);
    int visit(BinaryExp* e);
    int visit(NumberExp* e);
    int visit(IdExp* e) ;
    int visit(PrintStatement* e) ;
    int visit(AssignStatement* e) ;
    int visit(Program* e) ;
    int visit(SQRTExp* e);
    int visit (FACTExp* e);
};

class ASTPrinter : public ASTVisitor{
public:
    void print(Program*);
    int visit(BinaryExp* e);
    int visit(NumberExp* e);
    int visit(IdExp* e) ;
    int visit(PrintStatement* e) ;
    int visit(AssignStatement* e) ;
    int visit(Program* e) ;
    int visit(SQRTExp* e);
    int visit(FACTExp* e);
};

int ASTEvaluator::eval(Program* e) {
    return e->accept(this);
}

int ASTEvaluator::visit(BinaryExp* e) {
    int v1 = e->left->accept(this);
    int v2 = e->right->accept(this);
    int result = 0;
    switch(e->op) {
        case PLUS: result = v1+v2; break;
        case MINUS: result = v1-v2; break;
        case MUL: result = v1 * v2; break;
        case DIV: result = v1 / v2; break;
        case POW: result = pow(v1,v2); break;
    }
    return result;
}
int ASTEvaluator::visit(NumberExp* e) {
    return e->value;
}

///////////////////////////////
///
int ASTEvaluator::visit(IdExp* e) {
    if (memoria.find(e->id) == memoria.end()) {
        cout << "Error in identifier : " << e->id << endl;
        exit(0);
    }
    return memoria[e->id];
}

int ASTEvaluator::visit(SQRTExp* e){
    auto result = e->exp->accept(this);
    result = pow(result,0.5);
    return result;
}

int ASTEvaluator::visit(FACTExp* e){
    auto result = e->exp->accept(this);
    result = tgamma(result+1);
    return result;
}

int ASTEvaluator::visit(PrintStatement* p) {
    cout << p->e->accept(this)<<endl;
    return 0;
}
int ASTEvaluator::visit(AssignStatement* e) {
    memoria[e->id] = e->rhs->accept(this);
    return 0;
}
int ASTEvaluator::visit(Program* e) {
    for(auto it:e->slist) {
        it->accept(this);
    }
    return 0;
}
int ASTPrinter::visit(IdExp* e) {
    cout<< e->id;
    return 0;
}
int ASTPrinter::visit(PrintStatement* p) {
    cout << "print( " ;
    p->e ->accept(this) ;
    cout <<  " )" << endl;
    return 0;
}
int ASTPrinter::visit(AssignStatement* e) {
    cout << e->id << " = " ;
    e->rhs->accept(this);
    cout << endl;
    return 0;
}
int ASTPrinter::visit(Program* e) {
    for (auto it:e->slist) {
        it->accept(this);
    }
    return 0;
}
 //SQRT ASTPRINTER
int ASTPrinter::visit(SQRTExp* e){
    cout<<"sqrt(";
    e->exp->accept(this);
    cout<< ")";
    return 0;
}

int ASTPrinter::visit(FACTExp* e){
    cout<<"!(";
    e->exp->accept(this);
    cout<< ")";
    return 0;
}
///
///
//////////////////////////////


void ASTPrinter::print(Program* e) {
    cout << "expression: "<<endl;
    e->accept(this);
    cout << endl;
    return;
}

int ASTPrinter::visit(BinaryExp* e) {
    e->left->accept(this);
    cout << ' ' << Exp::binopToChar(e->op) << ' ';
    e->right->accept(this);
    return 0;
}
int ASTPrinter::visit(NumberExp* e) {
    cout << e->value;
    return 0;
}

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    Exp* parseExpression();
    Exp* parsePOW();
    Exp* parseTerm();
    Exp* parseFactor();
    Program* parseProgram();
    Stm* parseStatement();
    bool tokenToOp(Token* tk, BinaryOp& op);
public:
    Parser(Scanner* scanner);
    Program* parse();
};


Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = c; }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first,last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok ){
    if (tok.text.empty())
        return outs << tok.type;
    else
        return outs << "TOK" << tok.type<< "(" << tok.text << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

// SCANNER //

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    while (input[current]==' ') current++;
    if (input[current] == '\0') return new Token(Token::END);
    char c  = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (isdigit(input[current]))
            current++;
        token = new Token(Token::NUM,input,first,current-first);
    }
    else if(isalpha(c)) {
        current++;
        while (isalpha(input[current])) {
            current++;
        }

        if ("print" == input.substr(first, current - first)) {
            token = new Token(Token::PRINT, input, first, current - first);
        } else if ("sqrt" == input.substr(first, current - first)) {
            token = new Token(Token::SQRT, input, first, current - first);

        } else if ("true" == input.substr(first, current - first) || "false" == input.substr(first, current - first)) {
        token = new Token(Token::BOOL, input, first, current - first);
        }
        else{
            token = new Token(Token::ID,input, first, current-first);
        }
        }

    else if (strchr("+-*/();=^!", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS,c); break;
            case '-': token = new Token(Token::MINUS,c); break;
            case '*': token = new Token(Token::MUL,c); break;
            case '/': token = new Token(Token::DIV,c); break;
            case '(': token = new Token(Token::PI,c); break;
            case ')': token = new Token(Token::PD,c); break;
            case ';': token = new Token (Token::PC,c);break;
            case '=': token = new Token (Token::ASSIGN,c);break;
            case '^': token = new Token(Token::POW,c); break;
            case '!': token = new Token(Token::FACT,c); break;
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

Program* Parser::parse() {
    current = scanner->nextToken();
    if (check(Token::ERR)) {
        exit(0);
    }
    Program* p = parseProgram();
    if (current->type != Token::END) {
        delete p;
        p = NULL;
    }
    if (current) delete current;
    return p;
}
Program* Parser::parseProgram() {
    Program* p = new Program();
    p->add(parseStatement());
    while(match(Token::PC)) {
        p->add(parseStatement());
    }
    return p;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e;
    if (match(Token::ID)) {
        string lex = previous->text;
        if (!match(Token::ASSIGN)) {
            exit(0);
        }
        s = new AssignStatement(lex, parseExpression());
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            exit(0);
        }
        e = parseExpression();
        if (!match(Token::PD)) {
            exit(0);
        }
        s = new PrintStatement(e);
    } else {
        exit(0);
    }
    return s;
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

    Exp* left = parsePOW();

    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL;
        }
        else if (previous->type == Token::DIV){
            op = DIV;
        }
        Exp* right = parsePOW();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parsePOW(){
    Exp* a;
    Exp* b;
    BinaryOp op;
    a = parseFactor();
    if (match(Token::POW)) {
        b =  parseFactor();
        op=POW;
        a = new BinaryExp(a, b, op);
    }
    return a;

}
Exp* Parser::parseFactor() {
    Exp* e;
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    else if(match(Token::POW)){
        if(!match(Token::PI)){
            cout << "Falta parentesis IZQ" << endl;
            exit(1);
        }
        e = parseExpression();
        if(!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
            exit(1);
        }
        return new NumberExp(stoi(previous->text));
    }
    else if(match(Token::SQRT)) {
        if (!match(Token::PI)) {
            cout << "Falta parentesis IZQ" << endl;
            exit(1);
        }
        e = parseExpression();
        if (!match(Token::PD)) {
            cout << "Falta parentesis derecho" << endl;
            exit(1);
        }
        return new SQRTExp(e);
    }
    else if(match(Token::FACT)){
        if(!match(Token::PI)){
            cout << "Falta parentesis IZQ" << endl;
            exit(1);
        }
        e = parseExpression();
        if(!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
            exit(1);
        }
        return new FACTExp(e);
    }
    else if (match(Token::PI)){
        e = parseExpression();
        if (!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
            exit(1);
        }
        return e;
    }
    cout << "Error: se esperaba un número." << endl;
    exit(0);
}

char Exp::binopToChar(BinaryOp op) {
    char  c=' ';
    switch(op) {
        case PLUS: c = '+'; break;
        case MINUS: c = '-'; break;
        case MUL: c = '*'; break;
        case DIV: c = '/'; break;
        case POW: c = '^'; break;
        default: c = '$';
    }
    return c;
}

// AST //


BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }

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

// AST //

int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }

    Scanner scanner(argv[1]);

    //test_scanner(&scanner);

    Parser parser(&scanner);

    Program *exp = parser.parse();
    ASTPrinter printer;
    printer.print(exp);
    cout << "Ejecucion" <<endl;
    ASTEvaluator  eval;
    eval.eval(exp);
}