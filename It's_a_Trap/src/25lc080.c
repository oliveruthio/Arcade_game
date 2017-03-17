/*****************************************************************************
 *   Placeholder for 25LC080 related functions.
 *   Part of the proposed solutions for LPCXpresso Experiment Kit
 *
 *   Copyright(C) 2013, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/

#include "LPC11xx.h"
#include "type.h"
#include "board.h"
#include "delay.h"
#include "25lc080.h"
#include "gpio.h"
#include "spi.h"

#define LOCAL_MIN(__a, __b)  ( ((__a) < (__b)) ? (__a) : (__b) )

/*****************************************************************************
** Function name:    spiE2PROMwren
** Descriptions:     This function will set the write enable latch/bit
** parameters:       None
** Returned value:   None
**
*****************************************************************************/
static void spiE2PROMwren( void )
{
  uint8_t buf[1];

  //pull SSEL/CS low
  GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_LOW);

  //output write command and address
  buf[0] = INST_WREN;
  SSP0Send(&buf[0], 1);

  //pull SSEL/CS high
  GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_HIGH);
}

/*****************************************************************************
** Function name:    spiE2PROMread
** Descriptions:     This function will read bytes from the SPI E2PROM
** parameters:       address in memory region, buffer pointer and block length
** Returned value:   None
**
*****************************************************************************/
void spiE2PROMread( uint16_t address, uint8_t *pBuf, uint32_t length )
{
  uint8_t buf[3];

  //pull SSEL/CS low
  GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_LOW);

  //output read command and address
  buf[0] = INST_READ;
  buf[1] = (address >> 8) & 0xff;
  buf[2] = address & 0xff;
  SSP0Send(&buf[0], 3);

  //read bytes from E2PROM
  SSP0Receive(pBuf, length);

  //pull SSEL/CS high
  GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_HIGH);
}

/*****************************************************************************
** Function name:    spiE2PROMwrite
** Descriptions:     This function will write bytes to the SPI E2PROM
** parameters:       address in memory region, buffer pointer and block length
** Returned value:   None
**
*****************************************************************************/
void spiE2PROMwrite( uint16_t address, uint8_t *pBuf, uint32_t length )
{
  uint8_t buf[3];
  uint32_t left = length;
  uint32_t offset = address;
  uint32_t len = 0;

  // find length of first page write
  if ((address / PAGE_SIZE) != ((address + length) / PAGE_SIZE)) {
    //spans across at least one boundary
    len = PAGE_SIZE - (address % PAGE_SIZE);
  } else {
    // ends inside same page => use normal length
    len = length % PAGE_SIZE;
  }

  //insert code here to break up large write operation into several
  //page write operations (16 or 32 byte pages)
  while (left > 0) {

    //Enable write latch
    spiE2PROMwren();

    //pull SSEL/CS low
    GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_LOW);

    //output write command and address
    buf[0] = INST_WRITE;
    buf[1] = (offset >> 8) & 0xff;
    buf[2] = offset & 0xff;
    SSP0Send(&buf[0], 3);

    //send bytes to write E2PROM
    SSP0Send(&(pBuf[offset]), len);

    //pull SSEL/CS high
    GPIOSetValue(SSEL_GPIO_8_PORT, SSEL_GPIO_8_PIN, SSEL_HIGH);

    offset += len;
    left -= len;
    len = LOCAL_MIN(left, PAGE_SIZE);

    //wait until write operations is done
    //for now, just wait. A more sophisticated method is to poll the status register
    delayMS(5);
  }
}
