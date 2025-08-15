#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int NumberExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int BoolExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}


int VarDec::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int VarDecList::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}
int StatementList::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int Body::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}
int FCallExp::accept(Visitor *visitor)
{
    return visitor->visit(this);
}
int ReturnStatement::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}
int FunDecList::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int Program::accept(Visitor *visitor)
{
    visitor->visit(this);
    return 0;
}

int FunctionCallStatement::accept(Visitor* visitor){
    visitor->visit(this);
    return 0;
}

int BreakStatement::accept(Visitor* visitor){
    visitor->visit(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

void GenCodeVisitor::generar(Program* program) {
    program->accept(this);
}

void GenCodeVisitor::visit(Program* program) {
out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
    program->vardecs->accept(this);

    for (auto& [var, _] : memoriaGlobal) {
        out << var << ": .quad 0"<<endl;
    }

    out << ".text\n";
    program->fundecs->accept(this);
    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}

void GenCodeVisitor::visit(VarDec* stm) {
    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            memoriaGlobal[var] = true;
        } else {
            memoria[var] = offset;
            offset -= 8;
        }
    }
}

void GenCodeVisitor::visit(VarDecList* stm) {
    for (auto dec : stm->vardecs)
        dec->accept(this);
}

int GenCodeVisitor::visit(NumberExp* exp) {
    out << " movq $" << exp->value << ", %rax"<<endl;
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp* exp) {
    if (memoriaGlobal.count(exp->name))
        out << " movq " << exp->name << "(%rip), %rax"<<endl;
    else
        out << " movq " << memoria[exp->name] << "(%rbp), %rax"<<endl;
    return 0;
}


int GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    out << " pushq %rax\n";
    exp->right->accept(this);
    out << " movq %rax, %rcx\n popq %rax\n";

    switch (exp->op) {
        case PLUS_OP:  out << " addq %rcx, %rax\n"; break;
        case MINUS_OP: out << " subq %rcx, %rax\n"; break;
        case MUL_OP:   out << " imulq %rcx, %rax\n"; break;
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
                << " movl $0, %eax\n"
                << " setle %al\n"
                << " movzbq %al, %rax\n";
            break;
        case LT_OP:
            out << " cmpq %rcx, %rax\n"
                << " movl $0, %eax\n"
                << " setl %al\n"
                << " movzbq %al, %rax\n";
        case EQ_OP:
            out << " cmpq %rcx, %rax\n"
                << " movl $0, %eax\n"
                << " sete %al\n"
                << " movzbq %al, %rax\n";

            break;
    }
    return 0;
}



void GenCodeVisitor::visit(AssignStatement* stm) {
    stm->rhs->accept(this);
    if (memoriaGlobal.count(stm->id))
        out << " movq %rax, " << stm->id << "(%rip)"<<endl;
    else
        out << " movq %rax, " << memoria[stm->id] << "(%rbp)"<<endl;
}

void GenCodeVisitor::visit(PrintStatement* stm) {
    stm->e->accept(this);
    out <<
        " movq %rax, %rsi\n"
        " leaq print_fmt(%rip), %rdi\n"
        " movl $0, %eax\n"
        " call printf@PLT\n";
}



void GenCodeVisitor::visit(FunDecList* f) {
    for (auto dec : f->Fundecs)
        dec->accept(this);
}

void GenCodeVisitor::visit(StatementList* stm) {
    for (auto s : stm->stms)
        s->accept(this);
}

void GenCodeVisitor::visit(Body* b) {
    b->vardecs->accept(this);
    b->slist->accept(this);
}

void GenCodeVisitor::visit(IfStatement* stm) {
    int label = labelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax"<<endl;
    out << " je else_" << label << endl;
    stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << ":"<< endl;
    if (stm->els) stm->els->accept(this);
    out << "endif_" << label << ":"<< endl;
}

void GenCodeVisitor::visit(WhileStatement* stm) {
    int label = labelcont++;

    nombreLoop.push("while_" + to_string(label));

    out << "while_" << label << ":" << endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->b->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << ":" << endl;

    nombreLoop.pop();
}

void GenCodeVisitor::visit(BreakStatement* stm) {
    if(nombreLoop.empty()){
        cout << "Error: break fuera de algun loop" << endl;
        exit(0);
    }
    out << "jmp end" << nombreLoop.top() << endl;
}
int GenCodeVisitor::visit(BoolExp* exp) {
    out << " movq $" << exp->value << ", %rax"<< endl;
    return 0;
}

void GenCodeVisitor::visit(ReturnStatement* stm) {
    if(stm->e != nullptr) {
        stm->e->accept(this);
    }
    out << " jmp .end_"<< nombreFuncion << endl;
}

void GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    memoria.clear();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << " .globl "<< f->nombre << endl;
    out << f->nombre <<":" << endl;
    out << " pushq %rbp"<<endl;
    out << " movq %rsp, %rbp" << endl;
    int size = f->parametros.size();
    for (int i = 0; i < size; i++) {
        memoria[f->parametros[i]] = offset;
        out << " mov " << argRegs[i] << ", " << offset << "(%rbp)" << endl;
        offset -= 8;
    }
    f->cuerpo->vardecs->accept(this);
    int reserva = -offset -8 ;
    if (reserva>0) {
        out << " subq $" << reserva <<", %rsp"<<endl;
    }
    f->cuerpo->slist->accept(this);
    out << ".end_" << f->nombre <<":" << endl;
    out << "leave" << endl;
    out << "ret" << endl;
    entornoFuncion = false;
}

int GenCodeVisitor::visit(FCallExp* exp) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = exp->argumentos.size();
    for(int i = 0; i < size; i++) {
            exp->argumentos[i]->accept(this);
            out << " movq %rax," << argRegs[i]<<endl;
    }
    out << " call "<<exp->nombre<< endl;
    return 0;
}

void GenCodeVisitor::visit(FunctionCallStatement* stm) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = stm->argList.begin(); it != stm->argList.end(); ++it) {
        (*it)->accept(this);
        out << " mov %rax, " << argRegs[i] << endl;
        ++i;
    }
    out << " call " << stm->name << endl;
}