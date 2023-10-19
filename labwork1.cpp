// labwork1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <my_interaction_functions.h>

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

uInt8 bloquinho;


//Semaphores and Mailboxes declaration
xSemaphoreHandle sem_cylinder_start_start;
xSemaphoreHandle sem_cylinder_start_finished;
xQueueHandle mbx_check_package;
xSemaphoreHandle sem_check_package_start;


typedef struct part {
	char package_ref[50];
	int destination;
} Part;




void task_gotoCylinderStart(void* pvParameters);
void task_gotoCylinder1(void* pvParameters);
void task_gotoCylinder2(void* pvParameters);
void vAssertCalled(unsigned long ulLine, const char* const pcFileName);
static void  initialiseHeap(void);
void inicializarPortos();
void myDaemonTaskStartupHook(void);
void myTickHook(void);
void myIdleHook(void);
void vTask_calibrate_cilinder1(void* pvParameters);
void vTask_calibrate_cilinder2(void* pvParameters);
void vTask_getblock(void* pvParameters);

//test
void check_package_task(void* pvParameters);
void enter_package_task(void* pvParameters);
void cylinder_start_task(void* pvParameters);


int main(int argc, char** argv) {

	Sleep(3000);
	printf("started");


	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	

	//vApplicationTickHook              = &myTickHook;
	//vApplicationIdleHook              = &myIdleHook;
	//Sleep(3000);
	vTaskStartScheduler();

	closeChannels();
	return 0;
}

void myDaemonTaskStartupHook(void) {
	
	sem_cylinder_start_start = xSemaphoreCreateCounting(10, 0);
	sem_check_package_start = xSemaphoreCreateCounting(10, 0);
	sem_cylinder_start_finished = xSemaphoreCreateCounting(10, 0);
	mbx_check_package = xQueueCreate(10, sizeof(int));

	inicializarPortos();
	
	xTaskCreate(vTask_calibrate_cilinder1, "Clibrate Cylinder1 Task", 100, NULL, 0, NULL);
	xTaskCreate(vTask_calibrate_cilinder2, "Clibrate Cylinder2 Task", 100, NULL, 0, NULL);

	//xTaskCreate(vTask_getblock, "Read Block Task", 100, NULL, 0, NULL);
	//xTaskCreate(task_gotoCylinderStart, "Move CylinderStartFront Task", 100, NULL, 0, NULL);

	//juntar isto com a tua função martim
	xTaskCreate(cylinder_start_task, "Cylinder Start Task", 100, NULL, 0, NULL);
	xTaskCreate(check_package_task, "Check package Task", 100, NULL, 0, NULL);
	xTaskCreate(enter_package_task, "Enter Package Task", 100, NULL, 0, NULL);
}

void cylinder_start_task(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(sem_cylinder_start_start,
			portMAX_DELAY);
		gotoCylinderStart(1);
		xSemaphoreGive(sem_cylinder_start_finished);
		gotoCylinderStart(0);
	}
}

void vTask_calibrate_cilinder1(void* pvParameters) {
	calibrateCylinder1();
}

void vTask_calibrate_cilinder2(void* pvParameters) {
	calibrateCylinder2();
}

void vTask_getblock(void* pvParameters) {
	
	bloquinho = ReadTypeBlock();
	//TODO usar mailbox

}
//test
void check_package_task(void* pvParameters) {
	int packageType;
	while (TRUE) {
		xQueueSemaphoreTake(sem_check_package_start,
			portMAX_DELAY);
		packageType = 1; //substituir pela função ReadTypeBlock() falta implementar
		xQueueSend(mbx_check_package, &packageType,
			portMAX_DELAY);
	}						// task completion
}
//test
void enter_package_task(void* pvParameters) {
	int packageType;
	while (TRUE) {
		getchar(); //press key to enter a new package
		xSemaphoreGive(sem_cylinder_start_start);
		xSemaphoreGive(sem_check_package_start);
		//waits for task completion
		xQueueSemaphoreTake(sem_cylinder_start_finished, portMAX_DELAY);
		xQueueReceive(mbx_check_package, &packageType, portMAX_DELAY);
		printf("Package received, Package Type : %d", packageType);
	}
}

void task_gotoCylinderStart(void* pvParameters) {
	gotoCylinderStart(1);
	
	if (bloquinho == Block1) {
		printf("BLOCO 1\n");
	}
	if (bloquinho == Block2) {
		printf("BLOCO 2\n");

	}
	if (bloquinho == Block3) {
		printf("BLOCO 3\n");
	}
	gotoCylinderStart(0);
}
void task_gotoCylinder1(void* pvParameters) {
	gotoCylinder1(1);
	gotoCylinder1(0);
}
void task_gotoCylinder2(void* pvParameters) {
	gotoCylinder2(1);
	gotoCylinder2(0);
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