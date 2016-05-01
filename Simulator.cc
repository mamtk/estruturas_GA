#include <fstream>
#include <string>

#include "Simulator.hh"

// IO may happen here, only

void Simulator::runSimulation(std::string file)
{
	std::ifstream configFile(file);
	std::string buf;
	while (std::getline(configFile, buf))
	{
		// ler arquivo de configuração e assumir variáveis
		switch(buf.at(0)) {
			// máximo de tipos de eventos por ciclo simulado
			case '0':   // mensagem
			break;
			case '1':   // terminar
			break;
			case '2':   // bloquear
			break;
			case '3':   // continuar
			break;
			case '4':   // login bem-sucedido
			break;
			case '5':   // login mal-sucedido
			break;
			case '6':   // broadcast mensagem
			break;
			case '7':   // boardcast terminar
			break;
			case '8':   // broadcast continuar
			break;

			// outras configurações
			case '9':   // tempo (ms) real mínimo de cada ciclo
			break;
			case 'a':   // lista de usuários da simulação
			break;
			case 'b':   // arquivo de relatório
			break;
		}
	}
}
