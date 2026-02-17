#ifndef C4C_AST_TO_C_H
#define C4C_AST_TO_C_H

#include "../../../front_end/include/ast.hpp"
#include <string>
#include <vector>


class AstToC
{
public:
    std::string file_name;
    ASTProgram *program;
    std::string string;


	std::string C_types = R"(

#ifndef C4_TYPES_H

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long      u64;

typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long        i64;

#else

#include <sys/types.h>
#include <stddef.h>

typedef u_int8_t           u8;
typedef u_int16_t          u16;
typedef u_int32_t          u32;
typedef u_int64_t          u64;

typedef int8_t             i8;
typedef int16_t            i16;
typedef int32_t            i32;
typedef int64_t            i64;

#endif

typedef float              f32;
typedef double             f64;

typedef u8 *   u8_ptr;
typedef u16 *  u16_ptr;
typedef u32 *  u32_ptr;
typedef u64 *  u64_ptr;

typedef i8 *   i8_ptr;
typedef i16 *  i16_ptr;
typedef i32 *  i32_ptr;
typedef i64 *  i64_ptr;

typedef f32 *  f32_ptr;
typedef f64 *  f64_ptr;

typedef void * void_ptr;

)";



	void write_body(std::string string)
	{
		this->string += string;
	}

    AstToC(std::string file_name,ASTProgram *program)
    {
        this->file_name = file_name;
        this->program = program;

		write_body(this->C_types);

        for (ASTDeclaration *decl : this->program->decls)
		{
			if (decl == nullptr)
			{
				break;
			}
			
            convert_decl(decl);
		}


    }

    
	void convert_decl(ASTDeclaration *decl)
	{
		switch (decl->type)
		{
			case ASTDeclarationType::FUNCTION:
			{
				convert_function((ASTFunctionDecl *)decl->decl);
				break;
			}
			case ASTDeclarationType::ENUM:
			{
				convert_enum_decl((ASTEnumDecl *)decl->decl);
				break;
			}
			case ASTDeclarationType::UNION:
			{
				convert_union_decl((ASTUnionDecl *)decl->decl);
				break;
			}
			case ASTDeclarationType::STRUCT:
			{
				convert_struct_decl((ASTStructDecl *)decl->decl);
				break;
			}
			case ASTDeclarationType::IMPL:
			{
				convert_impl_decl((ASTImplDecl *)decl->decl);
				break;
			}
			case ASTDeclarationType::NATIVE:
			{
				convert_native_decl((ASTNativeDecl *)decl->decl);
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
	}

	void convert_enum_decl(ASTEnumDecl *decl)
	{
		write_body("enum " + decl->ident + "\n{\n");

		for (ASTEnumConstant *enum_const : decl->constants)
		{
			if(enum_const == nullptr)
			{
				continue;
			}

			write_body("\t" + enum_const->ident);

			if(enum_const->has_value)
			{
				write_body(" = " + std::to_string(enum_const->value));
			}

			write_body(",\n");
		}

		write_body("};\n\n");
	}


	void convert_type(ASTType *type)
	{
		switch(type->type_type)
		{
			case ASTDataType::VOID:
			{
				write_body("void ");
				break;
			}
			case ASTDataType::CHAR:
			{
				write_body("char ");
				break;
			}
			case ASTDataType::BOOL:
			{
				write_body("bool ");
				break;
			}
			case ASTDataType::I8:
			{
				write_body("i8 ");
				break;
			}
			case ASTDataType::I16:
			{
				write_body("i16 ");
				break;
			}
			case ASTDataType::I32:
			{
				write_body("i32 ");
				break;
			}
			case ASTDataType::I64:
			{
				write_body("i64 ");
				break;
			}
			case ASTDataType::U8:
			{
				write_body("u8 ");
				break;
			}
			case ASTDataType::U16:
			{
				write_body("u16 ");
				break;
			}
			case ASTDataType::U32:
			{
				write_body("u32 ");
				break;
			}
			case ASTDataType::U64:
			{
				write_body("u64 ");
				break;
			}
			case ASTDataType::F32:
			{
				write_body("f32 ");
				break;
			}
			case ASTDataType::F64:
			{
				write_body("f64 ");
				break;
			}
			case ASTDataType::POINTER:
			{
				puts("pointer");
				ASTPointer *ptr = (ASTPointer *)type->type;
				convert_type(ptr->type);
				write_body("*");
				break;
			}
			case ASTDataType::STRUCT:
			{
				ASTStruct *Struct = (ASTStruct *)type->type;
				write_body("struct " + Struct->ident + " ");
				break;
			}
			case ASTDataType::UNION:
			{
				ASTUnion *Union = (ASTUnion *)type->type;
				write_body("union " + Union->ident + " ");
				break;
			}/*
			case ASTDataType::VARIANT:
			{
				ASTStruct *Struct = (ASTStruct *)type->type;
				write_body("struct " + Struct->ident + " ");
				break;
			}*/
			case ASTDataType::ENUM:
			{
				ASTEnum *Enum = (ASTEnum *)type->type;
				write_body("Enum " + Enum->ident + " ");
				break;
			}
			default:
			{
				DEBUG_PANIC("ünsupported type");
			}
		}
	}
	

	void convert_union_decl(ASTUnionDecl *decl)
	{
		write_body("union " + decl->ident + "\n{\n");

		int arg_length = decl->properties.size();
		int i = 0;

		for (ASTStructProperty *arg : decl->properties)
		{
			if (arg == nullptr)
			{
				continue;
			}

			write_body("\t");
			convert_type(arg->type);
			write_body(arg->ident);
			write_body(";\n");
		}


		write_body("};\n\n");
	}





	void convert_struct_decl(ASTStructDecl *decl)
	{
		write_body("struct " + decl->ident + "\n{\n");

		int arg_length = decl->properties.size();
		int i = 0;

		for (ASTStructProperty *arg : decl->properties)
		{
			if (arg == nullptr)
			{
				continue;
			}

			write_body("\t");
			convert_type(arg->type);
			write_body(arg->ident);
			write_body(";\n");
		}


		write_body("};\n\n");
	}



	void convert_impl_decl(ASTImplDecl *decl)
	{

		for (ASTMethodDecl *method : decl->methods)
		{
			if (method == nullptr)
			{
				continue;
			}
			

			write_body("\n");

			convert_method_decl(method,decl->ident);

			write_body("\n");
		}

	}



	void convert_method_decl(ASTMethodDecl *decl,std::string base)
	{
		convert_type(decl->return_type);

		write_body(base + "_" + decl->ident + "(");

		int arg_length = decl->arguments.size();
		int i = 0;

		for (ASTFunctionArgument *arg : decl->arguments)
		{
			if (arg == nullptr)
			{
				continue;
			}

			

			convert_type(arg->type);
			write_body(arg->ident);

			if(i++ + 1 >= arg_length)
			{
				break;
			}

			write_body(",");

		}

		write_body(")\n");
		convert_block_stmt(decl->block);
	}








	void convert_function_native(ASTFunctionDeclNative *decl)
	{
		convert_type(decl->return_type);
		write_body(decl->ident + "(");

		int arg_length = decl->arguments.size();
		int i = 0;

		for (ASTFunctionArgument *arg : decl->arguments)
		{
			if (arg == nullptr)
			{
				continue;
			}

			convert_type(arg->type);
			write_body(arg->ident);

			if(i++ + 1 >= arg_length)
			{
				break;
			}

			write_body(",");

		}

		write_body(");\n");

	}


	void convert_native_decl(ASTNativeDecl *decl)
	{
		for(ASTFunctionDeclNative *fn : decl->functions)
		{
			convert_function_native(fn);
		}
	}


	void convert_function(ASTFunctionDecl *decl)
	{
		convert_type(decl->return_type);
		write_body(decl->ident + "(");

		int arg_length = decl->arguments.size();
		int i = 0;

		for (ASTFunctionArgument *arg : decl->arguments)
		{
			if (arg == nullptr)
			{
				continue;
			}

			convert_type(arg->type);
			write_body(arg->ident);

			if(i++ + 1 >= arg_length)
			{
				break;
			}

			write_body(",");

		}

		write_body(")\n");
		convert_block_stmt(decl->block);
	}



	void convert_block_stmt(ASTBlockStmt *block,std::string tab="")
	{
		write_body(tab + "{\n");
		for (ASTStatement *stmt : block->stmts)
		{
			convert_stmt(stmt,tab + "\t");
		}
		write_body(tab + "}\n");
	}


	void convert_stmt(ASTStatement *stmt,std::string tab)
	{
		switch(stmt->type)
		{
			case ASTStatementType::RETURN:
			{
				convert_return_stmt((ASTReturnStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::IF:
			{
				convert_if_stmt((ASTIfStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::SWITCH:
			{
				convert_switch_stmt((ASTSwitchStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::WHILE:
			{
				convert_while_stmt((ASTWhileStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::BREAK:
			{
				convert_break_stmt((ASTBreakStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::CONTINUE:
			{
				convert_continue_stmt((ASTContinueStmt *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::VARDECL:
			{
				convert_vardecl_stmt((ASTVarDecl *)stmt->stmt,tab);
				break;
			}
			case ASTStatementType::EXPR:
			{
				write_body(tab);
				convert_expr((ASTExpression *)stmt->stmt);
				write_body(";\n");
				break;
			}
		}
	}



	void convert_break_stmt(ASTBreakStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("break;\n");
	}

	void convert_continue_stmt(ASTContinueStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("continue;\n");
	}

	void convert_while_stmt(ASTWhileStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("while (");
		convert_expr(stmt->expr);
		write_body(")\n");
		convert_block_stmt(stmt->block,tab);
	}


	void convert_if_stmt(ASTIfStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("if(");
		convert_expr(stmt->expr);
		write_body(")\n");
		convert_block_stmt(stmt->block,tab);

		for (ASTIfElifBlock *elif_block : stmt->elif_blocks)
		{
			if (elif_block == nullptr)
            {
                continue;
            }

			write_body("else if(");
			convert_expr(elif_block->expr);
			write_body(")\n");
			convert_block_stmt(elif_block->block,tab );
		}

		if(stmt->else_block != nullptr)
		{
			write_body("else\n");
			convert_block_stmt(stmt->else_block->block,tab);
		}
	}



	void convert_switch_stmt(ASTSwitchStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("switch(");
		convert_expr(stmt->expr);
		write_body(")\n" + tab + "{\n");

		std::string new_tab = tab + "\t";
		for (ASTSwitchCase *case_block : stmt->cases)
		{
			if (case_block == nullptr)
            {
                continue;
            }

			write_body(new_tab + "case ");
			convert_expr(case_block->expr);
			write_body(":\n");
			convert_block_stmt(case_block->block,new_tab);
			write_body(new_tab + "break;\n ");
		}


		if(stmt->default_block != nullptr)
		{
			write_body(new_tab + "default:\n");
			convert_block_stmt(stmt->default_block->block,new_tab );
			write_body(new_tab + "break;\n ");
		}

		write_body(tab + "}\n");
	}

	void convert_array_init(ASTVarInit *init,std::string tab)
	{
		ASTVarArrayInit *Array = (ASTVarArrayInit *)init->init;
	
		write_body("{");
		
		int i = 0;
		int end = Array->elements.size() - 1;
		for(ASTVarInit *tmp : Array->elements)
		{
			convert_init(tmp,tab);
			if (i++ == end)
			{
				break;
			}

			write_body(",");
		}

		write_body("}");
	}


	std::string convert_init(ASTVarInit *init,std::string tab)
	{
		std::string new_line;
		if(init->type == ASTVarInitType::SINGLE)
		{
			convert_expr(((ASTVarSingleInit *)init->init)->expr);
		}
		else if(init->type == ASTVarInitType::ARRAY)
		{
			convert_array_init(init,tab);
		}
		else if(init->type == ASTVarInitType::STRUCT)
		{
			ASTVarStructMember map = ((ASTVarStructInit *)init->init)->members;
			write_body("(struct " + ((ASTVarStructInit *)init->init)->ident + "){\n");
			for (auto it = map.table.begin(); it != map.table.end(); ++it)
			{
				write_body(tab + "\t." + it->first + " = ");
				convert_expr(it->second);
				write_body(",\n");  
			}
			write_body(tab + "}");
			new_line = "\n";
		}

		return new_line;
	}


	void convert_vardecl_stmt(ASTVarDecl *stmt,std::string tab)
	{
		write_body(tab);
		
		std::vector<ASTExpression *> exprs;

		if(stmt->type->type_type == ASTDataType::ARRAY)
		{
			ASTArray *Array = (ASTArray *)stmt->type->type;
			exprs.push_back(Array->size);
			while(true)
			{
				if(Array->type->type_type == ASTDataType::ARRAY)
				{
					Array = (ASTArray *)Array->type->type;
					exprs.push_back(Array->size);
				}
				else
				{
					convert_type(Array->type);
					break;
				}
			}
		}
		else
		{
			convert_type(stmt->type);
		}

		write_body(stmt->true_ident);

		for(int i = exprs.size() - 1; i >= 0 ; i--)
		{
			ASTExpression *expr = exprs[i];
			write_body("[");
			convert_expr(expr);
			write_body("]");
		}

		write_body(" = ");

		std::string new_line = convert_init(stmt->init,tab);

		
		write_body(";\n" + new_line);
	}



	void convert_return_stmt(ASTReturnStmt *stmt,std::string tab)
	{
		write_body(tab);
		write_body("return ");
		convert_expr(stmt->expr);
		write_body(";\n");
	}


	void convert_expr(ASTExpression *expr,bool write = true)
	{
		switch (expr->type)
		{
			case ASTExpressionType::CAST:
			{
				convert_cast_expr(expr->expr);
				break;
			}
			case ASTExpressionType::STRING:
			{
				convert_string_expr(expr->expr);
				break;
			}
			case ASTExpressionType::I32:
			{
				convert_i32_expr(expr->expr);
				break;
			}
			case ASTExpressionType::I64:
			{
				convert_i64_expr(expr->expr);
				break;
			}
			case ASTExpressionType::U32:
			{
				convert_u32_expr(expr->expr);
				break;
			}
			case ASTExpressionType::U64:
			{
				convert_u64_expr(expr->expr);
				break;
			}
			case ASTExpressionType::F32:
			{
				convert_f32_expr(expr->expr);
				break;
			}
			case ASTExpressionType::F64:
			{
				convert_f64_expr(expr->expr);
				break;
			}
			case ASTExpressionType::FUNCTION_CALL:
			{
				convert_function_call_expr(expr->expr);
				break;
			}
			case ASTExpressionType::VARIABLE:
			{
				return convert_variable_expr(expr->expr);
				break;
			}
			case ASTExpressionType::SELF:
			{
				convert_self_expr(expr->expr);
				break;
			}
			case ASTExpressionType::ADDRESS_OF:
			{
				convert_address_of_expr(expr->expr);
				break;
			}
			case ASTExpressionType::PTR_READ:
			{
				write_body("(");
				convert_ptr_read_expr(expr->expr);
				write_body(")");
				break;
			}
			case ASTExpressionType::PTR_WRITE:
			{
				write_body("(");
				convert_ptr_write_expr(expr->expr);
				write_body(")");
				break;
			}
			case ASTExpressionType::PTR_OFFSET:
			{
				write_body("(");
				convert_ptr_offset_expr(expr->expr);
				write_body(")");
				break;
			}
			case ASTExpressionType::ASSIGN:
			{
				convert_assign_expr(expr->expr);
				break;
			}
			case ASTExpressionType::ENUM_ACCESS:
			{
				convert_enum_access_expr(expr->expr);
				break;
			}
			case ASTExpressionType::STRUCT_ACCESS:
			{
				return convert_struct_access_expr(expr->expr);
				break;
			}
			case ASTExpressionType::STRUCT_METHOD_CALL:
			{
				//DEBUG_PANIC("call");
				return convert_struct_method_call_expr(expr->expr);
				break;
			}
			case ASTExpressionType::STRUCT_PTR_ACCESS:
			{
				return convert_struct_ptr_access_expr(expr->expr);
				break;
			}
			case ASTExpressionType::UNARY:
			{
				convert_unary_expr(expr->expr);
				break;
			}
			case ASTExpressionType::BINARY:
			{
				convert_binary_expr(expr->expr);
				break;
			}
		}

	}



	void convert_struct_ptr_access_expr(void *expr)
	{	
		convert_expr(((ASTStructPtrAccessExpr *)expr)->base);
		write_body("->" + ((ASTStructPtrAccessExpr *)expr)->member);
	}


	void convert_struct_access_expr(void *expr)
	{
		convert_expr(((ASTStructAccessExpr *)expr)->base);

		write_body("." + ((ASTStructAccessExpr *)expr)->member);
	}

	void convert_struct_method_call_expr(void *expr)
	{
		ASTStructMethodCallExpr *Struct = (ASTStructMethodCallExpr *)expr;

		
		write_body(Struct->prefix + "_" + Struct->member + "(&");
		convert_expr(Struct->base);

		int arg_length = Struct->args.size();

	
		if(arg_length > 0)
		{
			write_body(",");
		}

		int i = 0;

		for ( ASTExpression *arg : Struct->args)
		{
			if(arg == nullptr)
			{
				break;
			}

			convert_expr(arg);
			if(i++ + 1 >= arg_length)
			{
				break;
			}

			write_body(",");
		}

		write_body(")");

	}



	void convert_enum_access_expr(void *expr)
	{
		write_body(((ASTEnumAccessExpr *)expr)->base + "_" + ((ASTEnumAccessExpr *)expr)->member);
	}

	void convert_string_expr(void *expr)
	{
		write_body("\"" + ((ASTStringExpr *)expr)->value +"\"");
	}

	void convert_i32_expr(void *expr)
	{
		write_body(std::to_string(((ASTI32Expr *)expr)->value));
	}


	void convert_i64_expr(void *expr)
	{
		write_body(std::to_string(((ASTI64Expr *)expr)->value));
	}



	void convert_u64_expr(void *expr)
	{
		write_body(std::to_string(((ASTU64Expr *)expr)->value));
	}


	void convert_u32_expr(void *expr)
	{
		write_body(std::to_string(((ASTU32Expr *)expr)->value));
	}




	void convert_f64_expr(void *expr)
	{
		write_body(std::to_string(((ASTF64Expr *)expr)->value));
	}


	void convert_f32_expr(void *expr)
	{
		write_body(std::to_string(((ASTF32Expr *)expr)->value));
	}


	void convert_function_call_expr(void *expr)
	{
		ASTFunctionCallExpr *fn_expr = (ASTFunctionCallExpr *)expr;
		
		convert_expr(fn_expr->base,true);

		write_body("(");

		int arg_length = fn_expr->args.size();

		int i = 0;

		for ( ASTExpression *arg : fn_expr->args)
		{
			convert_expr(arg);
			if(i++ + 1 >= arg_length)
			{
				break;
			}

			write_body(",");
		}

		write_body(")");
	}



	void convert_variable_expr(void *expr)
	{
		ASTVariableExpr *var_expr = (ASTVariableExpr *)expr;
		write_body(var_expr->true_ident);
	}



	void convert_self_expr(void *expr)
	{
		ASTSelfExpr *self_expr = (ASTSelfExpr *)expr;
		write_body(self_expr->ident);
	}



	void convert_address_of_expr(void *expr)
	{
		ASTAddressOfExpr *address_of_expr = (ASTAddressOfExpr *)expr;
		write_body("&");
		convert_expr(address_of_expr->expr);
	}



	void convert_ptr_read_expr(void *expr)
	{
		ASTPtrReadExpr *ptr_read_expr = (ASTPtrReadExpr *)expr;
		write_body("*");
		convert_expr(ptr_read_expr->expr);
	}



	void convert_ptr_write_expr(void *expr)
	{
		ASTPtrWriteExpr *ptr_write = (ASTPtrWriteExpr *)expr;
		write_body("*");
		convert_expr(ptr_write->expr);
		write_body(" = ");
		convert_expr(ptr_write->data);

	}



	void convert_ptr_offset_expr(void *expr)
	{
		ASTPtrOffsetExpr *ptr_write = (ASTPtrOffsetExpr *)expr;
		convert_expr(ptr_write->expr);
		write_body(" + ");
		convert_expr(ptr_write->offset);

	}


	void convert_assign_expr(void *expr)
	{
		ASTAssignExpr *assign_expr = (ASTAssignExpr *)expr;
		convert_expr(assign_expr->lhs);
		write_body(" = ");
		convert_expr(assign_expr->rhs);
	}



	void convert_cast_expr(void *expr)
	{
		ASTCastExpr *cast_expr = (ASTCastExpr *)expr;
	
		std::string data_type;
		switch(cast_expr->type)
		{
			case ASTDataType::I32:
			{
				data_type = "int";
				break;
			}
			case ASTDataType::I64:
			{
				data_type = "long int";
				break;
			}
			case ASTDataType::U32:
			{
				data_type = "unsigned int";
				break;
			}
			case ASTDataType::U64:
			{
				data_type = "unsigned long int";
				break;
			}
			case ASTDataType::F32:
			{
				data_type = "float ";
				break;
			}
			case ASTDataType::F64:
			{
				data_type = "double ";
				break;
			}
			default:
			{
				break;
			}
		}

		write_body("(" + data_type + ")");
		convert_expr(cast_expr->rhs);
	}
	

	void convert_binary_expr(void *expr)
	{
		ASTBinaryExpr *bin_expr = (ASTBinaryExpr *)expr;

		convert_expr(bin_expr->lhs);
		convert_binop(bin_expr->op);
		convert_expr(bin_expr->rhs);
	}



	void convert_unary_expr(void *expr)
	{
		convert_unop(((ASTUnaryExpr *)expr)->op);
		convert_expr(((ASTUnaryExpr *)expr)->rhs);
	}





	void convert_unop(ASTUnaryOperator op)
	{
		switch (op)
		{
			case ASTUnaryOperator::NEGATE:
			{
				write_body("-");
				break;
			}
			case ASTUnaryOperator::COMPLEMENT:
			{
				write_body("~");
				break;
			}
		}
		
	}





	void convert_binop(ASTBinaryOperator op)
	{
		switch (op)
		{
			case ASTBinaryOperator::ADD:
			{
				write_body(" + ");
				break;
			}
			case ASTBinaryOperator::SUB:
			{
				write_body(" - ");
				break;
			}
			case ASTBinaryOperator::LESS:
			{
				write_body(" < ");
				break;
			}
			case ASTBinaryOperator::LESS_EQUAL:
			{
				write_body(" <= ");
				break;
			}
			case ASTBinaryOperator::GREATER:
			{
				write_body(" > ");
				break;
			}
			case ASTBinaryOperator::GREATER_EQUAL:
			{
				write_body(" >= ");
				break;
			}
			case ASTBinaryOperator::AND:
			{
				write_body(" && ");
				break;
			}
			case ASTBinaryOperator::OR:
			{
				write_body(" || ");
				break;
			}
			case ASTBinaryOperator::EQUAL:
			{
				write_body(" == ");
				break;
			}

		}
	}


};


#endif