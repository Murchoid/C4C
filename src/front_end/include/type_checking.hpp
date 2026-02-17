#ifndef C4C_TYPE_CHECKING_H
#define C4C_TYPE_CHECKING_H

#include <string>
#include <map>
#include "ast.hpp"
#include "../../utils/include/arena.hpp"

/**

enum class DataType
{
    FUNCTION = 1,
    I32,
    I64,
};

*/



class Symbol
{
public:
    std::string name;
    ASTType *type;
    bool local = false;
    bool global = false;
    bool is_public = false;
    bool init = false;
    bool tentative = false;
    int int_init = 0;
    long int int64_init = 0;
    bool is_int = true;


    Symbol(std::string name,ASTType *type,bool local = true)
    {
        this->name = name;
        this->type = type;
        this->local = local;
    }

    void add_global(bool global)
    {
        this->global = global;
    }


    void add_public(bool is_public)
    {
        this->is_public = is_public;
    }


    void add_tentative(bool tentative)
    {
        this->tentative = tentative;
    }


    void add_init(bool init)
    {
        this->init = init;
    }

    void add_int_init(int int_init)
    {
        this->int_init = int_init;
    }

    void add_int64_init(int int64_init)
    {
        this->int64_init = int64_init;
    }
};


class SymbolTable
{
public:
    std::map<std::string,Symbol> table;
    bool lookup(std::string name) 
    {
        if (this->table.find(name) == this->table.end())
        {
            return false;
        }

        return true;
    }

    void add(std::string name,Symbol symbol)
    {
        //this->map[name] = symbol;
        this->table.emplace(name, symbol);
    }

    Symbol get(std::string name)
    {
        return this->table.at(name);
        //return this->map[name];
    }

    std::string get_name(std::string name)
    {
        return this->table.at(name).name;
        //return this->map[name].name;
    }


    ASTType *get_type(std::string name)
    {
        return this->table.at(name).type;
        //return this->map[name].name;
    }

    bool get_global(std::string name)
    {
        return this->table.at(name).global;
        //return this->map[name].name;
    }
};



class TypeChecking
{
public:
	std::string file_name;
	ASTProgram *program;
    std::string global_ident;
    int global_counter = 0;
    SymbolTable table;
	Arena *arena;

    TypeChecking(std::string file_name,ASTProgram *program,Arena *arena)
    {
        this->file_name = file_name;
        this->program = program;
        this->arena = arena;

        
        SymbolTable *symbol_table = &table;

        for (ASTDeclaration *decl : this->program->decls)
        {
            if (decl == nullptr)
            {
                continue;
            }

            check_decl(decl,symbol_table);
        }
    }


    /**
	 * The function uses an arena to allocate memory of
	 * size `size`.
	 * The function takes one parameter:
	 * -size -> the size of memroy to be allocated
	 */

	void *alloc(int size)
	{
		return this->arena->alloc(size);
	}


    void check_decl(ASTDeclaration *decl,SymbolTable *symbol_table)
    {
        switch (decl->type)
		{
			case ASTDeclarationType::FUNCTION:
			{
				check_function((ASTFunctionDecl *)decl->decl,symbol_table);
				break;
			}
            case ASTDeclarationType::NATIVE:
			{
				check_native((ASTNativeDecl *)decl->decl,symbol_table);
				break;
			}
            case ASTDeclarationType::VARDECL:
			{
				//check_global_vardecl((ASTVarDecl *)decl->decl,symbol_table);
				break;
			}
		}

    }


    /*

    void check_global_vardecl(ASTVarDecl *decl,SymbolTable *symbol_table)
    {
        
        switch (decl->type->type)
        {
            case ASTDataType::I32:
            {
                Symbol symbol(decl->ident,DataType::I32);
                symbol.add_public(decl->is_public);
                symbol.add_global(true);
                symbol.add_init(false);

                get_i32_init(decl->expr->expr);

                if (decl->expr == nullptr)
                {
                    if (decl->is_extern)
                    {
                        symbol.add_init(false);
                    }
                    else
                    {
                        symbol.add_init(true);
                        symbol.add_tentative(true);
                        symbol.add_int_init(0);
                    }
                }
                else if (decl->expr->type == ASTExpressionType::I32)
                {
                    symbol.add_init(true);
                    symbol.add_tentative(false);
                    symbol.add_int_init(get_i32_init(decl->expr->expr));
                }
                else
                {
                    fatal(" non-constant initializer used on local static variable");
                }

                bool is_public = not decl->is_static;

                if(symbol_table->lookup(decl->ident))
                {
                    if(symbol_table->get_type(decl->ident) != ASTDataType::I32)
                    {
                        fatal("function redeclared as a variable");
                    }

                    if (decl->is_extern)
                    {
                        is_public = symbol_table->get_global(decl->ident);
                    }
                    else if ( symbol_table->get_global(decl->ident) != is_public)
                    {
                        fatal(" variable linkage conflicts with previous declaration");
                    }

                    if (symbol_table->get(decl->ident).tentative == false )
                    {
                        if (symbol.tentative == false)
                        {
                            fatal(" conflicting file scope variable definitions ");
                        }
                        else
                        {
                            symbol.add_tentative(symbol_table->get(decl->ident).tentative);
                            symbol.add_init(symbol_table->get(decl->ident).init);
                            symbol.add_int_init(symbol_table->get(decl->ident).int_init);
                        }
                    }
                    else if (symbol.tentative)
                    {
                        symbol.add_tentative(true);
                    }
                }

                symbol_table->add(decl->ident,symbol);

                decl->expr->add_data_type(ASTDataType::I32);
                break;
            }
            case ASTDataType::I64:
            {
                Symbol symbol(decl->ident,DataType::I64);
                symbol.add_public(decl->is_public);
                symbol.add_global(true);
                symbol.add_init(false);

                get_i64_init(decl->expr->expr);

                if (decl->expr == nullptr)
                {
                    if (decl->is_extern)
                    {
                        symbol.add_init(false);
                    }
                    else
                    {
                        symbol.add_init(true);
                        symbol.add_tentative(true);
                        symbol.add_int64_init(0);
                    }
                }
                else if (decl->expr->type == ASTExpressionType::I32)
                {
                    symbol.add_init(true);
                    symbol.add_tentative(false);
                    symbol.add_int64_init(get_i64_init(decl->expr->expr));
                }
                else
                {
                    fatal(" non-constant initializer used on local static variable");
                }

                bool is_public = not decl->is_static;

                if(symbol_table->lookup(decl->ident))
                {
                    if(symbol_table->get_type(decl->ident) != ASTDataType::I64)
                    {
                        fatal("function redeclared as a variable");
                    }

                    if (decl->is_extern)
                    {
                        is_public = symbol_table->get_global(decl->ident);
                    }
                    else if ( symbol_table->get_global(decl->ident) != is_public)
                    {
                        fatal(" variable linkage conflicts with previous declaration");
                    }

                    if (symbol_table->get(decl->ident).tentative == false )
                    {
                        if (symbol.tentative == false)
                        {
                            fatal(" conflicting file scope variable definitions ");
                        }
                        else
                        {
                            symbol.add_tentative(symbol_table->get(decl->ident).tentative);
                            symbol.add_init(symbol_table->get(decl->ident).init);
                            symbol.add_int64_init(symbol_table->get(decl->ident).int64_init);
                        }
                    }
                    else if (symbol.tentative)
                    {
                        symbol.add_tentative(true);
                    }
                }

                symbol_table->add(decl->ident,symbol);
                decl->expr->add_data_type(ASTDataType::I64);
                break;
            }
        }
        
        
    }


    */

    void check_native(ASTNativeDecl *decl,SymbolTable *symbol_table)
    {
        for(ASTFunctionDeclNative *fn : decl->functions)
        {
            check_function_native(fn,symbol_table);
        }
    }


    bool check_type(ASTType *type)
    {
        switch(type->type_type)
        {
            case ASTDataType::I8:
            case ASTDataType::I16:
            case ASTDataType::I32:
            case ASTDataType::I64:
            case ASTDataType::U8:
            case ASTDataType::U16:
            case ASTDataType::U32:
            case ASTDataType::U64:
            case ASTDataType::CHAR:
            case ASTDataType::VOID:
            {
                return true;
                break;
            }
            case ASTDataType::POINTER:
            {
                ASTPointer *ptr = (ASTPointer *)type->type;
                return check_type(ptr->type);
                break;
            }
            case ASTDataType::STRUCT:
            {
                return true;
                break;
            }
        }

        return true;
    }
    

    void check_function_native(ASTFunctionDeclNative *decl,SymbolTable *symbol_table)
    {
        if (not check_type(decl->return_type))
        {
            fatal(" invalid return type in function " + decl->ident);
        }



        if (symbol_table->lookup(decl->ident))
        {
            fatal("redeclared function " + decl->ident);
        }

        
        std::vector<ASTType *> args;
        

        for (ASTFunctionArgument *arg : decl->arguments)
        {
            if (arg == nullptr)
            {
                continue;
            }

            if(not check_type(arg->type))
            {
                fatal(" unsupported type in function argument ");
            }

            symbol_table->add(arg->ident,Symbol(arg->ident,arg->type));
            args.push_back(arg->type);
        }

        void *mem = alloc(sizeof(ASTFunction));
        ASTFunction *f_type = new(mem)ASTFunction(decl->return_type,args);

        mem = alloc(sizeof(ASTType));
        ASTType *type = new(mem) ASTType();

        type->add_type_type(ASTDataType::FUNCTION);
        type->add_type(f_type);

        Symbol symbol(decl->ident,type);
        symbol_table->add(decl->ident,symbol);
    }

    void check_function(ASTFunctionDecl *decl,SymbolTable *symbol_table)
    {
        if (not check_type(decl->return_type))
        {
            fatal(" invalid return type in function " + decl->ident);
        }


        if (symbol_table->lookup(decl->ident))
        {
            fatal("redeclared function " + decl->ident);
        }

        
        std::vector<ASTType *> args;        

        for (ASTFunctionArgument *arg : decl->arguments)
        {
            if (arg == nullptr)
            {
                continue;
            }

            if(not check_type(arg->type))
            {
                fatal(" unsupported type in function argument ");
            }

            symbol_table->add(arg->ident,Symbol(arg->ident,arg->type));
            args.push_back(arg->type);
        }

        void *mem = alloc(sizeof(ASTFunction));
        ASTFunction *f_type = new(mem)ASTFunction(decl->return_type,args);

        mem = alloc(sizeof(ASTType));
        ASTType *type = new(mem) ASTType();

        type->add_type_type(ASTDataType::FUNCTION);
        type->add_type(f_type);

        Symbol symbol(decl->ident,type);
        symbol.add_global(decl->is_public);
        symbol_table->add(decl->ident,symbol);

        if (decl->block != nullptr)
        {
            check_block_stmt(decl->block,symbol_table,decl->return_type);
        }
    }



    void check_block_stmt(ASTBlockStmt *block,SymbolTable *symbol_table,ASTType *return_type)
	{
		for (ASTStatement *stmt : block->stmts)
		{
			check_stmt(stmt,symbol_table,return_type);
		}
	}

    void check_stmt(ASTStatement *stmt,SymbolTable *symbol_table,ASTType *return_type)
	{
		switch(stmt->type)
		{
			case ASTStatementType::RETURN:
			{
                check_return_stmt((ASTReturnStmt *)stmt->stmt,symbol_table,return_type);
				break;
			}
            case ASTStatementType::IF:
			{
                check_if_stmt((ASTIfStmt *)stmt->stmt,symbol_table,return_type);
				break;
			}
            case ASTStatementType::WHILE:
			{
                check_while_stmt((ASTWhileStmt *)stmt->stmt,symbol_table,return_type);
				break;
			}
            case ASTStatementType::VARDECL:
			{
                check_vardecl_stmt((ASTVarDecl *)stmt->stmt,symbol_table);
				break;
			}
			case ASTStatementType::EXPR:
			{
				check_expr((ASTExpression *)stmt->stmt,symbol_table);
				break;
			}
		}
	}

    int get_i32_init(void *expr)
    {
        ASTI32Expr *i32_expr = (ASTI32Expr *)expr;
        return i32_expr->value; 
    }


    int get_i64_init(void *expr)
    {
        ASTI64Expr *i64_expr = (ASTI64Expr *)expr;
        return i64_expr->value; 
    }


    void check_vardecl_stmt(ASTVarDecl *decl,SymbolTable *symbol_table)
    {
        if (not check_type(decl->type))
        {
            fatal(" invalid type in variable declaration " + decl->ident);
        }

        ASTType *type = decl->type;

        if (decl->is_extern)
        {
            if(decl->init != nullptr)
            {
                fatal(" local extern variable declared with an initializer is illegal");
            }



            if(symbol_table->lookup(decl->ident))
            {
                if(not compare_types(symbol_table->get_type(decl->ident),type))
                {
                    fatal("function redeclared as a variable");
                }
            }
            else
            {
                Symbol symbol(decl->ident,type);
                symbol.add_global(true);
                symbol.add_init(false);
                symbol_table->add(decl->ident,symbol);
            }
        }
        else if(decl->is_static)
        {
            Symbol symbol(decl->ident,type);
            symbol.add_global(false);
            symbol.add_init(false);

            if (decl->init == nullptr)
            {
                symbol.add_init(true);
                symbol.add_int_init(0);
            }/*
            else if (decl->init->type == ASTExpressionType::I32)
            {
                symbol.add_init(true);
                symbol.add_int_init(get_i32_init(decl->expr->expr));
            }*/
            else
            {
                fatal(" non-constant initializer used on local static variable");
            }

            symbol_table->add(decl->ident,symbol);
        }
        else
        {
            Symbol symbol(decl->ident,type,true);
            symbol_table->add(decl->ident,symbol);

            if (decl->init != nullptr)
            {
                if(decl->init->type == ASTVarInitType::SINGLE)
                {
                    check_expr(((ASTVarSingleInit *)decl->init->init)->expr,symbol_table);
                }
                else if(decl->init->type == ASTVarInitType::STRUCT)
                {
                    ASTVarStructMember map = ((ASTVarStructInit *)decl->init->init)->members;
                    
                    for (auto it = map.table.begin(); it != map.table.end(); ++it)
                    {
                        check_expr(it->second,symbol_table);
                    }
                    
                }
            }
        }

        //DEBUG_PRINT(decl->ident,"  in symbol");

        /*
        switch (decl->type->type_type)
        {
            case ASTDataType::I32:
            {
                if (decl->is_extern)
                {
                    if(decl->init != nullptr)
                    {
                        fatal(" local extern variable declared with an initializer is illegal");
                    }



                    if(symbol_table->lookup(decl->ident))
                    {
                        if(not compare_types(symbol_table->get_type(decl->ident),type))
                        {
                            fatal("function redeclared as a variable");
                        }
                    }
                    else
                    {
                        Symbol symbol(decl->ident,type);
                        symbol.add_global(true);
                        symbol.add_init(false);
                        symbol_table->add(decl->ident,symbol);
                    }
                }
                else if(decl->is_static)
                {
                    Symbol symbol(decl->ident,type);
                    symbol.add_global(false);
                    symbol.add_init(false);

                    if (decl->init == nullptr)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(0);
                    }/*
                    else if (decl->init->type == ASTExpressionType::I32)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(get_i32_init(decl->expr->expr));
                    }
                    else
                    {
                        fatal(" non-constant initializer used on local static variable");
                    }

                    symbol_table->add(decl->ident,symbol);
                }
                else
                {
                    Symbol symbol(decl->ident,type,true);
                    symbol_table->add(decl->ident,symbol);

                    if (decl->init != nullptr)
                    {
                        if(decl->init->type == ASTVarInitType::SINGLE)
                        {
                            check_expr(((ASTVarSingleInit *)decl->init->init)->expr,symbol_table);
                        }
                        else if(decl->init->type == ASTVarInitType::STRUCT)
                        {
                            ASTVarStructMember map = ((ASTVarStructInit *)decl->init->init)->members;
                            
                            for (auto it = map.table.begin(); it != map.table.end(); ++it)
                            {
                                check_expr(it->second,symbol_table);
                            }
                            
                        }
                    }
                }
                break;
            }
            case ASTDataType::I64:
            {
                if (decl->is_extern)
                {
                    if(decl->init != nullptr)
                    {
                        fatal(" local extern variable declared with an initializer is illegal");
                    }

                    if(symbol_table->lookup(decl->ident))
                    {
                        if(not compare_types(symbol_table->get_type(decl->ident),type))
                        {
                            fatal("function redeclared as a variable");
                        }
                    }
                    else
                    {
                        Symbol symbol(decl->ident,type);
                        symbol.add_global(true);
                        symbol.add_init(false);
                        symbol_table->add(decl->ident,symbol);
                    }
                }
                else if(decl->is_static)
                {
                    Symbol symbol(decl->ident,type);
                    symbol.add_global(false);
                    symbol.add_init(false);

                    if (decl->init == nullptr)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(0);
                    }/*
                    else if (decl->expr->type == ASTExpressionType::I32)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(get_i32_init(decl->expr->expr));
                    }
                    else
                    {
                        fatal(" non-constant initializer used on local static variable");
                    }

                    symbol_table->add(decl->ident,symbol);
                }

                else
                {
                    symbol_table->add(decl->ident,Symbol(decl->ident,type,true));

                    if (decl->init != nullptr)
                    {
                        if(decl->init->type == ASTVarInitType::SINGLE)
                        {
                            check_expr(((ASTVarSingleInit *)decl->init->init)->expr,symbol_table);
                        }
                        else if(decl->init->type == ASTVarInitType::STRUCT)
                        {
                            ASTVarStructMember map = ((ASTVarStructInit *)decl->init->init)->members;
                            
                            for (auto it = map.table.begin(); it != map.table.end(); ++it)
                            {
                                check_expr(it->second,symbol_table);
                            }
                            
                        }
                    }
                }
                break;
            }
            case ASTDataType::U32:
            {
                if (decl->is_extern)
                {
                    if(decl->init != nullptr)
                    {
                        fatal(" local extern variable declared with an initializer is illegal");
                    }

                    if(symbol_table->lookup(decl->ident))
                    {
                        if(not compare_types(symbol_table->get_type(decl->ident),type))
                        {
                            fatal("function redeclared as a variable");
                        }
                    }
                    else
                    {
                        Symbol symbol(decl->ident,type);
                        symbol.add_global(true);
                        symbol.add_init(false);
                        symbol_table->add(decl->ident,symbol);
                    }
                }
                else if(decl->is_static)
                {
                    Symbol symbol(decl->ident,type);
                    symbol.add_global(false);
                    symbol.add_init(false);

                    if (decl->init == nullptr)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(0);
                    }/*
                    else if (decl->expr->type == ASTExpressionType::U32)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(get_i32_init(decl->expr->expr));
                    }
                    else
                    {
                        fatal(" non-constant initializer used on local static variable");
                    }

                    symbol_table->add(decl->ident,symbol);
                }

                else
                {
                    symbol_table->add(decl->ident,Symbol(decl->ident,type,true));

                    if (decl->init != nullptr)
                    {
                        if(decl->init->type == ASTVarInitType::SINGLE)
                        {
                            check_expr(((ASTVarSingleInit *)decl->init->init)->expr,symbol_table);
                        }
                        else if(decl->init->type == ASTVarInitType::STRUCT)
                        {
                            ASTVarStructMember map = ((ASTVarStructInit *)decl->init->init)->members;
                            
                            for (auto it = map.table.begin(); it != map.table.end(); ++it)
                            {
                                check_expr(it->second,symbol_table);
                            }
                            
                        }
                    }
                }
                break;
            }
            case ASTDataType::U64:
            {
                if (decl->is_extern)
                {
                    if(decl->init != nullptr)
                    {
                        fatal(" local extern variable declared with an initializer is illegal");
                    }

                    if(symbol_table->lookup(decl->ident))
                    {
                        if(not compare_types(symbol_table->get_type(decl->ident),type))
                        {
                            fatal("function redeclared as a variable");
                        }
                    }
                    else
                    {
                        Symbol symbol(decl->ident,type);
                        symbol.add_global(true);
                        symbol.add_init(false);
                        symbol_table->add(decl->ident,symbol);
                    }
                }
                else if(decl->is_static)
                {
                    Symbol symbol(decl->ident,type);
                    symbol.add_global(false);
                    symbol.add_init(false);

                    if (decl->init == nullptr)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(0);
                    }/*
                    else if (decl->expr->type == ASTExpressionType::U32)
                    {
                        symbol.add_init(true);
                        symbol.add_int_init(get_i32_init(decl->expr->expr));
                    }
                    else
                    {
                        fatal(" non-constant initializer used on local static variable");
                    }

                    symbol_table->add(decl->ident,symbol);
                }

                else
                {
                    symbol_table->add(decl->ident,Symbol(decl->ident,type,true));

                    if (decl->init != nullptr)
                    {
                        if(decl->init->type == ASTVarInitType::SINGLE)
                        {
                            check_expr(((ASTVarSingleInit *)decl->init->init)->expr,symbol_table);
                        }
                        else if(decl->init->type == ASTVarInitType::STRUCT)
                        {
                            ASTVarStructMember map = ((ASTVarStructInit *)decl->init->init)->members;
                            
                            for (auto it = map.table.begin(); it != map.table.end(); ++it)
                            {
                                check_expr(it->second,symbol_table);
                            }
                            
                        }
                    }
                }
                break;
            }
            case ASTDataType::ENUM:
            {
                break;
            }
            case ASTDataType::STRUCT:
            {
                break;
            }
            default:
            {
                DEBUG_PANIC("unsupported type in vardecl ");
                break;
            }
        }

        */
    }



    void check_while_stmt(ASTWhileStmt *stmt,SymbolTable *symbol_table,ASTType *return_type)
    {
        check_expr(stmt->expr,symbol_table);
        check_block_stmt(stmt->block,symbol_table,return_type);
    }

    

    void check_if_stmt(ASTIfStmt *stmt,SymbolTable *symbol_table,ASTType *return_type)
    {
        check_expr(stmt->expr,symbol_table);
        check_block_stmt(stmt->block,symbol_table,return_type);

        for(ASTIfElifBlock *elif_block : stmt->elif_blocks)
        {
            if (elif_block == nullptr)
            {
                continue;
            }

            check_expr(elif_block->expr,symbol_table);
            check_block_stmt(elif_block->block,symbol_table,return_type);

        }

        if(stmt->else_block != nullptr)
        {
            check_block_stmt(stmt->else_block->block,symbol_table,return_type);
        }
    }

    bool compare_types(ASTType *type1,ASTType *type2)
    {
        if(type1->type_type != type2->type_type)
        {
            return false;
        }

        switch(type1->type_type)
        {
            case ASTDataType::I8:
            case ASTDataType::I16:
            case ASTDataType::I32:
            case ASTDataType::I64:
            case ASTDataType::U8:
            case ASTDataType::U16:
            case ASTDataType::U32:
            case ASTDataType::U64:
            case ASTDataType::CHAR:
            case ASTDataType::VOID:
            {
                return true;
                break;
            }
            case ASTDataType::POINTER:
            {
                ASTPointer *ptr1 = (ASTPointer *)type1->type;
                ASTPointer *ptr2 = (ASTPointer *)type2->type;
                return compare_types(ptr1->type,ptr2->type);
                break;
            }
            case ASTDataType::STRUCT:
            {
                return true;
                break;
            }
        }

        return true;
    }



    void check_return_stmt(ASTReturnStmt *stmt,SymbolTable *symbol_table,ASTType *return_type)
    {
        check_expr(stmt->expr,symbol_table);

        if(not compare_types(stmt->expr->data_type,return_type))
        {
            return;
            //std::cout << "  return type  "  << (int)return_type  << "  : stmt->expr->data_type  "  << (int)stmt->expr->data_type << std::endl;
            fatal("returned value data type conflicts with the function's data type");
        }
    }

    ASTType *build_builtin(ASTDataType type_type)
    {
        void *mem = alloc(sizeof(ASTType));
        ASTType *type = new(mem) ASTType();
        type->add_type_type(type_type);

        return type;
    }

    void check_expr(ASTExpression *expr,SymbolTable *symbol_table)
	{
		switch (expr->type)
		{
            case ASTExpressionType::I32:
            {
                ASTI32Expr *i32_expr = (ASTI32Expr *)expr->expr;
                i32_expr->add_data_type(build_builtin(ASTDataType::I32));
                expr->add_data_type(i32_expr->data_type);
                break;
            }
            case ASTExpressionType::I64:
            {
                ASTI64Expr *i64_expr = (ASTI64Expr *)expr->expr;
                i64_expr->add_data_type(build_builtin(ASTDataType::I64));
                expr->add_data_type(i64_expr->data_type);
                break;
            }
            case ASTExpressionType::U32:
            {
                ASTU32Expr *u32_expr = (ASTU32Expr *)expr->expr;
                u32_expr->add_data_type(build_builtin(ASTDataType::U32));
                expr->add_data_type(u32_expr->data_type);
                break;
            }
            case ASTExpressionType::U64:
            {
                ASTU64Expr *u64_expr = (ASTU64Expr *)expr->expr;
                u64_expr->add_data_type(build_builtin(ASTDataType::U64));
                expr->add_data_type(u64_expr->data_type);
                break;
            }
            case ASTExpressionType::CAST:
            {
                ASTCastExpr *cast_expr = (ASTCastExpr *)expr->expr;
                check_expr(cast_expr->rhs,symbol_table);

                switch (cast_expr->type)
                {
                    case ASTDataType::I32:
                    {
                        cast_expr->add_data_type(build_builtin(ASTDataType::I32));
                        break;
                    }
                    case ASTDataType::I64:
                    {
                        DEBUG_PANIC("the fuckery");
                        cast_expr->add_data_type(build_builtin(ASTDataType::I64));
                        break;
                    }
                    case ASTDataType::U32:
                    {
                        cast_expr->add_data_type(build_builtin(ASTDataType::U32));
                        break;
                    }
                    case ASTDataType::U64:
                    {
                        DEBUG_PANIC("the fuckery");
                        cast_expr->add_data_type(build_builtin(ASTDataType::U64));
                        break;
                    }
                    default:
                    {
                        fatal(" cast to unsupported type");
                        break;
                    }
                }
                
                expr->add_data_type(cast_expr->data_type);
                break;
            }
			case ASTExpressionType::ASSIGN:
			{
                ASTAssignExpr *assign_expr = (ASTAssignExpr *)expr->expr;

                check_expr(assign_expr->lhs,symbol_table);
                check_expr(assign_expr->rhs,symbol_table);

                switch (assign_expr->lhs->data_type->type_type)
                {
                    case ASTDataType::I32:
                    {
                        if(assign_expr->rhs->data_type->type_type == ASTDataType::I64)
                        {
                            fatal(" i64 used with an i32 => perform cast for this to compile");
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::U64)
                        {
                            fatal(" u64 used with an i32 => perform cast for this to compile");
                        }
                        if(assign_expr->rhs->data_type->type_type == ASTDataType::U32)
                        {
                            fatal(" u32 used with an i32 => perform cast for this to compile");
                        }
                        else
                        {
                            assign_expr->add_data_type(build_builtin(ASTDataType::I32));
                        }
                        break;
                    }
                    case ASTDataType::I64:
                    {
                        if(assign_expr->rhs->data_type->type_type == ASTDataType::I32)
                        {
                            if(assign_expr->rhs->type == ASTExpressionType::I32)
                            {
                                assign_expr->rhs->type = ASTExpressionType::I64;
                                ASTI32Expr *i32_expr = (ASTI32Expr *)assign_expr->rhs->expr;
                                i32_expr->add_data_type(build_builtin(ASTDataType::I64));
                                assign_expr->rhs->add_data_type(build_builtin(ASTDataType::I64));
                                assign_expr->add_data_type(build_builtin(ASTDataType::I64));
                            }
                            else
                            {
                                fatal(" i32 used with an i64 => perform cast for this to compile");
                            }
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::U32)
                        {
                            fatal(" u32 used with an i64 => perform cast for this to compile");
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::U64)
                        {
                            fatal(" u64 used with an i64 => perform cast for this to compile");
                        }
                        else
                        {
                            assign_expr->add_data_type(build_builtin(ASTDataType::I64));
                        }
                        break;
                    }
                    case ASTDataType::U32:
                    {
                        if(assign_expr->rhs->data_type->type_type == ASTDataType::I64)
                        {
                            if(assign_expr->rhs->type == ASTExpressionType::I64)
                            {
                                assign_expr->rhs->type = ASTExpressionType::U32;
                                ASTI64Expr *i64_expr = (ASTI64Expr *)assign_expr->rhs->expr;
                                
                                if(i64_expr->value >= 0 && i64_expr->value <= (std::pow(2,32)))
                                {
                                    i64_expr->add_data_type(build_builtin(ASTDataType::U32));
                                    assign_expr->rhs->add_data_type(build_builtin(ASTDataType::U32));
                                    assign_expr->add_data_type(build_builtin(ASTDataType::U32));
                                }
                                else
                                {
                                    fatal(" i64 used with an u32 => perform cast for this to compile");    
                                }
                            }
                            else
                            {
                                fatal(" i64 used with an u32 => perform cast for this to compile");
                            }
                            
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::U64)
                        {
                            fatal(" u64 used with an u32 => perform cast for this to compile");
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::I32)
                        {
                            if(assign_expr->rhs->type == ASTExpressionType::I32)
                            {
                                assign_expr->rhs->type = ASTExpressionType::U32;
                                ASTI32Expr *i32_expr = (ASTI32Expr *)assign_expr->rhs->expr;
                                
                                if(i32_expr->value >= 0)
                                {
                                    i32_expr->add_data_type(build_builtin(ASTDataType::U32));
                                    assign_expr->rhs->add_data_type(build_builtin(ASTDataType::U32));
                                    assign_expr->add_data_type(build_builtin(ASTDataType::U32));
                                }
                                else
                                {
                                    fatal(" i32 used with an u32 => perform cast for this to compile");
                                }
                            }
                            else
                            {
                                fatal(" i32 used with an u32 => perform cast for this to compile");
                            }
                        }
                        else
                        {
                            assign_expr->add_data_type(build_builtin(ASTDataType::U32));
                        }
                        break;
                    }
                    case ASTDataType::U64:
                    {
                        if(assign_expr->rhs->data_type->type_type == ASTDataType::I32)
                        {
                            if(assign_expr->rhs->type == ASTExpressionType::I32)
                            {
                                assign_expr->rhs->type = ASTExpressionType::U64;
                                ASTI32Expr *i32_expr = (ASTI32Expr *)assign_expr->rhs->expr;
                                
                                if(i32_expr->value >= 0)
                                {
                                    i32_expr->add_data_type(build_builtin(ASTDataType::U64));
                                    assign_expr->rhs->add_data_type(build_builtin(ASTDataType::U64));
                                    assign_expr->add_data_type(build_builtin(ASTDataType::U64));
                                }
                                else
                                {
                                    fatal(" i32 used with an u64 => perform cast for this to compile");
                                }
                            }
                            else
                            {
                                fatal(" i32 used with an u64 => perform cast for this to compile");
                            }
                        }
                        else if(assign_expr->rhs->data_type->type_type == ASTDataType::I64)
                        {
                            if(assign_expr->rhs->type == ASTExpressionType::I64)
                            {
                                assign_expr->rhs->type = ASTExpressionType::U64;
                                ASTI64Expr *i64_expr = (ASTI64Expr *)assign_expr->rhs->expr;
                                
                                if(i64_expr->value >= 0)
                                {
                                    i64_expr->add_data_type(build_builtin(ASTDataType::U64));
                                    assign_expr->rhs->add_data_type(build_builtin(ASTDataType::U64));
                                    assign_expr->add_data_type(build_builtin(ASTDataType::U64));
                                }
                                else
                                {
                                    fatal(" i32 used with an u64 => perform cast for this to compile");
                                }
                            }
                            else
                            {
                                fatal(" i32 used with an u64 => perform cast for this to compile");
                            }
                        }
                        else
                        {
                            assign_expr->add_data_type(build_builtin(ASTDataType::U64));
                        }
                        break;
                    }
                    default:
                    {
                        fatal("unsupported ASTType *encountered : assign");
                        break;
                    }
                }
                
                expr->add_data_type(assign_expr->lhs->data_type);

                break;
            }
            case ASTExpressionType::VARIABLE:
			{
                ASTVariableExpr *var_expr = (ASTVariableExpr *)expr->expr;
                //fatal(" fatal  -> " + var_expr->ident);
                std::string name = var_expr->ident;
                
                if (symbol_table->lookup(name)  and not is_data_type(symbol_table->get_type(name)) )
                {
                    fatal("function used as variable");    
                }
                

                var_expr->add_data_type(symbol_table->get_type(name));
                expr->add_data_type(var_expr->data_type);
                break;
            }
            case ASTExpressionType::STRUCT_METHOD_CALL:
            {
                ASTStructMethodCallExpr *Struct = (ASTStructMethodCallExpr *)expr->expr;
                check_expr(Struct->base,symbol_table);

                if(Struct->base->type != ASTExpressionType::VARIABLE)
                {
                    //fatal("expected identifier in method call");
                }

                std::string name = ((ASTVariableExpr *)Struct->base->expr)->ident;
                std::cout << "name >>>>>>>>    " << name <<std::endl;
                if (symbol_table->lookup(name) and not is_data_type(symbol_table->get_type(name)) )
                {
                    fatal("function used as variable");    
                }

                Struct->add_data_type(symbol_table->get_type(name));
                Struct->prefix = ((ASTStruct *)Struct->data_type->type)->ident;
                expr->add_data_type(Struct->data_type);

                break;
            }
            case ASTExpressionType::ADDRESS_OF:
            {
                ASTAddressOfExpr *addr = (ASTAddressOfExpr *)expr->expr;
                if(is_lvalue(addr->expr))
                {
                    check_expr(addr->expr,symbol_table);

                    void *mem = alloc(sizeof(ASTPointer));
                    ASTPointer *ptr = new(mem) ASTPointer(addr->expr->data_type);

                    mem = alloc(sizeof(ASTType));
                    ASTType *type = new(mem)ASTType();
                    type->add_type_type(ASTDataType::POINTER);
                    type->add_type(ptr);

                    addr->add_data_type(type);
                    expr->add_data_type(addr->data_type);
                }
                else
                {
                    fatal("taking an address of a non lvalue is an invalid operation");
                }
                break;
            }
            case ASTExpressionType::PTR_READ:
            {
                ASTPtrReadExpr *read = (ASTPtrReadExpr *)expr->expr;
                check_expr(read->expr,symbol_table);

                if(read->expr->data_type->type_type != ASTDataType::POINTER)
                {
                    fatal("trying to read from a non-pointer type is invalid");
                }

                ASTPointer *ptr = (ASTPointer *)read->expr->data_type->type;
                read->add_data_type((ASTType *)ptr->type);
                expr->add_data_type(read->data_type);

                break;
            }
            case ASTExpressionType::PTR_WRITE:
            {
                ASTPtrWriteExpr *write = (ASTPtrWriteExpr *)expr->expr;
                check_expr(write->expr,symbol_table);

                if(write->expr->data_type->type_type != ASTDataType::POINTER)
                {
                    fatal("trying to read from a non-pointer type is invalid");
                }

                ASTPointer *ptr = (ASTPointer *)write->expr->data_type->type;
                write->add_data_type((ASTType *)ptr->type);
                expr->add_data_type(write->data_type);
                break;
            }
            case ASTExpressionType::FUNCTION_CALL:
			{
                ASTFunctionCallExpr *fn_expr = (ASTFunctionCallExpr *)expr->expr;
                //fatal(" fatal  -> " + var_expr->ident);
                

                std::string name;

                if(fn_expr->base->type == ASTExpressionType::VARIABLE)
                {
                    name = ((ASTVariableExpr *)fn_expr->base)->ident;
                }
                else
                {
                    fatal("expected identifier as function name");
                }


                ASTType *type = symbol_table->get_type(name);

                if(type->type_type != ASTDataType::FUNCTION)
                {
                    fatal(" variable used as function name ");
                }

                ASTFunction *f_type = (ASTFunction *)type->type;
                ASTType *return_type = (ASTType *)f_type->return_type;

                fn_expr->add_data_type(return_type);
                expr->add_data_type(return_type);


                if (f_type->args.size() != fn_expr->args.size())
                {
                    fatal(" function call with invalid number of arguments");
                }

                int i = 0;

                for (ASTExpression *arg : fn_expr->args)
                {
                    if (arg == nullptr)
                    {
                        continue;
                    }

                    check_expr(arg,symbol_table);
                    if(not compare_types(arg->data_type,f_type->args[i++]))
                    {
                        fatal("unmatching types in function call");
                    }
                }

                break;
            }
            case ASTExpressionType::UNARY:
			{
                ASTUnaryExpr *unary_expr = (ASTUnaryExpr *)expr->expr;

                check_expr(unary_expr->rhs,symbol_table);
                
                switch(unary_expr->op)
                {
                    case ASTUnaryOperator::COMPLEMENT:
                    {
                        unary_expr->add_data_type(build_builtin(ASTDataType::I32));
                        break;
                    }
                    default:
                    {
                        unary_expr->add_data_type(unary_expr->rhs->data_type);
                        break;
                    }
                }

                expr->add_data_type(unary_expr->data_type);

                break;
            }
            case ASTExpressionType::BINARY:
			{
                ASTBinaryExpr *binary_expr = (ASTBinaryExpr *)expr->expr;

                check_expr(binary_expr->lhs,symbol_table);
                check_expr(binary_expr->rhs,symbol_table);

                switch (binary_expr->op)
                {
                    case ASTBinaryOperator::AND:
                    case ASTBinaryOperator::OR:
                    {
                        binary_expr->add_data_type(build_builtin(ASTDataType::I32));
                        break;
                    }
                    default:
                    {
                        switch(binary_expr->op)
                        {
                            case ASTBinaryOperator::ADD:
                            case ASTBinaryOperator::SUB:
                            case ASTBinaryOperator::MUL:
                            case ASTBinaryOperator::DIV:
                            case ASTBinaryOperator::MOD:
                            {
                                switch (binary_expr->lhs->data_type->type_type)
                                {
                                    case ASTDataType::I32:
                                    {
                                        if(binary_expr->rhs->data_type->type_type == ASTDataType::I64)
                                        {
                                            fatal(" i64 used with an i32 => perform cast for this to compile");
                                        }
                                        else
                                        {
                                            binary_expr->add_data_type(build_builtin(ASTDataType::I32));
                                        }
                                        break;
                                    }
                                    case ASTDataType::I64:
                                    {
                                        if(binary_expr->rhs->data_type->type_type == ASTDataType::I32)
                                        {
                                            if(binary_expr->rhs->type == ASTExpressionType::I32)
                                            {
                                                binary_expr->rhs->type = ASTExpressionType::I64;
                                                ASTI32Expr *i32_expr = (ASTI32Expr *)binary_expr->rhs->expr;
                                                i32_expr->add_data_type(build_builtin(ASTDataType::I64));
                                                binary_expr->rhs->add_data_type(build_builtin(ASTDataType::I64));
                                                binary_expr->add_data_type(build_builtin(ASTDataType::I64));
                                            }
                                            else
                                            {
                                                fatal(" i32 used with an i64 => perform cast for this to compile");
                                            }
                                        }
                                        else
                                        {
                                            binary_expr->add_data_type(build_builtin(ASTDataType::I64));
                                        }
                                        break;
                                    }
                                    case ASTDataType::U32:
                                    {
                                        if(binary_expr->rhs->data_type->type_type == ASTDataType::I32)
                                        {
                                            if(binary_expr->rhs->type == ASTExpressionType::I32)
                                            {
                                                binary_expr->rhs->type = ASTExpressionType::U32;
                                                ASTI32Expr *i32_expr = (ASTI32Expr *)binary_expr->rhs->expr;

                                                if(i32_expr->value >= 0)
                                                {
                                                    i32_expr->add_data_type(build_builtin(ASTDataType::U32));
                                                    binary_expr->rhs->add_data_type(build_builtin(ASTDataType::U32));
                                                    binary_expr->add_data_type(build_builtin(ASTDataType::U32));
                                                }
                                                else
                                                {
                                                    fatal(" i32 used with an u32 => perform cast for this to compile");
                                                }
                                            }
                                            else
                                            {
                                                fatal(" i32 used with an u32 => perform cast for this to compile");
                                            }
                                        }
                                        else
                                        {
                                            binary_expr->add_data_type(build_builtin(ASTDataType::I64));
                                        }
                                        break;
                                    }
                                    case ASTDataType::U64:
                                    {
                                        if(binary_expr->rhs->data_type->type_type == ASTDataType::I32)
                                        {
                                            if(binary_expr->rhs->type == ASTExpressionType::I32)
                                            {
                                                binary_expr->rhs->type = ASTExpressionType::U64;
                                                ASTI32Expr *i32_expr = (ASTI32Expr *)binary_expr->rhs->expr;
                                                
                                                if(i32_expr->value >= 0)
                                                {
                                                    i32_expr->add_data_type(build_builtin(ASTDataType::U64));
                                                    binary_expr->rhs->add_data_type(build_builtin(ASTDataType::U64));
                                                    binary_expr->add_data_type(build_builtin(ASTDataType::U64));
                                                }
                                                else
                                                {
                                                    fatal(" i32 used with an u64 => perform cast for this to compile");
                                                }
                                            }
                                            else
                                            {
                                                fatal(" i32 used with an u64 => perform cast for this to compile");
                                            }
                                        }
                                        else if(binary_expr->rhs->data_type->type_type == ASTDataType::I64)
                                        {
                                            if(binary_expr->rhs->type == ASTExpressionType::I64)
                                            {
                                                binary_expr->rhs->type = ASTExpressionType::U64;
                                                ASTI64Expr *i64_expr = (ASTI64Expr *)binary_expr->rhs->expr;
                                                
                                                if(i64_expr->value >= 0)
                                                {
                                                    i64_expr->add_data_type(build_builtin(ASTDataType::U64));
                                                    binary_expr->rhs->add_data_type(build_builtin(ASTDataType::U64));
                                                    binary_expr->add_data_type(build_builtin(ASTDataType::U64));
                                                }
                                                else
                                                {
                                                    fatal(" i32 used with an u64 => perform cast for this to compile");
                                                }
                                            }
                                            else
                                            {
                                                fatal(" i32 used with an u64 => perform cast for this to compile");
                                            }
                                        }
                                        else
                                        {
                                            binary_expr->add_data_type(build_builtin(ASTDataType::U64));
                                        }
                                        break;
                                    }
                                    default:
                                    {
                                        std::cout << "   TYPE   :  "  << (int)binary_expr->lhs->data_type->type_type << std::endl;
                                        fatal("binary expr case : unsupported ASTType *encountered");
                                        break;
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                switch(binary_expr->op)
                                {
                                    case ASTBinaryOperator::EQUAL:
                                    {
                                        compare_types(binary_expr->lhs->data_type,binary_expr->rhs->data_type);
                                        break;
                                    }
                                }
                                binary_expr->add_data_type(build_builtin(ASTDataType::I32));
                            }
                        }
                        break;
                    }
                }

                expr->add_data_type(binary_expr->lhs->data_type);
                break;
            }
        }
    }


    bool is_data_type(ASTType *type)
    {
        switch(type->type_type)
        {
            case ASTDataType::I8:
            case ASTDataType::I16:
            case ASTDataType::I32:
            case ASTDataType::I64:
            case ASTDataType::U8:
            case ASTDataType::U16:
            case ASTDataType::U32:
            case ASTDataType::U64:
            case ASTDataType::CHAR:
            case ASTDataType::VOID:
            case ASTDataType::POINTER:
            case ASTDataType::STRUCT:
            {
                return true;
            }
            default:
            {
                return false;
            }
        }
    }


    bool is_lvalue(ASTExpression *expr)
    {
        switch(expr->type)
        {
            case ASTExpressionType::VARIABLE:
            {
                return true;
            }
            default:
            {
                return false;
            }
        }
    }


    void fatal(std::string string)
    {
        DEBUG_PANIC(string);
    }
};



#endif
