#ifndef C4C_AST_TO_TAC_H
#define C4C_AST_TO_TAC_H


#include "tac.hpp"


class AstToTac
{
public:
	std::string file_name;
	ASTProgram *ast_program;
	TACProgram *program;
	std::string string;
	std::vector<TACInstruction *> *inst = nullptr;
	Arena *arena = nullptr;
	std::string global_ident;
	std::string global_label;
	int global_counter = 0;

	AstToTac(std::string file_name,ASTProgram *ast_program,Arena *arena,int global_counter,SymbolTable symbol_table)
	{
		this->file_name = file_name;
		this->ast_program = ast_program;
		this->arena = arena;
		this->global_ident = "c4_tmp";
		this->global_label = "__c4_label";
		this->global_counter = global_counter;

		void *mem = alloc(sizeof(TACProgram));
		this->program = new(mem) TACProgram();


		convert_symbols_to_tac(symbol_table);

		for (ASTDeclaration *decl : this->ast_program->decls)
		{
			if (decl == nullptr)
			{
				break;
			}
			TACDeclaration *tac_decl = convert_decl(decl);
			if (tac_decl == NULL)
			{
				continue;
			}
			this->program->add_decl(tac_decl);
		}

	}


	void convert_symbols_to_tac(SymbolTable symbol_table)
	{
		for (auto it = symbol_table.table.begin(); it != symbol_table.table.end(); ++it)
        {
			std::string name = it->first;
            Symbol symbol = it->second;
			TACGlobalVariable *tac_vardecl = nullptr;

			if (symbol.tentative)
			{
				void *mem = alloc(sizeof(TACGlobalVariable));
				tac_vardecl = new(mem) TACGlobalVariable(symbol.global,symbol.name);

				mem = alloc(sizeof(int));
				int *data = new(mem)int;
				*data = 0;

				tac_vardecl->add_data(TACType::I32,data);
			}
			else if (symbol.init)
			{
				void *mem = alloc(sizeof(TACGlobalVariable));
				tac_vardecl = new(mem) TACGlobalVariable(symbol.global,symbol.name);
				
				mem = alloc(sizeof(int));
				int *data = new(mem)int;
				*data = symbol.int_init;

				tac_vardecl->add_data(TACType::I32,data);

			}
			else
			{
				continue;
			}

			void *mem = alloc(sizeof(TACDeclaration));
			TACDeclaration *tac_decl = new(mem) TACDeclaration(TACDeclarationType::VARDECL,tac_vardecl);;
			this->program->add_decl(tac_decl);
        }
	}

	void *alloc(int size)
	{
		return this->arena->alloc(size);
	}


	TACDeclaration *convert_decl(ASTDeclaration *decl)
	{
		void *mem = alloc(sizeof(TACDeclaration));
		TACDeclaration *tac_decl = nullptr;

		switch (decl->type)
		{
			case ASTDeclarationType::FUNCTION:
			{
				DEBUG_PRINT("here "," function ");
				TACFunction *tac_fn = convert_function((ASTFunctionDecl *)decl->decl);
				tac_decl = new(mem) TACDeclaration(TACDeclarationType::FUNCTION,tac_fn);
				break;
			}
			default:
			{
				DEBUG_PRINT("here "," null tac_decl");
			}
			/*
			case ASTDeclarationType::VARDECL:
			{
				TACFunction *tac_vardecl = convert_global_vardecl((ASTVarDecl *)decl->decl);
				tac_decl = new(mem) TACDeclaration(TACDeclarationType::VARDECL,tac_vardecl);
				break;
			}*/
		}

		return tac_decl;
	}

	/*
	int get_i32_init(void *expr)
    {
        ASTI32Expr *i32_expr = (ASTI32Expr *)expr;
        return i32_expr->value; 
    }

	
	TACGlobalVariable *convert_global_vardecl(ASTVarDecl *decl)
	{
		void *mem = alloc(sizeof(TACGlobalVariable));
		TACGlobalVariable *tac_vardecl = new(mem) TACGlobalVariable(decl->is_public,decl->ident);
		
		switch(decl->expr->type)
		{
			case ASTExpressionType::I32:
			{
				tac_vardecl->add_int_init(get_i32_init(decl->expr));
				break;
			}
		}

		return tac_vardecl;
	}

	*/


	TACFunction *convert_function(ASTFunctionDecl *decl)
	{
		void *mem = alloc(sizeof(TACFunction));
		TACFunction *tac_fn = new(mem) TACFunction(decl->is_public,decl->ident);

		for (ASTFunctionArgument *arg : decl->arguments)
		{
			if (arg == nullptr)
			{
				continue;
			}

			tac_fn->add_argument(arg->ident);
		}

		this->inst = &tac_fn->instructions;
		convert_block_stmt(decl->block);
		this->inst = nullptr;
		return tac_fn;
	}


	void convert_block_stmt(ASTBlockStmt *block)
	{

		for (ASTStatement *stmt : block->stmts)
		{
			convert_stmt(stmt);
		}
	}


	void convert_stmt(ASTStatement *stmt)
	{
		switch(stmt->type)
		{
			case ASTStatementType::RETURN:
			{
				convert_return_stmt((ASTReturnStmt *)stmt->stmt);
				break;
			}
			case ASTStatementType::IF:
			{
				convert_if_stmt((ASTIfStmt *)stmt->stmt);
				break;
			}
			case ASTStatementType::WHILE:
			{
				convert_while_stmt((ASTWhileStmt *)stmt->stmt);
				break;
			}
			case ASTStatementType::BREAK:
			{
				convert_break_stmt((ASTBreakStmt *)stmt->stmt);
				break;
			}
			case ASTStatementType::CONTINUE:
			{
				convert_continue_stmt((ASTContinueStmt *)stmt->stmt);
				break;
			}
			case ASTStatementType::VARDECL:
			{
				convert_vardecl_stmt((ASTVarDecl *)stmt->stmt);
				break;
			}
			case ASTStatementType::EXPR:
			{
				convert_expr((ASTExpression *)stmt->stmt);
				break;
			}
		}
	}


	void convert_break_stmt(ASTBreakStmt *stmt)
	{
		std::string break_label_name = "break" + stmt->label;

	
		void *mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(break_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));

	}

	void convert_continue_stmt(ASTContinueStmt *stmt)
	{
		std::string continue_label_name = "continue" + stmt->label;

	
		void *mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(continue_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));

	}


	void convert_while_stmt(ASTWhileStmt *stmt)
	{
		std::string continue_label_name = "continue" + stmt->label;

		void *mem = alloc(sizeof(TACLabelInst));
		TACLabelInst *tac_label = new(mem) TACLabelInst(continue_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

		std::string break_label_name = "break" + stmt->label;


		TACValue *tac_expr = convert_expr(stmt->expr);
		
		mem = alloc(sizeof(TACJmpIfZeroInst));
		TACJmpIfZeroInst *tac_jz = new(mem) TACJmpIfZeroInst(tac_expr,break_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_ZERO,tac_jz));

		convert_block_stmt(stmt->block);


		mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(continue_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));


		mem = alloc(sizeof(TACLabelInst));
		tac_label = new(mem) TACLabelInst(break_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

	}


	void convert_if_stmt(ASTIfStmt *stmt)
	{
		std::string end_label_name = make_label();
		std::string label_name = make_label();
		
		TACValue *tac_expr = convert_expr(stmt->expr);
		
		void *mem = alloc(sizeof(TACJmpIfZeroInst));
		TACJmpIfZeroInst *tac_jz = new(mem) TACJmpIfZeroInst(tac_expr,label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_ZERO,tac_jz));

		convert_block_stmt(stmt->block);


		mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(end_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));

		mem = alloc(sizeof(TACLabelInst));
		TACLabelInst *tac_label = new(mem) TACLabelInst(label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));



		for(ASTIfElifBlock *elif_block : stmt->elif_blocks)
        {
            if (elif_block == nullptr)
            {
                continue;
            }

			tac_expr = convert_expr(elif_block->expr);

			label_name = make_label();
			
			void *mem = alloc(sizeof(TACJmpIfZeroInst));
			TACJmpIfZeroInst *tac_jz = new(mem) TACJmpIfZeroInst(tac_expr,label_name);

			mem = alloc(sizeof(TACInstruction));
			this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_ZERO,tac_jz));

			convert_block_stmt(elif_block->block);


			mem = alloc(sizeof(TACJmpInst));
			TACJmpInst *tac_jmp = new(mem) TACJmpInst(end_label_name);

			mem = alloc(sizeof(TACInstruction));
			this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));


			mem = alloc(sizeof(TACLabelInst));
			tac_label = new(mem) TACLabelInst(label_name);

			mem = alloc(sizeof(TACInstruction));
			this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

		}

		if (stmt->else_block != nullptr)
		{
			convert_block_stmt(stmt->else_block->block);
		}

		mem = alloc(sizeof(TACLabelInst));
		tac_label = new(mem) TACLabelInst(end_label_name);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

	}

	void convert_vardecl_stmt(ASTVarDecl *stmt)
	{
		TACValue *tac_src = convert_expr(stmt->expr);

		void *mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(stmt->ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);

		mem = alloc(sizeof(TACCopyInst));
		TACCopyInst *tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));
	}
	
	void convert_return_stmt(ASTReturnStmt *stmt)
	{
		TACValue *tac_value = convert_expr(stmt->expr);
		void *mem = alloc(sizeof(TACReturnInst));
		TACReturnInst *tac_inst = new(mem) TACReturnInst(tac_value);
		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::RETURN,tac_inst));
	}


	TACValue *convert_expr(ASTExpression *expr)
	{
		TACValue *value = nullptr;
		switch (expr->type)
		{
			case ASTExpressionType::I32:
			{
				value = convert_i32_expr(expr->expr);
				break;
			}
			case ASTExpressionType::FUNCTION_CALL:
			{
				value = convert_function_call_expr(expr->expr);
				break;
			}
			case ASTExpressionType::VARIABLE:
			{
				value = convert_variable_expr(expr->expr);
				break;
			}
			case ASTExpressionType::ASSIGN:
			{
				value = convert_assign_expr(expr->expr);
				break;
			}
			case ASTExpressionType::UNARY:
			{
				value = convert_unary_expr(expr->expr);
				break;
			}
			case ASTExpressionType::BINARY:
			{
				value = convert_binary_expr(expr->expr);
				break;
			}
		}

		if ( value == nullptr)
		{
			DEBUG_PANIC("NULL huh " +  std::to_string((int)expr->type));
		}
		return value;
	}


	TACValue *convert_function_call_expr(void *expr)
	{
		ASTFunctionCallExpr *fn_expr = (ASTFunctionCallExpr *)expr;


		std::string tac_dst_ident = make_tmp();

		void *mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(tac_dst_ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);

		mem = alloc(sizeof(TACFunctionCallInst));
		TACFunctionCallInst *tac_fn = new(mem) TACFunctionCallInst(fn_expr->ident,tac_dst);

		for ( ASTExpression *arg : fn_expr->args)
		{
			TACValue *tac_arg = convert_expr(arg);
			tac_fn->add_argument(tac_arg);
		}

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::FUNCTION_CALL,tac_fn));

		return tac_dst;
	}


	TACValue *convert_variable_expr(void *expr)
	{
		ASTVariableExpr *var_expr = (ASTVariableExpr *)expr;

		void *mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(var_expr->ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);

		return tac_dst;

	}


	TACValue *convert_assign_expr(void *expr)
	{
		ASTAssignExpr *assign_expr = (ASTAssignExpr *)expr;
		TACValue *tac_dst = convert_expr(assign_expr->lhs);
		TACValue *tac_src = convert_expr(assign_expr->rhs);

		void *mem = alloc(sizeof(TACCopyInst));
		TACCopyInst *tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));

		return tac_dst;
	}
	
	

	TACValue *convert_binary_expr(void *expr)
	{
		TACValue *tac_dst = nullptr;
		ASTBinaryExpr *bin_expr = (ASTBinaryExpr *)expr;

		if (bin_expr->op == ASTBinaryOperator::AND)
		{
			tac_dst = convert_binary_and(expr);
		}
		else if(bin_expr->op == ASTBinaryOperator::OR)
		{
			tac_dst = convert_binary_or(expr);
		}
		else
		{
			tac_dst = convert_binary_normal(expr);
		}
		
		
		return tac_dst;
	}

	TACValue *convert_binary_and(void *expr)
	{
		std::string false_label_ident = make_label();

		
		TACValue *tac_src1 = convert_expr(((ASTBinaryExpr *)expr)->lhs);
		
		void *mem = alloc(sizeof(TACJmpIfZeroInst));
		TACJmpIfZeroInst *tac_jz = new(mem) TACJmpIfZeroInst(tac_src1,false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_ZERO,tac_jz));



		TACValue *tac_src2 = convert_expr(((ASTBinaryExpr *)expr)->rhs);
		
		mem = alloc(sizeof(TACJmpIfZeroInst));
		tac_jz = new(mem) TACJmpIfZeroInst(tac_src2,false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_ZERO,tac_jz));



		std::string tac_dst_ident = make_tmp();

		mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(tac_dst_ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);



		mem = alloc(sizeof(int));
		int *constant = new(mem) int;
		*constant = 1;

		mem = alloc(sizeof(TACConstant));
		TACConstant *tac_const = new(mem) TACConstant(TACConstantType::I32,constant);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_src = new(mem)TACValue(TACValueType::CONSTANT,tac_const);

		mem = alloc(sizeof(TACCopyInst));
		TACCopyInst *tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);
		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));



		
		std::string end_label_ident = make_label();


		mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(end_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));


		mem = alloc(sizeof(TACLabelInst));
		TACLabelInst *tac_label = new(mem) TACLabelInst(false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

		
		mem = alloc(sizeof(int));
		constant = new(mem) int;
		*constant = 0;

		mem = alloc(sizeof(TACConstant));
		tac_const = new(mem) TACConstant(TACConstantType::I32,constant);

		mem = alloc(sizeof(TACValue));
		tac_src = new(mem)TACValue(TACValueType::CONSTANT,tac_const);

		mem = alloc(sizeof(TACCopyInst));
		tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));

		
		
		mem = alloc(sizeof(TACLabelInst));
		tac_label = new(mem) TACLabelInst(end_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));
		
		return tac_dst;

	}


	TACValue *convert_binary_or(void *expr)
	{
		std::string false_label_ident = make_label();

		
		TACValue *tac_src1 = convert_expr(((ASTBinaryExpr *)expr)->lhs);
		
		void *mem = alloc(sizeof(TACJmpIfNotZeroInst));
		TACJmpIfNotZeroInst *tac_jz = new(mem) TACJmpIfNotZeroInst(tac_src1,false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_NOT_ZERO,tac_jz));



		TACValue *tac_src2 = convert_expr(((ASTBinaryExpr *)expr)->rhs);
		
		mem = alloc(sizeof(TACJmpIfNotZeroInst));
		tac_jz = new(mem) TACJmpIfNotZeroInst(tac_src2,false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP_NOT_ZERO,tac_jz));



		std::string tac_dst_ident = make_tmp();

		mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(tac_dst_ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);



		mem = alloc(sizeof(int));
		int *constant = new(mem) int;
		*constant = 0;

		mem = alloc(sizeof(TACConstant));
		TACConstant *tac_const = new(mem) TACConstant(TACConstantType::I32,constant);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_src = new(mem)TACValue(TACValueType::CONSTANT,tac_const);

		mem = alloc(sizeof(TACCopyInst));
		TACCopyInst *tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);
		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));



		
		std::string end_label_ident = make_label();


		mem = alloc(sizeof(TACJmpInst));
		TACJmpInst *tac_jmp = new(mem) TACJmpInst(end_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::JMP,tac_jmp));


		mem = alloc(sizeof(TACLabelInst));
		TACLabelInst *tac_label = new(mem) TACLabelInst(false_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));

		
		mem = alloc(sizeof(int));
		constant = new(mem) int;
		*constant = 1;

		mem = alloc(sizeof(TACConstant));
		tac_const = new(mem) TACConstant(TACConstantType::I32,constant);

		mem = alloc(sizeof(TACValue));
		tac_src = new(mem)TACValue(TACValueType::CONSTANT,tac_const);

		mem = alloc(sizeof(TACCopyInst));
		tac_copy = new(mem) TACCopyInst(tac_dst,tac_src);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::COPY,tac_copy));

		
		
		mem = alloc(sizeof(TACLabelInst));
		tac_label = new(mem) TACLabelInst(end_label_ident);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::LABEL,tac_label));
		
		return tac_dst;

	}




	TACValue *convert_binary_normal(void *expr)
	{
		TACValue *tac_src1 = convert_expr(((ASTBinaryExpr *)expr)->lhs);
		TACValue *tac_src2 = convert_expr(((ASTBinaryExpr *)expr)->rhs);


		std::string tac_dst_ident = make_tmp();

		void *mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(tac_dst_ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);

		TACBinaryOperator op =  get_binary_op(((ASTBinaryExpr *)expr)->op);

		mem = alloc(sizeof(TACBinaryInst));
		TACBinaryInst *tac_inst = new(mem) TACBinaryInst(tac_dst,tac_src1,op,tac_src2);

		mem = alloc(sizeof(TACInstruction));
		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::BINARY,tac_inst));

		return tac_dst;
	}

	TACValue *convert_unary_expr(void *expr)
	{
		TACValue *tac_src = convert_expr(((ASTUnaryExpr *)expr)->rhs);

		std::string tac_dst_ident = make_tmp();

		void *mem = alloc(sizeof(TACVariable));
		TACVariable *tac_var = new(mem) TACVariable(tac_dst_ident);

		mem = alloc(sizeof(TACValue));
		TACValue *tac_dst = new(mem)TACValue(TACValueType::VARIABLE,tac_var);

		TACUnaryOperator op =  get_unary_op(((ASTUnaryExpr *)expr)->op);

		mem = alloc(sizeof(TACUnaryInst));
		TACUnaryInst *tac_inst = new(mem) TACUnaryInst(tac_dst,op,tac_src);

		mem = alloc(sizeof(TACInstruction));

		this->inst->push_back(new(mem) TACInstruction(TACInstructionType::UNARY,tac_inst));

		return tac_dst;
	}


	TACValue *convert_i32_expr(void *expr)
	{
		TACConstant *tac_const = convert_i32_constant((ASTI32Expr *)expr);
		void *mem = alloc(sizeof(TACValue));
		return new(mem)TACValue(TACValueType::CONSTANT,tac_const);
	}


	TACConstant *convert_i32_constant(ASTI32Expr *expr)
	{
		void *mem = alloc(sizeof(int));
		int *constant = new(mem) int;
		*constant = expr->value;
		mem = alloc(sizeof(TACConstant));
		TACConstant *tac_const = new(mem) TACConstant(TACConstantType::I32,constant);
		return tac_const;
	}

	TACUnaryOperator get_unary_op(ASTUnaryOperator op)
	{
		switch (op)
		{
			case ASTUnaryOperator::NEGATE:
			{
				return TACUnaryOperator::NEGATE;
				break;
			}
			case ASTUnaryOperator::COMPLEMENT:
			{
				return TACUnaryOperator::COMPLEMENT;
				break;
			}
		}

		return TACUnaryOperator::None;
	}



	TACBinaryOperator get_binary_op(ASTBinaryOperator op)
	{
		switch (op)
		{
			case ASTBinaryOperator::ADD:
			{
				return TACBinaryOperator::ADD;
				break;
			}
			case ASTBinaryOperator::SUB:
			{
				return TACBinaryOperator::SUB;
				break;
			}
			case ASTBinaryOperator::LESS:
			{
				return TACBinaryOperator::LESS;
				break;
			}
			case ASTBinaryOperator::LESS_EQUAL:
			{
				return TACBinaryOperator::LESS_EQUAL;
				break;
			}
			case ASTBinaryOperator::GREATER:
			{
				return TACBinaryOperator::GREATER;
				break;
			}
			case ASTBinaryOperator::GREATER_EQUAL:
			{
				return TACBinaryOperator::GREATER_EQUAL;
				break;
			}
			case ASTBinaryOperator::AND:
			{
				return TACBinaryOperator::AND;
				break;
			}
			case ASTBinaryOperator::OR:
			{
				return TACBinaryOperator::OR;
				break;
			}
		}

		return TACBinaryOperator::None;
	}


	std::string make_tmp(std::string base = "")
	{
		return this->global_ident + "." + std::to_string(this->global_counter++) ; 
	}

	std::string make_label(std::string base = "")
	{
		return this->global_label + "." + std::to_string(this->global_counter++) ; 
	}

};



#endif
