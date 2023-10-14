
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include <stdio.h>
#include "my_interaction_functions.h"

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


//Exmemplos
// declare the semaphore on top of the file, just below #defines
xSemaphoreHandle xSemaphore = NULL;
// declare the mailbox on top of the file, just below #defines
xQueueHandle xQueue = NULL;
xQueueHandle mbx_part;
typedef struct part {
	char package_ref[50];
	int destination;
} Part;

void cylinder_start_task(void* pvParameters) {
	while (TRUE) {
		xQueueSemaphoreTake(sem_cylinder_start_start,
			portMAX_DELAY);
		gotoCylinderStart(1);
		xSemaphoreGive(sem_cylinder_start_finished);
		gotoCylinderStart(0); //signals task completion
	}
}

void check_package_task(void* pvParameters) {
	int packageType;
	while (TRUE) {
		xQueueSemaphoreTake(sem_check_package_start,
			portMAX_DELAY);
		packageType = 1; //to be replaced by check	package funtion later
		xQueueSend(mbx_check_package, &packageType,
			portMAX_DELAY);
	}						// task completion
}

void enter_package_task(void* pvParameters) {
	int packageType;
	while (TRUE) {
		getchar(); //press key to enter a new package
		xSemaphoreGive(sem_cylinder_start_start);
		xSemaphoreGive(sem_check_package_start);
		//waits for task completion
		xQueueSemaphoreTake(sem_cylinder_start_finished, portMAX_DELAY);
		xQueueReceive(mbx_check_package, &packageType, portMAX_DELAY);
		printf("Package received, Package Type : % d", &packageType);
	}
}

void task_gotoCylinderStart(void* pvParameters) {
	gotoCylinderStart(1);
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
//test task
void vTaskCode_MbxSenderEx2(void* pvParameters) {
	Part part_info;
	strcpy_s(part_info.package_ref, sizeof(part_info.package_ref), "Test_message");
	part_info.destination = 69;
	xQueueSend(mbx_part, &part_info, portMAX_DELAY);
}

void vTaskCode_MbxReceiverEx2(void* pvParameters) {
	Part part_info;
	xQueueReceive(mbx_part, &part_info, portMAX_DELAY);
	printf("\n PlaPackage Ref = %s\n Destination = %d\n",
		part_info.package_ref, part_info.destination);
}

void vTaskCode_MbxSenderEx1(void* pvParameters) {
	double val = 0;
	for (;; ) {
		// send real numbers to another task
		// sender is faster than receiver
		val = val + 1;
		xQueueSend(xQueue, &val, 0);
		vTaskDelay(1000); //ms
	}
}
//test task
void vTaskCode_MbxReceiverEx1(void* pvParameters) {
	double val;
	for (;; ) {
		// receive real numbers from the sender
		// sender is faster than receiver
		xQueueReceive(xQueue, &val, portMAX_DELAY);
		printf("\nreceived_val =%1.3f", val);
		vTaskDelay(1000); //1s
	}
}
//test task
void vTaskCylinder1(void* pvParameters)
{
	while (TRUE)
	{
		//go front
		taskENTER_CRITICAL(); //Ler e escrever no atuador é um recurso critico
		uInt8 aa = readDigitalU8(2);
		writeDigitalU8(2, (aa & (0xff - 0x08)) | 0x10);
		taskEXIT_CRITICAL();
		
		// wait until front sensor
		while (readDigitalU8(0) & 0x08) {
			taskYIELD();
		}

		// go back
		taskENTER_CRITICAL();
		aa = readDigitalU8(2);
		vTaskDelay(10); // to simulate some latency
		writeDigitalU8(2, (aa & (0xff - 0x10)) | 0x08);
		taskEXIT_CRITICAL();
		
		// wait until back sensor
		while ((readDigitalU8(0) & 0x10)) {
			taskYIELD();
		}
	}
}

//test task
void vTaskCylinder2(void* pvParameters)
{
	while (TRUE)
	{
		// go front
		//taskENTER_CRITICAL();
		uInt8 aa = readDigitalU8(2);
		vTaskDelay(10); // to simulate some latency
		writeDigitalU8(2, (aa & (0xff - 0x20)) | 0x40);
		//taskEXIT_CRITICAL();

		// wait until front sensor
		while ((readDigitalU8(0) & 0x02)) { vTaskDelay(1); }
		
		
		//go back
		//taskENTER_CRITICAL();
		aa = readDigitalU8(2);
		writeDigitalU8(2, (aa & (0xff - 0x40)) | 0x20);
		//taskEXIT_CRITICAL();

		// wait until back sensor
		while ((readDigitalU8(0) & 0x04)) { vTaskDelay(1); }
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

//test task
void vTaskCode_1(void* pvParameters)
{
	for (;; ) {
		printf("\nHello from TASK_1");
		// Although the kernel is in preemptive mode, 
		// we should help switch to another
		// task with e.g. vTaskDelay(0) or taskYELD()
		taskYIELD(); //vai para a proxima task em queue
	}
}
//test task
void vTaskCode_2(void* pvParameters)
{
	for (;; )
	{
		printf("\nHello from TASK_2..");
		taskYIELD();
	}
}


//test task
void vTask_waits(void* pvParameters)
{
	while (1) {
		if (xSemaphoreTake(xSemaphore, 10000) == pdTRUE)
			printf("\nvTask_waits has been awaken\n");
		else
			printf("\nvTask_waits is tired of waiting\n");

	}
}
//test task
void vTask_signals(void* pvParameters)
{
	printf("write something\n");
		while (1) {
			char ch[101];
			gets_s(ch, 100);  // input from keyboard
			printf("\n vTask_signals got string '%s' from keyboard…", ch);
			if (strcmp(ch, "fim") == 0)
				exit(0); // terminate program...
			xSemaphoreGive(xSemaphore); //V
		}
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

void switch1_rising_isr(ULONGLONG lastTime) { 
	// GetTickCount64() current time in miliseconds 
	// since the system has started...
	ULONGLONG  time = GetTickCount64();
	printf("\nSwitch one RISING detected at time = %llu...", time);
}

void switch1_falling_isr(ULONGLONG lastTime) {	
	ULONGLONG  time = GetTickCount64();
	printf("\nSwitch one FALLING detected at  time = %llu...", time); 
}    

void switch2_change_isr(ULONGLONG lastTime) {	
	ULONGLONG  time = GetTickCount64();
	printf("\nSwitch two CHANGE detected at time = %llu...", time);
}

//Exemplos de declarações de tasks
	//xTaskCreate(vTaskCode_2, "vTaskCode_1", 100, NULL, 0, NULL);
	//xTaskCreate(vTaskCode_1, "vTaskCode_2", 100, NULL, 0, NULL);
	/*
	Semaphore = xSemaphoreCreateCounting(10, 0);
	xTaskCreate(vTask_waits, "vTask_waits", 100, NULL, 0, NULL);
	xTaskCreate(vTask_signals, "vTaskCode2", 100, NULL, 0, NULL);
	xTaskCreate(vTaskVertical, "vTaskVertical", 100, NULL, 0, NULL);
	xTaskCreate(vTaskHorizontal, "vTaskHorizontal", 100, NULL, 0, NULL);

	attachInterrupt(1, 5, switch1_rising_isr, RISING);
	attachInterrupt(1, 5, switch1_falling_isr, FALLING);
	attachInterrupt(1, 6, switch2_change_isr, CHANGE);

	xSemaphore = xSemaphoreCreateCounting(10, 0);
	xTaskCreate(vTask_waits, "vTask_waits", 100, NULL, 0, NULL);
	xTaskCreate(vTask_signals, "vTaskCode2", 100, NULL, 0, NULL);
	xTaskCreate(vTaskCylinder1, "vTaskCylinder1", 100, NULL, 0, NULL);
	xTaskCreate(vTaskCylinder2, "vTaskCylinder2", 100, NULL, 0, NULL);
		
		inicializarPortos();
		xTaskCreate(task_gotoCylinderStart, "task_gotoCylinderStart", 100, NULL, 0, NULL);
		xTaskCreate(task_gotoCylinder1, "task_gotoCylinder1", 100, NULL, 0, NULL);
		xTaskCreate(task_gotoCylinder2, "task_gotoCylinder2", 100, NULL, 0, NULL);

		// declare and create a mailbox
		xQueue = xQueueCreate(100, sizeof(double));
		xTaskCreate(vTaskCode_MbxReceiverEx1, "vTaskCode_MbxReceiver ", 100, xQueue, 0, NULL);
		xTaskCreate(vTaskCode_MbxSenderEx1, "vTaskCode_MbxSender ", 100, xQueue, 0, NULL);

		mbx_part = xQueueCreate(10, sizeof(Part));
		xTaskCreate(vTaskCode_MbxReceiverEx2, "vTaskCode_MbxReceiver ", 100, NULL, 0, NULL);
		xTaskCreate(vTaskCode_MbxSenderEx2, "vTaskCode_MbxSender ", 100, NULL, 0, NULL);
		*/
void myDaemonTaskStartupHook(void) {
	
	inicializarPortos();
	sem_cylinder_start_start = xSemaphoreCreateCounting(10, 0);
	sem_check_package_start = xSemaphoreCreateCounting(10, 0);
	sem_cylinder_start_finished = xSemaphoreCreateCounting(10, 0);
	mbx_check_package = xQueueCreate(10, sizeof(int));
	xTaskCreate(cylinder_start_task, "Cylinder Start Task", 100, NULL, 0, NULL);
	xTaskCreate(check_package_task, "Check package Task", 100, NULL, 0, NULL);
	xTaskCreate(enter_package_task, "Enter Package Task", 100, NULL, 0, NULL);

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

int main() {

	Sleep(3000);
	printf("started");

	initialiseHeap();
	vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
	//vApplicationTickHook              = &myTickHook;
	//vApplicationIdleHook              = &myIdleHook;

	vTaskStartScheduler();

	closeChannels();
	return 0;
}


