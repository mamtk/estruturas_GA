#include <iostream>

#include "Simulator.hh"

#include "Station.hh"

int main(int argc, char *argv[])
{
	Simulator simul;
	simul.runSimulation("./simulation_01.cfg");
	return 0;
}
