package main

import "fmt"


type MinVal struct {
	minval int
	minval_i int
}

// check in this value already exist in slice
func is_in_slice(allvals []int, value int) bool {
	for i := 0; i < len(allvals); i++ {
		if allvals[i] == value {
			return true
		}
	}

	return false
}


func get_minval(allvals []int) MinVal {
	var ans MinVal
	ans.minval = allvals[0]
	
	for i := 0; i < len(allvals); i++ {
		if (allvals[i] < allvals[ans.minval_i]) {
			ans.minval_i = i
			ans.minval = allvals[i]
		}
	}

	return ans
}


// replace minimal value with two new and return replaced value
func replace_minval(allvals []int) []int {
	// find min value
	var minval, minval_i int
	ans := get_minval(allvals)
	minval = ans.minval
	minval_i = ans.minval_i
	
	if (is_in_slice(allvals, minval * 2 + 1) != true) {
		allvals[minval_i] =  minval * 2 + 1
	} else if (is_in_slice(allvals, minval * 3 + 1) != true) {
		allvals[minval_i] =  minval * 3 + 1
	}

	if (is_in_slice(allvals, minval * 3 + 1) != true) {
		allvals = append(allvals, minval * 3 + 1)
	}

	
	return allvals
}


func DblLinear (n int) int {
	allvals := []int{1}

	for i := 0; i < n; i++ {
		allvals = replace_minval(allvals)
	}

	return get_minval(allvals).minval
}

func main() {
	fmt.Println("For 10:", DblLinear(10)) // should be equal to 22
	fmt.Println("For 50:", DblLinear(50))
	fmt.Println("For 100:", DblLinear(100))
	fmt.Println("For 500:", DblLinear(500))
	fmt.Println("For 1000:", DblLinear(1000))
	
}
/*
   dotest(50, 175)
   dotest(100, 447)
   dotest(500, 3355)
   dotest(1000, 8488)
*/
