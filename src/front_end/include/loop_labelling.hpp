#ifndef C4C_LOOP_LABELLING_H
#define C4C_LOOP_LABELLING_H

#include "identifier_resolution.hpp"

class LoopLabelling
{
public:
	std::string file_name;
	ASTProgram *program;
    std::string global_label;
    int global_counter = 0;

    LoopLabelling(std::string file_name,ASTProgram *program,int global_counter)
    {
        this->file_name = file_name;
        this->program = program;
        this->global_label = "__c4_label";
        this->global_counter = global_counter;

        for (ASTDeclaration *decl : this->program->decls)
        {
            if (decl == nullptr)
            {
                continue;
            }

            resolve_decl(decl);
        }
    }


    void resolve_decl(ASTDeclaration *decl)
    {
        switch (decl->type)
		{
			case ASTDeclarationType::FUNCTION:
			{
				resolve_function((ASTFunctionDecl *)decl->decl);
				break;
			}
		}

    }


    void resolve_function(ASTFunctionDecl *decl)
    {
        resolve_block_stmt(decl->block);
    }



    void resolve_block_stmt(ASTBlockStmt *block,std::string label = "")
	{

    	for (ASTStatement *stmt : block->stmts)
		{
			resolve_stmt(stmt,label);
		}
	}

    void resolve_stmt(ASTStatement *stmt,std::string label)
	{
		switch(stmt->type)
		{
            case ASTStatementType::IF:
			{
                resolve_if_stmt((ASTIfStmt *)stmt->stmt,label);
				break;
			}
            case ASTStatementType::WHILE:
			{
                std::string new_label = make_label();
                resolve_while_stmt((ASTWhileStmt *)stmt->stmt,new_label);
				break;
			}
            case ASTStatementType::BREAK:
            {
                resolve_break_stmt((ASTBreakStmt *)stmt->stmt,label);
                break;
            }
            case ASTStatementType::CONTINUE:
            {
                resolve_continue_stmt((ASTContinueStmt *)stmt->stmt,label);
                break;
            }
		}
	}

    void resolve_break_stmt(ASTBreakStmt *stmt,std::string label)
    {
        if (label == "")
        {
            fatal("break statement used outside of a loop");
        }
        else
        {
            stmt->add_label(label);
        }
    }


    void resolve_continue_stmt(ASTContinueStmt *stmt,std::string label)
    {
        if (label == "")
        {
            fatal("continue statement used outside of a loop");
        }
        else
        {
            stmt->add_label(label);
        }
    }



    void resolve_while_stmt(ASTWhileStmt *stmt,std::string label)
    {
        stmt->add_label(label);
        resolve_block_stmt(stmt->block,label);
    }

    

    void resolve_if_stmt(ASTIfStmt *stmt,std::string label)
    {
        resolve_block_stmt(stmt->block,label);

        for(ASTIfElifBlock *elif_block : stmt->elif_blocks)
        {
            if (elif_block == nullptr)
            {
                continue;
            }

            resolve_block_stmt(elif_block->block,label);

        }

        if(stmt->else_block != nullptr)
        {
            resolve_block_stmt(stmt->else_block->block,label);
        }
    }


    std::string make_label(std::string base = "")
	{
		return this->global_label + "." + std::to_string(this->global_counter++) ; 
	}


    void fatal(std::string string)
    {
        DEBUG_PANIC(string);
    }
};



#endif