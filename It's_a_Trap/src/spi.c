/*****************************************************************************
 *   Placeholder for SPI related functions.
 *   Part of the proposed solutions for LPCXpresso Experiment Kit
 *
 *   Copyright(C) 2013, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/

#include "LPC11xx.h"
#include "type.h"
#include "spi.h"

#define ABS_DIFF(__a, __b)   ( ((__a)<(__b)) ? ((__b)-(__a)) : ((__a)-(__b)) )

/*****************************************************************************
** Function name:    findFreq
**
** Descriptions:     Takes the wanted frequency and finds the closest match.
**
** parameters:       Wanted SCLK frequency and the resulting clock divider,
**                   serial clock rate and clock prescale values.
** Returned value:   The actual frequency
**
*****************************************************************************/
static uint32_t findFreq(uint32_t wanted, uint32_t* pClkDiv, uint32_t* pSCR, uint32_t* pCPSR)
{
  uint32_t scr = 0;
  uint32_t cpsr = 2;
  uint32_t div = 1;
  uint32_t tmp;
  uint32_t err;
  uint32_t best_err;

  *pClkDiv = div;
  *pSCR = scr;
  *pCPSR = cpsr;

  // freq = PCLK/(CPSR*(SCR+1))
  tmp = 48000000/(div * cpsr * (scr + 1));
  best_err = err = ABS_DIFF(tmp, wanted);

  if (wanted > tmp) {
    // the wanted frequency is too high, this is the best we can do
    return tmp;
  } else if (err == 0) {
    // found exact match
    return wanted;
  }

  for (div = 1; div < 256; div++) {
    for (cpsr = 2; cpsr<=256; cpsr+=2) {
      for (scr = 0; scr < 256; scr++) {
        // freq = PCLK/(CPSR*(SCR+1))
        tmp = 48000000/(div * cpsr * (scr + 1));
        err = ABS_DIFF(tmp, wanted);
        if (err < best_err) {
          *pClkDiv = div;
          *pSCR = scr;
          *pCPSR = cpsr;
          best_err = err;
        }

        if (wanted > tmp) {
          // the wanted frequency is too high, no point in searching for further dividers
          break;
        } else if (err == 0) {
          // found exact match
          return tmp;
        }
      }
    }
  }
  // have searched all possible combinations
  return 48000000/((*pClkDiv) * (*pCPSR) * ((*pSCR) + 1));
}

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
void SSP0Init( uint32_t freq )
{
  uint8_t i, dummy = dummy;
  uint32_t scr, cpsr, div;

  findFreq(freq, &div, &scr, &cpsr);

  LPC_SYSCON->PRESETCTRL    |= (0x1<<0);   /* Reset SSP0 block */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<11);    /* Enable SSP0 block */
  LPC_SYSCON->SSP0CLKDIV     = div;        /* Clock to SSP0 block is divided by 2 */
                                           /* which will equal 24MHz clock rate */

  /*  SSP I/O config */
  LPC_IOCON->PIO0_8           &= ~0x07;
  LPC_IOCON->PIO0_8           |= 0x01;     /* SSP0 MISO */
  LPC_IOCON->PIO0_9           &= ~0x07;
  LPC_IOCON->PIO0_9           |= 0x01;     /* SSP0 MOSI */
  LPC_IOCON->SCK_LOC           = 0x01;     /* Needed to conf. PIO2_11 as SCLK */
  LPC_IOCON->PIO2_11          &= ~0x07;
  LPC_IOCON->PIO2_11          |= 0x01;     /* SSP0 SCLK */

  /* SSPCPSR clock prescale register, master mode, minimum divisor is 0x02 */
  LPC_SSP0->CPSR = cpsr;

  /* Set DSS data to 8-bit, Frame format SPI, mode #0 (CPOL = 0, CPHA = 0)
     and SCR for the wanted frequency */
  LPC_SSP0->CR0 = 0x0007 | (scr << 8);

  /* clear the RxFIFO */
  for ( i = 0; i < FIFOSIZE; i++ )
    dummy = LPC_SSP0->DR;

  /* Master mode */
  LPC_SSP0->CR1 = SSPCR1_SSE;
}

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
void SSP0Send( uint8_t *pBuf, uint32_t length )
{
  uint32_t i;
  uint8_t dummy = dummy;

  for ( i = 0; i < length; i++ )
  {
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( (LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF )
      ;

    LPC_SSP0->DR = *pBuf;
    pBuf++;

    while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE )
      ;

    /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
       on MISO. Otherwise, when SSP0Receive() is called, previous data byte
       is left in the FIFO. */
    dummy = LPC_SSP0->DR;
  }
}

/*****************************************************************************
** Function name:    SSP0Receive
** Descriptions:     the module will receive a block of data from
**                   the SSP, the 2nd parameter is the block length.
** parameters:       buffer pointer, and block length
** Returned value:   None
**
*****************************************************************************/
void SSP0Receive( uint8_t *pBuf, uint32_t length )
{
  uint32_t i;

  for ( i = 0; i < length; i++ )
  {
    /* Write dummy output byte (0xFF) to shift in new byte */
    LPC_SSP0->DR = 0xFF;

    /* Wait until the Busy bit is cleared */
    while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE )
      ;

    *pBuf = LPC_SSP0->DR;
    pBuf++;
  }
}
