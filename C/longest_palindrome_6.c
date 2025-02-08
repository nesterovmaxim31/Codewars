#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))

static inline
int is_palindrome(const char *s, size_t length) {
  for(size_t i = 0; i < length / 2 + 1; i++) {
    if (s[i] != s[length - i])
      return 0;
  }

  return 1;
}


int longest_palindrome(const char *s) {
  size_t length = strlen(s);
  int mxlen = 1;
  
  if (length == 0)
    return 0;
  if (length == 1)
    return 1;

  for(size_t i = 0; i < length - 1; i++) {
    for(size_t j = i + 1; j < length; j++) {
      if (s[i] == s[j]) {
	mxlen = (is_palindrome(&s[i], j - i) ? \
		 MAX((int)(j - i + 1), mxlen) : mxlen);
      }
    }
  }
  
  return mxlen; 
}


/* https://www.codewars.com/kata/54bb6f887e5a80180900046b */
int main() {
  #ifdef TEST
  char *s = NULL;
  size_t buf_size;
  while(1) {
    getline(&s, &buf_size, stdin);
    printf("Longest palindrom in %s has length = %d\n", s,\
	   longest_palindrome(s));
  }

  #endif
}
