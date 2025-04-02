//copiar para leer files
//ejecutar colocando nombre del archivo
int main(int argc, const char* argv[]) {

    if (argc != 2) {
        cout << "Incorrect number of arguments" << endl;
        exit(1);
    }
    
    ifstream file(argv[1]);
    string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    const char* content_ptr = content.c_str();

    Scanner scanner(content_ptr);
    Token* tk = scanner.nextToken();
    while (tk->type != Token::END) {
        cout << "next token " << tk << endl;
        delete tk;
        tk =  scanner.nextToken();
    }
    cout << "last token " << tk << endl;
    delete tk;

}
