#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ZERO_LEXEM 1
#define END_OF_STR 2
#define EXPECTED_OTHER_TYPE 3

typedef enum {NaS = 0, SUBTRACTION = 1, ADDITION = 2, \
			  MULTIPLICATION = 3, DIVISION = 4} sign_t;

typedef enum {UNDEFINED, NUMBER, SIGN, BRACKET} value_t;

typedef enum {NUMBER_EXPR, SIGN_EXPR, BRACKET_EXPRESSION} expr_type;

typedef struct {
  value_t type;
  sign_t sign;
  char* lexem;
} lexem_t;

typedef union {
  sign_t sign;
  float value;
} expr_value;

typedef struct expr_st{
  struct expr_st** subexpr;
  size_t subexpr_amount;
  size_t subexpr_capacity;

  expr_type type;
  expr_value value;
  
  int is_negative;
  int is_calculated;
} expr_t;

// Error opcode
int error = 0;
// Expression to parse
char* str;


sign_t issign(char c)
{
  switch (c) {
  case '+':
	return ADDITION;
  case '-':
	return SUBTRACTION;
  case '*':
	return MULTIPLICATION;
  case '/':
	return DIVISION;
  default:
	return NaS;
  }
}


void free_expression(expr_t* expr)
{
  if (expr->type != BRACKET_EXPRESSION)
	free(expr);
  /* else { */
  /* 	for (size_t i = 0; i < expr->subexpr_amount; i++) { */
  /* 	  if (expr->subexpr[i]->type == BRACKET_EXPRESSION) */
  /* 		free(expr->subexpr[i]); */
  /* 	  else */
  /* 		free_expression(expr->subexpr[i]); */
  /* 	} */
  
  /* 	if (expr->subexpr_capacity > 0) */
  /* 	  free(expr->subexpr); */
  /* } */
}

void
get_lexem(value_t expected_t, lexem_t* lexem)
{
  // Initial block
  char c;
  int value_started = 0, value_negative = 0; 
  size_t length = strlen(str), lexem_length = 0;
  
  if (length == 0) {
	error = ZERO_LEXEM;
	return;
  }
  
  memset(lexem->lexem, 0, length + 1);
  lexem->type = expected_t;
  

  
  while (1) {
	// If expected sign
	if (expected_t == SIGN) {
	  // EOF
	  if (strlen(str) == 0) {
		error = END_OF_STR;
		return;
	  }
	  c = str[0];
	  
	  // Bracket
	  if (c == '(' || c == ')') {
		lexem->lexem[0] = c;
		lexem->sign = ADDITION;
		str = str + 1;
		lexem->type = BRACKET;
		return;
	  }

	  // Space
	  if (c == ' ') {
		str = str + 1;
		continue;
	  }

	  // Sign
	  if (issign(c)) {
		lexem->lexem[0] = c;
		str = str + 1;
		lexem->type = SIGN;
		return;
	  }

	  // Anythings else
	  error = EXPECTED_OTHER_TYPE;
	  return;
	} // end of if for Sign

	// If expected number
	else if(expected_t == NUMBER) {
	  // For empty result
	  if (!value_started && !value_negative) {
		// EOF
		if(strlen(str) == 0) {
		  error = END_OF_STR;
		  return;
		}
		c = str[0];

		// Bracket
		if (c == '(' || c == ')') {
		  lexem->type=BRACKET;
		  lexem->sign = ADDITION;
		  str = str + 1;
		  lexem->lexem[0] = c;
		  return;
		}

		// Space
		if (c == ' ') {
		  str = str + 1;
		  continue;
		}

		// Number or point
		if (isdigit(c) || c == '.') {
		  str = str + 1;
		  lexem->lexem[0] = c;
		  lexem_length++;
		  value_started = 1;
		  continue;
		}

		// Minus
		if (issign(c) == SUBTRACTION) {
		  str = str + 1;
		  lexem->lexem[0] = c;
		  lexem_length++;
		  value_negative = 1;
		  continue;
		}

		// Anything else		
		error = EXPECTED_OTHER_TYPE;
		return;
	  } // End of if for empty result	
	    

	  // Value 
	  else if(value_started) {
		// EOF
		if (strlen(str) == 0) {
		  error = END_OF_STR;
		  return;
		}
		c = str[0];

		// Number
		if (isdigit(c) || c == '.') {
		  lexem->lexem[lexem_length] = c;
		  lexem_length++;
		  str = str + 1;
		  continue;
		}

		// Any other
		return;
	  } // end of if for Value

	  // Minus without value
	  else if (!value_started && value_negative) {
		// EOF
		if (strlen(str) == 0) {
		  error = EXPECTED_OTHER_TYPE;
		  return;
		}
		c = str[0];
		
		// Bracket
		if (c == '(' || c == ')') {
		  lexem->type = BRACKET;
		  lexem->sign = SUBTRACTION;
		  lexem->lexem[0] = c;
		  str = str + 1;
		  return;
		}

		// Number
		if (isdigit(c)) {
		  lexem->lexem[lexem_length] = c;
		  lexem_length++;
		  value_started = 1;
		  str = str + 1;
		  continue;
		}

		// Else
		error = EXPECTED_OTHER_TYPE;
		return;
	  } // end of if for Minus withour value
  
	} // End of if for Number
  }
}

// Math operations
void calculate_expression(expr_t* expr)
{
  expr_t *a, *b, *c; 

  // If only one subexpression
  if (expr->subexpr_amount == 1 && expr->subexpr[0]->type == BRACKET_EXPRESSION) {
	calculate_expression(expr->subexpr[0]);
	goto END;
  }
  
  // Find multiplications and divisions
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	if (expr->subexpr[i]->type == SIGN_EXPR && \
		(expr->subexpr[i]->value.sign == MULTIPLICATION || \
		 expr->subexpr[i]->value.sign == DIVISION)) {
	  
	  a = expr->subexpr[i - 1];
	  b = expr->subexpr[i];
	  c = expr->subexpr[i + 1];

	  // If a or c expression, calculate them
	  if (a->type == BRACKET_EXPRESSION && \
		  !a->is_calculated)
		calculate_expression(a);
	  if (c->type == BRACKET_EXPRESSION && \
		  !c->is_calculated)
		calculate_expression(c);

	  // Make math operations
	  if (b->value.sign == MULTIPLICATION)
		a->value.value *= c->value.value;
	  else
		a->value.value /= c->value.value;

	  // a = c, and free c
	  free_expression(c);
	  expr->subexpr[i + 1] = expr->subexpr[i - 1];
	  /* c = a;  */
	}  
  }

  // Find additions and subtractions
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	if (expr->subexpr[i]->type == SIGN_EXPR && \
		(expr->subexpr[i]->value.sign == ADDITION ||\
		 expr->subexpr[i]->value.sign == SUBTRACTION)) {
	  a = expr->subexpr[i - 1];
	  b = expr->subexpr[i];
	  c = expr->subexpr[i + 1];
	  
	  // If a or c expression, calculate them
	  if (a->type ==  BRACKET_EXPRESSION &&\
		  !a->is_calculated)
		calculate_expression(a);
	  if (c->type == BRACKET_EXPRESSION &&\
		  !c->is_calculated)
		calculate_expression(c);

	  if (b->value.sign == ADDITION)
		a->value.value += c->value.value;
	  else
		a->value.value -= c->value.value;

	  expr->subexpr[i + 1] = expr->subexpr[i - 1];
	}
  }


 END:
  
  expr->value.value = expr->subexpr[0]->value.value; 
  
  if (expr->is_negative)
	expr->value.value *= -1;
  
  expr->is_calculated = 1;
}

// Create expression from lexems
void create_expression(expr_t* expr, value_t expected_t, lexem_t lexem)
{
  expr_t* new_expr;
  
  while(error == 0) {	
	get_lexem(expected_t, &lexem);

	/* // ERROR	 */
	/* printf("%s %d\n", lexem.lexem, lexem.sign); */
	 
	/* /\* else *\/ */
	/* /\*   break; *\/ */

	// If lexem is bracket 
	if (lexem.type == BRACKET) {
	
	  // Expected type changing
	  /* if(lexem.type == BRACKET) { */
	  /* 	if (lexem.lexem[0] == '(') */
	  /* 	  expected_t = NUMBER; */
	  /* 	else */
	  /* 	  expected_t = SIGN; */
	  /* } */

	  expected_t = SIGN;
	  

	  // Call recursion if (
	  if (lexem.lexem[0] == '(') {
		new_expr = (expr_t*)malloc(sizeof(expr_t));
		
		// If not enough space for new expression
		if (expr->subexpr_capacity <= expr->subexpr_amount) {
		  expr->subexpr = (expr_t**)realloc(expr->subexpr, \
											expr->subexpr_capacity * 2);
		  expr->subexpr_capacity *= 2;
		  
		}

		// Add new expression
		expr->subexpr[expr->subexpr_amount] = new_expr;
		expr->subexpr_amount++;
		new_expr->is_calculated = 0;
		new_expr->subexpr = (expr_t**)malloc(sizeof(expr_t) * 64);
		new_expr->subexpr_capacity = 64;
		new_expr->subexpr_amount = 0;
		new_expr->type = BRACKET_EXPRESSION;

		// Check if negative
		if (lexem.sign == SUBTRACTION)
		  new_expr->is_negative = 1;
		else
		  new_expr->is_negative = 0;

		// Recursion
		create_expression(new_expr, NUMBER, lexem);
	  } // if (

	  else {
		return;
	  }
	} // if for Bracket

	// If lexem is a sign or number
	else {
	  // Add new expression to current
	  new_expr = (expr_t*)malloc(sizeof(expr_t));
	  new_expr->is_calculated = 1;	  

	  // If number
	  if (lexem.type == NUMBER) {
		new_expr->type = NUMBER_EXPR;
		new_expr->value.value = atof(lexem.lexem);
	  }
	  // If sign
	  else if (lexem.type == SIGN) {
		new_expr->type = SIGN_EXPR;
		new_expr->value.sign = issign(lexem.lexem[0]);
	  }
	  
	  // If not enough space for new expression
	  if (expr->subexpr_capacity <= expr->subexpr_amount) {
		expr->subexpr = (expr_t**)realloc(expr->subexpr, \
										  expr->subexpr_capacity * 2);
		expr->subexpr_capacity *= 2;
		  
	  }
	  expr->subexpr[expr->subexpr_amount] = new_expr;
	  expr->subexpr_amount++;
		  
	  // If not bracket change to the opposite and add to the current expr	
	  expected_t = (expected_t == NUMBER) ? SIGN : NUMBER;
	}
	
  }

  // ERROR
  if (error == EXPECTED_OTHER_TYPE) {
	printf("Expectred other type\n\n");
	return;
  }
  
}

// evaluate expression
void evaluate(char* str_)
{
  // main expression 
  expr_t* main_expr = (expr_t*)malloc(sizeof(expr_t));
  main_expr->type = BRACKET_EXPRESSION;
  main_expr->subexpr_capacity = 64;
  main_expr->subexpr_amount = 0;
  main_expr->subexpr = (expr_t**)malloc(8 * main_expr->subexpr_capacity);
    
  str = str_;
  error = 0;

  // lexem
  char* lexem_text = (char*)malloc(strlen(str));
  lexem_t lexem;
  value_t expected_t = NUMBER;
  lexem.lexem = lexem_text;

  create_expression(main_expr, expected_t, lexem);
  calculate_expression(main_expr);
  printf("%s = %f\n", str_, main_expr->value.value);

  free_expression(main_expr);
  free(lexem_text);
}


int main()
{
  char* tests[] = {"1-1", "1 -1", "1- 1", "1 - 1", "1- -1", \
				   "1 - -1", "1--1", "6 + -(4)", "6 + -( -4)", \
				   "25 --10 *  -12", "1- -1", \
				   "324+234*-6846.45", "(2 / (2 + 3.33) * 4) - -6", \
				   "-(5+10)"};
  size_t tests_length = 14;

  
  for (size_t i = 0; i < tests_length; i++) {
	evaluate(tests[i]);
  }
  
  return 0;
}
