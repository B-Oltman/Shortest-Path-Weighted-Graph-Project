SOURCES=node.hpp graph.hpp schedule.hpp main.cpp

sched.out: $(SOURCES)
	g++ main.cpp -o $@