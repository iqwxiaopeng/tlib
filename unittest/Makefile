SRCS =  test.cpp

BIN = test

CLEAN = *.o \
        $(BIN)

LIB = -L../../components/gtest-1.6.0/lib -lgtest -lpthread -lm -L../ -ltlib

INC = -I../../components/gtest-1.6.0/include -I..

CPPFLAGS = -g

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
