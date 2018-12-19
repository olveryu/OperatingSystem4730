# Project 4

date 11/13/2018

zhenyu Yan(810-670-1500)

##Instructions

To clean my code(i.e remove any compiled files), type:

$ make clean

To compile my code, type:

& make

To run my code(after compiling it), type:

usage:
    ./thecoordinator [options] <input_file &>output_file
    ./thecoord [options] <input_file &>output_file (if renamed as described below)

Options:
-v              verbose output
-V <1 or 2>     more verbose 1 outputs non-empty queues; 2 outputs IO Request success 

-f              use mrand with random.txt
-s <seed>       fixing the seed for the RNG
-r <modulo>     Chance of IO request, default = 10
-c <modulo>     Chance of IO complete, default = 4

-S              Preemptive Shortest-Job-First Scheduling
-A              Agressive Preemptive Multilevel Feedback Scheduling
-N              Non-Agressive Preemptive Multilevel Feedback Scheduling (default)


Demo video link:
https://www.youtube.com/watch?v=0NjGa4WMGVU&feature=youtu.be
