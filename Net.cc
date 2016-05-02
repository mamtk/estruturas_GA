#include "Net.hh"

void Network::broadcast(netWorkCommandPOD cmd, bool onlyToBlocked)
{
	std::vector<std::int_fast16_t> targets;
	if(onlyToBlocked) {
		targets = getBlockedStations();
	} else {
		targets = getActiveStations();
	}
	switch(cmd.type) {
		case networkEventType::MSG_CAST:
			for(auto& i : _mClients) {
				i.message(cmd.msg, cmd.senderID, cmd.senderName);
			}
			break;
		case networkEventType::BLOCK_CAST:
			for(auto& i : _mClients) {
				i.block();
			}
			break;
		case networkEventType::TERMINATE_CAST:
			for(auto& i : _mClients) {
				i.terminate(_time);
			}
			break;
		case networkEventType::RESUME_CAST:
			for(auto& i : _mClients) {
				i.unblock();
			}
			break;
		default:
			/* ??? */
			break;
	}
}

void Network::command(netWorkCommandPOD cmd)
{
	switch (cmd.type) {
		case networkEventType::MSG_ID:
			for(auto& i : _mClients) {
				if(i.getID() == cmd.receiverID) {
					i.message(cmd.msg, cmd.senderID, cmd.senderName);
				}
			}
			break;
		case networkEventType::RESUME_ID:
			for(auto& i : _mClients) {
				if(i.getID() == cmd.receiverID) {
					i.unblock();
				}
			}
			break;
		case networkEventType::BLOCK_ID:
			for(auto& i : _mClients) {
				if(i.getID() == cmd.receiverID) {
					i.block();
				}
			}
			break;
		case networkEventType::TERMINATE_ID:
			for(auto& i : _mClients) {
				if(i.getID() == cmd.receiverID) {
					i.terminate(_time);
				}
			}
			break;
	}
}

std::int_fast16_t Network::addStation(std::string uname)
{
	std::int_fast16_t newID = 0;
	bool collision = false;
	do {
		newID = idGenerator(*_randEngine);
		for(const auto& i : _mClients) {
			if(i.getID() == newID)
				collision = true;
		}
	} while(collision);
	_mClients.push_back(Station(_time, newID, uname));
	return newID;
}

std::vector<std::int_fast16_t> Network::getBlockedStations()
{
	std::vector<int_fast16_t> ret;
	for(const auto& i : _mClients) {
		if(i.getBlocked())
			ret.push_back(i.getID());
	}
	return std::move(ret);
}

std::vector<std::int_fast16_t> Network::getActiveStations()
{
	std::vector<int_fast16_t> ret;
	for(const auto& i : _mClients) {
		if(i.getBlocked())
			ret.push_back(i.getID());
	}
	return std::move(ret);
}

std::vector<std::int_fast16_t> Network::getTerminatedStations()
{
	std::vector<int_fast16_t> ret;
	for(const auto& i : _mClients) {
		if(i.getTerminated())
			ret.push_back(i.getID());
	}
	return std::move(ret);
}

std::vector<std::int_fast16_t> Network::getAliveStations()
{
	std::vector<int_fast16_t> ret;
	for(const auto& i : _mClients) {
		if(!i.getBlocked())
			ret.push_back(i.getID());
	}
	return std::move(ret);
}
