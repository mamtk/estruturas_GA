#pragma once

#include <cstdint>

#include "Station.hh"

class Network {
public:

private:
	// vetores são simplesmente a opção mais eficiente;
	//      na vida real, std::vector::shrink_to_fit facilitaria a questão do uso de memória
	std::vector<Station> _mClients;
};
