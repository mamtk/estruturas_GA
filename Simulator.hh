#pragma once

#include <random>
#include <map>
#include <chrono>
#include <memory>
#include <functional>

#include "util.hh"
#include "Monitor.hh"

/*
 * Simulator: gera todos os eventos na rede, transmitindo comandos para o gerenciador de sessão
 */
class Simulator {
public:
	Simulator() : _currentCycle(0) {
		std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		_randEngine.seed(seed);
		ClockSubject::get().registerObserver(std::bind(&Simulator::updateTime, this));
	}

	void runSimulation(std::string);
	void updateTime() { _currentCycle++; }
	void logEvent(std::string event);
private:
	void loop();

	std::default_random_engine _randEngine;
	std::uint_fast32_t _currentCycle;
	std::unique_ptr<Monitor> _mMonitor;
};
