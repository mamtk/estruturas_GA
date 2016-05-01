#pragma once

#include <iostream>

#include <cstdint>
#include <functional>

#include "util.hh"

class Station {
public:
	Station(std::uint_fast32_t cicle, std::int_fast32_t id, std::string uname) : _mBlocked(false), _mAlive(true), \
		_mActiveTime(0), _mStartTime(cicle), _mID(id), _userName(uname)
		{ _mSubKey = ClockSubject::get().registerObserver(std::bind(&Station::updateTime, this)); }

	bool getBlocked() const { return _mBlocked; }
	bool getTerminated() const { return !(_mAlive); }
	std::int_fast32_t getID() const { return _mID; }
	std::string getUserName() const { return _userName; }

	void unblock();
	void block() { _mBlocked = true; ClockSubject::get().unregisterObserver(_mSubKey); _mSubKey = 0; }
	void message(std::string msg, std::int_fast32_t senderID, std::string senderName) { /* OH HAI */ }
	void terminate(uint_fast32_t cycle) { _mStoptTime = cycle; _mAlive = false; \
										  ClockSubject::get().unregisterObserver(_mSubKey); _mSubKey = 0; }
	void updateTime() { _mActiveTime++; }
private:
	std::int_fast32_t _mID;
	std::string _userName;
	std::uint_fast32_t _mStartTime;
	std::uint_fast32_t _mActiveTime;
	std::uint_fast32_t _mStoptTime;
	std::size_t _mSubKey;
	bool _mBlocked;
	bool _mAlive;
};
