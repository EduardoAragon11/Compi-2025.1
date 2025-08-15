#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"
#include "labelvisitor.h"

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
    Parser parser(&scanner); 
    try {
        Program* program = parser.parseProgram();     
        string inputFile(argv[1]);
        size_t dotPos = inputFile.find_last_of('.');
        string baseName = (dotPos == string::npos) ? inputFile : inputFile.substr(0, dotPos);
        string outputFilename = baseName + ".s";
        ofstream outfile(outputFilename);
        if (!outfile.is_open()) {
            cerr << "Error al crear el archivo de salida: " << outputFilename << endl;
            return 1;
        }
        cout << "Generando codigo ensamblador en " << outputFilename << endl;
        LabelVisitor labeler;
        labeler.visit(program);
        GenCodeVisitor codigo(outfile);
        codigo.generar(program);

        cout<<"Print visitor:"<<endl;
        OptimizedVisitor optimizedVisitor;
        optimizedVisitor.imprimir(program);

        outfile.close();
        delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }
    return 0;
}