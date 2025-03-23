#include <iostream>
#include <iterator>
#include <list>


int DblLinear(int n) {
  std::list<int> values {1};
  int element, element_y, element_z, val;

  for (size_t i = 0; i < n; i++) {
	// std::cout << i << std::endl;
	/* Step 1 - take last element (the smallest one) and
	   delete it from list */
	element = values.back();
	element_y = element * 2 + 1;
	element_z = element * 3 + 1;
	values.pop_back();

	/* Step 2 - iterating from the end insert z(element) and y(element) in
	 right position - after smaller one, before bigger one (like sorting).
	 If found duplicate - do nothing */

	// If zero or 1 element
	if (values.size() <= 1) {
	  values.push_front(element_y);
	  values.push_front(element_z);
	}
	
	else {
	  // element_z
	  values.push_front(element_z);

	  
	  // element_y (Binary seach)
	  int low = 0, high = values.size() - 1, mid, pr_mid = 0;
	  auto it = values.begin();

	  while (low <= high) {
		mid = (low + high) / 2;
		std::advance(it, mid - pr_mid - 1);

		// Borderline cases
		if (it == values.begin() && element_y > *it) {
		  values.push_front(element_y);
		  break;
		}

		if (it == values.end() && element_y < *it) {
		  values.push_back(element_y);
		  break;
		}
		// End of bordeline cases
		
		// If equal -> do nothing and exit loop
		if (element_y == *it) {
		  break;
		}

		// If bigger 
		else if (element_y > *it) {
		  high = mid;
		  pr_mid = mid;
		  continue;
		}

		// if less
		else if (element_y < *it) {
		  // check if element is > privious
		  it--;
		  // Insert element 
		  if (element > *it) {
			it++;
			values.insert(it, element_y);
			break;
		  }

		  // it other case
		  it++;
		  low = mid;
		  pr_mid = mid;
		  continue;
		}
		
	  } // End of while loop
	} // End of if size is bigger than 1  
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
