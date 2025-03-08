#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char hexvals[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',\
		    'B', 'C', 'D', 'E', 'F'};

void get_hex(int var, char* hex) {
  strcpy(hex, "00:\0");
  int k, i = 1;
  
  while (var > 0) {
    k = var % 16;
    var /= 16;
    hex[i] = hexvals[k];
    i--;
  }
}


void mcastIp2Mac(const char* ip, char macStr[18]) {
  char ipvar[17], hex[3];
  strcpy(ipvar, ip);
  memmove(macStr, "01:00:5E:\0", 10);
  
  int var, arr[3];
  memset(arr, sizeof(int) * 3, 0);
  
  char* chunk = strtok(ipvar, ".");
  
  chunk = strtok(NULL, ".");  
  var = atoi(chunk) & 0b01111111;
  get_hex(var, hex);
  strcat(macStr, hex);

  chunk = strtok(NULL, ".");  
  var = atoi(chunk) & 0b11111111;
  get_hex(var, hex);
  strcat(macStr, hex);

  chunk = strtok(NULL, ".");  
  var = atoi(chunk) & 0b11111111;
  get_hex(var, hex);
  strcat(macStr, hex);

  macStr[17] = '\0';  
}

/* https://www.codewars.com/kata/5a9d43ceba1bb52492000080/c */
int main() {
  char str[18] = "\0";
  
  mcastIp2Mac("224.0.0.1", str);
  printf("%s\n", str);
  
  mcastIp2Mac("225.0.0.1", str);
  printf("%s\n", str);
  
  mcastIp2Mac("239.255.255.255", str);
  printf("%s\n", str);  

  return 0;
}
