#ifndef GPTP_LOG_H
#define GPTP_LOG_H

#include "gptpcmn.h"

#define LOG_ERR       1
#define LOG_WARNING   2
#define LOG_NOTICE    3
#define LOG_INFO      4
#define LOG_DEBUG     5

#define GPTP_LOG_DEST_CONSOLE 0
#define GPTP_LOG_DEST_SYSLOG  1

#define GPTP_LOG_ERROR		LOG_ERR
#define GPTP_LOG_WARNING	LOG_WARNING
#define GPTP_LOG_NOTICE		LOG_NOTICE
#define GPTP_LOG_INFO		LOG_INFO
#define GPTP_LOG_DEBUG		LOG_DEBUG

#define GPTP_LOG_COLOR_ERROR    "\x1b[31;1m"
#define GPTP_LOG_COLOR_WARNING  "\x1b[35;1m"
#define GPTP_LOG_COLOR_NOTICE   "\x1b[34;1m"
#define GPTP_LOG_COLOR_INFO     "\x1b[33;1m"
#define GPTP_LOG_COLOR_DEBUG    "\x1b[30;1m"
#define GPTP_LOG_COLOR_RESET    "\x1b[0m"

#define GPTP_LOG_LVL_DEFAULT	LOG_ERR

void gPTP_openLog(int dest, int logLvl);
void gPTP_logMsg(int prio, char* format, ...);
#endif
extern char        uart_buf[256];
    #define SCI_UART_BAUDRATE_19200    (19200)

#ifndef RZT2_RSK_PNS
extern QueueHandle_t printf_queue;

    #define app_printf(format, ...); {\
            int bytes =  sprintf(uart_buf, format, ##__VA_ARGS__); \
            xQueueSend(printf_queue, &uart_buf[0], 0);\
            }
              
/*              R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)uart_buf, bytes);  \
              while(!g_transfer_complete); \
            g_transfer_complete = 0; \
            }*/

    #define app_vprintf(format, ...); {\
            int     bytes =  vsprintf(uart_buf, format, ##__VA_ARGS__); \
            xQueueSend(printf_queue, &uart_buf[0], 0);\
            }
#endif
              
      /*        R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)uart_buf, bytes);  \
            while(!g_transfer_complete); \
            g_transfer_complete = 0; \
            }
*/
