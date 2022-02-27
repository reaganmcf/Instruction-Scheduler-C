#/*
# *********************************************
# *  415 Compilers                            *
# *  Spring 2019                              *
# *  Students                                 *
# *  Authors: Ulrich Kremer                   *
# *********************************************
# */


CCFLAGS = -ggdb -Wall -lm -Werror


compile: Scheduler.c InstrUtils.c InstrUtils.h Utils.c Utils.h
	gcc $(CCFLAGS) DepGraph.c Scheduler.c InstrUtils.c Utils.c DepGraphUtils.c -o schedule

clean:
	rm -rf schedule

