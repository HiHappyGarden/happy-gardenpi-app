#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "rp2040_config.h"

extern uint64_t FreeRTOSRunTimeTicks;               //for clion monitoring
#ifdef __cplusplus
extern "C" {
#endif

extern void ConfigureTimerForRunTimeStats(void);

#ifdef __cplusplus
}
#endif

/* Use Pico SDK ISR handlers */
#define vPortSVCHandler         isr_svcall
#define xPortPendSVHandler      isr_pendsv
#define xPortSysTickHandler     isr_systick

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      133000000
#define configTICK_RATE_HZ                      1000
#define configMAX_PRIORITIES                    6
#define configMINIMAL_STACK_SIZE                256
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3
#define configUSE_MUTEXES                       1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_NEWLIB_REENTRANT              0
//#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        0
#define configTOTAL_HEAP_SIZE                   (141 * 1024)

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          1
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           1 //1 for clion monitoring
#define configUSE_TRACE_FACILITY                1 //1 for clion monitoring
#define configRECORD_STACK_HIGH_ADDRESS         1 //1 for clion monitoring
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    ConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()            FreeRTOSRunTimeTicks
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               4
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Define to trap errors during development. */
#define configASSERT( x )

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xTaskResumeFromISR              1

#define configENABLE_BACKWARD_COMPATIBILITY     1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

// SMP port only
//
#define configNUMBER_OF_CORES                   1
//#define configNUMBER_OF_CORES                   2
//#define configTICK_CORE                         0
//#define configRUN_MULTIPLE_PRIORITIES           1
//#define configUSE_CORE_AFFINITY                 1
//#define configSUPPORT_PICO_SYNC_INTEROP         1
//#define configSUPPORT_PICO_TIME_INTEROP         1
//#define configUSE_PASSIVE_IDLE_HOOK             0
//#define configRUN_MULTIPLE_PRIORITIES           1
//#define portSUPPORT_SMP                         1

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */
