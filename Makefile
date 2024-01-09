CFLAGS=-Wall -pedantic -Os -pthread
LDFLAGS=-pthread

all: capture_test clean

capture_test: capture_test.o my_capture.o

clean:
	rm -rf *.o *~