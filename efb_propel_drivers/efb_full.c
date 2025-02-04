/* Full efb instance C drivers for Propel by B. Jordan
** (C) 2025 B. Jordan - All Rights Reserved
** V0.1 Jan 2025
** 
** All Documentation References are to Lattice Semiconductor 
** Technical Note FPGA-TN-02163-2.8 "Using User Flash Memory
** and Hardened Control Functions in MachXO2 Devices Reference
** Guide" unless otherwise noted.
**
** This set of drivers is for Lattice MachXO2 devices only.
**
** THIS DRIVER IS PROVIDED AS-IS UNDER THE TERMS OF THE GNU
** GPL SOFTWARE LICENSE VERSION 3.0
**
** DISCLAIMER:
**
**  JordanDSP LLC MAKES NO WARRANTIES ON THIS FILE OR ITS CONTENTS,
**  WHETHER EXPRESSED, IMPLIED, STATUTORY, OR IN ANY PROVISION OF THE
**  LICENSE AGREEMENT OR COMMUNICATION WITH LICENSEE, AND JordanDSP LLC 
**  SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF MERCHANTABILITY OR
**  FITNESS FOR A PARTICULAR PURPOSE. 
*/
#include "efb_full.h"
#include "hal.h"

#ifndef _EFB_NO_INTERRUPTS_
#endif /* _EFB_NO_INTERRUPTS_ */

// EFB global functions and initialization
unsigned char efb_init(
	struct efb_instance *this_efb,
	unsigned int base_addr,
	unsigned int sys_clk)
{
	if (NULL == base_addr) {
		return 1;
	}
	this_efb->base = base_addr;
	this_efb->sys_clk = sys_clk;
#ifdef EFB0_INST_NAME
	this_efb->name = EFB0_INST_NAME;
#else
	this_efb->name = "EFB_0";
#endif
	return 0;
}

// SPI peripheral functions:

/*
** Initialize SPI Peripheral
**
** Arguments: 
**		volatile unsigned int *efb_base	: EFB context base address
** 		efb_spi_instance *this_spi		: SPI Peripheral context
**		unsigned int spi_slk			: SPI clock frequency (MHz)
**
** Return Value:
** 		char:
**			0 => Success
**			1 => Invalid or null pointer argument.
**
*/
unsigned char efb_spi_init(
	volatile unsigned int *efb_base,
	struct efb_spi_instance *this_spi,
	unsigned int spi_clk)
{
	volatile struct efb_spi_dev *dev;
	int cdiv = 0;
	if (NULL == this_efb){
		return 1;
	}
	if (NULL == this_spi){
		return 1;
	}
	dev = (volatile struct efb_spi_dev *)(efb_base + EFB_SPI_BASE_ADDR);
	this_spi->addr = (unsigned int)&dev;
	this_spi->spi_clk = spi_clk;
	cdiv = this_efb->sys_clk / spi_clk;
	if (cdiv < 6) {
		cdiv = 6;
		// R1 silicon cannot support WB clock > 6x SPI clock.
	}
	dev->br = cdiv;
	return 0;
}

/*
** Initiate a byte transmit
**
** Arguments:
**		efb_spi_instance *this_spi	: SPI Peripheral context
**		unsigned char data			: Data byte to send
**
** Return Value:
**		char:
**			0 => SPI peripheral status register contents prior to transmit.
*/
unsigned char efb_spi_putc(
	struct efb_spi_instance *this_spi,
	unsigned char data)
{
	volatile unsigned char status;
	volatile struct efb_spi_dev *dev;
	dev = (volatile struct efb_spi_dev *)(this_spi->addr);
	do {
		status = (unsigned char)dev->sr;
		if ((status & SPI_TRDY) != 0) // TRDY bit (1) means txdata byte is empty.
			break;
	} while (1); // blocking implementation.
	// Write a byte
	dev->txdr = (volatile unsigned int)(0x000000FF & data);
	return status;
}
	
/*
** Initiate a byte receive
**
** Arguments:
**		efb_spi_instance *this_spi	: SPI Peripheral context
**		unsigned char data			: Data byte received
**
** Return Value:
**		char:
**			0 => SPI peripheral status register contents prior to transmit.
*/
	
unsigned char efb_spi_getc(
	struct efb_spi_instance *this_spi,
	unsigned char *data)
{
	volatile unsigned char status;
	volatile struct efb_spi_dev *dev;
	dev = (volatile struct efb_spi_dev *)(this_spi->addr);
	do {
		status = (unsigned char)dev->sr;
		if ((status & SPI_RRDY) != 0)
			break;
	} while (1); // blocking implementation.
	//Read a byte
	*data = (unsigned char)dev->rxdr;
	return status;
}

/*
** Transfer a multi-byte transmit or receive sequence.
** 
** Arguments:
** 		efb_spi_instance *this_spi	: SPI Peripheral context
**		unsigned char *txBuffer		: Pointer to Transmit Buffer
**		unsigned char *rxBuffer		: Pointer to Receive Buffer
**		unsigned int length			: Number of bytes to send / receive
**		unsigned char readMode		: >0 if performing read (Versus only transmit)
**
** Returns:
**		char:
**			SPI status register contents after transfer.
**
*/
unsigned char efb_spi_transfer(
	struct efb_spi_instance *this_spi,
	unsigned char *txBuffer,
	unsigned char *rxBuffer,
	unsigned int length,
	unsigned char readMode)
{
	unsigned char status;
	unsigned char dummy;
	volatile struct efb_spi_dev *dev;
	dev = (volatile struct efb_spi_dev *)(this_spi->addr);
	do {
		status = (unsigned char)dev->sr;
		if ((status & SPI_TIP) == 0) // TIP: 1 means transfer in progress.
			break;
	} while (1);  // blocking implementation
	// SPI Master mode set
	dev->csr = ~SPI_CSN_0; // Chip Select bit 0.
	dev->cr2 = (SPI_MSTRSPI | SPI_MCSH);
	if (readMode)
		++bufferSize; // Account for additional read byte.
	unsigned int xfrCount = 1;
	do {
		dev->txdr = (volatile unsigned int)(*txBuffer++);
		do {
			status = dev->sr;
			if (status & SPI_RRDY)
				break;
		} while (1); // blocking implementation
		if (readMode && (xfrCount < length))
		{
			*rxBuffer++ = (unsigned char)dev->rxdr;
		}
		else
		{
			dummy = (unsigned char)dev->rxdr;
		}
		if (xfrCount == length)
			break;
		xfrCount++;
	} while (1); // blocking implementation
	dev->cr2 = ~SPI_MSTRSPI;
	return (0);
}
