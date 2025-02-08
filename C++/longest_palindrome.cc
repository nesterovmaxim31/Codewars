#include <iostream>
#include <string>
#include <cstddef>
#include <bits/stdc++.h>


using namespace std;

int longest_palindrome(const std::string &s) {
  string s_;
  size_t length = s.length(), mxlen = 0;
    
  if(length == 0)
    return 0;
  if(length == 1)
    return 1;

  for(size_t i = 0; i < length;  i++) {
    for (size_t j = 0; j < length - i; j++) {
      s_ = s.substr(i, length - i - j);
      reverse(s_.begin(), s_.end());

      if (s.compare(i, length - i - j, s_) == 0)
	mxlen = max(mxlen, length - j - i);
    }
  }
  
  return mxlen; 
}


int main() {
  string s;
  
  while(1) {
    cin >> s;
    cout << longest_palindrome(s) << endl;
  }
  
  return 0;
} 
