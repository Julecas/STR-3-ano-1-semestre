// labwork1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <my_interaction_functions.h>

extern "C" {
#include <interface.h>

}


int main(int argc, char** argv) {
	// INPUT PORTS
	createDigitalInput(0);
	createDigitalInput(1);
	// OUTPUT PORTS
	createDigitalOutput(2);
	printf("\ncallibrate kit manually and press enter...");
	
	//calibrate cylinders
	calibrateCylinder1();
	calibrateCylinder2();

	int tecla = 0;
		while (tecla != 27) {
			tecla = _getch();
			if (tecla == 'q') {
				//printf("vai mover CylinderStart front");
				moveCylinderStartFront(); //0 -> cylinder 0
			}
			if (tecla == 'a') {
				//printf("vai mover CylinderStart back");
				moveCylinderStartBack(); //1 -> cylinder 0
			}
			if (tecla == 'z') {
				//printf("vai parar CylinderStart");
				stopCylinderStart(); //0 -> cylinder 0
			}
			if (tecla == 'w') {
				//printf("vai mover Cylinder1 front");
				moveCylinder1Front(); //3 -> cylinder 1
			}
			if (tecla == 's') {
				//printf("vai mover Cylinder2 back");
				moveCylinder1Back(); //4 -> cylinder 1
			}
			if (tecla == 'x') {
				//printf("vai parar CylinderStart");
				stopCylinder1(); //3 -> cylinder 1
			}
			if (tecla == 'e') {
				//printf("vai mover Cylinder1 front");
				moveCylinder2Front(); //5 -> cylinder 2
			}
			if (tecla == 'd') {
				//printf("vai mover Cylinder2 back");
				moveCylinder2Back(); //6 -> cylinder 2
			}
			if (tecla == 'c') {
				//printf("vai parar CylinderStart");
				stopCylinder2(); //5 -> cylinder 2
			}
			if (tecla == 'r') {
				printf("goto back c0");
				gotoCylinderStart(0); //0 back pos
			}
			if (tecla == 't') {
				printf("goto front c0");
				gotoCylinderStart(1); //0 front pos
			}
			if (tecla == 'f') {
				printf("goto back c1");
				gotoCylinder1(0); //0 back pos
			}
			if (tecla == 'g') {
				printf("goto front c1");
				gotoCylinder1(1); //0 front pos
			}
			if (tecla == 'v') {
				printf("goto back c2");
				gotoCylinder2(0); //0 back pos
			}
			if (tecla == 'b') {
				printf("goto front c2");
				gotoCylinder2(1); //0 front pos
			}
		}
		
	closeChannels();
	return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln