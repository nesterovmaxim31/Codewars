#include <iostream>
#include <list>


int DblLinear(int n) {
  std::list<int> values {1};
  int element, element_y, element_z;
  bool flag; 

  for (size_t i = 0; i < n; i++) {
	/* Step 1 - take first element (the smallest one) and
	   delete it from list */
	element = values.front();
	element_y = element * 2 + 1;
	element_z = element * 3 + 1;
	values.pop_front();

	// flag - if element_z is inserted
	flag = false;

	/* Step 2 - iterating from the end insert z(element) and y(element) in
	 right position - after smaller one, before bigger one (like sorting).
	 If found duplicate - do nothing */

	// If zero or 1 element
	if (values.size() <= 1) {
	  values.push_back(element_y);
	  values.push_back(element_z);
	}
	
	else {
	  for (auto it = values.rbegin(); it != values.rend(); ++it) {
		// element_z
		if (!flag) {
		  // If biggest than the last element
		  if (it == values.rbegin() && element_z > *it) {
			values.push_back(element_z);
			flag = true;
		  }
		  // Skip if found duplicate
		  else if (element_z == *it)
			flag = true;
		  // If bigger than current *it and smallet than *(it + 1) - insert new el
		  else if (element_z > *it) {
			flag = true;
			values.insert(it.base(), element_z);
		  }
		  // If smaller that the first one
		  else if (it == values.rend() && element_z < *it) {
			flag = true;
			values.push_front(element_z);
		  }
		}
		// element_y
		if (flag) {		
		  if (it == values.rbegin() && element_y > *it) {
			values.push_back(element_y);
			break;
		  }
		  // Skip if found duplicate
		  else if (element_y == *it) {
			break;
		  }		   
		  // If bigger than current *it and smallet than *(it + 1) - insert new el
		  else if (element_y > *it) {
			// values.push_back(element_y);
			values.insert(it.base(), element_y);
			break;
		  }
		  // If smaller that the first one
		  else if (it == values.rend() && element_y < *it) {		   
			values.push_front(element_y);
			break;
		  }
		}
	  }
	}
  }

  return values.front();
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
