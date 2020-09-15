SOURCES := test.cpp
OBJS := $(SOURCES:.cpp=.o)
LDFLAGS=-pthread


test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o test $(OBJS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

check: test
	./test


