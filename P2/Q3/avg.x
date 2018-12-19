const MAXAVGSIZE  = 101;

struct input_data {
  double input_data<101>;
};	 

typedef struct input_data input_data;

program STAT_PROGS {
    version STAT_VERS {
        input_data LIST(input_data) = 1;
    } = 1;
} = 21500;
