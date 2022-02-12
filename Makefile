#/*
# *********************************************
# *  415 Compilers                            *
# *  Spring 2019                              *
# *  Students                                 *
# *  Authors: Ulrich Kremer                   *
# *********************************************
# */


CCFLAGS = -ggdb -Wall -lm


compile: Scheduler.c InstrUtils.c InstrUtils.h Utils.c Utils.h
	gcc $(CCFLAGS) Scheduler.c InstrUtils.c Utils.c -o schedule

clean:
	rm -rf schedule

