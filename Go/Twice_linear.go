package main

import (
	"fmt"
	"sort"
	"math"
)


func get_value(allvals []int, index int) int {
	shift := 0
	pr := allvals[0]

	for i := 1; i < len(allvals); i++ {
		if (pr != allvals[i]) {			
			shift++ 
			pr = allvals[i]
		}

		if index == shift {
			return allvals[i]
		}
	}

	return allvals[0]
}


func get_power3 (n int) int {
	ans := 0

	for i := 0; i <= n; i++ {
		ans += int(math.Pow(3, float64(i)))
	}

	return ans
}


func get_power2 (n int) int {
	ans := 0

	for i := 0; i <= n; i++ {
		ans += int(math.Pow(2, float64(i)))
	}

	return ans	
}


func DblLinear (n int) int {
	// If zero -> return 1 
	if n == 0 {
		return 1;
	}

	// Variables to store number
	size := int(math.Log2(float64(n)))
	size += (get_power3(n) / get_power2(n)) + 1
	fmt.Println(size)
	values := [][]int{}
	allvals := []int{1}

	// Fill allvals array with number
	index := 0
	values = append(values, []int{1})
	
	for j := 0; j < size; j++ {
		val := []int{}
		for i := 0; i < len(values[index]); i++ {
			val = append(val, values[index][i] * 2 + 1)
			val = append(val, values[index][i] * 3 + 1)
		}

		for i := 0; i < len(val); i++ {
			allvals = append(allvals, val[i])
		}

		values = append(values, val)
		index++
	}
	
	sort.Ints(allvals)

	return get_value(allvals, n)
}

func main() {
	fmt.Println(DblLinear(10)) // should be equal to 22
	fmt.Println(DblLinear(50))
	fmt.Println(DblLinear(100))
	fmt.Println(DblLinear(500))
	fmt.Println(DblLinear(1000))
	
}
/*
   dotest(50, 175)
   dotest(100, 447)
   dotest(500, 3355)
   dotest(1000, 8488)
*/
