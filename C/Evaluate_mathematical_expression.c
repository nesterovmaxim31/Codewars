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
typedef enum {NUMBER, SIGN, EXPRESSION} expr_type;

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
  struct exrp_st** subexpr;
  size_t subexpr_amount;
  size_t subexpr_capacity;

  expr_t type;
  expr_value value;
  
  int is_negative;
  int is_alive;
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
  for (size_t i = 0; i < expr->subexpr_capacity; i++) {
	if (expr->subexpr[i]->type != EXPRESSION)
	  free(expr->subexpr[i]);
	else
	  free_expression(expr->subexpr[i]);
  }
  
  if (expr->subexpr_capacity > 0)
	free(expr->subexpr);  
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
  lexem->type = UNDEFINED;

  
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
void calculate_expression(expr_t* expr) {
  expr_t *a, *b, *c; 
  // Find multiplications and divisions
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	if (expr->subexpr[i]->type == SIGN && (expr->subexpr[i]->value.sign == \
			  MULTIPLICATION || expr->subexpr[i]->value.sign == DIVISION)) {
	  a = expr->subexpr[i - 1];
	  b = expr->subexpr[i];
	  c = expr->subexpr[i + 1];

	  // If a or c expression, calculate them
	  if (a.type ==  EXPRESSION)
		calculate_expression(a);
	  if (c.type == EXPRESSION)
		calculate_expression(c);

	  // Make math operations
	  if (b.value.sign == MULTIPLICATION)
		a.value.value *= c.value.value;
	  else (b.value.sign == DIVISION)
		 a.value.value /= c.value.value;

	  // a = c, and free c
	  free_expression(c);
	  c = a; 
	}  
  }

  // Find additions and subtractions
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	
  }

  expr.is_calculated = 1;
}

// Create expression from lexems
void create_expression(expr_t* expr, value_t expected_t, lexem_t lexem)
{
  while(error == 0) {	
	get_lexem(expected_t, &lexem);

	// creating new subexpression
	if (error != EXPECTED_OTHER_TYPE)
	  printf("%s %d\n", lexem.lexem, lexem.sign);
	else
	  break;

	
	// If bracket, expected type also changing
	if(lexem.type == BRACKET) {
	  if (lexem.lexem[0] == '(')
		expected_t = NUMBER;
	  else
		expected_t = SIGN;
	}
	
	// If not bracket change to the opposite and add to the current expr	
	else {
	  
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
  expr_t main_expr;
  main_expr.is_alive = 1;
  main_expr.subexpr_capacity = 64;
  main_expr.subexpr_amount = 0;
  main_expr.subexpr = (expr_t*)malloc(8 * main_expr.subexpr_capacity);
  
  
  str = str_;
  error = 0;

  // lexem
  char* lexem_text = (char*)malloc(strlen(str));
  lexem_t lexem;
  value_t expected_t = NUMBER;
  lexem.lexem = lexem_text;

  create_expression(main_expr, expected_t, lexem);
  calculate_expression(main_expr);
  
  free(lexem_text);
}


int main()
{
  char* tests[] = {"1-1", "1 -1", "1- 1", "1 - 1", "1- -1", \
				   "1 - -1", "1--1", "6 + -(4)", "6 + -( -4)", \
				   "25 --10 *  -12", "1 - - 1", "1- -1", \
				   "6 + - (4)", "6 + -(- 4)", "324+234*-6846.45",\
				   "(2 / (2 + 3.33) * 4) - -6", "-(5+10)"};
  size_t tests_length = 17;

  
  for (size_t i = 0; i < tests_length; i++) {
	evaluate(tests[i]);
  }
  
  return 0;
}
