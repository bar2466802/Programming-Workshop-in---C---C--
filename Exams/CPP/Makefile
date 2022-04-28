CC=g++
CXXFLAGS= -Wall -Wvla -Wextra -Werror -g -std=c++14
LDFLAGS= -lm
OBJS= VLVector.hpp
%.o : %.c


mlpnetwork: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJS) : $(HEADERS)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf exam.tar
tar:
	rm -rf *.o
	rm -rf exam.tar
	tar  tar -cvf exam.tar STATEMENT.pdf VLVector.hpp




