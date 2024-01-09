majority of the code in capture_test.c

Instructions
Test program takes two parameters, a number between 1 and 10 and a filename
usage: ./capture_test [number] [filename]
Use for example log.txt as file

Description:
Library captures the standard output and writes it to a file given as a
parameter. The test program uses threads and fifos to create n random
numbers and writes them into a file given as a parameter. Here n is a number
between 1 and 10. The first printed random  number is between 0-9, second one is between
10-19 and so on.
