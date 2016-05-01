#pragma once

#include <random>
#include <map>
#include <chrono>
#include <memory>

#include "util.hh"

// inspiração para o observer estilo C++11 obtida de:
//  https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
class ClockSubject {
	using Observer = std::function<void(char)> const&;
	using ObserverP = void(*)(char);

public:
//	ClockSubject(std::default_random_engine* rePointer) : randEngine(rePointer) {}
	ClockSubject() {
		std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		_randEngine.seed(seed);
	}

	std::size_t registerObserver(Observer observer) {
		std::size_t newID = 0;
		do {
			newID = randGaussian(_randEngine);
			if(!_mObservers.count(newID)) {
				_mObservers.insert(std::pair<std::size_t, std::function<void(char)>>(std::move(newID), std::forward<Observer>(observer)));
				return(newID);
			}
		} while (newID);
		return 0;
	}

	void unregisterObserver(std::size_t obsID) {
		for(auto it = _mObservers.begin(), iend = _mObservers.end(); it != iend; ++it) {
			if(it->first == obsID)
				it = _mObservers.erase(it);
		}
	}

	void notify() const	{
	  for (const auto& o : _mObservers) o.second('u'); // such a happy little face
	}

	static ClockSubject& get(){
	  static ClockSubject clockSingleton;
	  return clockSingleton;
	}

   private:
	std::default_random_engine _randEngine;
	std::uniform_int_distribution<std::size_t> randGaussian;
	std::map<std::size_t, std::function<void(char)>> _mObservers;
};


/*
 * Simulator:
 */
class Simulator {
public:
	Simulator() {
		std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
		_randEngine.seed(seed);
//		_mClockSubject = make_unique<ClockSubject>(&_randEngine);
	}

	void runSimulation(std::string);
private:
	void loop();

	std::default_random_engine _randEngine;
//	std::unique_ptr<ClockSubject> _mClockSubject;
};


class SimulatorClock {

};
