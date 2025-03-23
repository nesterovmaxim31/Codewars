#include <iostream>
#include <list>


int DblLinear(int n) {
  std::list<int> values {1};
  int element, element_y, element_z, val;
  bool flag; 

  for (size_t i = 0; i < n; i++) {
	/* Step 1 - take last element (the smallest one) and
	   delete it from list */
	element = values.back();
	element_y = element * 2 + 1;
	element_z = element * 3 + 1;
	values.pop_back();

	// flag - if element_z is inserted
	flag = false;

	/* Step 2 - iterating from the end insert z(element) and y(element) in
	 right position - after smaller one, before bigger one (like sorting).
	 If found duplicate - do nothing */

	// If zero or 1 element
	if (values.size() <= 1) {
	  values.push_front(element_y);
	  values.push_front(element_z);
	}
	
	else {
	  for (auto it = values.begin(); it != values.end(); ++it) {
		// element_z
		val = *it;
		if (!flag) {
		  // If biggest than the last element
		  if (it == values.begin() && element_z > val) {
			values.push_front(element_z);
			flag = true;
		  }
		  // Skip if found duplicate
		  else if (element_z == val)
			flag = true;
		  // If bigger than current val and smallet than *(it + 1) - insert new el
		  else if (element_z > val) {
			flag = true;
			values.insert(it, element_z);
		  }
		  // If smaller that the first one
		  else if (it == values.end() && element_z < val) {
			flag = true;
			values.push_back(element_z);
		  }
		}
		// element_y
		val = *it;
		if (flag) {		
		  if (it == values.begin() && element_y > val) {
			values.push_front(element_y);
			break;
		  }
		  // Skip if found duplicate
		  if (element_y == val) {
			break;
		  }		   
		  // If bigger than current val and smallet than *(it + 1) - insert new el
		  if (element_y > val) {
			values.insert(it, element_y);
			break;
		  }
		  // If smaller that the first one
		  if (it == values.end() && element_y < val) {		   
			values.push_back(element_y);
			break;
		  }
		}
	  }
	}
  }

  return values.back();
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
