// storage.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>

extern "C" {
	//If observe your project contents.We are sixing C files with cpp ones.
	// Therefore, inside cpp files, we need to tell which functions are written in C.
	// That is why so use extern "C" directive
#include <interface.h>
#include "storage.h"
}

/* IO Functions */
//void showMenu();
//void showStates();
//void executeControl(int keyboard);

int main() {
	printf("Welcome to the Compact  Storage Application\n");
	printf("press key");

	initializeHardwarePorts();

	int keyboard = 0;

	//showMenu();
	while (keyboard != 27) {
		if (_kbhit()) {
			keyboard = _getch();
			//executeControl(keyboard);
		}
		else {
			keyboard = 0;
		}
	}

	writeDigitalU8(2, 0x00);
	printf("\nPress a key to finish");
	int c = _getch();
	closeChannels;
	return 0;
}
/*
void showMenu() {
	//todo
}

void showStates() {
	//todo
}

void executeControl(int keyboard) {
	//todo
}
*/

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
