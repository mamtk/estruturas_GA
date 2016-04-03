#include <iostream>
#include <functional>

#include <vector>
#include <algorithm>
#include "Simulator.hh"
using namespace std;

void update1(char a)
{
	std::cout << "Update1" << std::endl;
}

void update2(char a)
{
	std::cout << "Update2" << std::endl;
}


int main(int argc, char *argv[])
{
	ClockSubject cs;
	cs.registerObserver(update1);
	cs.registerObserver(update1);
	cs.registerObserver(update1);
	cs.registerObserver(update2);
	cs.notify();
	cs.unregisterObserver(update1);
	std::cout << std::endl;
	cs.notify();

	return 0;
}
