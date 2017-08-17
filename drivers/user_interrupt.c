
#include "LPC17xx.h"
#include "drivers.h"
#include "../app/cfg/cfg_handler.h"
//GPIO0 �� GPIO2��������Ϊ�ⲿ�жϣ������ж����ֻ��һ��EINT3
//INPUT1 p2.6  INPUT2  P2.7

#define BIT7_MASK  0x0000080
#define BIT6_MASK  0x0000040

void Interrupt_Init( void )
{
	//����Ϊ����״̬
	LPC_GPIO2->FIODIR &= (~(1<<7))|(~(1<<6));;
	LPC_GPIOINT->IO2IntEnR |=BIT7_MASK | BIT6_MASK;	//����Ϊ����״̬
//LPC_GPIOINT->IO2IntEnF |=BIT7_MASK ;
	LPC_GPIOINT->IO2IntClr|=BIT7_MASK|BIT6_MASK ;
	NVIC_SetPriority( EINT3_IRQn , 0 );
	NVIC_EnableIRQ(EINT3_IRQn);
}


uint32_t idx_7,idx_6;
uint32_t idx_6_total,idx_7_total,flow_sensor_pulses_total;
uint16_t idx_7_freq,idx_6_freq,flow_sensor_value;
uint8_t flow_cali_flag = 0;
uint32_t flow_sensor_tick;
extern uint32_t idx_7_tick,idx_6_tick;

void flow_cali_enble(void)
{
	flow_cali_flag = START_FLOW_CALI;
}

void flow_cali_clear(void)
{
  flow_cali_flag = CLEAR_FLOW_CALI_PARA;
}


uint32_t get_cali_flow_pulses_total(void)
{
	if(idx_7_total > idx_6_total)
		flow_sensor_pulses_total = idx_7_total;
	else
		flow_sensor_pulses_total = idx_6_total;
	
	return flow_sensor_pulses_total;
}


uint16_t get_cali_flow_freq(void)
{
	if(idx_7_freq > idx_6_freq)
	  flow_sensor_value =	idx_7_freq;
	else
		flow_sensor_value =	idx_6_freq;
	
	return flow_sensor_value;
}

uint32_t get_cail_flow_time(void)
{
	if(idx_7_tick > idx_6_tick)
	  flow_sensor_tick =	idx_7_tick;
	else
		flow_sensor_tick =	idx_6_tick;
	
 return flow_sensor_tick;
}


void EINT3_IRQHandler(void)
{
	get_cali_flow_pulses_total();
	get_cail_flow_time();
	if( LPC_GPIOINT->IO2IntStatR & BIT7_MASK )
	{
		idx_7++;
		if(flow_cali_flag == START_FLOW_CALI)
		{
		    idx_7_total++;
		}
		else if(flow_cali_flag == CANCEL_FLOW_CALI)
		{
		  idx_7_total = 0;
		}
		if(idx_7_total == 1) //��һ��ʱ��ʼ��ʱ
				idx_7_tick = 0;

		LPC_GPIOINT->IO2IntClr |= BIT7_MASK ;
	}
	
	if( LPC_GPIOINT->IO2IntStatR & BIT6_MASK )
	{
		idx_6++;
		if(flow_cali_flag == START_FLOW_CALI)
		{
		    idx_6_total++;
			  if(idx_6_total == 1) //��һ��ʱ��ʼ��ʱ
					idx_6_tick = 0;
		}
		else if(flow_cali_flag == CANCEL_FLOW_CALI)
		{
		  idx_6_total = 0;
		}
		LPC_GPIOINT->IO2IntClr |= BIT6_MASK ;
	}
}

