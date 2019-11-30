SOURCES=node.hpp sched.hpp main.cpp

sched.out: $(SOURCES)
	g++ main.cpp -o $@