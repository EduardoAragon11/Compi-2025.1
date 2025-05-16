#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
ImpValue BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
ImpValue IFExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
ImpValue NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
ImpValue BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
ImpValue IdentifierExp::accept(Visitor* visitor) {
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
int ForStatement::accept(Visitor* visitor) {
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

ImpValue UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

ImpValue PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return ImpValue();
}

ImpValue PrintVisitor::visit(UnaryExp* exp){
    if(exp->op == NOTHING_OP){
    }else{
        cout << ' ' << Exp::unaryToChar(exp->op) << ' ';
    }

    exp->exp->accept(this);
    return ImpValue();
}

ImpValue PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return ImpValue();
}

ImpValue PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return ImpValue();
}

ImpValue PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return ImpValue();
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout<< string(iden, '\t');

    cout << stm->id << " = ";
    stm->rhs->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout<< string(iden, '\t');

    cout << "print(";
    stm->e->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout<< string(iden, '\t');

    cout << "if ";
    stm->condition->accept(this);
    iden += 1;

    cout << " then" << endl;
    stm->then->accept(this);
    iden -=1;

    if(stm->els){
        cout<< string(iden, '\t');
        cout << "else" << endl;
        iden +=1;
        stm->els->accept(this);
        iden -=1;
    }

    cout<< string(iden, '\t');
    cout << "endif";
}

void PrintVisitor::imprimir(Program* program){
    program->body->accept(this);
};


ImpValue PrintVisitor::visit(IFExp* pepito) {
    cout<< string(iden, '\t');

    cout<< "ifexp(";
    pepito->cond->accept(this);
    cout<< ",";
    pepito->left->accept(this);
    cout<< ",";
    pepito->right->accept(this);
    cout << ")" ;
    return ImpValue();
}

void PrintVisitor::visit(WhileStatement* stm){
    cout<< string(iden, '\t');
    cout << "while ";
    stm->condition->accept(this);
    iden +=1;
    cout<< string(iden, '\t');
    cout << " do" << endl;
    stm->b->accept(this);
    iden-=1;
    cout << string(iden,'\t');
    cout << "endwhile";
}

void PrintVisitor::visit(ForStatement* stm){
    cout << string(iden, '\t') << "for " << stm->id << " in range(";
    stm->start->accept(this);
    cout << ", ";
    stm->end->accept(this);
    cout << ", ";
    stm->step->accept(this);
    cout << ")" << endl;
    iden++;
    stm->b->accept(this);
    iden--;
    cout << string(iden, '\t') << "endfor";
}

void PrintVisitor::visit(VarDec* stm){
    cout<<string(iden,'\t');

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
    stm->slist->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////
ImpValue EVALVisitor::visit(BinaryExp* exp) {
    ImpValue v1 = exp->left->accept(this);
    ImpValue v2 = exp->right->accept(this);

    ImpValue result;
    if(v1.type == "int" && v2.type == "int") {
        result.type = "int";
    } else result.type = "bool";

    switch(exp->op) {
        case PLUS_OP: result.int_value = v1.int_value + v2.int_value; break;
        case MINUS_OP: result.int_value = v1.int_value - v2.int_value; break;
        case MUL_OP: result.int_value = v1.int_value * v2.int_value; break;
        case DIV_OP: result.int_value = v1.int_value / v2.int_value; break;

        case LT_OP: result.bool_value = v1.int_value < v2.int_value; break;
        case LE_OP: result.bool_value = v1.int_value <= v2.int_value; break;
        case EQ_OP: result.bool_value = v1.int_value == v2.int_value; break;

        case AND_OP: result.bool_value = v1.bool_value && v2.bool_value; break;
        case OR_OP: result.bool_value = v1.bool_value || v2.bool_value; break;

        default:
            cout << "Operador desconocido" << endl;
    }

    return result;
}

ImpValue  EVALVisitor::visit(NumberExp* exp) {
    auto i = exp->value;
    return ImpValue("int",i,false);
}

ImpValue EVALVisitor::visit(BoolExp* exp) {
    auto i = exp->value;
    return ImpValue("bool",0,i);
}

ImpValue EVALVisitor::visit(UnaryExp* exp) {
    ImpValue e = exp->exp->accept(this);
    if (exp->op == NOT_OP) {
        e.bool_value = !e.bool_value;
        return e;
    }
    return e;
}

ImpValue EVALVisitor::visit(IdentifierExp* exp) {
    string i = env.lookup_type(exp->name);
    int x = env.lookup(exp->name);
    if(i == "int") {
        return ImpValue("int", x, false);
    }
    else if(i == "bool") {
        return ImpValue("bool",0,x);
    }
    else {
        cout << "Error: variable no definida" << endl;
        exit(0);
    }
}

void EVALVisitor::visit(AssignStatement* stm) {
    ImpValue x = stm->rhs->accept(this);
    if(x.type == "int"){
        env.update(stm->id, x.int_value);
    }
    else if (x.type == "bool") {
        env.update(stm->id, x.bool_value);
    }
    else {
        cout << "Error: Type de assign invalido" << endl;
    }
}

void EVALVisitor::visit(PrintStatement* stm) {
    ImpValue i = stm->e->accept(this);

    if(i.type == "int") cout << i.int_value<<endl;
    else if(i.type == "bool"){
        if(i.bool_value) cout << "true"<<endl;
        else cout << "false"<<endl;
    }
}



void EVALVisitor::ejecutar(Program* program){
    env.add_level();
    program->body->accept(this);
    env.remove_level();
}

void EVALVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this).bool_value) {
        stm->then->accept(this);
    } else if(stm->els){
        stm->els->accept(this);
    }
}

void EVALVisitor::visit(WhileStatement* stm) {
    while(stm->condition->accept(this).bool_value) {
        stm->b->accept(this);
    }
}

ImpValue EVALVisitor::visit(IFExp* pepito) {
    if(pepito->cond->accept(this).bool_value) {
        return pepito->left->accept(this);
    } else {
        return pepito->right->accept(this);
    }
}

void EVALVisitor::visit(ForStatement* stm){
    string var = stm->id;
    ImpValue start = stm->start->accept(this);
    ImpValue end = stm->end->accept(this);
    ImpValue step = stm->step->accept(this);

    for(int i = start.int_value; i <= end.int_value; i += step.int_value){
        env.update(var, i);
        stm->b->accept(this);
    }

}

void EVALVisitor::visit(VarDec* stm){
    for(auto i : stm->vars){
        env.add_var(i, stm->type);
    }
}

void EVALVisitor::visit(VarDecList* stm){
    for(auto i: stm->vardecs){
        i->accept(this);
    }
}

void EVALVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
    }
}

void EVALVisitor::visit(Body* b){
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}

///////////////////////////////////////////////////////////////////////////////////

//0 = undefined
//1 = int
//2 = bool

void TypeVisitor::check(Program* program){
    program->body->accept(this);

}

ImpValue TypeVisitor::visit(BinaryExp* exp) {
    ImpValue v1 = exp->left->accept(this);
    ImpValue v2 = exp->right->accept(this);

    switch (exp->op) {
        case PLUS_OP:
        case MINUS_OP:
        case MUL_OP:
        case DIV_OP:
            if (v1.type== "int" && v2.type == "int") return ImpValue("int");
            else {
                cout << "Error: operación aritmética requiere enteros (int)" << endl;
                exit(0);
            }
        case LT_OP: case LE_OP: case EQ_OP:
            if (v1.type=="int" && v2.type=="int") return ImpValue("bool"); // comparación válida
            else {
                cout << "Error: operación de comparación requiere enteros (int)" << endl;
                exit(0);
            }
        case AND_OP: case OR_OP:
            if (v1.type=="bool" && v2.type=="bool") return ImpValue("bool"); // comparación válida
            else {
                cout << "Error: operación lógica requiere booleanos (bool)" << endl;
                exit(0);
            }

        default:
            cout << "Error: operador no reconocido" << endl;
            exit(0);
    }
}

ImpValue TypeVisitor::visit(NumberExp* exp) {
    return ImpValue("int", exp->value, false);
}

ImpValue TypeVisitor::visit(BoolExp* exp) {
    return ImpValue("bool",0,exp->value);
}

ImpValue TypeVisitor::visit(IdentifierExp* exp) {
    string tipo = env.lookup_type(exp->name);
    if (tipo == "int") return ImpValue("int");
    if (tipo == "bool") return ImpValue("bool");

    cout << "Tipo no reconocido para variable " << exp->name << endl;
    exit(0);
}

ImpValue TypeVisitor::visit(IFExp* exp) {
    auto cond_type = exp->cond->accept(this);
    if (cond_type.type != "bool") { // 2 = bool
        cout << "Condición de IFEXP debe ser booleana" << endl;
        exit(0);
    }

    auto left_type = exp->left->accept(this);
    auto right_type = exp->right->accept(this);

    if (left_type.type != right_type.type) {
        cout << "Los tipos de las ramas de IFEXP no coinciden" << endl;
        exit(0);
    }

    return left_type;
}


ImpValue TypeVisitor::visit(UnaryExp* exp) {
    ImpValue e = exp->exp->accept(this);
    ImpValue result;
    result.type = e.type;
    if (exp->op == NOT_OP) {
        if(result.type == "bool"){
            return result;
        }
        else{
            cout<<"Debería ser un booleano";
            exit(0);
        }
    }
    return result;
}

void TypeVisitor::visit(AssignStatement* stm) {
    if(!env.check(stm->id)) {
        cout << "Variable no declarada";
        exit(0);
    }
    if(env.lookup_type(stm->id) == "int" && stm->rhs->accept(this).type == "int") {

    }
    else if(env.lookup_type(stm->id) == "bool" && stm->rhs->accept(this).type == "bool") {

    }
    else {
        cout << "No coincide la asignación";
        exit(0);
    }
}

void TypeVisitor::visit(PrintStatement* stm) {
    stm->e->accept(this);
}

void TypeVisitor::visit(IfStatement* stm) {
    if(stm->condition->accept(this).type == "bool"){
        stm->then->accept(this);
        if(stm->els) stm->els->accept(this);
    }
    else {
        cout << "Condicional If no es bool";
        exit(0);
    }
}

void TypeVisitor::visit(WhileStatement* stm) {
    if(stm->condition->accept(this).type == "bool"){
        stm->b->accept(this);
    }
    else {
        cout << "Condicional while no es bool";
        exit(0);
    }

}

void TypeVisitor::visit(ForStatement* stm) {
    // in range
    if(stm->start->accept(this).type == "int"){
        if(stm->end->accept(this).type == "int"){
            if(stm->step->accept(this).type == "int"){
                stm->b->accept(this);
            }
            else {
                cout << "step FOR no es int";
                exit(0);
            }
        }
        else {
            cout << "end FOR no es int";
            exit(0);
        }
    }
    else {
        cout << "start FOR no es int";
        exit(0);
    }
}

void TypeVisitor::visit(VarDec* stm) {
    if(stm->type=="int"){
        for(auto i:stm->vars){
            env.add_var(i, "int");
        }
    }
    else if(stm->type=="bool"){
        for(auto i:stm->vars){
            env.add_var(i, "bool");
        }
    }
    else {
        cout << "Tipo no reconocido";
        exit(0);
    }
}

void TypeVisitor::visit(VarDecList* stm) {
    for (auto i:stm->vardecs) {
        i->accept(this);
    }
}

void TypeVisitor::visit(StatementList* stm) {
    for (auto i:stm->stms) {
        i->accept(this);
    }
}

void TypeVisitor::visit(Body* b) {
    env.add_level();
    b->vardecs->accept(this);
    b->slist->accept(this);
    env.remove_level();
}