SOURCES=utility.hpp node.hpp graph.hpp schedule.hpp main.cpp

schedule.out: $(SOURCES)
	g++ main.cpp -o $@