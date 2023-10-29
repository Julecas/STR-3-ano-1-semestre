// labwork1.cpp : Este arquivo cont�m a fun��o 'main'. A execu��o do programa come�a e termina ali.
//

//DEBUG
#include <bitset>

//#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include <my_interaction_functions.h>
#include <iostream>
#include <string>
#include <ctime>

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
#define MAXBLOC				1000 //Max Number of block info stored

//Semaphores and Mailboxes declaration
xSemaphoreHandle sem_check_package_start;
xSemaphoreHandle sem_led_reject;
xSemaphoreHandle sem_conveyor;
xSemaphoreHandle sem_list;
xSemaphoreHandle sem_cylinder_start_back;
xSemaphoreHandle sem_cylinder1_sen;
xSemaphoreHandle sem_Block1;
xSemaphoreHandle sem_Block2;
xSemaphoreHandle sem_Lixo;
xSemaphoreHandle sem_conveyor_off;
xSemaphoreHandle sem_hist;

//mailboxes
xQueueHandle mbx_check_package;
xQueueHandle mbx_blockstat;
xQueueHandle mbx_p0;
xQueueHandle mbx_p1;
xQueueHandle mbx_p2;

// tasks handler declaration
xTaskHandle emergencyTask;
xTaskHandle resumeTask;
xTaskHandle taskSender;
xTaskHandle taskEnterPack;
xTaskHandle taskCheckPack;
xTaskHandle taskConveyor;
xTaskHandle taskLedInt;
xTaskHandle taskLedReject;
xTaskHandle taskGoBack;
xTaskHandle taskLixo;
xTaskHandle taskBlock1;
xTaskHandle taskBlock2;
xTaskHandle task_Cylinder1_sen;
xTaskHandle task_hist;


struct strj {

	uInt8	type;
	bool	rejected;
	time_t	date;

	// Constructor to set default values
	strj() : type(0), rejected(false), date(0) {}
};

typedef struct {

	int accepted = 0;
	int rejected = 0;

}stats[3];


static void  initialiseHeap (void);
void vAssertCalled			(unsigned long ulLine, const char* const pcFileName);
void inicializarPortos		();
void myDaemonTaskStartupHook(void);
void myTickHook				(void);
void myIdleHook				(void);
void check_package_task		(void* pvParameters);
void enter_package_task		(void* pvParameters);
void vTask_TurnRejectLedOn	(void* pvParameters);
void vTaskList				(void* pvParameters);
void Task_Go_Back			(void* Parameters);
void vTask_ConveyorOn		(void* pvParameters);
void Task_Cylinder1_sen		(void* Parameters);
void Task_Block1			(void* Parameters);
void Task_Block2			(void* Parameters);
void Task_Lixo				(void* Parameters);
void vTask_LedInt			(void* pvParameters);
void switch2_rising_isr		(ULONGLONG lastTime);
void switch1_rising_isr		(ULONGLONG lastTime);
void vTaskEmergency			(void* pvParameters);
void vTaskResume			(void* pvParameters);
void show_hist				(strj inv[MAXBLOC], int inv_pos);


int main(int argc, char** argv) {

	Sleep(2000);
	initialiseHeap();

	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;

	//vApplicationIdleHook              = &myIdleHook;
	//vApplicationTickHook              = &myTickHook;
	
	vTaskStartScheduler();
	closeChannels();
	return 0;
}

void myDaemonTaskStartupHook(void) {
	
	sem_check_package_start	= xSemaphoreCreateCounting(10, 0);
	sem_led_reject			= xSemaphoreCreateBinary();
	sem_conveyor			= xSemaphoreCreateCounting(10, 0);
	sem_list				= xSemaphoreCreateBinary();
	sem_cylinder_start_back	= xSemaphoreCreateBinary();
	sem_cylinder1_sen		= xSemaphoreCreateBinary();
	sem_Block1				= xSemaphoreCreateCounting(10, 0);
	sem_Block2				= xSemaphoreCreateCounting(10, 0);
	sem_Lixo				= xSemaphoreCreateCounting(10, 0);
	sem_conveyor_off		= xSemaphoreCreateCounting(10, 0);
	sem_hist				= xSemaphoreCreateBinary();

	mbx_check_package	= xQueueCreate(10, sizeof(int));
	mbx_blockstat		= xQueueCreate(1,  sizeof(stats*));
	mbx_p0				= xQueueCreate(1,  sizeof(int));
	mbx_p1				= xQueueCreate(1,  sizeof(int));
	mbx_p2				= xQueueCreate(1,  sizeof(int));


	inicializarPortos();

	//cyl tasks	

	xTaskCreate(Task_Go_Back, "Cylinder Start front to back", 1000, NULL, 1, &taskGoBack);
	xTaskCreate(Task_Cylinder1_sen, "Turn Led On Task", 100, NULL, 0, &task_Cylinder1_sen);
	xTaskCreate(Task_Block1, "Process blocks type 1", 100, NULL, 0, &taskBlock1);
	xTaskCreate(Task_Block2, "Process blocks type 2", 100, NULL, 0, &taskBlock2);
	xTaskCreate(Task_Lixo, "Process blocks type 3 and trash", 100, NULL, 0, &taskLixo);

	//list task
	xTaskCreate(vTaskList, "List Task", 100, NULL, 0, NULL);

	//led task
	xTaskCreate(vTask_TurnRejectLedOn, "Turn Led Reject On Task", 100, NULL, 0, &taskLedReject);
	xTaskCreate(vTask_LedInt, "Turn Led Int On Task", 100, NULL, 0, &taskLedInt);

	//conveyor task
	xTaskCreate(vTask_ConveyorOn, "Conveyor On Task", 100, NULL, 0, &taskConveyor);

	//brick input and verification tasks
	xTaskCreate(check_package_task, "Check package Task", 100, NULL, 0, &taskCheckPack);
	xTaskCreate(enter_package_task, "Enter Package Task", 100, NULL, 0, &taskEnterPack);

	//interrupt tasks
	attachInterrupt(1, 4, switch1_rising_isr, RISING);
	attachInterrupt(1, 3, switch2_rising_isr, RISING);
	xTaskCreate(vTaskEmergency, "vTaskEmergency ", 100, NULL, 0, &emergencyTask);
	xTaskCreate(vTaskResume, "vTaskResume ", 100, NULL, 0, &resumeTask);

}

void Task_Go_Back(void* Parameters) {

	while (true) {

		if (xSemaphoreTake(sem_cylinder_start_back, portMAX_DELAY) == pdTRUE) {
			gotoCylinderStart(0);
		}
	}
}

void Task_Cylinder1_sen(void* Parameters) {

	while (true) {

		if (senseBlockCylinder1value()) {
			
			xSemaphoreGive(sem_cylinder1_sen);

			//so it doesnt give too many semaphores
			while (senseBlockCylinder1value()) {
				continue;
			}

		}
	}
}

void Task_Block1(void* Parameters) {

	while (true) {

		if (xSemaphoreTake(sem_Block1, portMAX_DELAY) == pdTRUE) {

			std::cout << "1\n";
			senseBlockCylinder1();
			std::cout << "2\n";
			ConveyorOff();
			std::cout << "3\n";
			cylinder1FrontBack();
			std::cout << "4\n";
			xSemaphoreGive(sem_conveyor_off);
			std::cout << "5\n";
		}
	}
}

void Task_Block2(void* Parameters) {

	while (true) {
		
		if (xSemaphoreTake(sem_Block2, portMAX_DELAY) == pdTRUE){

			vTaskDelay(100);
			senseBlockCylinder2();
			ConveyorOff();
			cylinder2FrontBack();
			xSemaphoreGive(sem_conveyor_off);
		}
	}
}

void Task_Lixo(void* Parameters) {

	while (true) {

		if (xSemaphoreTake(sem_Lixo, portMAX_DELAY) == pdTRUE) {

			vTaskDelay(5000);
			ConveyorOff();
			xSemaphoreGive(sem_conveyor_off);

		}
	}
}

void vTaskEmergency(void* pvParameters) {
	// The task being suspended and resumed.
	for (;; ) {
		// The task suspends itself.
		vTaskSuspend(NULL);
		//GUARDAR BITS 

		uInt8 p0 = readDigitalU8(0);
		uInt8 p1 = readDigitalU8(1);
		uInt8 p2 = readDigitalU8(2);
				
		ConveyorOff();
		stopCylinderStart();
		stopCylinder1();
		stopCylinder2();

		xQueueSend(mbx_p0, &p0, portMAX_DELAY);
		xQueueSend(mbx_p1, &p1, portMAX_DELAY);
		xQueueSend(mbx_p2, &p2, portMAX_DELAY);

		vTaskResume(taskLedInt);

		vTaskSuspend(taskConveyor);
		vTaskSuspend(taskLedReject);
		vTaskSuspend(taskCheckPack);
		vTaskSuspend(taskEnterPack);
		vTaskSuspend(taskLixo);
		vTaskSuspend(taskBlock1);
		vTaskSuspend(taskBlock2);
		vTaskSuspend(task_Cylinder1_sen);
		vTaskSuspend(taskGoBack);

	
		// The task is now suspended, so will
		//not reach here until the ISR resumes it.
		printf("\n **** EMERGENCY task\n");
	}
}

void vTaskResume(void* pvParameters) {
	// The task being suspended and resumed.
	for (;; ) {

		// The task suspends itself.
		vTaskSuspend(NULL);

		uInt8 p0;
		uInt8 p1;
		uInt8 p2;

		xQueueReceive(mbx_p0, &p0, portMAX_DELAY);
		xQueueReceive(mbx_p1, &p1, portMAX_DELAY);
		xQueueReceive(mbx_p2, &p2, portMAX_DELAY);

		//REPOR BITS
		writeDigitalU8(0, p0);    // update ports
		writeDigitalU8(1, p1);
		writeDigitalU8(2, p2);

		vTaskSuspend(taskLedInt);

		vTaskResume(taskConveyor);
		vTaskResume(taskLedReject);
		vTaskResume(taskCheckPack);
		vTaskResume(taskEnterPack);
		vTaskResume(taskLixo);
		vTaskResume(taskBlock1);
		vTaskResume(taskBlock2);
		vTaskResume(task_Cylinder1_sen);
		vTaskResume(taskGoBack);

		// The task is now suspended, so will
		//not reach here until the ISR resumes it.
		printf("\n **** RESUME task\n");

	}
}

void switch2_rising_isr(ULONGLONG lastTime) {
	ULONGLONG time = GetTickCount64();
	printf("\nSwitch two RISING detected at time = %llu...", time);
	BaseType_t xYieldRequired;
	// Resume the suspended task.
	xYieldRequired = xTaskResumeFromISR(resumeTask);
}

void switch1_rising_isr(ULONGLONG lastTime) {
	// GetTickCount64() current time in miliseconds
	// since the system has started...
	ULONGLONG time = GetTickCount64();
	printf("\nSwitch one RISING detected at time = %llu...\n", time);
	BaseType_t xYieldRequired;
	// Resume the suspended task.
	xYieldRequired = xTaskResumeFromISR(emergencyTask);

}

void vTaskList(void* pvParameters) {

	//int blocksRecDoc1 = 0;
	//int blocksRecDoc2 = 0;
	//int blocksRecDoc3 = 0;
	//int blocksRegect  = 0;


	//AQUI DECLARAS AS VARIAVEIS A RECEBER DA MAILBOX
	stats BlockStat;

	int	  inv_pos = 0;

	//While operating, the system can provide information (e.g., number of bricks delivered at dock 1)
	while (TRUE) {
		
		if (xSemaphoreTake(sem_list, portMAX_DELAY) == pdTRUE) {
			//AQUI COLOCAS O ENDERE�O DAS VARIAVEIS EQUIVALENTES
			xQueueReceive(mbx_blockstat, &BlockStat, portMAX_DELAY);

			printf("Lista: \n");
			printf("Blocos aceites tipo 1: %d \n", BlockStat[Block1].accepted);
			printf("Blocos aceites tipo 2: %d \n", BlockStat[Block2].accepted);
			printf("Blocos aceites tipo 3: %d \n", BlockStat[Block3].accepted);
			printf("Blocos rejeitados: %d \n",
				BlockStat[Block1].rejected + BlockStat[Block2].rejected + BlockStat[Block3].rejected);
		}
		//xSemaphoreTake(sem_detailed_list, portMAX_DELAY);
		//printf("Lista detalhada: \n");
		//Show list of stored bricks (type, rejected, date when inserted).
		//Given a brick type, shows the number of correctly delivered and rejected(bricks that are type X but the user said type Y).
	}
}

void show_hist(strj inv[MAXBLOC], int inv_pos) {

	for (int i = 0; i < inv_pos; ++i) {

		std::string s = ctime( &(inv[i].date) ) + '\n';
		std::cout << "Block n" + std::to_string(i) + '\n'
			+ "	Type Block: " + std::to_string( inv[i].type) + '\n'
			+ "	Date: " + s
			+ "	Rejected: " + ( inv[i].rejected ? "Yes" : "No") + '\n';
	}
}

void vTask_TurnRejectLedOn(void* pvParameters) {

	while (true) {

		if (xSemaphoreTake(sem_led_reject, portMAX_DELAY) == pdTRUE) {

		
			for (int j = 0; j < 6; ++j) {


				ledRejectOn();
				//printf("led on");
				vTaskDelay(250);//ms
				ledRejectOff();
				//printf("led off");
				vTaskDelay(250);//ms
			}
		}
	}
}

void vTask_LedInt(void* pvParameters) {

	vTaskSuspend(NULL);

	while (true) {

		ledRejectOn();
		vTaskDelay(250);//ms

		ledRejectOff();
		vTaskDelay(250);//ms
	
	}
}

void vTask_ConveyorOn(void* pvParameters) {

	bool on = false;

	while (TRUE) {

		if (!on && xSemaphoreTake(sem_conveyor, portMAX_DELAY) == pdTRUE) {
			ConveyorOn();
			on = true;
		}

		if (on && xSemaphoreTake(sem_conveyor_off, portMAX_DELAY) == pdTRUE) {
			ConveyorOff();
			on = false;
		}
	}
}


void check_package_task(void* pvParameters) {

	uInt8 blockType;

	while (TRUE) {

		if (xSemaphoreTake(sem_check_package_start, portMAX_DELAY) == pdTRUE) {

			blockType = ReadTypeValue();
			xSemaphoreGive(sem_cylinder_start_back);

			xQueueSend(mbx_check_package, &blockType,
				portMAX_DELAY);
		}
	}						// task completion
}

void enter_package_task(void* pvParameters) {

	uInt8 blockType,
		  blockInput;

	bool ignore;

	stats BlockStat;
	strj  inv[MAXBLOC];

	int	  inv_pos = 0;

	while (TRUE) {
		
	loop:

		blockType	= 0;
		blockInput	= 0;
		ignore		= false;

		//METES AQUI O ENDERE�O DAS VARIAVEIS EQUIVALENTES 
		xQueueSend(mbx_blockstat, &BlockStat, portMAX_DELAY);
		//E MUDAS NOME DAS VARIVAIES MBX PARA FICAR COERENTE
		xSemaphoreGive(sem_list);

		vTaskDelay(50);//50ms

		printf("\nInsert Block type (1, 2 or 3)\nType (m) for manual control\nType(l) for block history\n");
		
		std::cin >> blockInput;

		switch (blockInput) {
			case 'm':
				cylinderTest();
				goto loop;

			case 'l':
				show_hist(inv, inv_pos);
				goto loop;
			case '0':
				ignore = true;
		}


		if ( blockInput < '0' || blockInput > '3') {//catch bad input
			continue;
		}
		
		xSemaphoreGive(sem_conveyor);
		xSemaphoreGive(sem_check_package_start);
		//waits for task completion
			
		//vTaskDelay(10);
		//xSemaphoreTake(sem_cylinder1_sen, portMAX_DELAY);
		xQueueReceive(mbx_check_package, &blockType, portMAX_DELAY);

		while (true) {
			if (xSemaphoreTake(sem_cylinder1_sen, portMAX_DELAY) == pdTRUE)
				std::cout << "break1\n";
				break;
		}
		std::cout << "break2\n";
		blockInput = ignore ? blockType : blockInput - ('0' + 1);

		switch (blockType) {
			case Block1: {printf("\nPackage received, Package Type : 1\n"); break; } //blockType � binario
			case Block2: {printf("\nPackage received, Package Type : 2\n"); break; }
			case Block3: {printf("\nPackage received, Package Type : 3\n"); break; }
		}
		
		inv[inv_pos++].date = time(nullptr);
		inv[inv_pos].type	= blockInput; 

		if (blockType != blockInput) {  
			xSemaphoreGive(sem_led_reject); //aciona o led piscante se input != sensores
			printf("BLOCO ERRADO\nInserido:%d \nRecebido%d\n", blockInput + 1, blockType + 1 );
			
			//blocksRegect++;//TODO DELETE

			inv[inv_pos].rejected = true;
			BlockStat[blockType].rejected++;

			xSemaphoreGive(sem_Lixo);
		}
		else {

			BlockStat[blockType].accepted++;
			inv[inv_pos].rejected = false;

			switch (blockType) {

				case Block1:

					xSemaphoreGive(sem_Block1);
				
					//blocksRecDoc1++;
					break;

				case Block2:

					xSemaphoreGive(sem_Block2);
					//blocksRecDoc2++;
					break;

				default:

					//blocksRecDoc3++;			

					xSemaphoreGive(sem_Lixo);
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



// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depura��o
// Depurar programa: F5 ou menu Depurar > Iniciar Depura��o

// Dicas para Come�ar: 
//   1. Use a janela do Gerenciador de Solu��es para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do c�digo-fonte
//   3. Use a janela de Sa�da para ver mensagens de sa�da do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de c�digo, ou Projeto > Adicionar Item Existente para adicionar arquivos de c�digo existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, v� para Arquivo > Abrir > Projeto e selecione o arquivo. sln