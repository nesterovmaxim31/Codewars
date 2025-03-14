#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ZERO_LEXEM 1
#define END_OF_STR 2
#define EXPECTED_OTHER_TYPE 3
#define INVALID_IDENTIFIER 4

#define MOD(x, y) (x - (int)(x/y) * y)

typedef enum {NaS = 0, SUBTRACTION = 1, ADDITION = 2, \
			  MULTIPLICATION = 3, DIVISION = 4, MODULUS = 5} sign_t;

typedef enum {UNDEFINED, NUMBER, SIGN, BRACKET, VARIABLE} value_t;

typedef enum {NUMBER_EXPR, SIGN_EXPR, BRACKET_EXPRESSION} expr_type;

typedef struct {
  value_t type;
  sign_t sign;
  char* lexem;
} lexem_t;

typedef union {
  sign_t sign;
  double value;
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

typedef struct indentifier_st {
  char* name;
  double value;  
} indentifier_t;

struct indendtifiers_set_st {
  indentifier_t* indentifiers;
  size_t length;
  size_t capacity;
} indentifiers;

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
  case '%':
    return MODULUS;
  default:
    return NaS;
  }
}


void free_expression(expr_t* expr)
{
  if (expr->type != BRACKET_EXPRESSION)
	free(expr);
  
  else {
	free(expr->subexpr[0]);
	
	for (size_t i = 1; i < expr->subexpr_amount; i+=2) {
	  if (expr->subexpr[i]->type == SIGN_EXPR) {
		free(expr->subexpr[i]);
	  }
	  else
		free_expression(expr->subexpr[i]);

	  expr->subexpr[i] = NULL; 
	}
  
	if (expr->subexpr_capacity > 0)
	  free(expr->subexpr);
	
	free(expr);
  }
}

void get_lexem(value_t expected_t, lexem_t* lexem)
{
  // Initial block
  char c;
  int value_started = 0, value_negative = 0, is_variable = 0; 
  size_t length = strlen(str), lexem_length = 0;
  
  if (length == 0) {
	error = ZERO_LEXEM;
	return;
  }
  
  memset(lexem->lexem, 0, length);
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

		// Number or point and not variable
		if ((isdigit(c) || c == '.') && !is_variable) {
		  str = str + 1;
		  lexem->lexem[0] = c;
		  lexem_length++;
		  value_started = 1;
		  continue;
		}

		// Letter -> variable (name with 1 char) 
		if (isalpha(c)) {
		  lexem->lexem[0] = c;
		  str = str + 1;
		  lexem->type = VARIABLE;
		  return;
		}

		//  _ -> variable
		if (c == '_') {
		  is_variable = 1;
		  lexem->lexem[0] = c;
		  lexem_length++;
		  value_started = 1;
		  str = str + 1;
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
		if ((isdigit(c) || c == '.') && !is_variable) {
		  lexem->lexem[lexem_length] = c;
		  lexem_length++;
		  str = str + 1;
		  continue;
		}

		// Variable started with _
		if (is_variable && (isalnum(c) || c == '_')) {
		  str = str + 1;
		  lexem->lexem[lexem_length] = c;
		  lexem_length++;
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

  // If zero subexpression
  if (expr->subexpr_amount == 0)
    return;
  
  // If only one subexpression
  if (expr->subexpr_amount == 1 && expr->subexpr[0]->type == BRACKET_EXPRESSION) {
	calculate_expression(expr->subexpr[0]);
	expr->value.value = expr->subexpr[0]->value.value; 
	goto END;
  }
  
  // Find multiplications and divisions
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	if (expr->subexpr[i]->type == SIGN_EXPR && \
		(expr->subexpr[i]->value.sign == MULTIPLICATION || \
		 expr->subexpr[i]->value.sign == DIVISION || \
		 expr->subexpr[i]->value.sign == MODULUS)) {
	  
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
	  else if (b->value.sign == DIVISION)
		a->value.value /= c->value.value;
	  else
	    a->value.value = MOD(a->value.value, c->value.value);

	  free_expression(c);
	  expr->subexpr[i + 1] = expr->subexpr[i - 1];
	}  
  }

  // Find additions and subtractions
  // Check if zero expressioin is bracket expression
  if (expr->subexpr[0]->type == BRACKET_EXPRESSION && !expr->subexpr[0]->is_calculated)
    calculate_expression(expr->subexpr[0]);  
  expr->value.value += expr->subexpr[0]->value.value;
  
  for (size_t i = 0; i < expr->subexpr_amount; i++) {
	if (expr->subexpr[i]->type == SIGN_EXPR && \
		(expr->subexpr[i]->value.sign == ADDITION ||\
		 expr->subexpr[i]->value.sign == SUBTRACTION)) {
	  
	  b = expr->subexpr[i];
	  c = expr->subexpr[i + 1];
	  
	  // If c expression, calculate them
	  if (c->type == BRACKET_EXPRESSION &&\
		  !c->is_calculated)
		calculate_expression(c);

	  if (b->value.sign == ADDITION)
		expr->value.value += c->value.value;
	  else
		expr->value.value -= c->value.value;
	}
  }


 END:
  if (expr->is_negative)
	expr->value.value *= -1;
  
  expr->is_calculated = 1;
}

// Get value of indentifier or set error to 'Invalid indentifer'
double get_indentifier(const char* name) {
  for (size_t i = 0; i < indentifiers.length; i++) {
	if (!strcmp(name, indentifiers.indentifiers[i].name)) {
	  return indentifiers.indentifiers[i].value;
	}
  }

  error =  INVALID_IDENTIFIER;
}

// Create expression from lexems
void create_expression(expr_t* expr, value_t expected_t, lexem_t lexem)
{
  double val;
  expr_t* new_expr;
  
  while(error == 0) {	
	get_lexem(expected_t, &lexem);

	// If lexem is bracket 
	if (lexem.type == BRACKET) {
	  expected_t = SIGN;

	  // Call recursion if (
	  if (lexem.lexem[0] == '(') {
		new_expr = (expr_t*)malloc(sizeof(expr_t));
		
		// If not enough space for new expression
		if (expr->subexpr_capacity <= expr->subexpr_amount) {
		  expr->subexpr = (expr_t**)realloc(expr->subexpr, \
											sizeof(expr_t)	* expr->subexpr_capacity * 2 );
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
		new_expr->value.value = 0;

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

	// If lexem is a sign, number or variable
	// In case of variable, get value of this variable and use NUMBER_EXPR
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
	  // If variable
	  else if (lexem.type == VARIABLE) {
		val = get_indentifier(lexem.lexem);		
		// Indentifier doesn't exist
		if (error == INVALID_IDENTIFIER) {
		  printf("Invalid indentifier %s\n", lexem.lexem);
		  return;
		}
		
		new_expr->type = NUMBER_EXPR;
		new_expr->value.value = val;		
	  }
	  
	  // If not enough space for new expression
	  if (expr->subexpr_capacity <= expr->subexpr_amount) {
		expr->subexpr = (expr_t**)realloc(expr->subexpr, \
										  sizeof(expr_t) *  expr->subexpr_capacity * 2);
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
double evaluate_expr(const char* str_)
{
  // main expression 
  expr_t* main_expr = (expr_t*)malloc(sizeof(expr_t));
  main_expr->type = BRACKET_EXPRESSION;
  main_expr->subexpr_capacity = 64;
  main_expr->subexpr_amount = 0;
  main_expr->is_negative = 0;
  main_expr->value.value = 0;
  main_expr->subexpr = (expr_t**)malloc(8 * main_expr->subexpr_capacity);
    
  str = str_;
  error = 0;

  // lexem
  char* lexem_text = (char*)malloc(strlen(str));
  lexem_t lexem;
  value_t expected_t = NUMBER;
  lexem.lexem = lexem_text;

  create_expression(main_expr, expected_t, lexem);

  if (error != INVALID_IDENTIFIER)
	calculate_expression(main_expr);

  /* if (fabs(main_expr->value.value - (int)main_expr->value.value) > 0) */
  /* 	printf("%s = %f\n", str_, main_expr->value.value); */
  /* else */
  /* 	printf("%s = %d\n", str_, (int)main_expr->value.value); */

  // free_expression(main_expr);
  free(lexem_text);

  return main_expr->value.value;
}

// Create new indentifier or change value of already created
void set_indentifier(char* name, double value) {
  // Detele space in name
  for (size_t i = 0; i < strlen(name); i++) {
	if (name[i] == ' ') {
	  name[i] = '\0';
	  break;
	}
  }
  
  // If variable with this name already exist -> change is value
  for (size_t i = 0; i < indentifiers.length; i++) {
	if (!strcmp(name, indentifiers.indentifiers[i].name)) {
	  indentifiers.indentifiers[i].value = value;
	  return;
	}
  }

  // Create a new variable in other case

  // Check for free space
  if (indentifiers.capacity <= indentifiers.length) {
	indentifiers.capacity *= 2; 
	indentifiers.indentifiers = (indentifier_t*)realloc(indentifiers.indentifiers, \
								sizeof(indentifier_t) * indentifiers.capacity);
  }

  indentifiers.indentifiers[indentifiers.length].name = (char*)malloc(strlen(name) + 1);
  strcpy(indentifiers.indentifiers[indentifiers.length].name, name);
  indentifiers.indentifiers[indentifiers.length].value = value;     
  indentifiers.length++;  
}

// evaluate 
void evaluate (char* original_expr)
{
  size_t index;
  int is_assignment = 0;
  char indent[1024], expr[1024];
  double result;
    

  // check if  assignment
  for (size_t i = 0; i < strlen(original_expr); i++) {
    if (original_expr[i] == '=') {
	  index = i;
      is_assignment = 1;
	  break;
    }
  }

  // If assignment -> create new indentifier (or change value of already created)
  if (is_assignment) {
	memcpy(indent, original_expr, index);
	/* indent = strtok(original_expr, "="); */
	memcpy(expr, original_expr + index + 1, index);  
	/* expr = strtok(NULL, "="); */
	result = evaluate_expr(expr);
	set_indentifier(indent, result);
  }
  // If expression -> calculate
  else {
	result = evaluate_expr(original_expr);
  }


  // Check for error and output result
  if (error != INVALID_IDENTIFIER) 
    printf("result = %lf\n", result);
}


int main()
{
  char* tests[] = {"x = 7", "x + 6", "y + 7"};
  size_t tests_length = 3;

  // Init for indentifiers array   
  indentifiers.indentifiers = (indentifier_t*)malloc(sizeof(indentifier_t) * 32);
  indentifiers.capacity = 32;
  indentifiers.length = 0;

  for (size_t i = 0; i < tests_length; i++) {
	evaluate(tests[i]);
  }
  
  return 0;
}
