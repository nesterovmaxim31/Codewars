package main

import "fmt"

func DblLinear(n int) int {
	if n == 0 {
		return 1;
	} else if n == 1 {
		return 3;
	} else if n == 2 {
		return 4;
	}

	values_y := []int{3}
	values_z := []int{4}
	var val, val_y, val_z int

	for i := 0; i < n - 1; i++ {
		if values_y[0] < values_z[0] {
			val = values_y[0]
			values_y = values_y[1:]
		} else if values_y[0] > values_z[0] {
			val = values_z[0]
			values_z = values_z[1:]
		} else if values_y[0] == values_z[0] {
			val = values_z[0]
			values_z = values_z[1:]
			values_y = values_y[1:]
		}
		
		val_y = val * 2 + 1
		val_z = val * 3 + 1

		values_y = append(values_y, val_y)
		values_z = append(values_z, val_z)
	}

	if values_y[0] <= values_z[0] {
		return values_y[0]
	} else {
		return values_z[0]
	}
}

func main() {
	fmt.Println(DblLinear(10)) // should be equal to 22
	fmt.Println(DblLinear(50))
	fmt.Println(DblLinear(100))
	fmt.Println(DblLinear(500))
	fmt.Println(DblLinear(1000))
	fmt.Println(DblLinear(100000))
}
