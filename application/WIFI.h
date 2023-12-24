#ifndef WIFI_H
#define WIFI_H


#include "main.h"
/* 引脚定义 */
#define ATK_MW8266D_RST_GPIO_PORT           GPIOE
#define ATK_MW8266D_RST_GPIO_PIN            GPIO_PIN_9
#define ATK_MW8266D_RST_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0) /* PF口时钟使能 */

/* IO操作 */
#define ATK_MW8266D_RST(x)                  do{ x ?                                                                                     \
                                                HAL_GPIO_WritePin(ATK_MW8266D_RST_GPIO_PORT, ATK_MW8266D_RST_GPIO_PIN, GPIO_PIN_SET) :  \
                                                HAL_GPIO_WritePin(ATK_MW8266D_RST_GPIO_PORT, ATK_MW8266D_RST_GPIO_PIN, GPIO_PIN_RESET); \
                                            }while(0)
/* UART收发缓冲大小 */
#define ATK_MW8266D_UART_RX_BUF_SIZE            128
#define ATK_MW8266D_UART_TX_BUF_SIZE            64
																						
																						
/* 错误代码 */
#define ATK_MW8266D_EOK         0   /* 没有错误 */
#define ATK_MW8266D_ERROR       1   /* 通用错误 */
#define ATK_MW8266D_ETIMEOUT    2   /* 超时错误 */
#define ATK_MW8266D_EINVAL      3   /* 参数错误 */
uint8_t wifi_tcp_init(void);
void wifi_tcp(void);																					
uint8_t atk_mw8266d_init(uint32_t baudrate);																			
static void atk_mw8266d_hw_init(void);
void atk_mw8266d_uart_rx_restart(void);
void atk_mw8266d_hw_reset(void);
void atk_mw8266d_uart_printf(char *fmt, ...);
uint8_t atk_mw8266d_at_test(void);		
uint8_t *atk_mw8266d_uart_rx_get_frame(void);				

uint8_t atk_mw8266d_send_at_cmd(char *cmd, char *ack, uint32_t timeout);

uint8_t atk_mw8266d_restore(void);
uint8_t atk_mw8266d_set_mode(uint8_t mode);
uint8_t atk_mw8266d_sw_reset(void);
uint8_t atk_mw8266d_ate_config(uint8_t cfg);
uint8_t atk_mw8266d_join_ap(char *ssid, char *pwd);
uint8_t atk_mw8266d_get_ip(char *buf);

uint8_t atk_mw8266d_connect_tcp_server(char *server_ip, char *server_port);
uint8_t atk_mw8266d_enter_unvarnished(void);
static void demo_upload_data(uint8_t is_unvarnished);
uint8_t atk_mw8266d_enter_unvarnished(void);
void ATK_MW8266D_UART_IRQHandler(void);
uint8_t atk_mw8266d_connect_udp_server(char *server_ip, char *server_port);
uint8_t atk_mw8266d_connect_mode(uint8_t i);
#endif