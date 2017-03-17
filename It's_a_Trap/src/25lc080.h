/*****************************************************************************
 *   Placeholder for 25LC080 related functions.
 *   Part of the proposed solutions for LPCXpresso Experiment Kit
 *
 *   Copyright(C) 2013, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/
#ifndef __25LC080_H__
#define __25LC080_H__

#include "type.h"

/* SPI E2PROM command set */
#define INST_WREN      0x06    /* MSB A8 is set to 0, simplifying test */
#define INST_WRDI      0x04
#define INST_RDSR      0x05
#define INST_WRSR      0x01
#define INST_READ      0x03
#define INST_WRITE     0x02

/* RDSR status bit definition */
#define RDSR_RDY       0x01
#define RDSR_WEN       0x02

/* Page size of E2PROM */
#define PAGE_SIZE        16

/*****************************************************************************
** Function name:    spiE2PROMread
** Descriptions:     This function will read bytes from the SPI E2PROM
** parameters:       address in memory region, buffer pointer and block length
** Returned value:   None
**
*****************************************************************************/
void spiE2PROMread( uint16_t address, uint8_t *pBuf, uint32_t length );

/*****************************************************************************
** Function name:    spiE2PROMwrite
** Descriptions:     This function will write bytes to the SPI E2PROM
** parameters:       address in memory region, buffer pointer and block length
** Returned value:   None
**
*****************************************************************************/
void spiE2PROMwrite( uint16_t address, uint8_t *pBuf, uint32_t length );

#endif /* __25LC080_H__ */
