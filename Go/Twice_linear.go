package main

import (
	"fmt"
	"sort"
	"math"
)


func get_value(allvals []int, index int) int {
	shift := 0
	prval := allvals[0]
	for i := 1; i < len(allvals); i++ {
		if prval == allvals[i] {
			shift++ 
		} else {
			prval = allvals[i]
		}

		if 
	}

	return shift
}


func main() {
	var number float64 = 50
	size := int(math.Log2(number)) + 1
	
	values := [][]int{}
	allvals := []int{1}
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

	// Delete repeating elements
	fmt.Println(allvals)
	fmt.Println(allvals[int(number) + del_repetition(allvals)])
}
