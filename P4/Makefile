GCCOPTS=-Wall -Wextra -Wpedantic 


all: compile 

compile:
	gcc $(GCCOPTS) -o thecoordinator thecoordinator.c mrand.c utils.c queue.c multilevel_feedback.c sjf_schedule.c lottery_schedule.c

clean:
	rm -f thecoordinator
	rm -f *.o

