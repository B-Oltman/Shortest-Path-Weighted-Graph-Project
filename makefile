SOURCES=utility.hpp station.hpp trip.hpp station_graph.hpp schedule.hpp main.cpp

schedule.out: $(SOURCES)
	g++ main.cpp -o $@