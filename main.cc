#include <iostream>

#include "Simulator.hh"

#include "Station.hh"

int main(int argc, char *argv[])
{
/*	std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine re;
	re.seed(seed);
	std::uniform_int_distribution<std::int_fast32_t> idGenerator;
	std::cout << idGenerator.min() << std::endl;*/
	Simulator simul;
	simul.runSimulation("./simulation_01.cfg");
	return 0;
}
