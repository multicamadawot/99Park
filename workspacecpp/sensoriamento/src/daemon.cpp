/*
 * daemon.cpp
 *
 * Leitor de sensores que executa em background
 * no LINUX. Pode ser iniciado e parada a partir
 * da linha de comando.
 *
 *  Created on: 7 de jan de 2016
 *      Author: Luis Paulo
 */
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

using namespace std;

#define ARQUIVO_LOG 	"/home/acer/Embarcado/embarcados-ir/unidadeiii/workspacecpp/sensoriamento/sensoramento.log"
#define ARQUIVO_PIPE 	"/home/acer/Embarcado/embarcados-ir/unidadeiii/workspacecpp/sensoriamento/sensor"

bool continuar = true;
int unsigned  sensores = 0;
char* porta = "/dev/ttyACM0";
int hPorta=0;
bool portainiciada = false;
int resultado = EXIT_SUCCESS;

int daemonize() {
	int resultado = EXIT_SUCCESS;

	pid_t daemon_id = 0;
	pid_t sid = 0;
	// PASSO 1 : CRIAR UM PROCESSO A PARTIR DO MAIN
	daemon_id = fork();
	// SE FALHAR O ID DO PROCESSO É MENOR QUE 0
	if (daemon_id < 0) {
		cout << "Falha de criacao do daemon!" << endl;
		// NÃO FAZ NADA, DESISTE DA EXECUCAO
		resultado = EXIT_FAILURE;
	} else {
		//  SE O DAEMON FOR CRIADO, FINALIZA O PROCESSO ORIGINAL
		// O TESTE CONDICIONAL ABAIXO GARANTE QUE SOMENTE SERA FINALIZADO O PROCESSO ORIGINAL
		if (daemon_id > 0) {
			cout << "Daemon criado! Saindo do processo principal" << endl;
			exit(0);
		}
		// CONFIGURA SESSÃO DE EXECUCAO DO DAEMON - START - STOP
		umask(0);

		sid = setsid();
		if (sid < 0) {
			exit(1);
		}
		// DESLIGA OS RECURSOS DE ENTRADA E SAIDA DE DADOS
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}

	return resultado;
}

bool pipeIniciado() {

	// CRIA O ARQUIVO DE PIPE
	// PERMISSÃO DE LEITURA E ESCRITA NO ARQUIVO
	int resultado = mkfifo(ARQUIVO_PIPE, 666);

	return (resultado != -1);
}


void lerSensores() {
	// SUBSTITUIR PELA CAPTURA DE INFORMAÇÕES DO ARDUINO
	hPorta  = open(porta, O_RDWR | O_NOCTTY | O_NDELAY);
				if (hPorta == -1){ // nao conseguiu pegar a porta retorna que falhou
					resultado = EXIT_FAILURE;
					usleep(2000*1000);
				} else { //configuraçao que precisa
					fcntl(hPorta, F_SETFL, 0);
					resultado = EXIT_SUCCESS;
				}

			read(hPorta, (int*)&sensores, sizeof(sensores));
			usleep(500*1000);

	}

int enviarParaPipe(int sensores) {
	int bytes = 0;
	// ABRIR O PIPE (ABRIR O ARQUIVO)
	int apipe = open(ARQUIVO_PIPE, O_RDWR);
	if (apipe != -1) {
		// CONVERTER SENSORES INTEIRO PARA STRING
		// A INTEGRACAO FUNCIONA MELHOR COM STRINGS
		stringstream ss;
		ss << sensores;
		string strSensores = ss.str();
		// ESCREVER A STRING DE SENSORES NO PIPE
		bytes = write(apipe, strSensores.c_str(), strSensores.length());
	}
	// FECHAR O PIPE
	close(apipe);

	return bytes;
}

void tratarSinal(int sinal) {
	// TRATAR OS SINAIS DE PARADA DO DAEMON (O DAEMON DEVE PARAR)
	switch (sinal) {
	case SIGHUP:
		continuar = false;
		break;
	case SIGTERM:
		continuar = false;
		break;
	case SIGQUIT:
		continuar = false;
		break;
	default:
		break;
	}
}

void iniciarTratamentoSinais() {
	// MAPEAMENTO ENTRE SINAL E FUNCAO DE TRATAMENTO DE SINAIS
	signal(SIGHUP, tratarSinal);
	signal(SIGTERM, tratarSinal);
	signal(SIGINT, tratarSinal);
	signal(SIGQUIT, tratarSinal);
}

int main(int argc, char* argv[]) {
	// ISE FOR POSSIVEL CRIAR O DAEMON
	if (daemonize() == EXIT_SUCCESS) {
		// INICIA O TRATAMENTO DE SINAIS
		iniciarTratamentoSinais();
		// LOG - REGISTRO DE OPERAÇÕES
		ofstream log(ARQUIVO_LOG, ios_base::out | ios_base::app);
		log << "Arquivo de log iniciado!" << endl;

		log << "Pipe sendo criando..." << endl;
		if (!pipeIniciado()) {
			log << "Falha de inicializacao do PIPE" << endl;

			exit(1);
		}

		continuar = true;
		while (continuar) {
		   lerSensores();

			if(resultado == 0){

			log << "Enviando sensores: " << sensores << endl;
			// ENVIA PARA O PIPE
			enviarParaPipe(sensores);
			close(hPorta);
			sleep(1);
			}else{
			log << "Problemas em conectar com a porta"<<endl;

			}
		}
		// REMOVE O ARQUIVO DE PIPE APOS USO
		unlink(ARQUIVO_PIPE);

		log << "Daemon de sensoriamento finalizado!" << endl;
	}

	return 0;
}

