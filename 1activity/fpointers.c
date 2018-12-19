#include <stdio.h>
#include <stdlib.h> 

//  
// function pointer tutorial 

//  
//  -- pointer to a 1 int argument function 
// 	with 1 input integeger "int" argument.
//      that returns void.
//
// 	void (*ptr_func)(int);
//		*ptr_func 	pointer that points to a function
//		void 		return type of that function
//		int 		the argument type of that function. 
//
//  -- pointer to a 2 int argument function that returns void.
//
// 	void (*ptr_func)(int, int);
//
//  -- function pointers - and how to use it with qsort. in <stdlib>
//	https://www.geeksforgeeks.org/comparator-function-of-qsort-in-c/  
//
//  -- comparison function in quicksort uses generic input pointers:
// 	int (*ptr_func)(const void *, const void *);
//	read about them here:
// 		https://www.geeksforgeeks.org/void-pointer-c/
//

int compare( const void* left, const void* right )
{
    return (*(int*)right - *(int*)left);
}


// sorts a preinitialized array iarray using qsort().
int main()
{

    int (*cmp) (const void* , const void*);

    cmp = &compare;

    int iarray[] = {1,2,3,4,5,6,7,8,9,10};
    //iarray_sz  = (int) sizeof(iarray)/sizeof(iarray[0]);

    int  iarray_sz       = (int) sizeof(iarray)/sizeof(int);
    int  iarray_item_sz  = sizeof(int);

    // sorts iarray according to the comprison function: cmp 
    //	cmp has 2 arguments. 
    qsort( iarray, iarray_sz, iarray_item_sz, cmp );

    int c = 0;

    printf("%2c iarray\n", 'c');
    printf("-- ------ \n");
    while( c < iarray_sz )
    {
        printf("%2d %2d\n", c, iarray[c] );
        c++;
    }

return 0;
}
