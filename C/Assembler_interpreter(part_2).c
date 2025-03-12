#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


enum compare_result{equal, greater, less};

typedef struct stack {
  char **values;
  size_t length;
  size_t index;
} stack_t;


stack_t* init_stack() {
  stack_t *stack;

  stack = (stack_t*)malloc(sizeof(stack_t));
  stack->length = 16;
  stack->index = 0;
  stack->values = (char**)malloc(8 * stack->length);
  
  return stack;
}


void push_stack(stack_t *stack, char *value) {
  if (stack->length <= stack->index) {
    stack->length *= 2;
    stack->values=(char**)realloc(stack->values, 8 * stack->length);
  }

  stack->values[stack->index] = value;
  stack->index++;
}


char* pop_stack(stack_t *stack){
  if(stack->index == 0) {
    puts("stack is empty!");
    return NULL;
  }

  else {
    stack->index--;
    return stack->values[stack->index];
  }  
}


void deallocate_stack(stack_t *stack) {
  free(stack->values);
  free(stack);
}
  
// clean program from comments
void erase_comments(char *program) {
  size_t length = strlen(program);
  int flag = 0, quotion_mark = 0;

  for(size_t i = 0; i < length; i++) {
    // comment cleaner
    if(program[i] == ';') {
      flag = 1;
      program[i] = ' ';
    }
    else if(flag && program[i] == '\n') {
      flag = 0;
    }
    else if(flag) {
      program[i] = ' ';
    }

    // comma replace on @
    if (program[i] == 39) {
      quotion_mark++;
    }

    if (program[i] == ',' && quotion_mark % 2 == 0) {
      program[i] = '&';
    }
  }
}


char* assembler_interpreter (const char *program_origin) {
  char *program = (char*)malloc(strlen(program_origin) + 1);
  strcpy(program, program_origin);
  erase_comments(program);
  
  int registers[127], value1, value2, shift = 0, flag = 0;
  char *chunk, *return_value = NULL, label[1024], *saveptr1, \
    *saveptr2, *arg1, *arg2, *subchunk;
  size_t  shift_ = 0;
  enum compare_result res;
  stack_t *stack = init_stack();

  chunk = strtok_r(program, " \n", &saveptr1);
  while(chunk != NULL) {
 
    /* ---------------- end -----------------*/
    if ((!memcmp(chunk, "end", 3))){
      free(program);
      deallocate_stack(stack);
      if(return_value == NULL) {
	return (char *)-1;
      }
      else {
	return return_value;
      }
    }

    else if(flag) {}
    
    /* ---------------- ret -----------------*/
    else if(!memcmp(chunk, "ret", 3)) {
      if(flag)
	flag = 0;
      else {
	
	memmove(program, program_origin, strlen(program_origin));
	erase_comments(program);
	chunk = strtok_r(pop_stack(stack), " \n", &saveptr1);
      }

    }
    
    /* ---------------- mov -----------------*/    
    else if (!memcmp(chunk, "mov", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);

      if (isalpha(arg2[0])) {
	registers[arg1[0]] = registers[arg2[0]];
      }
      else {
	registers[arg1[0]] = atoi(arg2);
      }
    }

    /* ---------------- inc -----------------*/ 
    else if(!memcmp(chunk, "inc", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);
      registers[arg1[0]] = registers[arg1[0]] + 1;
    }

    /* ---------------- dec -----------------*/ 
    else if(!memcmp(chunk, "dec", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);
      registers[arg1[0]] = registers[arg1[0]] - 1;
    }

    /* ---------------- add -----------------*/ 
    else if(!memcmp(chunk, "add", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);
      
      if (isalpha(arg2[0])) {
	registers[arg1[0]] += registers[arg2[0]];
      }
      else {
	registers[arg1[0]] += atoi(arg2);
      }
    }

    /* ---------------- sub -----------------*/ 
    else if(!memcmp(chunk, "sub", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);
      
      if (isalpha(arg2[0])) {
	registers[arg1[0]] -= registers[arg2[0]];
      }
      else {
	registers[arg1[0]] -= atoi(arg2);
      }
    }

    /* ---------------- mul -----------------*/ 
    else if(!memcmp(chunk, "mul", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);
      
      if (isalpha(arg2[0])) {
	registers[arg1[0]] *= registers[arg2[0]];
      }
      else {
	registers[arg1[0]] *= atoi(arg2);
      }
    }

    /* ---------------- div -----------------*/ 
    else if(!memcmp(chunk, "div", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);
      
      if (isalpha(arg2[0])) {
	registers[arg1[0]] /= registers[arg2[0]];
      }
      else {
	registers[arg1[0]] /= atoi(arg2);
      }
    }
    /* ---------------- jmp -----------------*/
    else if(!memcmp(chunk, "jmp", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);
      goto call;
    }

    /* ---------------- cmp -----------------*/ 
    else if(!memcmp(chunk, "cmp", 3)) {
      arg1 = strtok_r(NULL, " \n&", &saveptr1);
      arg2 = strtok_r(NULL, " \n", &saveptr1);
      
      if (isalpha(arg1[0])) {
	value1 = registers[arg1[0]];
      }
      else {
	value1 = atoi(arg1);
      }

      if (isalpha(arg2[0])) {
	value2 = registers[arg2[0]];
      }
      else {
	value2 = atoi(arg2);
      }

      if (value1 == value2)
	res = equal;
      else if(value1 > value2)
	res = greater;
      else if(value1 < value2)
	res = less;
    }

    /* ---------------- jne -----------------*/    
    else if(!memcmp(chunk, "jne", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res != equal) {
	goto call;
      }
    }
    
    /* ---------------- je -----------------*/    
    else if(!memcmp(chunk, "je", 2)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res == equal) {
	goto call;
      }
    }

    /* ---------------- jge -----------------*/
    else if(!memcmp(chunk, "jge", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res == greater || res == equal) {
	goto call;
      }
    }

    /* ---------------- jg -----------------*/
    else if(!memcmp(chunk, "jg", 2)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res == greater) {
	goto call;
      }
    }

    /* ---------------- jle -----------------*/
    else if(!memcmp(chunk, "jle", 3)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res == less || res == equal) {
	goto call;
      }
    }

    /* ---------------- jl -----------------*/
    else if(!memcmp(chunk, "jl", 2)) {
      arg1 = strtok_r(NULL, " \n", &saveptr1);

      if (res == less) {
	goto call;
      }
    }

    /* ---------------- call -----------------*/
    else if(!memcmp(chunk, "call", 4)) {
      arg1 = strtok_r(NULL, " \n\0", &saveptr1);
      push_stack(stack, arg1);
      
    call:
      memmove(program, program_origin, strlen(program_origin));
      erase_comments(program);

      sprintf(label, "%.*s:", (int)strcspn(arg1, " \n\0"),\
	      arg1);
      chunk = strtok_r(strstr((const char*)program, \
		  (const char*)label)," \n", &saveptr1);
    }
    
    /* ---------------- msg -----------------*/
    else if(!memcmp(chunk, "msg", 3)) {
      return_value = (char*)malloc(1024);
      arg1 = strtok_r(NULL, "\n", &saveptr1);

      for(arg2 = arg1; ; arg2 = NULL) {
	subchunk = strtok_r(arg2, "&", &saveptr2);
	
	if(subchunk == NULL)
	  break;
	  
	if(strchr(subchunk, 39) != NULL) {
	  subchunk[strlen(subchunk) - 1] = '\0';
	  shift += sprintf(return_value + shift, "%s", \
			   strchr(subchunk, 39) + 1);
      
	}
	else {
	  shift_ = strspn(subchunk, " ");
	  shift += sprintf(return_value + shift, "%d",\
			   registers[subchunk[shift_]]);
	}
      }
    }
    
    chunk = strtok_r(NULL, " \n\0", &saveptr1);
  }

  free(program);
  deallocate_stack(stack);
  
  return (char *)-1;
}


int main() {
  char *result;
  // test 0_0
  const char test0_0[] = "mov a 5\n\
;another comment\n\
inc a    ; commentarie \n\
add a 10 \n\
call tester \n\
tester: \n\
mov a 10\n\
end";

    result = assembler_interpreter(test0_0);

  if (result != (char *)-1)
    free(result);


  // test 1
  const char test1[]= "msg 'Hello World!'\n\
  testlabel:\n\
  ; test comment\n\
  mov a,1\n\
  end";
  result = assembler_interpreter(test1);

  if (result != (char *)-1) {
    printf("%s\n", result);
      free(result);
  }
  

  // test 2
  const char test2[]="\
; My first program\n\
mov  a, 5\n\
inc  a\n\
call function\n\
msg  '(5+1)/2 = ', a    ; output message\n\
end\n\
\n\
function:\n\
    div  a, 2\n\
    ret\n";
  
  result = assembler_interpreter(test2);

  if (result != (char *)-1) {
    printf("%s\n", result);
    free(result);
  }

  // test 3

  const char test3[]="\
mov   a, 5\n\
mov   b, a\n\
mov   c, a\n\
call  proc_fact\n\
call  print\n\
end\n\
\n\
proc_fact:\n\
    dec   b\n\
    mul   c, b\n\
    cmp   b, 1\n\
    jne   proc_fact\n\
    ret\n\
\n\
print:\n\
    msg   a, '! = ', c ; output text\n\
    ret\n\
";
  result = assembler_interpreter(test3);

  if (result != (char *)-1) {
    printf("%s\n", result);
      free(result);
  }
  

  // test 4
  const char test4[] = "\
mov   a, 8            ; value\n\
mov   b, 0            ; next\n\
mov   c, 0            ; counter\n\
mov   d, 0            ; first\n\
mov   e, 1            ; second\n\
call  proc_fib\n\
call  print\n\
end\n\
\n\
proc_fib:\n\
    cmp   c, 2\n\
    jl    func_0\n\
    mov   b, d\n\
    add   b, e\n\
    mov   d, e\n\
    mov   e, b\n\
    inc   c\n\
    cmp   c, a\n\
    jle   proc_fib\n\
    ret\n\
\n\
func_0:\n\
    mov   b, c\n\
    inc   c\n\
    jmp   proc_fib\n\
\n\
print:\n\
    msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text\n\
    ret\n\
";

  
  result = assembler_interpreter(test4);

  if (result != (char *)-1) {
    printf("%s\n", result);
      free(result);
  }

  // test 5
  const char test5[] = "\
mov   a, 11           ; value1\n\
mov   b, 3            ; value2\n\
call  mod_func\n\
msg   'mod(', a, ', ', b, ') = ', d        ; output\n\
end\n\
\n\
; Mod function\n\
mod_func:\n\
    mov   c, a        ; temp1\n\
    div   c, b\n\
    mul   c, b\n\
    mov   d, a        ; temp2\n\
    sub   d, c\n\
    ret\n\
";

  result = assembler_interpreter(test5);

  if (result != (char *)-1) {
    printf("%s\n", result);
      free(result);
  }

  // test 6
  char test6[] = "\
mov   a, 81         ; value1\n\
mov   b, 153        ; value2\n\
call  init\n\
call  proc_gcd\n\
call  print\n\
end\n\
\n\
proc_gcd:\n\
    cmp   c, d\n\
    jne   loop\n\
    ret\n\
\n\
loop:\n\
    cmp   c, d\n\
    jg    a_bigger\n\
    jmp   b_bigger\n\
\n\
a_bigger:\n\
    sub   c, d\n\
    jmp   proc_gcd\n\
\n\
b_bigger:\n\
    sub   d, c\n\
    jmp   proc_gcd\n\
\n\
init:\n\
    cmp   a, 0\n\
    jl    a_abs\n\
    cmp   b, 0\n\
    jl    b_abs\n\
    mov   c, a            ; temp1\n\
    mov   d, b            ; temp2\n\
    ret\n\
\n\
a_abs:\n\
    mul   a, -1\n\
    jmp   init\n\
\n\
b_abs:\n\
    mul   b, -1\n\
    jmp   init\n\
\n\
print:\n\
    msg   'gcd(', a, ', ', b, ') = ', c\n\
    ret\n\
";

  result = assembler_interpreter(test6);

  if (result != (char *)-1) {
    printf("%s\n", result);
    free(result);
  }

  // test 7

  const char test7[] = "\
call  func1\n\
call  print\n\
end\n\
\n\
func1:\n\
    call  func2\n\
    ret\n\
\n\
func2:\n\
    ret\n\
\n\
print:\n\
    msg 'This program should return -1'\n\
";

  result = assembler_interpreter(test7);

  if (result != (char *)-1) {
    printf("%s\n", result);
    free(result);
  }

  // test 8

  const char test8[] = "\
mov   a, 2            ; value1\n\
mov   b, 10           ; value2\n\
mov   c, a            ; temp1\n\
mov   d, b            ; temp2\n\
call  proc_func\n\
call  print\n\
end\n\
\n\
proc_func:\n\
    cmp   d, 1\n\
    je    continue\n\
    mul   c, a\n\
    dec   d\n\
    call  proc_func\n\
\n\
continue:\n\
    ret\n\
\n\
print:\n\
    msg a, '^', b, ' = ', c\n\
    ret\n\
";


  
  result = assembler_interpreter(test8);

  if (result != (char *)-1) {
    printf("%s\n", result);
    free(result);
  }


  // test 9
  const char test9[] = "mov q, 1   ; instruction mov q, 1 \n\
mov s, 8   ; instruction mov s, 8\n\
call func\n\
msg 'Random result: ', a\n\
end\n\
func:\n\
	cmp q, s\n\
	jg exit\n\
	mov a, q\n\
	div a, sn\n\
	ret\n\
; Do nothing\n\
exit:\n\
	msg 'Do nothing'";

  result = assembler_interpreter(test9);
  
  if (result != (char *)-1) {
    printf("%s\n", result);
    free(result);
  }

  
}
