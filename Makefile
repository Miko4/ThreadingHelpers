SOURCES := test.cpp
OBJS := $(SOURCES:.cpp=.o)

test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o test $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

check: test
	./test


