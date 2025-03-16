/*
URL: https://www.codewars.com/kata/53005a7b26d12be55c000243/c
Name: Simpler Interactive Interpreter
Difficulty: 2 kyu
Notes: This code was modified before sending for tests because of some codewars requirements
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ZERO_LEXEM 2
#define END_OF_STR 3
#define EXPECTED_OTHER_TYPE 4
#define INVALID_IDENTIFIER 5

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
  int value;
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
  int value;  
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
  // If not bracket
  if (expr->type != BRACKET_EXPRESSION)
	free(expr);
  
  // If bracket
  else {
	if (expr->subexpr_amount > 0)
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

// Check is this string is a emtpy string
int is_empty(char* string)
{
  int flag = 1;
  for (size_t i = 0; i < strlen(string); i++) {
	if (string[i] != ' ' && string[i] != '\n' && string[i] != '\0') {
	  flag = 0;
	}
  }

  return flag;
}


void get_lexem(value_t expected_t, lexem_t* lexem)
{
  // Initial block
  char c;
  int value_started = 0, value_negative = 0, is_variable = 0; 
  size_t length = strlen(str), lexem_length = 0;

  //If string is empty 
  if (is_empty(str)) {
	lexem->type = UNDEFINED;
	error = ZERO_LEXEM;
	return;
  }
  
  memset(lexem->lexem, 0, length);
  lexem->type = expected_t;  
    
  while (1) {
	// If expected sign
	if (expected_t == SIGN) {
	  // EOF
	  if (strlen(str) == 0 || str[0] == '\n') {
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
		if(strlen(str) == 0 || str[0] == '\n') {
		  error = END_OF_STR;
		  return;
		}
		c = str[0];

		// Bracket
		if (c == '(' || c == ')') {
		  lexem->type = BRACKET;
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

		// Variable
		if (c == '_' || isalpha(c)) {
		  is_variable = 1;
		  lexem->type = VARIABLE;
		  lexem->sign = ADDITION;
		  lexem->lexem[0] = c;
		  lexem_length++;
		  value_started = 1;
		  str = str + 1;
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
		if (strlen(str) == 0 || str[0] == '\n') {
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

		// Variable
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
		if (strlen(str) == 0 || str[0] == '\n') {
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

		// Variable 
		if (c == '_' && isalpha(c)) {
		  is_variable = 1;
		  value_started = 1;
		  lexem->lexem[0] = c;
		  lexem->type = VARIABLE;
		  lexem->sign = SUBTRACTION;
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
	  else if (b->value.sign == DIVISION) {
		if (c->value.value == 0) {
		  error = EXPECTED_OTHER_TYPE;
		  return;
		}
		a->value.value /= c->value.value;
	  }
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
int get_indentifier(const char* name) {
  for (size_t i = 0; i < indentifiers.length; i++) {
	if (!strcmp(name, indentifiers.indentifiers[i].name)) {
	  return indentifiers.indentifiers[i].value;
	}
  }

  error =  INVALID_IDENTIFIER;
}

int recursion_counter = 1;
// Create expression from lexems
void create_expression(expr_t* expr, value_t expected_t, lexem_t lexem)
{
  int value_required = 0;
  int val;
  expr_t* new_expr;
  
  while(error == 0) {	
	get_lexem(expected_t, &lexem);

	// If lexem is bracket 
	if (lexem.type == BRACKET) {
	  expected_t = SIGN;
	  value_required = 0;

	  // Call recursion if (
	  if (lexem.lexem[0] == '(') {
		recursion_counter++;
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

	  else if(lexem.lexem[0] == ')') {
		recursion_counter--;
		return;
	  }
	  else {
		return;
	  }
	} // if for Bracket

	// If lexem is a sign, number or variable
	// In case of variable, get value of this variable and use NUMBER_EXPR
	else if (lexem.type == UNDEFINED) {
	  if (value_required)
		error = EXPECTED_OTHER_TYPE;
	  return;
	}
	else {
	  // Add new expression to current
	  new_expr = (expr_t*)malloc(sizeof(expr_t));
	  new_expr->is_calculated = 1;	  

	  // If number
	  if (lexem.type == NUMBER) {
		value_required = 0;
		new_expr->type = NUMBER_EXPR;
		new_expr->value.value = atof(lexem.lexem);
	  }
	  // If sign
	  else if (lexem.type == SIGN) {
		value_required = 1;
		new_expr->type = SIGN_EXPR;
		new_expr->value.sign = issign(lexem.lexem[0]);
	  }
	  // If variable
	  else if (lexem.type == VARIABLE) {
		value_required = 0;
		val = get_indentifier(lexem.lexem);
		// check if variable is negative
		if (lexem.sign == SUBTRACTION)
		  val *= -1;
		
		// Indentifier doesn't exist
		if (error == INVALID_IDENTIFIER) {
		  printf("ERROR: Invalid identifier. No variable with name '%s' was found.\n", lexem.lexem);
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
  if (recursion_counter != 1 || value_required)
	error = EXPECTED_OTHER_TYPE;
}

// evaluate expression
int evaluate_expr(const char* str_)
{
  int result;
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
  recursion_counter = 1;

  // lexem
  char* lexem_text = (char*)malloc(strlen(str_));
  lexem_t lexem;
  value_t expected_t = NUMBER;
  lexem.lexem = lexem_text;

  create_expression(main_expr, expected_t, lexem);

  if (error != INVALID_IDENTIFIER && error != EXPECTED_OTHER_TYPE)
	calculate_expression(main_expr);

  result = main_expr->value.value;
	
  free_expression(main_expr);
  free(lexem_text);

  return result;
}

// Create new indentifier or change value of already created
void set_indentifier(char* name, int value) {
  int flag = 1;
  // Detele space in name
  for (size_t i = 0; i < strlen(name); i++) {
	// Skip spaces before variable
	if (!flag && (name[i] == '_' || isalpha(name[i])))
	  flag = 1;
	  
	if (name[i] == ' ' && flag) {
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
  // Do nothing if string is empty
  if (is_empty(original_expr))
	return;
  
  size_t index;
  int is_assignment = 0;
  char *indent, *expr;  
  int result;

  indent = (char*)malloc(strlen(original_expr));
  expr = (char*)malloc(strlen(original_expr));
  
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
	indent[index] = '\0';
	memcpy(expr, original_expr + index + 1, strlen(original_expr) - index - 1);
	expr[strlen(original_expr) - index - 1] = '\0';
	
	if(is_empty(indent) || is_empty(expr)) {
	  error = INVALID_IDENTIFIER;
	  return;
	}
	
	result = evaluate_expr(expr);
	// Check if function evaluate expr finished without error
	if (error != EXPECTED_OTHER_TYPE || error != INVALID_IDENTIFIER)	
	  set_indentifier(indent, result);


  }
  // If expression -> calculate
  else {
	result = evaluate_expr(original_expr);
  }


  // Check for error and output result
  if (error != INVALID_IDENTIFIER && error != EXPECTED_OTHER_TYPE) 
    printf("%d\n", result);
  

  free(indent);
  free(expr);
}


int main()
{
  size_t buf_size;  
  char* tests[] = {"x = 76", "x + 6", "y + 7", "x - -4\n"}, *buffer;
  size_t tests_length = 4;

  // Init for indentifiers array   
  indentifiers.indentifiers = (indentifier_t*)malloc(sizeof(indentifier_t) * 32);
  indentifiers.capacity = 32;
  indentifiers.length = 0;

  for (size_t i = 0; i < tests_length; i++) {
	evaluate(tests[i]);
  }

  #ifdef TEST
  while (1) {
	getline(&buffer, &buf_size, stdin);
	evaluate(buffer);
  }
  #endif

  // free indentifiers
  for(size_t i = 0; i < indentifiers.length; i++) {
	free(indentifiers.indentifiers[i].name);
  }

  return 0;
}
