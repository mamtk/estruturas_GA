#include <fstream>
#include <string>
#include <iostream>

#include "Simulator.hh"

// IO may happen here, only

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
							*_config[configType::MAX_MSG].first);
			break;
			case '2':   // bloquear
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_BLOCK, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_BLOCK)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_BLOCK)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '3':   // continuar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_RESUME, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_RESUME)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_RESUME)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '4':   // login bem-sucedido
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_LOGIN, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_LOGIN)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_LOGIN)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '5':   // login mal-sucedido
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_LOGIN_FAIL, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_LOGIN_FAIL)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_LOGIN_FAIL)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '6':   // broadcast mensagem
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_MSG_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_MSG_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_MSG_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '7':   // broadcast terminar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_TERM_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_TERM_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_TERM_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '8':   // broadcast bloquear
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_BLOCK_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_BLOCK_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_BLOCK_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
			break;
			case '9':   // broadcast continuar
				_config.insert(std::pair<configType, configValue>\
							   (configType::MAX_RESUME_CAST, configValue(make_unique<float>\
																 (atof(buf.substr(2).c_str())), nullptr)));
				_chanceAccumulators[static_cast<int>(configType::MAX_RESUME_CAST)] = 0;
				_vecEventGenerators[static_cast<int>(configType::MAX_RESUME_CAST)] = std::uniform_real_distribution<float>(0,\
							*_config[configType::MAX_MSG].first);
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
	// [DEBUG]
	std::cout << "We better check our config map!" << std::endl;
	for (auto& kv : _config) {
		switch(kv.first) {
			case configType::PATH_UNUSEFUL_MSGS:
			case configType::PATH_LOG_STORE:
			case configType::PATH_LOGIN_DATA:
				std::cout << "\t" << (int)kv.first << " = " << *kv.second.second << std::endl;
				break;
			default:
				std::cout << "\t" << (int)kv.first << " = " << *kv.second.first << std::endl;
				break;
		}
	}
	// [DEBUG/END]
	// ler dados de usuários válidos no formato "usuario:senha"
	// iniciar ponteiro para o monitor

	loop();
}

void Simulator::logEvent(std::string event)
{
	std::cout << event << std::endl;
}

void Simulator::loop()
{
	while(_currentCycle <= *_config[configType::MAX_CYCLES].first) {
		std::cout << _vecEventGenerators[static_cast<int>(configType::MAX_TERM_CAST)](_randEngine) << std::endl;
		ClockSubject::get().notify();
	}
}
