#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>  //for std::generate_n
#include <thread>

#include "Simulator.hh"


// random string, fonte: https://stackoverflow.com/a/24586587
std::string random_string(std::size_t length, std::default_random_engine re)
{
	static const std::string alphanums =
		"0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	static std::uniform_int_distribution<> pick(0, alphanums.size() - 1);

	std::string s;

	s.reserve(length);

	while(length--)
		s += alphanums[pick(re)];

	return s;
}


void Simulator::runSimulation(std::string file)
{
	std::ifstream configFile(file);
	if(!configFile.is_open())
		return;
	std::string buf;
	while (std::getline(configFile, buf))
	{
		if(buf.length() < 2)
			continue;
		// ler arquivo de configuração e assumir variáveis
		switch(buf.at(0)) {
			// máximo de tipos de eventos por ciclo simulado
			case '0':   // mensagem
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_MSG, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_MSG)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_MSG)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '1':   // terminar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_TERM, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_TERM)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_TERM)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_TERM].first);
			break;
			case '2':   // bloquear
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_BLOCK, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_BLOCK)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_BLOCK)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_BLOCK].first);
			break;
			case '3':   // continuar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_RESUME, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_RESUME)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_RESUME)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_RESUME].first);
			break;
			case '4':   // login bem-sucedido
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_LOGIN, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_LOGIN)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_LOGIN)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_LOGIN].first);
			break;
			case '5':   // login mal-sucedido
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_LOGIN_FAIL, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_LOGIN_FAIL)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_LOGIN_FAIL)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_LOGIN_FAIL].first);
			break;
			case '6':   // broadcast mensagem
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_MSG_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_MSG_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_MSG_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG_CAST].first);
			break;
			case '7':   // broadcast terminar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_TERM_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_TERM_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_TERM_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_TERM_CAST].first);
			break;
			case '8':   // broadcast bloquear
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_BLOCK_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_BLOCK_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_BLOCK_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_BLOCK_CAST].first);
			break;
			case '9':   // broadcast continuar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_RESUME_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_RESUME_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_RESUME_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_RESUME_CAST].first);
			break;

			// outras configurações
			case 'a':   // tempo (ciclos) maximo da simulação
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_CYCLES, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
			break;
			case 'b':   // tempo (ms) real mínimo de cada ciclo
				_config.insert(std::pair<configType, configValue>\
							   (configType::MIN_CYCLE_TIME, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
			break;
			case 'c':   // lista de usuários da simulação
				_config.insert(std::pair<configType, configValue>\
							   (configType::PATH_LOGIN_DATA, configValue(nullptr,make_unique<std::string>\
																 (buf.substr(2).c_str()))));
			break;
			case 'd':   // arquivo de relatório
				_config.insert(std::pair<configType, configValue>\
							   (configType::PATH_LOG_STORE, configValue(nullptr,make_unique<std::string>\
																 (buf.substr(2).c_str()))));
			break;
			case 'e':   // arquivo de mensagens inúteis
				_config.insert(std::pair<configType, configValue>\
							   (configType::PATH_UNUSEFUL_MSGS, configValue(nullptr,make_unique<std::string>\
																 (buf.substr(2).c_str()))));
			break;
		}

	}
	configFile.close();
	// ler dados de usuários válidos no formato "usuario:senha"
	std::ifstream loginFile(*_config[configType::PATH_LOGIN_DATA].second);
	std::string buf2;
	while (loginFile)
	{
		buf2.clear();
		if (!std::getline( loginFile, buf )) break;

		std::istringstream stream(buf);

		while (stream)
		{
			if (!std::getline( stream, buf, ':' ))
				break;
			if(buf2.empty())
				buf2=buf;
			else
				_loginDataSet.insert(std::make_pair(buf2, buf));
		}
	}
	loginFile.close();
	// iniciar ponteiro para o monitor
	_mMonitor = make_unique<Monitor>(Monitor(_loginDataSet, &_randEngine));
	// carregar frases inúteis
	std::ifstream msgsFile(*_config[configType::PATH_UNUSEFUL_MSGS].second);
	if(!msgsFile.is_open())
		return;
	while (std::getline(msgsFile, buf))
	{
		_phrases.push_back(buf);
	}
	msgsFile.close();

	// finalmente iniciar loop de simulação
	loop();
}

void Simulator::logEvent(std::string event)
{
	std::cout << event << std::endl;
}

void Simulator::manageEvent(configType type)
{
	_chanceAccumulators[static_cast<int>(type)] += _vecEventGenerators[static_cast<int>(type)](_randEngine);
	if(_chanceAccumulators[static_cast<int>(type)] > 1) {
		double times = 0;
		modf (_chanceAccumulators[static_cast<int>(type)] , &times);
		_chanceAccumulators[static_cast<int>(type)] -= times;
		for(int i = 0; i < times; ++i) {
			// evento
			switch(type) {
				case configType::MAX_MSG:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getActiveStations();
					if(ids.size() < 2)
						break;
					std::uniform_int_distribution<int> pickPhrase(0, _phrases.size()-1);
					std::uniform_int_distribution<std::int_fast16_t> pickStation(0, ids.size()-1);
					std::int_fast16_t fromID = pickStation(_randEngine);
					std::int_fast16_t toID = pickStation(_randEngine);
					while (toID == fromID)
						toID = pickStation(_randEngine);

					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::MSG_ID;
					cmd.senderID = ids.at(fromID);
					cmd.receiverID = ids.at(toID);
					cmd.senderName = _mMonitor->getUserName(ids.at(fromID));
					cmd.msg = _phrases.at(pickPhrase(_randEngine));
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_TERM:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getAliveStations();
					if(ids.size() < 1)
						break;
					std::uniform_int_distribution<std::int_fast16_t> pickStation(0, ids.size()-1);
					std::int_fast16_t toID = pickStation(_randEngine);
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::TERMINATE_ID;
					cmd.senderID = -1;
					cmd.receiverID = ids.at(toID);
					cmd.senderName = std::string("MONITOR");
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_BLOCK:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getActiveStations();
					if(ids.size() < 1)
						break;
					std::uniform_int_distribution<std::int_fast16_t> pickStation(0, ids.size()-1);
					std::int_fast16_t toID = pickStation(_randEngine);
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::BLOCK_ID;
					cmd.senderID = -1;
					cmd.receiverID = ids.at(toID);
					cmd.senderName = "MONITOR";
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_RESUME:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getBlockedStations();
					if(ids.size() < 1)
						break;
					std::uniform_int_distribution<std::int_fast16_t> pickStation(0, ids.size()-1);
					std::int_fast16_t toID = pickStation(_randEngine);
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::RESUME_ID;
					cmd.senderID = -1;
					cmd.receiverID = ids.at(toID);
					cmd.senderName = "MONITOR";
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_LOGIN:
				{
					std::uniform_int_distribution<std::int_fast16_t> pickUser(0, _loginDataSet.size()-1);
					auto loginItem = _loginDataSet.begin();
					std::advance(loginItem, pickUser(_randEngine));
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::LOGIN;
					cmd.senderID = 0; // para diferenciar logins (completados)
					//cmd.receiverID = 0;
					cmd.senderName = loginItem->first;
					cmd.msg = loginItem->second;
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_LOGIN_FAIL:
				{
					std::uniform_int_distribution<std::int_fast16_t> pickRandomLength(4, 14);
					// dividir chance de usar usuário cadastrado, ou alteatório
					if(pickRandomLength(_randEngine) % 2) {
						std::uniform_int_distribution<std::int_fast16_t> pickUser(0, _loginDataSet.size()-1);
						auto loginItem = _loginDataSet.begin();
						std::advance(loginItem, pickUser(_randEngine));
						netWorkCommandPOD cmd;
						cmd.time = _currentCycle;
						cmd.type = networkEventType::LOGIN;
						cmd.senderID = -1;  // usado para detectar tentativas (falhas)
						//cmd.receiverID = 0;
						cmd.senderName = loginItem->first;
						cmd.msg = random_string(pickRandomLength(_randEngine), _randEngine);
						_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1));
						_vCommands.push_back(cmd);
					} else {
						netWorkCommandPOD cmd;
						cmd.time = _currentCycle;
						cmd.type = networkEventType::LOGIN;
						cmd.senderID = -1;  // usado para detectar tentativas (falhas)
						//cmd.receiverID = 0;
						std::string uname = random_string(pickRandomLength(_randEngine), _randEngine);
						while(uname.length() < 3){
							uname = random_string(pickRandomLength(_randEngine), _randEngine);
						}
						cmd.senderName = uname;
						cmd.msg = random_string(pickRandomLength(_randEngine), _randEngine);
						_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1));
						_vCommands.push_back(cmd);
					}
				}
					break;
				case configType::MAX_MSG_CAST:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getActiveStations();
					if(ids.size() < 1)
						break;
					std::uniform_int_distribution<int> pickPhrase(0, _phrases.size()-1);
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::MSG_CAST;
					cmd.senderID = -1;
					//cmd.receiverID = -1;
					cmd.senderName = std::string("Monitor");
					cmd.msg = _phrases.at(pickPhrase(_randEngine));
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_TERM_CAST:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getAliveStations();
					if(ids.size() < 1)
						break;
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::TERMINATE_CAST;
					cmd.senderID = -1;
					//cmd.receiverID = -1;
					cmd.senderName = "MONITOR";
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_BLOCK_CAST:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getActiveStations();
					if(ids.size() < 1)
						break;
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::BLOCK_CAST;
					cmd.senderID = -1;
					//cmd.receiverID = -1;
					cmd.senderName = "MONITOR";
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
				case configType::MAX_RESUME_CAST:
				{
					std::vector<std::int_fast16_t> ids = _mMonitor->getBlockedStations();
					if(ids.size() < 1)
						break;
					netWorkCommandPOD cmd;
					cmd.time = _currentCycle;
					cmd.type = networkEventType::RESUME_CAST;
					cmd.senderID = -1;
					//cmd.receiverID = -1;
					cmd.senderName = "MONITOR";
					if(_mMonitor->execute(cmd, std::bind(&Simulator::logEvent, this, std::placeholders::_1)))
						_vCommands.push_back(cmd);
				}
					break;
			}
		}
	}
}

void Simulator::loop()
{
	while(_currentCycle < *_config[configType::MAX_CYCLES].first) {
		for(int i = 0; i <= static_cast<int>(configType::MAX_RESUME_CAST); ++i) {
			manageEvent(static_cast<configType>(i));
		}
		_vOnStationsCycle.push_back(_mMonitor->getActiveStations().size());
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(*_config[configType::MIN_CYCLE_TIME].first)));
		ClockSubject::get().notify();
	}
	printStats();
}

void Simulator::printStats()
{
	/* Total de ciclos
	 * Total de eventos
	 * Total de estações ativas no ciclo final
	 * Total de logins
	 * Total de tentativas de login
	 * Total de mensagens individuais
	 * Total de broadcasts de mensagens
	 * Total de continuações individuais
	 * Total de broadcasts de continuação
	 * Total de bloqueios individuais
	 * Total de broadcasts de bloqueios
	 * Total de terminações individuais
	 * Total de broadcasts de terminação
	 * Tempo médio ativo por estação
	 * Média de eventos por ciclo
	 * Média de estações online por ciclo
	 * Média de logins por ciclo
	 * Média de tentativas login por ciclo
	 * Média de mensagens individuais por ciclo
	 * Média de broadcasts de mensagens por ciclo
	 * Média de continuações individuais por ciclo
	 * Média de broadcasts de continuação por ciclo
	 * Média de bloqueios individuais por ciclo
	 * Média de broadcasts de bloqueio por ciclo
	 * Média de terminações individuais por ciclo
	 * Média de broadcasts de terminação por ciclo
	 * Estação ativa mais tempo
	 * Estação ativa menos tempo
	 * Usuário com estações de maior tempo ativo total
	 * Usuário com estações de menor tempo ativo total
	 * Usuário com maior número de logins
	 * Usuário com maior número de tentativas de login
	 * Usuário com menor número de logins
	 * Usuário com menor número de tentativas de login
	 * */
	std::cout << "\n\n===============================\n=== ESTATISTICAS FINAIS =======\n===============================\n";
	std::cout << "Total de ciclos simulados: " << _currentCycle << "\n"\
		<< "Total de eventos: " << _vCommands.size() << "\nTotal de estacoes ativas no ciclo final: "
		<< _mMonitor->getActiveStations().size() << "\nTotal de logins: " << _mMonitor->getAllStations().size() << "\n"
		<< "Total de tentativas de login: "
		<<	std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::LOGIN && c.senderID == -1;})
		<< "\nTotal de mensagens individuais: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::MSG_ID;})
		<< "\nTotal de broadcasts de mensagens: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::MSG_CAST;})
		<< "\nTotal de continuações individuais: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::RESUME_ID;})
		<< "\nTotal de broadcasts de continuação: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::RESUME_CAST;})
		<< "\nTotal de bloqueios individuais: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::BLOCK_ID;})
		<< "\nTotal de broadcasts de bloqueio: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::BLOCK_CAST;})
		<< "\nTotal de terminações individuais: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::TERMINATE_ID;})
		<< "\nTotal de broadcasts de terminação: " <<
		   std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::TERMINATE_CAST;})
		<< "\nMédia de ciclos ativos por estação: "
		<< ""


		<< "\nMédia de eventos por ciclo: " << static_cast<float>(_vCommands.size())/_currentCycle << "\nMédia de estações online por ciclo simulado: ";
	float counter = 0;
	for(const auto& i : _vOnStationsCycle)
		counter += i;
	counter = static_cast<float>(counter/_currentCycle);
	std::cout << counter << "\nMédia de logins por ciclo: "
		<< static_cast<float>(_mMonitor->getAllStations().size())/_currentCycle << "\nMédia de tentativas de login por ciclo: " <<
		static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::LOGIN && c.senderID == -1;}))/_currentCycle
	<< "\nMédia de mensagens individuais por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::MSG_ID;}))/_currentCycle
	<< "\nMédia de broadcasts de mensagens por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::MSG_CAST;}))/_currentCycle
	<< "\nMédia de continuações individuais por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::RESUME_ID;}))/_currentCycle
	<< "\nMédia de broadcasts de continuação por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::RESUME_CAST;}))/_currentCycle
	<< "\nMédia de bloqueios individuais por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::BLOCK_ID;}))/_currentCycle
	<< "\nMédia de broadcasts de bloqueio por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::BLOCK_CAST;}))/_currentCycle
	<< "\nMédia de terminações individuais por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::TERMINATE_ID;}))/_currentCycle
	<< "\nMédia de broadcasts de terminação por ciclo: "
	<< static_cast<float>(std::count_if(_vCommands.begin(), _vCommands.end(), [](netWorkCommandPOD c){ return c.type == networkEventType::TERMINATE_CAST;}))/_currentCycle
	<< "\nEstação(ões) ativa mais tempo: "
	<< ""
	<< "\nEstação(ões) ativa menos tempo: "
	<< ""
	<< "\nUsuário(s)  com estações de maior tempo ativo total: "
	<< ""
	<< "\nUsuário(s)  com estações de menor tempo ativo total: "
	<< ""
	<< "\nUsuário(s) com maior número de logins: \n";
	auto userEventFilter = [&](bool bigger,std::function<bool(netWorkCommandPOD)> func) {
		std::map<std::string, std::int_fast16_t> userCounter;
		for(const auto& c : _vCommands) {
			if(func(c)) {
				auto ret = userCounter.insert(std::pair<std::string, std::int_fast16_t>(c.senderName,1));
				// caso já exista a chave, insert retorna um iterador para o elemento
				if(!ret.second)
					ret.first->second++;
			}
		}
		counter = 0;
		if(bigger) {
			for(const auto& v : userCounter)
				if(v.second > counter)
					counter = v.second;
		} else {
			for(const auto& v : userCounter)
				if(v.second < counter)
					counter = v.second;
		}
		for(const auto& v : userCounter)
			if(v.second == counter)
				std::cout << "\t" << v.first << " com " << v.second << " login(s).\n";
	};
	userEventFilter(true, [](netWorkCommandPOD c) -> bool{return c.type == networkEventType::LOGIN && c.senderID != -1;});
/*	std::map<std::string, std::int_fast16_t> userCounter;
	for(const auto& c : _vCommands) {
		if(c.type == networkEventType::LOGIN && c.senderID != -1) {
			auto ret = userCounter.insert(std::pair<std::string, std::int_fast16_t>(c.senderName,1));
			// caso já exista a chave, insert retorna um iterador para o elemento
			if(!ret.second)
				ret.first->second++;
		}
	}
	counter = 0;
	for(const auto& v : userCounter)
		if(v.second > counter)
			counter = v.second;
	for(const auto& v : userCounter)
		if(v.second == counter)
			std::cout << "\t" << v.first << " com " << v.second << " login(s).\n";*/

	std::cout << ""
	<< "Usuário com maior número de tentativas de login: "
	<< ""
	<< "\nUsuário com menor número de logins: "
	<< ""
	<< "\nUsuário com menor número de tentativas de login: "
	<< ""
	<< "\n";

}
