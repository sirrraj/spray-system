/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : BL_WKM2_MAIN.uvproj
  * @File Name          : can_hw.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-08-31
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_HW_H__
#define __CAN_HW_H__

/* Private define ------------------------------------------------------------*/
/* for can pin p0.0,p0.1 */
//#define CAN1_RD_PIN           DEF_BIT_00  /* p0.0,p0.1 FCNT2 -- RD1,TD1 */
//#define CAN1_TD_PIN  		    DEF_BIT_01

#define CAN1_RD_PIN             DEF_BIT_21  /* p0.21,p0.22 FCNT4 -- RD1,TD1 */
#define CAN1_TD_PIN  		    DEF_BIT_22

#define CAN1_PORT           	GPIO_PORT0_FAST
#define CAN1_PINS				(CAN1_RD_PIN | CAN1_TD_PIN)

#define CAN2_RD_PIN             DEF_BIT_04  /* p0.4,p0.5 FCNT3 -- RD2,TD2 */
#define CAN2_TD_PIN  		    DEF_BIT_05

/* ICR bit field & bit mask & bit set */
#define ICR_RI_SET						DEF_BIT_00
#define ICR_RI_MASK                     DEF_BIT_00

#define ICR_TI1_SET						DEF_BIT_01
#define ICR_TI1_MASK                    DEF_BIT_01

#define ICR_EI_SET                      DEF_BIT_02
#define ICR_EI_MASK                     DEF_BIT_02

#define ICR_DOI_SET                     DEF_BIT_03
#define ICR_DOI_MASK                    DEF_BIT_03

#define ICR_WUI_SET                     DEF_BIT_04
#define ICR_WUI_MASK                    DEF_BIT_04

#define ICR_EPI_SET                     DEF_BIT_05
#define ICR_EPI_MASK                    DEF_BIT_05

#define ICR_ALI_SET                     DEF_BIT_06
#define ICR_ALI_MASK                    DEF_BIT_06

#define ICR_BEI_SET                     DEF_BIT_07
#define ICR_BEI_SET                     DEF_BIT_07

#define ICR_IDI_SET                     DEF_BIT_08
#define ICR_IDI_MASK                    DEF_BIT_08

#define ICR_TI2_SET                     DEF_BIT_09
#define ICR_TI2_MASK                    DEF_BIT_09

#define ICR_TI3_SET                     DEF_BIT_10
#define ICR_TI3_MASK                    DEF_BIT_10

#define ICR_ERRBIT_MASK                 0x001f0000

#define ICR_ERRDIR_MASK                 DEF_BIT_21

#define ICR_ERRC_MASK                   0x00C00000

#define ICR_ALCBIT_MASK                 0xff000000

#define ICR_ERRSTA_BIT_MASK             0x000001fc

/* IER bit field & bit mask & bit set */
#define IER_RIE						    DEF_BIT_00
#define IER_TIE1						DEF_BIT_01
#define IER_EIE						    DEF_BIT_02
#define IER_DOIE						DEF_BIT_03
#define IER_WUIE						DEF_BIT_04
#define IER_EPIE						DEF_BIT_05
#define IER_ALIE						DEF_BIT_06
#define IER_BEIE						DEF_BIT_07
#define IER_IDIE						DEF_BIT_08
#define IER_TIE2						DEF_BIT_09
#define IER_TIE3						DEF_BIT_10

#define CAN_TBS1_EN( x )                     { x->CMR = 0x21; }
#define CAN_TBS2_EN( x )                     { x->CMR = 0x41; }
#define CAN_TBS3_EN( x )                     { x->CMR = 0x81; }
#define CAN_TBS123_EN( x )                   { x->CMR = 0xE1; }

#define RELEASE_RECEIVE_BUFF_HW(x)         do{ x->CMR = 0x04;}while(0)

/*
 * CAN_BITRATE  = (1/Fpclk)*(BRP+1)*(M+1+N+1+1)
 * Sample point = (N+1+1)/(M+1+N+1+1)
 * register BTR = (BRP << 0) | (SJW << 14) | (N << 16) | (M << 20) | (SAM << 23)
 */
//#define BITRATE_1000K36MHZ              (0 << 23)|(0 << 20)|(6 << 16)|(0 << 14)|3  /* BRP=3,SJW=0,TESG1=6,TESG2=0,SAM=0 -- 36/(3+1)*(6+3)*/
#define BITRATE_1000K25MHZ            (0 << 23)|(0 << 20)|(2 << 16)|(0 << 14)|4  /* BRP=4,SJW=0,TESG1=2,TESG2=0,SAM=0 -- 25/(4+1)*(2+3)*/
//#define BITRATE_1000K25MHZ			    (0 << 23)|(6 << 20)|(16 << 16)|(0 << 14)|0   /* BRP=0,SJW=0,TESG1=16,TESG2=6,SAM=0 -- 25/(0+1)*(16+6+3)*/



/* public typedef ------------------------------------------------------------*/
/* 注意:在结构体定义的时候确保没有空洞 */
typedef struct
{
	uint32_t cnt;
	uint32_t id;
	union
	{
	    uint32_t dwData[2];
	    uint8_t  bData[8];
	}data;	
} CAN_MSG_Type;

/* CAN send or recieve ring buffer */
typedef struct
{
    uint32_t volatile wrIdx;
	uint32_t volatile rdIdx;
	uint32_t          mask;
	uint32_t		  size;
	uint32_t          id;
	uint8_t           *pBuf;
	
	uint8_t           state;      /* 自己填充,避免在结构体数组中,出现空洞,保证为4的整数倍 */         
}CAN_RING_BUF_Type;

/* public function declaration -----------------------------------------------*/
void CAN_Init(void);
void CAN_DeInit(void);
void CAN_tx_action( LPC_CAN_TypeDef *pCANx, CAN_RING_BUF_Type* buf,uint32_t cnt,uint32_t flag );
void CAN_tx_action_all( LPC_CAN_TypeDef *pCANx, CAN_RING_BUF_Type* buf,uint32_t cnt );
void CAN_TxINTRoute( LPC_CAN_TypeDef *pCANx );
void CAN_RxINTRoute( LPC_CAN_TypeDef *pCANx );
/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif
