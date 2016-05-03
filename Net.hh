#pragma once

#include <cstdint>
#include <deque>

#include "Station.hh"
#include "util.hh"

class Network {
public:
	Network(std::default_random_engine* re) : _randEngine(re) { }
	std::vector<std::int_fast16_t> getBlockedStations() const ;
	std::vector<std::int_fast16_t> getActiveStations() const ;

	std::vector<std::int_fast16_t> getTerminatedStations() const ;
	std::vector<std::int_fast16_t> getAliveStations() const ;
	std::vector<std::int_fast16_t> getAllStations() const {
		std::vector<int_fast16_t> ret;
		for(const auto& i : _mClients)
			ret.push_back(i.getID());
		return ret;
	}

	bool getBlockedID(std::int_fast16_t id) const
		{ for(auto& i : _mClients) { if(i.getID() == id) return i.getBlocked(); } }
	bool getTerminatedID(std::int_fast16_t id) const
	{ for(auto& i : _mClients) { if(i.getTerminated() == id) return i.getBlocked(); } }
	std::string getUserName(std::int_fast16_t id) const {
		for(const auto& i : _mClients) {
			if(i.getID() == id)
				return i.getUserName();
		}
		return std::string();
	}
	std::deque<Station> getNetworkState() { return _mClients; }

	void broadcast(netWorkCommandPOD cmd, bool onlyToBlocked = false);
	void command(netWorkCommandPOD cmd);
	std::int_fast16_t addStation(std::string uname);

	void updateTime() { _time++; }
private:
	// vetores são mais eficientes, mas seria preciso reservar um tamanho máximo, para não invalidar ponteiros;
	std::deque<Station> _mClients;
	std::uint_fast32_t _time;
	std::default_random_engine* _randEngine;
	std::uniform_int_distribution<std::int_fast16_t> _idGenerator;
};
