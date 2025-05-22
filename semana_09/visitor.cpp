#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int IFExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}


int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int StatementList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int Body::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int FCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
int ReturnStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int FunDecList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << "if ";
    stm->condition->accept(this);
    cout << " then" << endl;
    stm->then->accept(this);
    if(stm->els){
        cout << "else" << endl;
        stm->els->accept(this);
    }
    cout << "endif";
}

void PrintVisitor::imprimir(Program* program){
    program->vardecs->accept(this);
    cout << endl;
    program->fundecs->accept(this);
};


int PrintVisitor::visit(IFExp* pepito) {
    cout<< "ifexp(";
    pepito->cond->accept(this);
    cout<< ",";
    pepito->left->accept(this);
    cout<< ",";
    pepito->right->accept(this);
    cout << ")" ;
    return 0;
}

void PrintVisitor::visit(WhileStatement* stm){
    cout << "while ";
    stm->condition->accept(this);
    cout << " do" << endl;
    stm->b->accept(this);
    cout << "endwhile";
}


void PrintVisitor::visit(VarDec* stm){
    cout << "var ";
    cout << stm->type;
    cout << " ";
    for(auto i: stm->vars){
        cout << i;
        if(i != stm->vars.back()) cout << ", ";
    }
    cout << ";";
}

void PrintVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body* stm){
    stm->vardecs->accept(this);
    cout << endl;
    stm->slist->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch(exp->op) {
        case PLUS_OP: result = v1 + v2; break;
        case MINUS_OP: result = v1 - v2; break;
        case MUL_OP: result = v1 * v2; break;
        case DIV_OP:
            if(v2 != 0) result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case LT_OP: result = v1 < v2; break;
        case LE_OP: result = v1 <= v2; break;
        case EQ_OP: result = v1 == v2; break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}
int EVALVisitor::visit(BoolExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(IdentifierExp* exp) {
    if(env.check(exp->name)){
        return env.lookup(exp->name);
    }
    else{
        cout << "Variable no declarada: " << exp->name << endl;
        return 0;
    }
}

void EVALVisitor::visit(AssignStatement* stm) {
    if(!env.check(stm->id)){
        cout << "Variable no declarada: " << stm->id << endl;
        return;
    }

    int value = stm->rhs->accept(this);
    env.update(stm->id, value);
}

void EVALVisitor::visit(PrintStatement* stm) {
    cout << stm->e->accept(this);
    cout<< endl;
}
void EVALVisitor::ejecutar(Program* p){
    env.add_level();
    p-> vardecs ->accept(this);
    p-> fundecs ->accept(this);
    if (!fdecs.count("main")) {
        exit(0);
    }

    FunDec* main_dec = fdecs["main"];
    retcall = false;
    main_dec->cuerpo->accept(this);

    if (!retcall) {
        cout << "Error: Funcion main no ejecuto RETURN" << endl;
        exit(0);
    }
};

void EVALVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this)){
        stm->then->accept(this);
    }
    else{
        if(stm->els) stm->els->accept(this);
    }
}
void EVALVisitor::visit(WhileStatement* stm) {
    while(stm->condition->accept(this)){
        stm->b->accept(this);
    }
}


int EVALVisitor::visit(IFExp* pepito) {
    if(pepito->cond->accept(this)){
        return pepito->left->accept(this);
    }
    else{
        return pepito->right->accept(this);
    }
}

void EVALVisitor::visit(VarDec* stm){
    list<string>::iterator it;
    for(it = stm->vars.begin(); it != stm->vars.end(); it++){
        env.add_var(*it, stm->type);
    }
}

void EVALVisitor::visit(VarDecList* stm){
    list<VarDec*>::iterator it;
    for(it = stm->vardecs.begin(); it != stm->vardecs.end(); it++){
        (*it)->accept(this);
    }
}

void EVALVisitor::visit(StatementList* stm){
    list<Stm*>::iterator it;
    for(it = stm->stms.begin(); it != stm->stms.end(); it++){
        (*it)->accept(this);
    }
}

void EVALVisitor::visit(Body* b){
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}

///////////////////////////////////////////////////////////////////////////////////

int PrintVisitor::visit(FCallExp* exp) {
    cout << exp->nombre << "(";
    for (auto it = exp->argumentos.begin(); it != exp->argumentos.end(); ++it) {
        (*it)->accept(this);
        if (next(it) != exp->argumentos.end()) cout << ",";
    }
    cout << ")";
    return 0;
}

void PrintVisitor::visit(FunDec* e) {

    cout << "fun " << e->tipo << " " << e->nombre << "(";
    /*
    for(auto i: e->parametros){
        cout << i << ",";
    }
     */

    for(auto it = e->parametros.begin(), it2 = e->tipos.begin(); it!= e->parametros.end(), it2 != e->tipos.end(); ++it, ++it2){
        cout<<(*it2)<<" ";
        cout<<(*it);
        if (next(it) != e->parametros.end()) cout << ",";
    }

    cout << ")" << endl;
    e->cuerpo->accept(this);
    cout << "endfun";
}
void PrintVisitor::visit(FunDecList* e) {
    for(auto i: e->Fundecs){
        i->accept(this);
        cout << endl;
    }
}
void PrintVisitor::visit(ReturnStatement* e){
    cout << "return (";
    e->e->accept(this);
    cout << ");";
}

// EVAL VISITOR

int EVALVisitor::visit(FCallExp* f) {
    env.add_level();
    retcall = false;
    FunDec* func= fdecs[f->nombre];
    /*
    for(auto it = f->argumentos.begin(), auto it2 = func->parametros.begin(); it!= f->argumentos.end(), it2 !=func->parametros.end(); ++it, ++it2){
        cout<<(*it2)<<" ";
        cout<<(*it);
        if (next(it) != e->parametros.end()) cout << ",";
    }
     */
    auto it = f->argumentos.begin();
    auto it2 = func->parametros.begin();
    auto it_tipos = func->tipos.begin();

    while (it != f->argumentos.end() && it2 != func->parametros.end() && it_tipos != func->tipos.end()){
        env.add_var(*(it2), (*(it))->accept(this) , *(it_tipos));
        env.update(*(it2),(*(it))->accept(this));
        it++;
        it2++;
        it_tipos++;
    }

    func->cuerpo->accept(this);
    env.remove_level();

    return retval;


}
void EVALVisitor::visit(FunDec* e) {
    fdecs[e->nombre]= e;
}
void EVALVisitor::visit(FunDecList* e) {
    for(auto i : e->Fundecs){
        i->accept(this);
    }
}
void EVALVisitor::visit(ReturnStatement* e){
    retcall = true;
    retval = e->e->accept(this);
}