/*****************************************************************************
 *   Placeholder for SPI related functions.
 *   Part of the proposed solutions for LPCXpresso Experiment Kit
 *
 *   Copyright(C) 2013, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

#include "type.h"

#define FIFOSIZE        8

/* SSP Status register */
#define SSPSR_TFE     (1 << 0)
#define SSPSR_TNF     (1 << 1)
#define SSPSR_RNE     (1 << 2)
#define SSPSR_RFF     (1 << 3)
#define SSPSR_BSY     (1 << 4)

/* SSP CR0 register */
#define SSPCR0_DSS    (1 << 0)
#define SSPCR0_FRF    (1 << 4)
#define SSPCR0_SPO    (1 << 6)
#define SSPCR0_SPH    (1 << 7)
#define SSPCR0_SCR    (1 << 8)

/* SSP CR1 register */
#define SSPCR1_LBM    (1 << 0)
#define SSPCR1_SSE    (1 << 1)
#define SSPCR1_MS     (1 << 2)
#define SSPCR1_SOD    (1 << 3)

/*****************************************************************************
** Function name:    SSP0Init
**
** Descriptions:     SSP port #0 initialization routine
**                   Note that GPIO control of SSEL signal is not done, must
**                   be done separately.
**
** parameters:       Wanted SCLK frequency
** Returned value:   None
**
*****************************************************************************/
void SSP0Init( uint32_t freq );

/*****************************************************************************
** Function name:    SSP0Send
**
** Descriptions:     Send a block of data to the SSP port, the
**                   first parameter is the buffer pointer, the 2nd
**                   parameter is the block length.
**
** parameters:       buffer pointer, and the block length
** Returned value:   None
**
*****************************************************************************/
void SSP0Send( uint8_t *pBuf, uint32_t length );

/*****************************************************************************
** Function name:    SSP0Receive
** Descriptions:     the module will receive a block of data from
**                   the SSP, the 2nd parameter is the block length.
** parameters:       buffer pointer, and block length
** Returned value:   None
**
*****************************************************************************/
void SSP0Receive( uint8_t *pBuf, uint32_t length );

#endif /* __SPI_H__ */
