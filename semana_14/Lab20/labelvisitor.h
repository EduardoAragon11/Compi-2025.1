#ifndef LABEL_VISITOR_H
#define LABEL_VISITOR_H

#include "visitor.h"
#include "exp.h"
#include <algorithm>
#include <iostream>
using namespace std;

class LabelVisitor : public Visitor {
public:
    bool leftChild = true;

    int visit(NumberExp* e) override {
        e->etiqueta = leftChild ? 1 : 0;
        cout << "NumberExp(" << e->value << ") => etiqueta = " << e->etiqueta << endl;
        return e->etiqueta;
    }

    int visit(BoolExp* e) override {
        e->etiqueta = leftChild ? 1 : 0;
        cout << "BoolExp(" << e->value << ") => etiqueta = " << e->etiqueta << endl;
        return e->etiqueta;
    }

    int visit(IdentifierExp* e) override {
        e->etiqueta = leftChild ? 1 : 0;
        cout << "IdentifierExp(" << e->name << ") => etiqueta = " << e->etiqueta << endl;
        return e->etiqueta;
    }

    int visit(BinaryExp* e) override {
        leftChild = true;
        int l = e->left->accept(this);

        leftChild = false;
        int r = e->right->accept(this);

        e->etiqueta = (l == r) ? l + 1 : std::max(l, r);

        string op;
        switch (e->op) {
            case PLUS_OP:  op = "+"; break;
            case MINUS_OP: op = "-"; break;
            case MUL_OP:   op = "*"; break;
            case DIV_OP:   op = "/"; break;
            case EQ_OP:    op = "=="; break;
            case LT_OP:    op = "<"; break;
            case LE_OP:    op = "<="; break;
            default:       op = "?"; break;
        }

        cout << "BinaryExp(" << op << ") con etiquetas hijos (" << l << ", " << r << ") => etiqueta = " << e->etiqueta << endl;

        return e->etiqueta;
    }

    int visit(FCallExp* e) override {
        int max_arg = 0;
        for (auto arg : e->argumentos) {
            leftChild = true; // neutral
            max_arg = std::max(max_arg, arg->accept(this));
        }
        e->etiqueta = max_arg;
        cout << "FCallExp(" << e->nombre << ") => etiqueta = " << e->etiqueta << endl;
        return e->etiqueta;
    }

    void visit(AssignStatement* s) override {
        s->rhs->accept(this);
    }

    void visit(PrintStatement* s) override {
        s->e->accept(this);
    }

    void visit(ReturnStatement* s) override {
        s->e->accept(this);
    }

    void visit(IfStatement* s) override {
        s->condition->accept(this);
        if (s->then) s->then->accept(this);
        if (s->els) s->els->accept(this);
    }

    void visit(WhileStatement* s) override {
        s->condition->accept(this);
        if (s->b) s->b->accept(this);
    }

    void visit(VarDec* v) override {}
    void visit(VarDecList* v) override {}

    void visit(StatementList* sl) override {
        for (auto s : sl->stms) {
            s->accept(this);
        }
    }

    void visit(Body* b) override {
        if (b->slist) b->slist->accept(this);
    }

    void visit(FunDec* f) override {
        if (f->cuerpo) f->cuerpo->accept(this);
    }

    void visit(FunDecList* l) override {
        for (auto f : l->Fundecs) {
            f->accept(this);
        }
    }
    void visit(Program* p) override {
        if (p->fundecs) p->fundecs->accept(this);
    }
};

#endif // LABEL_VISITOR_H
