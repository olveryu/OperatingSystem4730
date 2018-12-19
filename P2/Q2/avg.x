const MAXAVGSIZE  = 100;

struct input_data {
  double input_data<100>;
};	 

struct stat{
  double average;
  double mode;
  double median;
};

typedef struct input_data input_data;

program STAT_PROGS {
    version STAT_VERS {
        stat STAT(input_data) = 1;
    } = 1;
} = 21500;
