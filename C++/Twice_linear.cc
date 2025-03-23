/*
URL: https://www.codewars.com/kata/5672682212c8ecf83e000050
Name: Twice linear
Difficulty: 4 kyu
*/

#include <iostream>
#include <iterator>
#include <algorithm>
#include <list>


int DblLinear(int n) {
  if (n == 0)
	return 1;
  else if (n == 1)
	return 3;
  else if (n == 2)
	return 4;
	  
  /* values_y store values from y(x) = 2 * x + 1,
	 values_z store values form z(x) = 3 * x + 1 */
  std::list<int> values_y{3}, values_z{4};
  int element, element_y, element_z, val;

  for (size_t i = 0; i < n - 1; i++) {
	// std::cout << i << std::endl;
	// If smallest value stores in values_y
	if (values_y.front() < values_z.front()) {
	  // Get value from list and delete it, then calculate two new values 
	  element = values_y.front();
	  element_y = element * 2 + 1;
	  element_z = element * 3 + 1;
	  values_y.pop_front();

	  // Add two new value in values_y and values_z
	  if (values_y.back() != element_y)
		values_y.push_back(element_y);
	  if (values_z.back() != element_z)
		values_z.push_back(element_z);
	}

	// If smallest value stores in values_z
	else if (values_y.front() > values_z.front()) {
	  // Get value from list and delete it, then calculate two new values 
	  element = values_z.front();
	  element_y = element * 2 + 1;
	  element_z = element * 3 + 1;
	  values_z.pop_front();

	  // Add two new value in values_y and values_z
	  if (values_y.back() != element_y)
		values_y.push_back(element_y);
	  if (values_z.back() != element_z)
		values_z.push_back(element_z);
	}

	// If equal min values in both lists
	else if (values_z.front() == values_y.front()) {
	  // Get value from list and delete it, then calculate two new values 
	  element = values_z.front();
	  element_y = element * 2 + 1;
	  element_z = element * 3 + 1;
	  values_z.pop_front();
	  values_y.pop_front();

	  // Add two new value in values_y and values_z
	  if (values_y.back() != element_y)
		values_y.push_back(element_y);
	  if (values_z.back() != element_z)
		values_z.push_back(element_z);
	}
	
  }

  return std::min(values_y.front(), values_z.front());
}


int main() {
  std::cout << "for n = 0: " << DblLinear(0) << std::endl;
  std::cout << "for n = 1: " << DblLinear(1) << std::endl;
  std::cout << "for n = 2: " << DblLinear(2) << std::endl;
  std::cout << "for n = 3: " << DblLinear(3) << std::endl;
  std::cout << "for n = 4: " << DblLinear(4) << std::endl;
  std::cout << "for n = 10: " << DblLinear(10) << std::endl;
  std::cout << "for n = 20: " << DblLinear(20) << std::endl;
  std::cout << "for n = 30: " << DblLinear(30) << std::endl;
  std::cout << "for n = 50: " << DblLinear(50) << std::endl;
  std::cout << "for n = 100: " << DblLinear(100) << std::endl;
  std::cout << "for n = 500: " << DblLinear(500) << std::endl;
  std::cout << "for n = 1000: " << DblLinear(1000) << std::endl;
  std::cout << "for n = 100000: " << DblLinear(100000) << std::endl;
  return 0;
}
