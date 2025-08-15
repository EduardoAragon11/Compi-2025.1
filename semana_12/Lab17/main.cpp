#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner); 
    try {
        Program* program = parser.parseProgram();
        cout << "Parsing exitoso" << endl << endl;
        cout << "Iniciando Visitor:" << endl;

        TypeCheckerVisitor typechecker;
        cout << "Variables:" << endl;
        typechecker.revisar(program);
        cout  << typechecker.cantidad << endl;

        cout<<"PRINT VISITOR"<<endl;
        PrintVisitor printVisitor;
        printVisitor.imprimir(program);
        cout<<endl;
        GenCodeVisitor codigo;
        cout << "CODIGO:" << endl;
        codigo.gencode(program);
        cout  << endl;
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}



