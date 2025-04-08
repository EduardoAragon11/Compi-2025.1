#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Token {
public:
    enum Type { LPAREN=0, RPAREN, PLUS, MINUS, MULT, DIV, POW, NUM, ERR, END, ID, SIN, COS, LOG, PI, E, SEMICOLON, EQUAL, ASSIGN, GREATER, LESS, NOTEQUAL, TRUE, FALSE, BIN, OCT, HEX, PRINT, LESS_EQUAL, GREATER_EQUAL};
    static const char* token_names[30];
    Type type;
    string lexema;
    int line;
    Token(Type);
    Token(Type, char c);
    Token(Type, const string source);
    Token(Type, int line);
    Token(Type, char c, int line);
    Token(Type, const string source, int line);
};

const char* Token::token_names[30] = { "LPAREN", "RPAREN", "PLUS", "MINUS", "MULT", "DIV", "POW", "NUM", "ERR", "END", "ID", "SIN", "COS", "LOG", "PI", "E", "SEMICOLON", "EQUAL", "ASSIGN", "GREATER", "LESS", "NOTEQUAL", "TRUE", "FALSE", "BIN", "OCT", "HEX", "PRINT", "LESS_EQUAL", "GREATER_EQUAL" };

Token::Token(Type type):type(type) { lexema = ""; line = 0; }
Token::Token(Type type, char c):type(type) { lexema = c; line = 0; }
Token::Token(Type type, const string source):type(type) { lexema = source; line = 0; }
Token::Token(Type type, int line):type(type), line(line) { lexema = ""; }
Token::Token(Type type, char c, int line):type(type), line(line) { lexema = c; }
Token::Token(Type type, const string source, int line):type(type), line(line) { lexema = source; }

std::ostream& operator << ( std::ostream& outs, const Token & tok ) {
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}

class Scanner {
public:
    Scanner(const char* in_s);
    Token* nextToken();
    Token* nextTokenWithLine();
    ~Scanner();
    void printTokenTable();
private:
    string input;
    int first, current;
    int currentLine;
    vector<Token*> tokensTable;
    char nextChar();
    void rollBack();
    void startLexema();
    string getLexema();
    bool isNumberChar(char c);
};

Scanner::Scanner(const char* s):input(s),first(0),current(0),currentLine(1) { }

bool Scanner::isNumberChar(char c) {
    return isdigit(c) || c == '.';
}

Token* Scanner::nextToken() {
    char c;
    startLexema();
    
    while (true) {
        c = nextChar();
        
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            startLexema();
            continue;
        }
        
        if (c == '\0') {
            if (first == current) {
                return new Token(Token::END);
            } else {
                return new Token(Token::ERR, "Unexpected end of input");
            }
        }
        
        if (c == '(') return new Token(Token::LPAREN);
        if (c == ')') return new Token(Token::RPAREN);
        if (c == '+') return new Token(Token::PLUS, c);
        if (c == '-') return new Token(Token::MINUS, c);
        if (c == ';') return new Token(Token::SEMICOLON, c);
        if (c == '#'){
            while(c != '\n'){
                c = nextChar();
            }
            continue;
        }

        if (c == '*') {
            char next = nextChar();
            if (next == '*') {
                return new Token(Token::POW, "**");
            } else {
                rollBack();
                return new Token(Token::MULT, '*');
            }
        }
        
        if (c == '=') {
            char next = nextChar();
            if(next == '=') {
                return new Token(Token::EQUAL, "==");
            }
            else {
                current--;
                return new Token(Token::ASSIGN, '=');
            }
        }

        if (c == '>') {
            char next = nextChar();
            if(next == '=') {
                return new Token(Token::GREATER_EQUAL, ">=");
            }
            else{
                current--;
                return new Token(Token::GREATER, c);
            }
        }
        if (c == '<') {
            char next = nextChar();
            if(next == '=') {
                return new Token(Token::LESS_EQUAL, "<=");
            }
            else{
                current--;
                return new Token(Token::GREATER, c);
            }
        }
        if (c == '!') {
            char next = nextChar();
            if(next == '=') {
                return new Token(Token::NOTEQUAL, "!=");
            }
            current--;
        }

        if (c == '/') return new Token(Token::DIV, c);
        
        if (isalpha(c)) {
            string id;
            id += c;
            while ((c = nextChar()) && (isalnum(c))) {
                id += c;
            }
            rollBack();
            
            if (id == "sin") return new Token(Token::SIN);
            if (id == "cos") return new Token(Token::COS);
            if (id == "log") return new Token(Token::LOG);
            if (id == "pi") return new Token(Token::PI);
            if (id == "e") return new Token(Token::E);
            if (id == "True") return new Token(Token::TRUE);
            if (id == "False") return new Token(Token::FALSE);
            if (id == "Print") return new Token(Token::PRINT);
            return new Token(Token::ID, id);
        }
        
        if (isdigit(c)) {
            string num;
            num += c;
            if(c == '0'){
                char next = nextChar();
                if(next == 'b'){
                    num+=next;
                    next = nextChar();
                    while('0' <= next && next <= '1'){ // digit
                        //num += next;

                        num += next;
                        next = nextChar();
                    }
                    rollBack();
                    return new Token(Token::BIN, num);
                }
                if(next == 'o'){ // octadecimal
                    num += next;
                    next = nextChar();
                    while('0'<= next && next <= '7'){
                        num += next;
                        next = nextChar();
                    }
                    rollBack();
                    return new Token(Token::OCT, num);


                }
                if(next == 'x'){
                    num += next;
                    next = nextChar();
                    while(('0'<= next && next <= '9') || ('A' <= next && 'F'>= next)){
                        num += next;
                        next = nextChar();
                    }
                    rollBack();
                    return new Token(Token::HEX, num);
                }
                else {
                    rollBack();
                    return new Token(Token::NUM, num);
                }
            }

            bool hasDecimal = false;
            
            while ((c = nextChar()) && (isdigit(c) || (c == '.' && !hasDecimal))) {
                if (c == '.') hasDecimal = true;
                num += c;
            }

            if (c == ';') {
                rollBack();
                Token* numToken = new Token(Token::NUM, num);
                startLexema();
                return numToken;
            }            
            rollBack();
            return new Token(Token::NUM, num);
        }
        
        return new Token(Token::ERR, string(1, c));
    }
}
/*
Token* Scanner::nextTokenWithLine() {
    char c;
    startLexema();
    
    while (true) {
        c = nextChar();
        
        if (c == ' ' || c == '\t' || c == '\r') {
            startLexema();
            continue;
        }
        
        if (c == '\n') {
            currentLine++;
            startLexema();
            continue;
        }
        
        if (c == '\0') {
            if (first == current) {
                tokensTable.push_back(new Token(Token::END, currentLine));
                return tokensTable.back();
            } else {
                tokensTable.push_back(new Token(Token::ERR, "Unexpected end of input", currentLine));
                return tokensTable.back();
            }
        }
        
        if (c == '(') {
            tokensTable.push_back(new Token(Token::LPAREN,"(" ,currentLine));
            return tokensTable.back();
        }
        if (c == ')') {
            tokensTable.push_back(new Token(Token::RPAREN, ")",currentLine));
            return tokensTable.back();
        }
        if (c == '+') {
            tokensTable.push_back(new Token(Token::PLUS, c, currentLine));
            return tokensTable.back();
        }
        if (c == '-') {
            tokensTable.push_back(new Token(Token::MINUS, c, currentLine));
            return tokensTable.back();
        }
        if (c == ';') {
            tokensTable.push_back(new Token(Token::SEMICOLON, c, currentLine));
            return tokensTable.back();
        }
        
        if (c == '*') {
            char next = nextChar();
            if (next == '*') {
                tokensTable.push_back(new Token(Token::POW, "**", currentLine));
                return tokensTable.back();
            } else {
                rollBack();
                tokensTable.push_back(new Token(Token::MULT, '*', currentLine));
                return tokensTable.back();
            }
        }
        
        if (c == '/') {
            tokensTable.push_back(new Token(Token::DIV, c, currentLine));
            return tokensTable.back();
        }
        
        if (isalpha(c)) {
            string id;
            id += c;
            while ((c = nextChar()) && (isalnum(c))) {
                id += c;
            }
            rollBack();
            
            if (id == "sin") {
                tokensTable.push_back(new Token(Token::SIN,"sin", currentLine));
                return tokensTable.back();
            }
            if (id == "cos") {
                tokensTable.push_back(new Token(Token::COS,"cos", currentLine));
                return tokensTable.back();
            }
            if (id == "log") {
                tokensTable.push_back(new Token(Token::LOG,"log", currentLine));
                return tokensTable.back();
            }
            if (id == "pi") {
                tokensTable.push_back(new Token(Token::PI,"pi", currentLine));
                return tokensTable.back();
            }
            if (id == "e") {
                tokensTable.push_back(new Token(Token::E,"e", currentLine));
                return tokensTable.back();
            }
            tokensTable.push_back(new Token(Token::ID, id, currentLine));
            return tokensTable.back();
        }
        
        if (isdigit(c)) {
            string num;
            num += c;
            bool hasDecimal = false;
            
            while ((c = nextChar()) && (isdigit(c) || (c == '.' && !hasDecimal))) {
                if (c == '.') hasDecimal = true;
                num += c;
            }
            
            if (c == ';') {
                rollBack();
                tokensTable.push_back(new Token(Token::NUM, num, currentLine));
                return tokensTable.back();
            }            
            rollBack();
            tokensTable.push_back(new Token(Token::NUM, num, currentLine));
            return tokensTable.back();
        }
        
        tokensTable.push_back(new Token(Token::ERR, string(1, c), currentLine));
        return tokensTable.back();
    }
}
*/

void Scanner::printTokenTable() {
    cout << "+------------+------------+-------+" << endl;
    cout << "| Lexema     | Token      | Linea |" << endl;
    cout << "+------------+------------+-------+" << endl;
    
    for (Token* token : tokensTable) {
        if (token->type == Token::END) continue;
        
        cout << "| " << left << setw(10) << token->lexema << " | " 
             << setw(10) << Token::token_names[token->type] << " | "
             << setw(5) << token->line << " |" << endl;
    }
    
    cout << "+------------+------------+-------+" << endl;
}

Scanner::~Scanner() {
    for (Token* token : tokensTable) {
        delete token;
    }
}

char Scanner::nextChar() {
    int c = input[current];
    if (c != '\0') current++;
    return c;
}

void Scanner::rollBack() {
    if (current > 0)
        current--;
}

void Scanner::startLexema() {
    first = current;
}

string Scanner::getLexema() {
    return input.substr(first, current-first);
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <archivo>" << endl;
        return 1;
    }

    
    ifstream file(argv[1]);
    if (!file) {
        cout << "Error al abrir el archivo " << argv[1] << endl;
        return 1;
    }

    string input((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    if (input.empty() || input.back() != ';') {
        cout << "Error: El archivo debe terminar con un punto y coma (;)" << endl;
        return 1;
    }

    // Mostrar salida original
    cout << "=== SALIDA SCANNER ===" << endl;
    Scanner scanner1(input.c_str());
    Token* tk = scanner1.nextToken();
    while (tk->type != Token::END) {
        cout << "next token " << tk << endl;
        delete tk;
        tk = scanner1.nextToken();
    }
    cout << "last token " << tk << endl;
    delete tk;

    return 0;
}