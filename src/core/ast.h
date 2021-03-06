// Copyright (c) 2014 Dropbox, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PYSTON_CORE_AST_H
#define PYSTON_CORE_AST_H

#include <cassert>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <string>

namespace pyston {

namespace AST_TYPE {
    // These are in a pretty random order (started off alphabetical but then I had to add more).
    // These can be changed freely as long as parse_ast.py is also updated
    enum AST_TYPE {
        alias = 1,
        arguments = 2,
        Assert = 3,
        Assign = 4,
        Attribute = 5,
        AugAssign = 6,
        BinOp = 7,
        BoolOp = 8,
        Call = 9,
        ClassDef = 10,
        Compare = 11,
        comprehension = 12,
        Delete = 13,
        Dict = 14,
        Exec = 16,
        ExceptHandler = 17,
        ExtSlice = 18,
        Expr = 19,
        For = 20,
        FunctionDef = 21,
        GeneratorExp = 22,
        Global = 23,
        If = 24,
        IfExp = 25,
        Import = 26,
        ImportFrom = 27,
        Index = 28,
        keyword = 29,
        Lambda = 30,
        List = 31,
        ListComp = 32,
        Module = 33,
        Num = 34,
        Name = 35,
        Pass = 37,
        Pow = 38,
        Print = 39,
        Raise = 40,
        Repr = 41,
        Return = 42,
        Slice = 44,
        Str = 45,
        Subscript = 46,
        TryExcept = 47,
        TryFinally = 48,
        Tuple = 49,
        UnaryOp = 50,
        With = 51,
        While = 52,
        Yield = 53,

        Store = 54,
        Load = 55,
        Param = 56,
        Not = 57,
        In = 58,
        Is = 59,
        IsNot = 60,
        Or = 61,
        And = 62,
        Eq = 63,
        NotEq = 64,
        NotIn = 65,
        GtE = 66,
        Gt = 67,
        Mod = 68,
        Add = 69,
        Continue = 70,
        Lt = 71,
        LtE = 72,
        Break = 73,
        Sub = 74,
        Del = 75,
        Mult = 76,
        Div = 77,
        USub = 78,
        BitAnd = 79,
        BitOr = 80,
        BitXor = 81,
        RShift = 82,
        LShift = 83,
        Invert = 84,
        UAdd = 85,
        FloorDiv = 86,

        DictComp = 15,
        Set = 43,

        // Pseudo-nodes that are specific to this compiler:
        Branch = 200,
        Jump = 201,
        ClsAttribute = 202,
        AugBinOp = 203,
    };
};

class ASTVisitor;
class ExprVisitor;
class StmtVisitor;
class AST_keyword;

class AST {
    public:
        virtual ~AST() {}

        const AST_TYPE::AST_TYPE type;
        uint32_t lineno, col_offset;

        virtual void accept(ASTVisitor *v) = 0;

        AST(AST_TYPE::AST_TYPE type) : type(type) {}
};

class AST_expr : public AST {
    public:
        virtual void* accept_expr(ExprVisitor *v) = 0;

        AST_expr(AST_TYPE::AST_TYPE type) : AST(type) {}
};

class AST_stmt : public AST {
    public:
        virtual void accept_stmt(StmtVisitor *v) = 0;

        AST_stmt(AST_TYPE::AST_TYPE type) : AST(type) {}
};



class AST_alias : public AST {
    public:
        std::string name, asname;

        virtual void accept(ASTVisitor *v);

        AST_alias() : AST(AST_TYPE::alias) {}
};

class AST_arguments : public AST {
    public:
        // no lineno, col_offset attributes
        std::vector<AST_expr*> args, defaults;
        AST_expr *kwarg;
        std::string vararg;

        virtual void accept(ASTVisitor *v);

        AST_arguments() : AST(AST_TYPE::arguments) {}
};

class AST_Assign : public AST_stmt {
    public:
        std::vector<AST_expr*> targets;
        AST_expr* value;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Assign() : AST_stmt(AST_TYPE::Assign) {}
};

class AST_AugAssign : public AST_stmt {
    public:
        AST_expr* value;
        AST_expr* target;
        AST_TYPE::AST_TYPE op_type;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_AugAssign() : AST_stmt(AST_TYPE::AugAssign) {}
};

class AST_AugBinOp : public AST_expr {
    public:
        AST_TYPE::AST_TYPE op_type;
        AST_expr *left, *right;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_AugBinOp() : AST_expr(AST_TYPE::AugBinOp) {}
};

class AST_Attribute : public AST_expr {
    public:
        AST_expr* value;
        AST_TYPE::AST_TYPE ctx_type;
        std::string attr;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Attribute() : AST_expr(AST_TYPE::Attribute) {}
};

class AST_BinOp : public AST_expr {
    public:
        AST_TYPE::AST_TYPE op_type;
        AST_expr *left, *right;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_BinOp() : AST_expr(AST_TYPE::BinOp) {}
};

class AST_BoolOp : public AST_expr {
    public:
        AST_TYPE::AST_TYPE op_type;
        std::vector<AST_expr*> values;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_BoolOp() : AST_expr(AST_TYPE::BoolOp) {}
};

class AST_Break : public AST_stmt {
    public:
        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Break() : AST_stmt(AST_TYPE::Break) {}
};

class AST_Call : public AST_expr {
    public:
        AST_expr *starargs, *kwargs, *func;
        std::vector<AST_expr*> args;
        std::vector<AST_keyword*> keywords;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Call() : AST_expr(AST_TYPE::Call) {}

        static const AST_TYPE::AST_TYPE TYPE = AST_TYPE::Call;
};

class AST_Compare : public AST_expr {
    public:
        std::vector<AST_TYPE::AST_TYPE> ops;
        std::vector<AST_expr*> comparators;
        AST_expr* left;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Compare() : AST_expr(AST_TYPE::Compare) {}
};

class AST_comprehension : public AST {
    public:
        AST_expr* target;
        AST_expr* iter;
        std::vector<AST_expr*> ifs;

        virtual void accept(ASTVisitor *v);

        AST_comprehension() : AST(AST_TYPE::comprehension) {}

        static const AST_TYPE::AST_TYPE TYPE = AST_TYPE::comprehension;
};

class AST_ClassDef : public AST_stmt {
    public:
        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        std::vector<AST_expr*> bases, decorator_list;
        std::vector<AST_stmt*> body;
        std::string name;

        AST_ClassDef() : AST_stmt(AST_TYPE::ClassDef) {}
};

class AST_Continue : public AST_stmt {
    public:
        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Continue() : AST_stmt(AST_TYPE::Continue) {}
};

class AST_Dict : public AST_expr {
    public:
        std::vector<AST_expr*> keys, values;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Dict() : AST_expr(AST_TYPE::Dict) {}
};

class AST_Expr : public AST_stmt {
    public:
        AST_expr* value;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Expr() : AST_stmt(AST_TYPE::Expr) {}
};

class AST_For : public AST_stmt {
    public:
        const static int TYPE = AST_TYPE::For;
        std::vector<AST_stmt*> body, orelse;
        AST_expr *target, *iter;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_For() : AST_stmt(AST_TYPE::For) {}
};

class AST_FunctionDef : public AST_stmt {
    public:
        const static int TYPE = AST_TYPE::FunctionDef;
        std::vector<AST_stmt*> body;
        std::vector<AST_expr*> decorator_list;
        std::string name;
        AST_arguments *args;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_FunctionDef() : AST_stmt(AST_TYPE::FunctionDef) {}
};

class AST_Global : public AST_stmt {
    public:
        std::vector<std::string> names;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Global() : AST_stmt(AST_TYPE::Global) {}
};

class AST_If : public AST_stmt {
    public:
        std::vector<AST_stmt*> body, orelse;
        AST_expr* test;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_If() : AST_stmt(AST_TYPE::If) {}
};

class AST_IfExp : public AST_expr {
    public:
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::IfExp;
        AST_expr *body, *test, *orelse;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_IfExp() : AST_expr(AST_TYPE::IfExp) {}
};

class AST_Import : public AST_stmt {
    public:
        std::vector<AST_alias*> names;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Import() : AST_stmt(AST_TYPE::Import) {}
};

class AST_Index : public AST_expr {
    public:
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::Index;
        AST_expr *value;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Index() : AST_expr(AST_TYPE::Index) {}
};

class AST_keyword : public AST {
    public:
        // no lineno, col_offset attributes
        AST_expr *value;
        std::string arg;

        virtual void accept(ASTVisitor *v);

        AST_keyword() : AST(AST_TYPE::keyword) {}

        static const AST_TYPE::AST_TYPE TYPE = AST_TYPE::keyword;
};

class AST_List : public AST_expr {
    public:
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::List;

        std::vector<AST_expr*> elts;
        AST_TYPE::AST_TYPE ctx_type;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_List() : AST_expr(AST_TYPE::List) {}
};

class AST_ListComp : public AST_expr {
    public:
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::ListComp;

        std::vector<AST_comprehension*> generators;
        AST_expr* elt;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_ListComp() : AST_expr(AST_TYPE::ListComp) {}
};

class AST_Module : public AST {
    public:
        // no lineno, col_offset attributes
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::Module;
        std::vector<AST_stmt*> body;

        virtual void accept(ASTVisitor *v);

        AST_Module() : AST(AST_TYPE::Module) {}
};

class AST_Name : public AST_expr {
    public:
        const static AST_TYPE::AST_TYPE TYPE = AST_TYPE::Name;
        AST_TYPE::AST_TYPE ctx_type;
        std::string id;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Name() : AST_expr(AST_TYPE::Name) {}
};

class AST_Num : public AST_expr {
    public:
        enum NumType {
            // These values must correspond to the values in parse_ast.py
            INT = 0x10,
            FLOAT = 0x20,
        } num_type;

        union {
            int64_t n_int;
            double n_float;
        };

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Num() : AST_expr(AST_TYPE::Num) {}
};

class AST_Pass : public AST_stmt {
    public:
        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Pass() : AST_stmt(AST_TYPE::Pass) {}
};

class AST_Print : public AST_stmt {
    public:
        AST_expr* dest;
        bool nl;
        std::vector<AST_expr*> values;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Print() : AST_stmt(AST_TYPE::Print) {}
};

class AST_Return : public AST_stmt {
    public:
        AST_expr* value;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Return() : AST_stmt(AST_TYPE::Return) {}
};

class AST_Slice : public AST_expr {
    public:
        AST_expr *lower, *upper, *step;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Slice() : AST_expr(AST_TYPE::Slice) {}
};

class AST_Str : public AST_expr {
    public:
        std::string s;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Str() : AST_expr(AST_TYPE::Str) {}
};

class AST_Subscript : public AST_expr {
    public:
        AST_expr *value, *slice;
        AST_TYPE::AST_TYPE ctx_type;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Subscript() : AST_expr(AST_TYPE::Subscript) {}
};

class AST_Tuple : public AST_expr {
    public:
        std::vector<AST_expr*> elts;
        AST_TYPE::AST_TYPE ctx_type;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_Tuple() : AST_expr(AST_TYPE::Tuple) {}
};

class AST_UnaryOp : public AST_expr {
    public:
        AST_expr *operand;
        AST_TYPE::AST_TYPE op_type;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_UnaryOp() : AST_expr(AST_TYPE::UnaryOp) {}
};

class AST_While : public AST_stmt {
    public:
        AST_expr *test;
        std::vector<AST_stmt*> body, orelse;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_While() : AST_stmt(AST_TYPE::While) {}
};

class AST_With : public AST_stmt {
    public:
        AST_expr *optional_vars, *context_expr;
        std::vector<AST_stmt*> body;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_With() : AST_stmt(AST_TYPE::With) {}
};


// AST pseudo-nodes that will get added during CFG-construction.  These don't exist in the input AST, but adding them in
// lets us avoid creating a completely new IR for this phase

class CFGBlock;

class AST_Branch : public AST_stmt {
    public:
        AST_expr *test;
        CFGBlock *iftrue, *iffalse;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Branch() : AST_stmt(AST_TYPE::Branch) {}
};

class AST_Jump : public AST_stmt {
    public:
        CFGBlock *target;

        virtual void accept(ASTVisitor *v);
        virtual void accept_stmt(StmtVisitor *v);

        AST_Jump() : AST_stmt(AST_TYPE::Jump) {}
};

class AST_ClsAttribute : public AST_expr {
    public:
        AST_expr *value;
        std::string attr;

        virtual void accept(ASTVisitor *v);
        virtual void* accept_expr(ExprVisitor *v);

        AST_ClsAttribute() : AST_expr(AST_TYPE::ClsAttribute) {}
};

class ASTVisitor {
    protected:
    public:
        virtual ~ASTVisitor() {}

        virtual bool visit_alias(AST_alias *node) { assert(0); abort(); }
        virtual bool visit_arguments(AST_arguments *node) { assert(0); abort(); }
        virtual bool visit_assign(AST_Assign *node) { assert(0); abort(); }
        virtual bool visit_augassign(AST_AugAssign *node) { assert(0); abort(); }
        virtual bool visit_augbinop(AST_AugBinOp *node) { assert(0); abort(); }
        virtual bool visit_attribute(AST_Attribute *node) { assert(0); abort(); }
        virtual bool visit_binop(AST_BinOp *node) { assert(0); abort(); }
        virtual bool visit_boolop(AST_BoolOp *node) { assert(0); abort(); }
        virtual bool visit_break(AST_Break *node) { assert(0); abort(); }
        virtual bool visit_call(AST_Call *node) { assert(0); abort(); }
        virtual bool visit_clsattribute(AST_ClsAttribute *node) { assert(0); abort(); }
        virtual bool visit_compare(AST_Compare *node) { assert(0); abort(); }
        virtual bool visit_comprehension(AST_comprehension *node) { assert(0); abort(); }
        virtual bool visit_classdef(AST_ClassDef *node) { assert(0); abort(); }
        virtual bool visit_continue(AST_Continue *node) { assert(0); abort(); }
        virtual bool visit_dict(AST_Dict *node) { assert(0); abort(); }
        virtual bool visit_expr(AST_Expr *node) { assert(0); abort(); }
        virtual bool visit_for(AST_For *node) { assert(0); abort(); }
        virtual bool visit_functiondef(AST_FunctionDef *node) { assert(0); abort(); }
        virtual bool visit_global(AST_Global *node) { assert(0); abort(); }
        virtual bool visit_if(AST_If *node) { assert(0); abort(); }
        virtual bool visit_ifexp(AST_IfExp *node) { assert(0); abort(); }
        virtual bool visit_import(AST_Import *node) { assert(0); abort(); }
        virtual bool visit_index(AST_Index *node) { assert(0); abort(); }
        virtual bool visit_keyword(AST_keyword *node) { assert(0); abort(); }
        virtual bool visit_list(AST_List *node) { assert(0); abort(); }
        virtual bool visit_listcomp(AST_ListComp *node) { assert(0); abort(); }
        virtual bool visit_module(AST_Module *node) { assert(0); abort(); }
        virtual bool visit_name(AST_Name *node) { assert(0); abort(); }
        virtual bool visit_num(AST_Num *node) { assert(0); abort(); }
        virtual bool visit_pass(AST_Pass *node) { assert(0); abort(); }
        virtual bool visit_print(AST_Print *node) { assert(0); abort(); }
        virtual bool visit_return(AST_Return *node) { assert(0); abort(); }
        virtual bool visit_slice(AST_Slice *node) { assert(0); abort(); }
        virtual bool visit_str(AST_Str *node) { assert(0); abort(); }
        virtual bool visit_subscript(AST_Subscript *node) { assert(0); abort(); }
        virtual bool visit_tuple(AST_Tuple *node) { assert(0); abort(); }
        virtual bool visit_unaryop(AST_UnaryOp *node) { assert(0); abort(); }
        virtual bool visit_while(AST_While *node) { assert(0); abort(); }
        virtual bool visit_with(AST_With *node) { assert(0); abort(); }

        virtual bool visit_branch(AST_Branch *node) { assert(0); abort(); }
        virtual bool visit_jump(AST_Jump *node) { assert(0); abort(); }
};

class NoopASTVisitor : public ASTVisitor {
    protected:
    public:
        virtual ~NoopASTVisitor() {}

        virtual bool visit_alias(AST_alias *node) { return false; }
        virtual bool visit_arguments(AST_arguments *node) { return false; }
        virtual bool visit_assign(AST_Assign *node) { return false; }
        virtual bool visit_augassign(AST_AugAssign *node) { return false; }
        virtual bool visit_augbinop(AST_AugBinOp *node) { return false; }
        virtual bool visit_attribute(AST_Attribute *node) { return false; }
        virtual bool visit_binop(AST_BinOp *node) { return false; }
        virtual bool visit_boolop(AST_BoolOp *node) { return false; }
        virtual bool visit_break(AST_Break *node) { return false; }
        virtual bool visit_call(AST_Call *node) { return false; }
        virtual bool visit_clsattribute(AST_ClsAttribute *node) { return false; }
        virtual bool visit_compare(AST_Compare *node) { return false; }
        virtual bool visit_comprehension(AST_comprehension *node) { return false; }
        virtual bool visit_classdef(AST_ClassDef *node) { return false; }
        virtual bool visit_continue(AST_Continue *node) { return false; }
        virtual bool visit_dict(AST_Dict *node) { return false; }
        virtual bool visit_expr(AST_Expr *node) { return false; }
        virtual bool visit_for(AST_For *node) { return false; }
        virtual bool visit_functiondef(AST_FunctionDef *node) { return false; }
        virtual bool visit_global(AST_Global *node) { return false; }
        virtual bool visit_if(AST_If *node) { return false; }
        virtual bool visit_ifexp(AST_IfExp *node) { return false; }
        virtual bool visit_import(AST_Import *node) { return false; }
        virtual bool visit_index(AST_Index *node) { return false; }
        virtual bool visit_keyword(AST_keyword *node) { return false; }
        virtual bool visit_list(AST_List *node) { return false; }
        virtual bool visit_listcomp(AST_ListComp *node) { return false; }
        virtual bool visit_module(AST_Module *node) { return false; }
        virtual bool visit_name(AST_Name *node) { return false; }
        virtual bool visit_num(AST_Num *node) { return false; }
        virtual bool visit_pass(AST_Pass *node) { return false; }
        virtual bool visit_print(AST_Print *node) { return false; }
        virtual bool visit_return(AST_Return *node) { return false; }
        virtual bool visit_slice(AST_Slice *node) { return false; }
        virtual bool visit_str(AST_Str *node) { return false; }
        virtual bool visit_subscript(AST_Subscript *node) { return false; }
        virtual bool visit_tuple(AST_Tuple *node) { return false; }
        virtual bool visit_unaryop(AST_UnaryOp *node) { return false; }
        virtual bool visit_while(AST_While *node) { return false; }
        virtual bool visit_with(AST_With *node) { return false; }

        virtual bool visit_branch(AST_Branch *node) { return false; }
        virtual bool visit_jump(AST_Jump *node) { return false; }
};

class ExprVisitor {
    protected:
    public:
        virtual ~ExprVisitor() {}

        virtual void* visit_augbinop(AST_AugBinOp *node) { assert(0); abort(); }
        virtual void* visit_attribute(AST_Attribute *node) { assert(0); abort(); }
        virtual void* visit_binop(AST_BinOp *node) { assert(0); abort(); }
        virtual void* visit_boolop(AST_BoolOp *node) { assert(0); abort(); }
        virtual void* visit_call(AST_Call *node) { assert(0); abort(); }
        virtual void* visit_clsattribute(AST_ClsAttribute *node) { assert(0); abort(); }
        virtual void* visit_compare(AST_Compare *node) { assert(0); abort(); }
        virtual void* visit_dict(AST_Dict *node) { assert(0); abort(); }
        virtual void* visit_ifexp(AST_IfExp *node) { assert(0); abort(); }
        virtual void* visit_index(AST_Index *node) { assert(0); abort(); }
        virtual void* visit_list(AST_List *node) { assert(0); abort(); }
        virtual void* visit_listcomp(AST_ListComp *node) { assert(0); abort(); }
        virtual void* visit_name(AST_Name *node) { assert(0); abort(); }
        virtual void* visit_num(AST_Num *node) { assert(0); abort(); }
        virtual void* visit_slice(AST_Slice *node) { assert(0); abort(); }
        virtual void* visit_str(AST_Str *node) { assert(0); abort(); }
        virtual void* visit_subscript(AST_Subscript *node) { assert(0); abort(); }
        virtual void* visit_tuple(AST_Tuple *node) { assert(0); abort(); }
        virtual void* visit_unaryop(AST_UnaryOp *node) { assert(0); abort(); }
};

class StmtVisitor {
    protected:
    public:
        virtual ~StmtVisitor() {}

        virtual void visit_assign(AST_Assign *node) { assert(0); abort(); }
        virtual void visit_augassign(AST_AugAssign *node) { assert(0); abort(); }
        virtual void visit_break(AST_Break *node) { assert(0); abort(); }
        virtual void visit_classdef(AST_ClassDef *node) { assert(0); abort(); }
        virtual void visit_continue(AST_Continue *node) { assert(0); abort(); }
        virtual void visit_expr(AST_Expr *node) { assert(0); abort(); }
        virtual void visit_for(AST_For *node) { assert(0); abort(); }
        virtual void visit_functiondef(AST_FunctionDef *node) { assert(0); abort(); }
        virtual void visit_global(AST_Global *node) { assert(0); abort(); }
        virtual void visit_if(AST_If *node) { assert(0); abort(); }
        virtual void visit_import(AST_Import *node) { assert(0); abort(); }
        virtual void visit_pass(AST_Pass *node) { assert(0); abort(); }
        virtual void visit_print(AST_Print *node) { assert(0); abort(); }
        virtual void visit_return(AST_Return *node) { assert(0); abort(); }
        virtual void visit_while(AST_While *node) { assert(0); abort(); }
        virtual void visit_with(AST_With *node) { assert(0); abort(); }

        virtual void visit_branch(AST_Branch *node) { assert(0); abort(); }
        virtual void visit_jump(AST_Jump *node) { assert(0); abort(); }
};

void print_ast(AST *ast);
class PrintVisitor : public ASTVisitor {
    private:
        int indent;
        void printIndent();
    public:
        PrintVisitor(int indent=0) : indent(indent) {}
        virtual ~PrintVisitor() {}

        virtual bool visit_alias(AST_alias *node);
        virtual bool visit_arguments(AST_arguments *node);
        virtual bool visit_assign(AST_Assign *node);
        virtual bool visit_augassign(AST_AugAssign *node);
        virtual bool visit_augbinop(AST_AugBinOp *node);
        virtual bool visit_attribute(AST_Attribute *node);
        virtual bool visit_binop(AST_BinOp *node);
        virtual bool visit_boolop(AST_BoolOp *node);
        virtual bool visit_break(AST_Break *node);
        virtual bool visit_call(AST_Call *node);
        virtual bool visit_compare(AST_Compare *node);
        virtual bool visit_comprehension(AST_comprehension *node);
        virtual bool visit_classdef(AST_ClassDef *node);
        virtual bool visit_clsattribute(AST_ClsAttribute *node);
        virtual bool visit_continue(AST_Continue *node);
        virtual bool visit_dict(AST_Dict *node);
        virtual bool visit_expr(AST_Expr *node);
        virtual bool visit_for(AST_For *node);
        virtual bool visit_functiondef(AST_FunctionDef *node);
        virtual bool visit_global(AST_Global *node);
        virtual bool visit_if(AST_If *node);
        virtual bool visit_ifexp(AST_IfExp *node);
        virtual bool visit_import(AST_Import *node);
        virtual bool visit_index(AST_Index *node);
        virtual bool visit_keyword(AST_keyword *node);
        virtual bool visit_list(AST_List *node);
        virtual bool visit_listcomp(AST_ListComp *node);
        virtual bool visit_module(AST_Module *node);
        virtual bool visit_name(AST_Name *node);
        virtual bool visit_num(AST_Num *node);
        virtual bool visit_pass(AST_Pass *node);
        virtual bool visit_print(AST_Print *node);
        virtual bool visit_return(AST_Return *node);
        virtual bool visit_slice(AST_Slice *node);
        virtual bool visit_str(AST_Str *node);
        virtual bool visit_subscript(AST_Subscript *node);
        virtual bool visit_tuple(AST_Tuple *node);
        virtual bool visit_unaryop(AST_UnaryOp *node);
        virtual bool visit_while(AST_While *node);
        virtual bool visit_with(AST_With *node);

        virtual bool visit_branch(AST_Branch *node);
        virtual bool visit_jump(AST_Jump *node);
};

// Given an AST node, return a vector of the node plus all its descendents.
// This is useful for analyses that care more about the constituent nodes than the
// exact tree structure; ex, finding all "global" directives.
void flatten(const std::vector<AST_stmt*> &roots, std::vector<AST*> &output, bool expand_scopes);
void flatten(AST_expr *root, std::vector<AST*> &output, bool expand_scopes);
// Similar to the flatten() function, but filters for a specific type of ast nodes:
template <class T, class R>
void findNodes(const R &roots, std::vector<T*> &output, bool expand_scopes) {
    std::vector<AST*> flattened;
    flatten(roots, flattened, expand_scopes);
    for (AST* n : flattened) {
        if (n->type == T::TYPE)
            output.push_back(reinterpret_cast<T*>(n));
    }
}

};

#endif
