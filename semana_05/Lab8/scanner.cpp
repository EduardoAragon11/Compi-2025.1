#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() && (input[current] == ' ' || input[current] == '\n')) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    } else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);
        if (word == "print") {
            token = new Token(Token::PRINT, word, 0, word.length());
        } 
        else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        }
        else if (word == "then") {
            token = new Token(Token::THEN, word, 0, word.length());
        }
        else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        }
        else if (word == "endif") {
            token = new Token(Token::ENDIF, word, 0, word.length());
        }
        else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    } else if (strchr("+-*/()=;<", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '=':
                current++;
                if(input[current]== '='){ // EQUALS '=='
                    token = new Token(Token::EQUALS,c); break;
                }
                else{
                    current--;
                    token = new Token(Token::ASSIGN, c); break;
                }
            case ';': token = new Token(Token::PC, c); break;
            case '<':
                current++;
                if(input[current] == '='){
                    token = new Token(Token::MENOREQUALS,c); break;

                } else{
                    current--;
                    token = new Token(Token::MENOR, c); break;
                }
            default: 
            cout << "No debería llegar acá" << endl;
            token = new Token(Token::ERR, c);
        }
        current++;
    } else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}