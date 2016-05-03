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
	using configValue = std::pair<std::unique_ptr<float>,std::unique_ptr<std::string>>;
public:
	Simulator() : _currentCycle(0) {
		_chanceAccumulators.reserve(static_cast<int>(configType::NUM_ELEMENTS));
		_vecEventGenerators.reserve(static_cast<int>(configType::NUM_ELEMENTS));
		std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		_randEngine.seed(seed);
		ClockSubject::get().registerObserver(std::bind(&Simulator::updateTime, this));
	}

	void runSimulation(std::string);
	void updateTime() { _currentCycle++; }
	void logEvent(std::string event);
private:
	void loop();
	void printStats();

	enum class configType {
		MAX_MSG,
		MAX_TERM,
		MAX_BLOCK,
		MAX_RESUME,
		MAX_LOGIN,
		MAX_LOGIN_FAIL,
		MAX_MSG_CAST,
		MAX_TERM_CAST,
		MAX_BLOCK_CAST,
		MAX_RESUME_CAST,
		MAX_CYCLES,

		MIN_CYCLE_TIME,
		PATH_LOGIN_DATA,
		PATH_LOG_STORE,
		PATH_UNUSEFUL_MSGS,
		NUM_ELEMENTS
	};

	std::default_random_engine _randEngine;
	std::uint_fast32_t _currentCycle;
	std::unique_ptr<Monitor> _mMonitor;
	std::vector<float> _chanceAccumulators;
	std::vector<std::uniform_real_distribution<float>> _vecEventGenerators;
	std::vector<std::string> _phrases;
	std::map<configType, configValue> _config;
	std::map<std::string, std::string> _loginDataSet;
	std::vector<netWorkCommandPOD> _vCommands;
	std::vector<std::int_fast32_t> _vOnStationsCycle;

	void manageEvent(configType type);
};
