SRCS =  arrays.cpp \
        threadpool.cpp \
        connectpool.cpp

STATICLIB = libtlib.a

CLEAN = *.o\
        $(STATICLIB)

CPPFLAGS = -g -O2

#need not change
all: $(STATICLIB)
$(STATICLIB) : $(SRCS:.cpp=.o)
	ar rc $@ $^

%o : %cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(CLEAN)
