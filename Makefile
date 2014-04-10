CXXFLAGS += -O3 -g -march=native -mfpmath=sse -msse4a

#gcc -march=native -Q --help=target -v

all: main

main: main.cpp

clean:
	rm main
