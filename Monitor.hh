#pragma once

#include <functional>
#include <string>

#include "Net.hh"
#include "util.hh"

class Monitor {
public:
	Monitor(std::map<std::string, std::string> userDataSet, std::default_random_engine* re) : _time(0), \
		_randEngine(re), _mNet(re), _loginData(userDataSet) { }
	bool execute(netWorkCommandPOD command, const std::function<void(std::string)>& logFunction);

	std::vector<std::int_fast16_t> getBlockedStations() const { return _mNet.getBlockedStations(); }
	std::vector<std::int_fast16_t> getActiveStations() const { return _mNet.getActiveStations(); }

	std::vector<std::int_fast16_t> getTerminatedStations() const { return _mNet.getTerminatedStations(); }
	std::vector<std::int_fast16_t> getAliveStations() const { return  _mNet.getAliveStations(); }
	std::vector<std::int_fast16_t> getAllStations() const { return  _mNet.getAllStations(); }

	bool getBlockedID(std::int_fast16_t id) const { return  _mNet.getBlockedID(id); }
	bool getTerminatedID(std::int_fast16_t id) const { return _mNet.getTerminatedID(id); }
	std::string getUserName(std::int_fast16_t id) const { return _mNet.getUserName(id); }
	void updateTime() { _time++; }
private:
	Network _mNet;
	std::map<std::string, std::string> _loginData;
	std::int_fast32_t _time;
	std::default_random_engine* _randEngine;
};
