#pragma once


/*
 * Simulator: basicamente um factory method para SessionMon e Station,
 *  que instancia as classes de simulação
 */
class Simulator {

};

// inspiração para o observer estilo C++11 obtida de:
//  https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
class ClockObserver;

class ClockSubject {
	using Observer = std::function<void(char)> const&;
	using ObserverP = void(*)(char);
public:

	void registerObserver(Observer observer) {
	  _mObservers.push_back(std::forward<Observer>(observer));
	}

	void unregisterObserver(ObserverP observer) { // tão funcional, tão hipster...
		// se eu pudesse usar C++14...
		//_mObservers.erase(std::remove_if(_mObservers.begin(), _mObservers.end(), [&_mObservers](const auto &x)
		_mObservers.erase(std::remove_if(_mObservers.begin(), _mObservers.end(), [observer](Observer x)
		{
			void (*const* ptr)(char) = x.target<void(*)(char)>();
			return (ptr && *ptr == observer);
		}), _mObservers.end());
	}

	void notify() const	{
	  for (const auto& o : _mObservers) o('u');
	}

   private:
	std::vector<std::function<void(char)>> _mObservers;
};

class SimulatorClock {

};
