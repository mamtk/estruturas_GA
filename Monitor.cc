#include <functional>
#include <string>

#include "Monitor.hh"

bool Monitor::execute(netWorkCommandPOD command, std::function<void(std::string)> const& logFunction)
{
	switch (command.type) {
		case networkEventType::MSG_ID:
			if(_mNet.getBlockedID(command.receiverID) || _mNet.getTerminatedID(command.receiverID))
				return false;
			_mNet.command(command);
			logFunction(std::string("MENSAGEM: " + command.senderName + "@" +std::to_string((command.senderID)) + \
									" -> " + _mNet.getUserName(command.receiverID) + "@" + \
									std::to_string(command.receiverID) + "; TXT: " + command.msg));
			break;
		case networkEventType::RESUME_ID:
			if(!_mNet.getBlockedID(command.receiverID) || _mNet.getTerminatedID(command.receiverID))
				return false;
			_mNet.command(command);
			logFunction(std::string("DESBLOQUEAR: MONITOR -> " + _mNet.getUserName(command.receiverID) + "@" + \
									std::to_string(command.receiverID)));
			break;
		case networkEventType::BLOCK_ID:
			if(_mNet.getBlockedID(command.receiverID) || _mNet.getTerminatedID(command.receiverID))
				return false;
			_mNet.command(command);
			logFunction(std::string("BLOQUEAR: MONITOR -> " + _mNet.getUserName(command.receiverID) + "@" + \
									std::to_string(command.receiverID)));
			break;
		case networkEventType::TERMINATE_ID:
			if(_mNet.getTerminatedID(command.receiverID))
				return false;
			_mNet.command(command);
			logFunction(std::string("TERMINAR: MONITOR -> " + _mNet.getUserName(command.receiverID) + "@" + \
									std::to_string(command.receiverID)));
			break;
		case networkEventType::LOGIN:
			if(command.senderName.length() < 3)
				break;
			// para evitar side-channel leakeage, na vida real isso seria em tempo constante, sem branch prediction (if)
			if(_loginData.find(command.senderName) != _loginData.end()) {
				if(_loginData.find(command.senderName)->second == command.msg) {
					std::int_fast16_t id =_mNet.addStation(command.senderName);
					logFunction(std::string("LOGIN ACEITO: " + command.senderName + " @ maquina #" + std::to_string((id))));
					return true;
				}
			}
			logFunction(std::string("LOGIN FALHOU: " + command.senderName));
			return false;
			break;
		case networkEventType::MSG_CAST:
			_mNet.broadcast(command);
			logFunction(std::string("MENSAGEM: BROADCAST de MONITOR; TXT: " + command.msg));
			break;
		case networkEventType::BLOCK_CAST:
			_mNet.broadcast(command);
			logFunction(std::string("BLOQUEAR: BROADCAST de MONITOR"));
			break;
		case networkEventType::TERMINATE_CAST:
			_mNet.broadcast(command);
			logFunction(std::string("TERMINAR: BROADCAST de MONITOR"));
			break;
		case networkEventType::RESUME_CAST:
			_mNet.broadcast(command, true);
			logFunction(std::string("DESBLOQUEAR: BROADCAST de MONITOR"));
			break;
		default:
			/* ??? */
			break;
	}
	return true;
}
