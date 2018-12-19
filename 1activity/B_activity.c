#include <stdio.h>


// Copy last three addresses of intarray into ptr_array which is an array of 
// pointers to an int.
	
    int main() 
    {

    	int intarray[5] = {1,2,3,4,5};

       // Q1. make pointer point to the 3rd element of intarray '3' (currently
       //	it points to the first element '1')

	// point pointer to the (beginning) element 
        int *pointer = (int *) &intarray; // points to the first element here.

	// Q1: Insert code for Q1 below:
	pointer += 2;


        // array of 3 pointers
        int *ptr_array[3];

        // copy last three addresses of intarray into ptr_array
        int i;

        for( int i = 0; i < 3; i++ ) 
	{
            // Q2.insert code below, so that the elements in ptr_array points to
	   //	the last three elements in intarray
           // 		pointer should initially point to '3'
           // 			then you should increent the pointer
	   //			to the next element of intarray.

	   // Q2: Insert code for Q2 below:
	  ptr_array[i] = pointer;
	  pointer++;
        }

        for (i = 0; i < 3; i++) 
	{
        	if (ptr_array[i] == &pointer[i]) 
	 	{
		// Q3 - add printing out the content intarray ('3', '4', and '5' 
		//	in addition to pointer addresses

	   	// Q3: replace '666' code with proper variable name (using ptr_array)
                	printf("ptr_array[%d] ==  &pointer[%d] : %p : %d \n", 
				i, i, ptr_array[i], *ptr_array[i]);
            	} 
		else 
		{
                	printf("ptr_array[%d] !=  &pointer[%d] : %p : %d\n", 
				i, i, ptr_array[i], *ptr_array[i]);
            	}
        }

        return 0;
    }
