#include <stdio.h>
#include <stdlib.h>

int cmpfunc (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int main(){
  // create a file to store user input or redirection file value
  FILE *fp;
  fp=fopen("tmp.txt","w");	
  int inputNumber = 0;
  int counter = 0;
  do{
    fscanf(stdin, "%d", &inputNumber);
    fprintf(fp, "%d\n", inputNumber);
    if(inputNumber !=0){
      counter++;
    }
  }while(inputNumber != 0);
  fclose(fp);
  if(counter == 0){
    printf("you have 0 value, exit program\n");
    return 0;
  }
  
  // create an array to store all the value in the tmp file
  int numbers[counter];
  FILE *file;
  file = fopen("tmp.txt", "r");
  for(int i = 0; i < counter; i++){
    fscanf(file,"%d", &inputNumber);
    numbers[i] = inputNumber;
  }
  fclose(file);
  remove("tmp.txt");
  
  // sort the array
  qsort(numbers,counter,sizeof(int),cmpfunc);
  
  
  // calculate statistics
  int min = numbers[0];
  int max = numbers[counter-1];

  // halfIndex
  int halfIndex = counter/2;

  // median index
  int medianIndex = (counter%2 == 0) ? (halfIndex) : (halfIndex+1);

  // median
  double  median;
  if(counter%2 == 0){
    median = ((double)numbers[halfIndex-1] + (double)numbers[halfIndex])/2;
  }else{
    median = numbers[halfIndex-1];
  }

  // odd or even
  char *oddEven = (counter%2 == 0) ? "even" : "odd";
  
  // sum
  long sum = 0;
  for(int i = 0; i < counter; i++){
    sum += numbers[i];
  }
  // mean
  double mean = (double)sum/counter;
  int mode = numbers[0];
  int lastMode = mode;
  int count = 1;
  int maxCount = 1;
  for(int i = 1; i < counter; i++){
    if(lastMode == numbers[i]){
      count++;
    }else{
      count = 1;
      lastMode = numbers[i];
    }
    if(count > maxCount){
      maxCount = count;
      mode = lastMode;
    }
  }
  // print out result
  printf("range[%d %d]\n",min,max);
  printf("median index(floor) = %d\n", medianIndex);
  printf("the mean = %f (%ld/%d)\n", mean, sum, counter);
  if(counter%2 == 0){
    printf("the median = %f at index %d and index %d (%s)\n", median, halfIndex, halfIndex+1, oddEven);
  }else{
    printf("the median = %f at index %d (%s)\n", median, halfIndex, oddEven);  
  }
  printf("the mode = %d (%d occurences)\n", mode, maxCount);
  return 0;
}// main
