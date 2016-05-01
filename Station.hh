#pragma once

#include <cstdint>
#include <functional>

#include "Simulator.hh"

class Station {
public:
	Station(std::uint_fast32_t cicle) : _mBlocked(false), _mAlive(true), _mActiveTime(0), \
		_mStartTime(cicle) { ClockSubject::get().registerObserver(std::bind(&Station::updateTime, this)); }
	bool getBlocked() { return _mBlocked; }
	void resume() { _mBlocked = false; }
	void block() { _mBlocked = true; }
	void message(std::string msg, std::int_fast32_t senderID) {}
	void terminate() { _mAlive = false; }
	void updateTime() { _mActiveTime++; }
private:
	std::int_fast32_t _mID;
	std::uint_fast32_t _mStartTime;
	std::uint_fast32_t _mActiveTime;
	bool _mBlocked;
	bool _mAlive;
};
