CC=g++
CXXFLAGS= -Wall -Wvla -Wextra -Werror -g -std=c++17
LDFLAGS= -lm
HEADERS= Matrix.h Activation.h Dense.h MlpNetwork.h Digit.h
OBJS= Matrix.o Activation.o Dense.o MlpNetwork.o main.o

%.o : %.c


mlpnetwork: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS) : $(HEADERS)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf mlpnetwork
tar:
	tar cvf cpp_ex4.tar Matrix.h Matrix.cpp Activation.h Activation.cpp Dense.h Dense.cpp MlpNetwork.h MlpNetwork.cpp Makefile




