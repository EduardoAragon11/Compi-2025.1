#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

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
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = nullptr;
    if (match(Token::VAR)) {
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        string type = previous->text;
        list<string> ids;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        while (match(Token::COMA)) {
            if (!match(Token::ID)) {
                cout << "Error: se esperaba un identificador después de ','." << endl;
                exit(1);
            }
            ids.push_back(previous->text);
        }
        if (!match(Token::PC)) {
            cout << "Error: se esperaba un ';' al final de la declaración." << endl;
            exit(1);
        }
        vd = new VarDec(type, ids);
    }
    return vd;
}

VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux = parseVarDec();
    while (aux != nullptr) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        sl->add(parseStatement());
    }
    return sl;
}

Body* Parser::parseBody() {
    VarDecList* vdl = parseVarDecList();
    StatementList* sl = parseStatementList();
    return new Body(vdl, sl);
}

Program* Parser::parseProgram() {
    Program* p = new Program();
    p->vardecs = parseVarDecList();
    p->fundecs = parseFunDecList();
    return p;
}

FunDecList* Parser::parseFunDecList() {
    FunDecList* vdl = new FunDecList();
    FunDec* aux = parseFunDec();
    while (aux != nullptr) {
        vdl->add(aux);
        aux = parseFunDec();
    }
    return vdl;
}

FunDec* Parser::parseFunDec() {
    FunDec* vd = nullptr;
    if (match(Token::FUN)) {
        FunDec* fu = new FunDec();
        match(Token::ID);
        fu->tipo = previous->text;
        match(Token::ID);
        fu->nombre = previous->text;
        match(Token::PI);
        while (match(Token::ID)) {
            fu->tipos.push_back(previous->text);
            match(Token::ID);
            fu->parametros.push_back(previous->text);
            if (!match(Token::COMA)) break;
        }
        match(Token::PD);
        fu->cuerpo = parseBody();
        match(Token::ENDFUN);
        vd = fu;
    }
    return vd;
}

Stm* Parser::parseStatement() {
    Stm* s = nullptr;
    Exp* e = nullptr;
    Body* tb = nullptr;
    Body* fb = nullptr;

    if (current == nullptr) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }
    if (match(Token::ID)) {
        string lex = previous->text;
        if (!match(Token::ASSIGN)) {
            cout << "Error: se esperaba un '=' después del identificador." << endl;
            exit(1);
        }
        e = parseCExp();
        s = new AssignStatement(lex, e);
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }
    else if (match(Token::RETURN)) {
        ReturnStatement* rs = new ReturnStatement();
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de 'return'." << endl;
            exit(1);
        }
        if (check(Token::PD)) {
            advance();
            rs->e = nullptr;
        } else {
            rs->e = parseCExp();
            if (!match(Token::PD)) {
                cout << "Error: se esperaba ')' después de la expresión de return." << endl;
                exit(1);
            }
        }
        return rs;
    } else if (match(Token::IF)) {
        e = parseCExp();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (match(Token::ELSE)) {
            fb = parseBody();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'endif' al final de la declaración de if." << endl;
            exit(1);
        }
        s = new IfStatement(e, tb, fb);
    }
    else if (match(Token::WHILE)) {
        e = parseCExp();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        s = new WhileStatement(e, tb);
    }
    else {
        cout << "Error: Se esperaba un identificador, 'print', o estructura válida, pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        } else if (previous->type == Token::LE){
            op = LE_OP;
        } else {
            op = EQ_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op = (previous->type == Token::PLUS) ? PLUS_OP : MINUS_OP;
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op = (previous->type == Token::MUL) ? MUL_OP : DIV_OP;
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    if (match(Token::TRUE)){
        return new BoolExp(1);
    } else if (match(Token::FALSE)){ 
        return new BoolExp(0); 
    } else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    } else if (match(Token::ID)) {
        string nombre = previous->text;
        if (match(Token::PI)) {
            FCallExp* fc = new FCallExp();
            vector<Exp*> lista;
            lista.push_back(parseCExp());
            while (match(Token::COMA)) {
                lista.push_back(parseCExp());
            }
            if (!match(Token::PD)) {
                cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
                exit(1);
            }
            fc->argumentos = lista;
            fc->nombre = nombre;
            return fc;
        } else {
            return new IdentifierExp(nombre);
        }
    } else if (match(Token::PI)){
        Exp* e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta parentesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}