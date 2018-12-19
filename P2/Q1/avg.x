/*
 * The average procedure receives an array of real
 * numbers and returns the average of their
 * values. This toy service handles a maximum of
 * 200 numbers.
 * http://www.linuxjournal.com/article/2204?page=0,1
 */

const MAXAVGSIZE  = 200;

struct input_data {
  char text[200];
};


struct input_data_echo {
  char text_echo[200];
};


program ECHO_PROGS {
    version ECHO_VERS {
        input_data_echo ECHO(input_data) = 1;
    } = 1;
} = 21500;
