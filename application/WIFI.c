#include "WIFI.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Lcd_Driver.h"
#include	"GUI.h"
//#define DEMO_WIFI_SSID          "vivoX50"
//#define DEMO_WIFI_PWD           "123123123"

#define DEMO_WIFI_SSID          "xinjiuIPHONE"
#define DEMO_WIFI_PWD           "huangxijie612"
//#define DEMO_TCP_SERVER_IP      "172.20.10.6"
//#define DEMO_TCP_SERVER_PORT    "8080"
#define DEMO_UDP_SERVER_IP      "172.20.10.6"
#define DEMO_UDP_SERVER_PORT    "9090"
//#define DEMO_TCP_SERVER_IP      "10.15.5.214"
//#define DEMO_TCP_SERVER_PORT    "8080"
extern UART_HandleTypeDef huart6;
static struct
{
    uint8_t buf[ATK_MW8266D_UART_RX_BUF_SIZE];              /* ֡���ջ��� */
    struct
    {
        uint16_t len    : 15;                               /* ֡���ճ��ȣ�sta[14:0] */
        uint16_t finsh  : 1;                                /* ֡������ɱ�־��sta[15] */
    } sta;                                                  /* ֡״̬��Ϣ */
} g_uart_rx_frame = {0};                                    /* ATK-MW8266D UART����֡������Ϣ�ṹ�� */



static uint8_t g_uart_tx_buf[ATK_MW8266D_UART_TX_BUF_SIZE]; /* ATK-MW8266D UART���ͻ��� */
    char ip_buf[16];
uint8_t wifi_tcp_init(void)
{
	   uint8_t ret;

    uint8_t key;
    uint8_t is_unvarnished = 0;
			atk_mw8266d_hw_init();
	atk_mw8266d_hw_reset();
    if (atk_mw8266d_at_test() != ATK_MW8266D_EOK)   /* ATK-MW8266D ATָ����� */
    {
        return ATK_MW8266D_ERROR;
    } 
	  ret  = atk_mw8266d_restore();                               /* �ָ��������� */
    ret += atk_mw8266d_at_test();                               /* AT���� */
    ret += atk_mw8266d_set_mode(3);                             /* Stationģʽ */
    ret += atk_mw8266d_sw_reset();                              /* �����λ */
  //  ret += atk_mw8266d_ate_config(0);                           /* �رջ��Թ��� */
    ret += atk_mw8266d_join_ap(DEMO_WIFI_SSID, DEMO_WIFI_PWD);  /* ����WIFI */
    ret += atk_mw8266d_get_ip(ip_buf);                          /* ��ȡIP��ַ */	
	
    /* ����	UDP������ */
    ret = atk_mw8266d_connect_mode(1);
		ret=atk_mw8266d_connect_udp_server(DEMO_UDP_SERVER_IP,DEMO_UDP_SERVER_PORT);
    /* ���¿�ʼ�����µ�һ֡���� */
    atk_mw8266d_uart_rx_restart();	
	//	 ret = atk_mw8266d_enter_unvarnished();		
		if(ret!=0)
			{
			Gui_DrawFont_Num32(10,10, BLACK, WHITE,111);
			return ATK_MW8266D_ERROR;		
			}
		else{
		Gui_DrawFont_Num32(10,10, BLACK, WHITE,000);
			return ATK_MW8266D_EOK;
		}

}
//uint8_t wifi_receive[20];
//extern uint8_t RxBuffer_2[50];
//void	wifi_tcp(void)
//{
//	for(uint8_t i=0;i<=19;i++)
//	{
//			if(RxBuffer_2[i]!=0x00)
//			{
//			wifi_receive[i]=RxBuffer_2[i];
//			RxBuffer_2[i]=0x00;
//				if(RxBuffer_2[i+1]==0x00)
//				{
//					for(uint8_t j=i+1;j<=19;j++)
//			wifi_receive[j]=0x00;			
//						break;
//				}
//			}

//	}
//        /* ����͸��������TCP Server�����ݵ����ڵ������� */
// //    demo_upload_data(1);
//}
/**
 * @brief       ATK-MW8266D UART�жϻص�����
 * @param       ��
 * @retval      ��
 */
 uint8_t receive_u6[128];
void ATK_MW8266D_UART_IRQHandler(void)
{
    uint8_t tmp;
    
    if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_ORE) != RESET)        /* UART���չ��ش����ж� */
    {
        __HAL_UART_CLEAR_OREFLAG(&huart6);                           /* ������չ��ش����жϱ�־ */
        (void)huart6.Instance->SR;                                   /* �ȶ�SR�Ĵ������ٶ�DR�Ĵ��� */
        (void)huart6.Instance->DR;
    }
    
    if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE) != RESET)       /* UART�����ж� */
    {
        HAL_UART_Receive(&huart6, &tmp, 1, HAL_MAX_DELAY);           /* UART�������� */
        
        if (g_uart_rx_frame.sta.len < (ATK_MW8266D_UART_RX_BUF_SIZE - 1))   /* �ж�UART���ջ����Ƿ����
                                                                             * ����һλ��������'\0'
                                                                             */
        {
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
            g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
        }
        else                                                                /* UART���ջ������ */
        {
            g_uart_rx_frame.sta.len = 0;                                    /* ����֮ǰ�յ������� */
            g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = tmp;             /* �����յ�������д�뻺�� */
            g_uart_rx_frame.sta.len++;                                      /* ���½��յ������ݳ��� */
        }
    }
    
    if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_IDLE) != RESET)       /* UART���߿����ж� */
    {
        g_uart_rx_frame.sta.finsh = 1;                                      /* ���֡������� */
        
        __HAL_UART_CLEAR_IDLEFLAG(&huart6);                          /* ���UART���߿����ж� */
    }
		for(uint8_t i;i<=127;i++)
		receive_u6[i]=g_uart_rx_frame.buf[i];
}
/**
 * @brief       ����͸��ʱ����������TCP Server�����ݷ��͵����ڵ�������
 * @param       is_unvarnished: 0��δ����͸��
 *                              1���ѽ���͸��
 * @retval      ��
 */
static void demo_upload_data(uint8_t is_unvarnished)
{
    uint8_t *buf;
    
    if (is_unvarnished == 1)
    {
        /* ��������ATK-MW8266D UART��һ֡���� */
        buf = atk_mw8266d_uart_rx_get_frame();
        if (buf != NULL)
        {
            /* �ؿ���ʼ��������ATK-MW8266D UART������ */
            atk_mw8266d_uart_rx_restart();
        }
    }
}

/**
 * @brief       ATK-MW8266D����TCP������
 * @param       server_ip  : TCP������IP��ַ
 *              server_port: TCP�������˿ں�
 * @retval      ATK_MW8266D_EOK  : ����TCP�������ɹ�
 *              ATK_MW8266D_ERROR: ����TCP������ʧ��
 */
uint8_t atk_mw8266d_connect_tcp_server(char *server_ip, char *server_port)
{
    uint8_t ret;
    char cmd[64];
    
    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s", server_ip, server_port);
    ret = atk_mw8266d_send_at_cmd(cmd, "CONNECT", 5000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

/**
 * @brief       ATK-MW8266D����UCP������
 * @param       server_ip  : UCP������IP��ַ
 *              server_port: UCP�������˿ں�
 * @retval      ATK_MW8266D_EOK  : ����UCP�������ɹ�
 *              ATK_MW8266D_ERROR: ����UCP������ʧ��
 */
uint8_t atk_mw8266d_connect_mode(uint8_t i)
{
    uint8_t ret;
    char cmd[64];
    
    //sprintf(cmd, "AT+CIPMUX=1");
            ret = atk_mw8266d_send_at_cmd("AT+CIPMUX=1", "OK", 500);    /* Stationģʽ */
  //  ret = atk_mw8266d_send_at_cmd(cmd, "CONNECT", 5000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}



/**
 * @brief       ATK-MW8266D����UDP������
 * @param       server_ip  : UDP������IP��ַ
 *              server_port: UDP�������˿ں�
 * @retval      ATK_MW8266D_EOK  : ����UDP�������ɹ�
 *              ATK_MW8266D_ERROR: ����UDP������ʧ��
 */
uint8_t atk_mw8266d_connect_udp_server(char *server_ip, char *server_port)
{
    uint8_t ret;
    char cmd[64];
    
    sprintf(cmd, "AT+CIPSTART=4,\"UDP\",\"%s\",%s,9090,0", server_ip, server_port);
    ret = atk_mw8266d_send_at_cmd(cmd, "CONNECT", 5000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}


/**
 * @brief       ATK-MW8266D����ATָ��
 * @param       cmd    : �����͵�ATָ��
 *              ack    : �ȴ�����Ӧ
 *              timeout: �ȴ���ʱʱ��
 * @retval      ATK_MW8266D_EOK     : ����ִ�гɹ�
 *              ATK_MW8266D_ETIMEOUT: �ȴ�����Ӧ��ʱ������ִ��ʧ��
 */
uint8_t atk_mw8266d_send_at_cmd(char *cmd, char *ack, uint32_t timeout)
{
    uint8_t *ret = NULL;
    
    atk_mw8266d_uart_rx_restart();
    atk_mw8266d_uart_printf("%s\r\n", cmd);
    
    if ((ack == NULL) || (timeout == 0))
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        while (timeout > 0)
        {
            ret = atk_mw8266d_uart_rx_get_frame();
            if (ret != NULL)
            {
                if (strstr((const char *)ret, ack) != NULL)
                {
                    return ATK_MW8266D_EOK;
                }
                else
                {
                    atk_mw8266d_uart_rx_restart();
                }
            }
            timeout--;
            HAL_Delay(1);
        }
        
        return ATK_MW8266D_ETIMEOUT;
    }
}
/**
 * @brief       ATK-MW8266D UART���¿�ʼ��������
 * @param       ��
 * @retval      ��
 */
void atk_mw8266d_uart_rx_restart(void)
{
    g_uart_rx_frame.sta.len     = 0;
    g_uart_rx_frame.sta.finsh   = 0;
}
/**
 * @brief       ATK-MW8266D UART printf
 * @param       fmt: ����ӡ������
 * @retval      ��
 */
void atk_mw8266d_uart_printf(char *fmt, ...)
{
    va_list ap;
    uint16_t len;
    
    va_start(ap, fmt);
    vsprintf((char *)g_uart_tx_buf, fmt, ap);
    va_end(ap);
    
    len = strlen((const char *)g_uart_tx_buf);
    HAL_UART_Transmit(&huart6, g_uart_tx_buf, len, HAL_MAX_DELAY);
}
/**
 * @brief       ��ȡATK-MW8266D UART���յ���һ֡����
 * @param       ��
 * @retval      NULL: δ���յ�һ֡����
 *              ����: ���յ���һ֡����
 */
uint8_t *atk_mw8266d_uart_rx_get_frame(void)
{
    if (g_uart_rx_frame.sta.finsh == 1)
    {
        g_uart_rx_frame.buf[g_uart_rx_frame.sta.len] = '\0';
        return g_uart_rx_frame.buf;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief       ATK-MW8266D ATָ�����
 * @param       ��
 * @retval      ATK_MW8266D_EOK  : ATָ����Գɹ�
 *              ATK_MW8266D_ERROR: ATָ�����ʧ��
 */
uint8_t atk_mw8266d_at_test(void)
{
    uint8_t ret;
    uint8_t i;
    
    for (i=0; i<10; i++)
    {
        ret = atk_mw8266d_send_at_cmd("AT", "OK", 500);
        if (ret == ATK_MW8266D_EOK)
        {
            return ATK_MW8266D_EOK;
        }
    }
    
    return ATK_MW8266D_ERROR;
}


/**
 * @brief       ATK-MW8266D�ָ���������
 * @param       ��
 * @retval      ATK_MW8266D_EOK  : �ָ��������óɹ�
 *              ATK_MW8266D_ERROR: �ָ���������ʧ��
 */
uint8_t atk_mw8266d_restore(void)
{
    uint8_t ret;
    
    ret = atk_mw8266d_send_at_cmd("AT+RESTORE", "ready", 3000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

/**
 * @brief       ����ATK-MW8266D����ģʽ
 * @param       mode: 1��Stationģʽ
 *                    2��APģʽ
 *                    3��AP+Stationģʽ
 * @retval      ATK_MW8266D_EOK   : ����ģʽ���óɹ�
 *              ATK_MW8266D_ERROR : ����ģʽ����ʧ��
 *              ATK_MW8266D_EINVAL: mode�������󣬹���ģʽ����ʧ��
 */
uint8_t atk_mw8266d_set_mode(uint8_t mode)
{
    uint8_t ret;
    
    switch (mode)
    {
        case 1:
        {
            ret = atk_mw8266d_send_at_cmd("AT+CWMODE=1", "OK", 500);    /* Stationģʽ */
            break;
        }
        case 2:
        {
            ret = atk_mw8266d_send_at_cmd("AT+CWMODE=2", "OK", 500);    /* APģʽ */
            break;
        }
        case 3:
        {
            ret = atk_mw8266d_send_at_cmd("AT+CWMODE=3", "OK", 500);    /* AP+Stationģʽ */
            break;
        }
        default:
        {
            return ATK_MW8266D_EINVAL;
        }
    }
    
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}
/**
 * @brief       ATK-MW8266D�����λ
 * @param       ��
 * @retval      ATK_MW8266D_EOK  : �����λ�ɹ�
 *              ATK_MW8266D_ERROR: �����λʧ��
 */
uint8_t atk_mw8266d_sw_reset(void)
{
    uint8_t ret;
    
    ret = atk_mw8266d_send_at_cmd("AT+RST", "OK", 500);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}
/**
 * @brief       ATK-MW8266D���û���ģʽ
 * @param       cfg: 0���رջ���
 *                   1���򿪻���
 * @retval      ATK_MW8266D_EOK  : ���û���ģʽ�ɹ�
 *              ATK_MW8266D_ERROR: ���û���ģʽʧ��
 */
uint8_t atk_mw8266d_ate_config(uint8_t cfg)
{
    uint8_t ret;
    
    switch (cfg)
    {
        case 0:
        {
            ret = atk_mw8266d_send_at_cmd("ATE0", "OK", 500);   /* �رջ��� */
            break;
        }
        case 1:
        {
            ret = atk_mw8266d_send_at_cmd("ATE1", "OK", 500);   /* �򿪻��� */
            break;
        }
        default:
        {
            return ATK_MW8266D_EINVAL;
        }
    }
    
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}

/**
 * @brief       ATK-MW8266D����WIFI
 * @param       ssid: WIFI����
 *              pwd : WIFI����
 * @retval      ATK_MW8266D_EOK  : WIFI���ӳɹ�
 *              ATK_MW8266D_ERROR: WIFI����ʧ��
 */
uint8_t atk_mw8266d_join_ap(char *ssid, char *pwd)
{
    uint8_t ret;
    char cmd[64];
    
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
    ret = atk_mw8266d_send_at_cmd(cmd, "WIFI GOT IP", 10000);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}
/**
 * @brief       ATK-MW8266D��ȡIP��ַ
 * @param       buf: IP��ַ����Ҫ16�ֽ��ڴ�ռ�
 * @retval      ATK_MW8266D_EOK  : ��ȡIP��ַ�ɹ�
 *              ATK_MW8266D_ERROR: ��ȡIP��ַʧ��
 */
uint8_t atk_mw8266d_get_ip(char *buf)
{
    uint8_t ret;
    char *p_start;
    char *p_end;
    
    ret = atk_mw8266d_send_at_cmd("AT+CIFSR", "OK", 500);
    if (ret != ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_ERROR;
    }
    
    p_start = strstr((const char *)atk_mw8266d_uart_rx_get_frame(), "\"");
    p_end = strstr(p_start + 1, "\"");
    *p_end = '\0';
    sprintf(buf, "%s", p_start + 1);
    
    return ATK_MW8266D_EOK;
}
/**
 * @brief       ATK-MW8266D����͸��
 * @param       ��
 * @retval      ATK_MW8266D_EOK  : ����͸���ɹ�
 *              ATK_MW8266D_ERROR: ����͸��ʧ��
 */
uint8_t atk_mw8266d_enter_unvarnished(void)
{
    uint8_t ret;
    
    ret  = atk_mw8266d_send_at_cmd("AT+CIPMODE=1", "OK", 500);
    ret += atk_mw8266d_send_at_cmd("AT+CIPSEND", ">", 500);
    if (ret == ATK_MW8266D_EOK)
    {
        return ATK_MW8266D_EOK;
    }
    else
    {
        return ATK_MW8266D_ERROR;
    }
}
/**
 * @brief       ATK-MW8266DӲ����λ
 * @param       ��
 * @retval      ��
 */
void atk_mw8266d_hw_reset(void)
{
    ATK_MW8266D_RST(0);
    HAL_Delay(100);
    ATK_MW8266D_RST(1);
    HAL_Delay(500);
}
/**
 * @brief       ATK-MW8266DӲ����ʼ��
 * @param       ��
 * @retval      ��
 */
static void atk_mw8266d_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    ATK_MW8266D_RST_GPIO_CLK_ENABLE();
    
    gpio_init_struct.Pin = ATK_MW8266D_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ATK_MW8266D_RST_GPIO_PORT, &gpio_init_struct);
}
