#pragma once

#include <functional>
#include <string>

#include "Net.hh"
#include "util.hh"

class Monitor {
public:
	Monitor(std::map<std::string, std::string> userDataSet, std::default_random_engine* re) : _time(0), \
		_randEngine(re), _mNet(re) { }
	void execute(netWorkCommandPOD command, const std::function<void(std::string)>& logFunction);

	void updateTime() { _time++; }
private:
	Network _mNet;
	std::map<std::string, std::string> _loginData;
	std::int_fast32_t _time;
	std::default_random_engine* _randEngine;
};
