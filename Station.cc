#include "Station.hh"

void Station::unblock() {
	if(_mSubKey) {
		ClockSubject::get().unregisterObserver(_mSubKey);
		_mSubKey = 0;
	}
	_mBlocked = false;
	_mSubKey = ClockSubject::get().registerObserver(std::bind(&Station::updateTime, this));
}
