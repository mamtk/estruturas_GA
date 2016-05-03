#pragma once

#include <chrono>
#include <map>
#include <memory>
#include <random>

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
	return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

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
	  static ClockSubject clockSingleton;   // uau! que belo anti-padrão! vem até em c++11!
	  return clockSingleton;
	}

   private:
	std::default_random_engine _randEngine;
	std::uniform_int_distribution<std::size_t> randGaussian;
	std::map<std::size_t, std::function<void(char)>> _mObservers;
};

enum class networkEventType {
	MSG_ID,
	MSG_CAST,
	TERMINATE_ID,
	TERMINATE_CAST,
	BLOCK_ID,
	BLOCK_CAST,
	RESUME_ID,
	RESUME_CAST,
	LOGIN
};

struct netWorkCommandPOD {
	networkEventType type;
	std::int_fast16_t senderID;     // -1 == Monitor
	std::int_fast16_t receiverID;   // -1 == broadcast
	std::uint_fast32_t time;
	std::string senderName;         // username for login
	std::string msg;                // password for login
};
