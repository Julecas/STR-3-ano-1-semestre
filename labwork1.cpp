// labwork1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

//DEBUG
#include <bitset>

#include <time.h>
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <my_interaction_functions.h>
#include <iostream>

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <interface.h>	
#include <interrupts.h>
}


#define mainREGION_1_SIZE   8201
#define mainREGION_2_SIZE   29905
#define mainREGION_3_SIZE   7607

//Semaphores and Mailboxes declaration
xSemaphoreHandle sem_cylinder_start_start;
xSemaphoreHandle sem_cylinder_start_finished;
xQueueHandle mbx_check_package;
xSemaphoreHandle sem_check_package_start;
xSemaphoreHandle sem_led_reject;
xSemaphoreHandle sem_conveyor;


void vAssertCalled(unsigned long ulLine, const char* const pcFileName);
static void  initialiseHeap(void);
void inicializarPortos();
void myDaemonTaskStartupHook(void);
void myTickHook(void);
void myIdleHook(void);
void vTask_calibrate_cilinder1(void* pvParameters);
void vTask_calibrate_cilinder2(void* pvParameters);
void check_package_task(void* pvParameters);
void enter_package_task(void* pvParameters);
void vTask_TurnRejectLedOn(void* pvParameters);

//test
void vTask_ConveyorOn(void* pvParameters);


int main(int argc, char** argv) {

	Sleep(2000);
	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;

	//vApplicationTickHook              = &myTickHook;
	//vApplicationIdleHook              = &myIdleHook;

	vTaskStartScheduler();

	closeChannels();
	return 0;
}

void myDaemonTaskStartupHook(void) {


	sem_cylinder_start_start = xSemaphoreCreateCounting(10, 0);
	sem_check_package_start = xSemaphoreCreateCounting(10, 0);
	sem_cylinder_start_finished = xSemaphoreCreateCounting(10, 0);
	mbx_check_package = xQueueCreate(10, sizeof(int));
	sem_led_reject = xSemaphoreCreateBinary();
	sem_conveyor = xSemaphoreCreateCounting(10, 0);

	inicializarPortos();

	//cylinder calibration tasks

	//led task
	xTaskCreate(vTask_TurnRejectLedOn, "Turn Led On Task", 100, NULL, 0, NULL);

	//conveyor task
	xTaskCreate(vTask_ConveyorOn, "Conveyor On Task", 100, NULL, 0, NULL);

	//brick input and verification tasks
	//xTaskCreate(cylinder_start_task, "Cylinder Start Task", 100, NULL, 0, NULL);
	xTaskCreate(check_package_task, "Check package Task", 100, NULL, 0, NULL);
	xTaskCreate(enter_package_task, "Enter Package Task", 100, NULL, 0, NULL);

}

void vTask_TurnRejectLedOn(void* pvParameters) {


	//waits for task completion
	//time_t secs = 5; //for de 3 segundos
	//time_t startTime = time(NULL);

	while (true) {
		xSemaphoreTake(sem_led_reject, portMAX_DELAY);

		for (int j = 0; j < 6;++j) {


			ledRejectOn();
			//printf("led on");
			vTaskDelay(250);//ms
			ledRejectOff();
			//printf("led off");
			vTaskDelay(250);//ms
		}
	}

}

void vTask_ConveyorOn(void* pvParameters) {

	while (TRUE) {

		xQueueSemaphoreTake(
			sem_conveyor,
			portMAX_DELAY
		);
		ConveyorOn();

		//ConveyorOff();
	}
}

void vTask_calibrate_cilinder1(void* pvParameters) {
	cylinder1FrontBack();
}

void vTask_calibrate_cilinder2(void* pvParameters) {
	cylinder2FrontBack();
}

void check_package_task(void* pvParameters) {
	
	uInt8 blockType;
	
	while (TRUE) {
		xQueueSemaphoreTake(sem_check_package_start,
			portMAX_DELAY);
		blockType = ReadTypeBlock(); 
		xQueueSend(mbx_check_package, &blockType,
			portMAX_DELAY);
	}						// task completion
}

void enter_package_task(void* pvParameters) {

	uInt8 blockType,
		  blockInput;

	while (TRUE) {
		
		blockType  = 0;
		blockInput = 0;

		printf("\nInsert Block type (1, 2 or 3) or type (m) for manual control ");
		
		std::cin >> blockInput;

		if (blockInput == 'm') {
			cylinderTest();
		}
		blockInput -= ( '0' + 1 );//convert to same format 


		if ( blockInput < 0 || blockInput > Block3 ) {//catch bad input
			break;
		}
		
		xSemaphoreGive(sem_conveyor);
		std::cout << "DEBUG";
		xSemaphoreGive(sem_check_package_start);
		//waits for task completion

		xQueueReceive(mbx_check_package, &blockType, portMAX_DELAY);

		switch (blockType) {
		case Block1: {printf("\nPackage received, Package Type : 1\n"); break; } //blockType é binario
		case Block2: {printf("\nPackage received, Package Type : 2\n"); break; }
		case Block3: {printf("\nPackage received, Package Type : 3\n"); break; }
		}
		if (blockType != blockInput) {  
			xSemaphoreGive(sem_led_reject); //aciona o led piscante se input != sensores
			printf("BLOCO ERRADO\nInserido:%d \nRecebido%d\n", blockInput + 1, blockType + 1 );
			goto lixo;
		}
		else {

			switch (blockType) {

			case Block1:

				senseBlockCylinder1();
				ConveyorOff();
				cylinder1FrontBack();
				//ConveyorOn();
				break;

			case Block2:

				senseBlockCylinder2();
				ConveyorOff();
				cylinder2FrontBack();
				//ConveyorOn();
				break;

			default:
				lixo:
				vTaskDelay(5000);
				ConveyorOff();
				break;

			}
		}
	}
}



void vAssertCalled(unsigned long ulLine, const char* const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
	/* Called if an assertion passed to configASSERT() fails.  See
	http://www.freertos.org/a00110.html#configASSERT for more information. */
	/* Parameters are not used. */
	(void)ulLine;
	(void)pcFileName;
	printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());

	taskENTER_CRITICAL();
	{
		/* Cause debugger break point if being debugged. */
		__debugbreak();
		/* You can step out of this function to debug the assertion by using
		   the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		   value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0)
		{
			__asm { NOP };
			__asm { NOP };
		}
	}
	taskEXIT_CRITICAL();
}


static void  initialiseHeap(void)
{
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
	/* Just to prevent 'condition is always true' warnings in configASSERT(). */
	volatile uint32_t ulAdditionalOffset = 19;
	const HeapRegion_t xHeapRegions[] =
	{
		/* Start address with dummy offsetsSize */
		{ ucHeap + 1,mainREGION_1_SIZE },
		{ ucHeap + 15 + mainREGION_1_SIZE,mainREGION_2_SIZE },
		{ ucHeap + 19 + mainREGION_1_SIZE +
				mainREGION_2_SIZE,mainREGION_3_SIZE },
		{ NULL, 0 }
	};


	configASSERT((ulAdditionalOffset +
		mainREGION_1_SIZE +
		mainREGION_2_SIZE +
		mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);
	/* Prevent compiler warnings when configASSERT() is not defined. */
	(void)ulAdditionalOffset;
	vPortDefineHeapRegions(xHeapRegions);
}


void inicializarPortos() {
	printf("\nwaiting for hardware simulator...");
	printf("\nReminding: gotoXZ requires kit calibration first...");
	createDigitalInput(0);
	createDigitalInput(1);
	createDigitalOutput(2);
	writeDigitalU8(2, 0);
	printf("\ngot access to simulator...");

}


void myTickHook(void) { //to perform periodic operations
	// do something here
	printf("\ntick hook...");
	Sleep(0);
}


void myIdleHook(void) { //runs when all task are occupied
	// do something here
	printf("\nidle hook...");
	Sleep(0);
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