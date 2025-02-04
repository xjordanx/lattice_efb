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
#ifndef EFB_H_
#define EFB_H_

#include <stdint.h>
#include <stdbool.h>

/*************************************************************************
// EFB Base Register Map - Table 1.1
// These are 8-bit Wishbone addresses
// Would be accessed as (int) in 32-bit RISCV system whilst
// ignoring the most significant 3-bytes in read/write.
// RISCV is byte-addressed so we must multiply these by 4.
*************************************************************************/
#define EFB_PLL0_BASE_ADDR		(0x00)		// PLL0 Dynamic Access1
#define EFB_PLL1_BASE_ADDR		(0x20*4)	// PLL1 Dynamic Access1
#define EFB_I2C1_BASE_ADDR		(0x40*4)	// I2C Primary
#define EFB_I2C2_BASE_ADDR		(0x4A*4)	// I2C Secondary
#define EFB_SPI_BASE_ADDR		(0x54*4)	// SPI
#define EFB_TCNT_BASE_ADDR		(0x5E*4)	// Timer/Counter
#define EFB_UFMC_BASE_ADDR		(0x70*4)	// Flash Memory (UFM/Configuration)
#define EFB_INTR_BASE_ADDR		(0x76*4)	// EFB Interrupt Source

// PLL0 Registers
struct efb_pll0_dev { // PLL0 Control Registers start at 0x00
	volatile unsigned int divfbk0;
		// Fractional-N Divider Value LSB
	volatile unsigned int divfbk1;
		// Fractional-N Divider Value MSB
	volatile unsigned int ldreg_dela;
		// Load Reg Command [7] : CLKOP Delay [6:0]
	volatile unsigned int pllpd_delb;
		// PLL Power-down Command [7] : CLKOS Delay [6:0]
	volatile unsigned int wbrst_delc;
		// Wishbone Reset [7]  : CLKOS2 Delay [6:0]
	volatile unsigned int udesi_deld;
		// Use Frac-N Divider [7] : CLKOS3 Delay [6:0]
	volatile unsigned int rfinr_diva;
		// Ref In Reset [7] 	: CLKOP Divider [6:0]
	volatile unsigned int prsen_divb;
		// PLL Reset Enable [7] : CLKOS Divider [6:0]
	volatile unsigned int mrsen_divc;
		// RESETM Port Enable [7] : CLKOS2 Divider [6:0]
	volatile unsigned int stdby_divd;
		// STANDBY Port Enable [7] : CLKOS3 Divider [6:0]
	volatile unsigned int ensync_phib_ils_phia;
		// Sync Enable [7] : PHIB [6:4] : PHIA [2:0]
	volatile unsigned int dcrsten_phid_phic; 
		// RESETD Port Enable [7] : PHID [6:4] : PHIC [2:0] - bit [3] not used
	volatile unsigned int ddrsten_seloutb_intfb_selouta;
		// RESETDC Port Enable [7] : MUXB2 Select [6:4] : INTFB [3] : MUXA2 Select [2:0]
	volatile unsigned int lock_seloutc_seloutd;		
		// LOCK Resolution [7:6] : MUXC2 Select [5:3] : MUXD2 Select [2:0]
	volatile unsigned int seldiva_seldivb_seldivc_seldivd;
		// MUXA1 Select [7:6] : MUXB1 Select [5:4] : MUXC1 Select [3:2] : MUXD1 Select [1:0]
	volatile unsigned int clkoptrim_clkostrim;
		// CLKOP Trim [7:4] : CLKOS Trim [3:0]
	volatile unsigned int dynsrc_locksel_enableclk;
		// Dynamic Phase Source [7] : LOCK Select [6:4] : Enable CLK Outs [3:0]
	volatile unsigned int trimbypass_dynsel_dir_rot;
		// Trim Output Bypasses [7:4] : Dynamic Phase Select [3:2] : Rotate Direction [1] : Rotate Command [0]
	volatile unsigned int reserved0;	// These reserved registers are either read-only or
	volatile unsigned int reserved1;	// reserved for internal Lattice Semiconductor use.
	volatile unsigned int reserved2;	// 
	volatile unsigned int reserved3;	// 
	volatile unsigned int reserved4;	// 
	volatile unsigned int reserved5;	// DO NOT WRITE TO THESE REGISTERS
	volatile unsigned int reserved6;	// 
	volatile unsigned int reserved7;	// 
	volatile unsigned int reserved8;	// 
	volatile unsigned int reserved9;	// 
	volatile unsigned int reserved10;	// REFER TO: FPGA-TN-02157-3.0 Appendix D.
};

// PLL1 Registers
struct efb_pll1_dev { // PLL1 Control Registers start at 0x20
	volatile unsigned int divfbk0;
		// Fractional-N Divider Value LSB (0x20)
	volatile unsigned int divfbk1;
		// Fractional-N Divider Value MSB
	volatile unsigned int ldreg_dela;
		// Load Reg Command [7] : CLKOP Delay [6:0]
	volatile unsigned int pllpd_delb;
		// PLL Power-down Command [7] : CLKOS Delay [6:0]
	volatile unsigned int wbrst_delc;
		// Wishbone Reset [7]  : CLKOS2 Delay [6:0]
	volatile unsigned int udesi_deld;
		// Use Frac-N Divider [7] : CLKOS3 Delay [6:0]
	volatile unsigned int rfinr_diva;
		// Ref In Reset [7] 	: CLKOP Divider [6:0]
	volatile unsigned int prsen_divb;
		// PLL Reset Enable [7] : CLKOS Divider [6:0]
	volatile unsigned int mrsen_divc;
		// RESETM Port Enable [7] : CLKOS2 Divider [6:0]
	volatile unsigned int stdby_divd;
		// STANDBY Port Enable [7] : CLKOS3 Divider [6:0]
	volatile unsigned int ensync_phib_ils_phia;
		// Sync Enable [7] : PHIB [6:4] : PHIA [2:0]
	volatile unsigned int dcrsten_phid_phic; 
		// RESETD Port Enable [7] : PHID [6:4] : PHIC [2:0] - bit [3] not used
	volatile unsigned int ddrsten_seloutb_intfb_selouta;
		// RESETDC Port Enable [7] : MUXB2 Select [6:4] : INTFB [3] : MUXA2 Select [2:0]
	volatile unsigned int lock_seloutc_seloutd;		
		// LOCK Resolution [7:6] : MUXC2 Select [5:3] : MUXD2 Select [2:0]
	volatile unsigned int seldiva_seldivb_seldivc_seldivd;
		// MUXA1 Select [7:6] : MUXB1 Select [5:4] : MUXC1 Select [3:2] : MUXD1 Select [1:0]
	volatile unsigned int clkoptrim_clkostrim;
		// CLKOP Trim [7:4] : CLKOS Trim [3:0]
	volatile unsigned int dynsrc_locksel_enableclk;
		// Dynamic Phase Source [7] : LOCK Select [6:4] : Enable CLK Outs [3:0]
	volatile unsigned int trimbypass_dynsel_dir_rot;
		// Trim Output Bypasses [7:4] : Dynamic Phase Select [3:2] : Rotate Direction [1] : Rotate Command [0]
	volatile unsigned int reserved0;	// These reserved registers are either read-only or
	volatile unsigned int reserved1;	// reserved for internal Lattice Semiconductor use.
	volatile unsigned int reserved2;	// 
	volatile unsigned int reserved3;	// 
	volatile unsigned int reserved4;	// 
	volatile unsigned int reserved5;	// DO NOT WRITE TO THESE REGISTERS
	volatile unsigned int reserved6;	// 
	volatile unsigned int reserved7;	// 
	volatile unsigned int reserved8;	// 
	volatile unsigned int reserved9;	// 
	volatile unsigned int reserved10;	// REFER TO: FPGA-TN-02157-3.0 Appendix D. (0x3C)
	// Register addresses 0x3D - 0x3F are not used.
};

// I2C Controller (Primary and Secondary) Bit Masks:
// Control Register "cr":
#define I2C_EN		(0x80)
#define I2C_GCEN		(0x40)
#define I2C_WKUPEN		(0x20)
#define I2C_SDA_DEL_SEL (0x0C) // Remaining bits not used/reserved
// Command Register "cmdr":
#define I2C_STA		(0x80)
#define I2C_STO     (0x40)
#define I2C_RD      (0x20)
#define I2C_WR      (0x10)
#define I2C_ACK     (0x08)
#define I2C_CKSDIS  (0x04) // Remaining bits not used/reserved
// Status Register "sr":
#define I2C_TIP		(0x80)
#define I2C_BUSY    (0x40)
#define I2C_RARC    (0x20)
#define I2C_SRW     (0x10)
#define I2C_ARBL    (0x08)
#define I2C_TRRDY   (0x04)
#define I2C_TROE    (0x02)
#define I2C_HGC     (0x01)
// Interrupt Request Register "irq":
#define I2C_IRQARBL	 (0x08)
#define I2C_IRQTRRDY (0x04)
#define I2C_IRQTROE  (0x02)
#define I2C_IRQHGC   (0x01) // Upper nibble not used.
// Interrupt Enable Register "irqen":
#define I2C_IRQARBLEN	(0x08)
#define I2C_IRQTRRDYEN	(0x04)
#define I2C_IRQTROEEN	(0x02)
#define I2C_IRQHGCEN	(0x01) // Upper nibble not used.

// I2C1 Registers - Start at 0x40
struct efb_i2c1_dev {
	volatile unsigned int cr;	// I2C Primary Control (0x40)
	volatile unsigned int cmdr;	// I2C Primary Command
	volatile unsigned int br0;	// I2C Primary Clock Prescale 0
	volatile unsigned int br1;	// I2C Primary Clock Prescale 1 - only [1:0] used.
	volatile unsigned int txdr;	// I2C Primary Transmit Data
	volatile unsigned int sr;	// I2C Primary Status
	volatile unsigned int gcdr;	// I2C Primary General Call
	volatile unsigned int rxdr;	// I2C Primary Received Data
	volatile unsigned int irq;	// I2C Primary Interrupts
	volatile unsigned int irqen;// I2C Primary Interrupts Enables (0x49)
};

// I2C2 Registers - Start at 0x4A
struct efb_i2c2_dev {
	volatile unsigned int cr;	// I2C Secondary Control (0x4A)
	volatile unsigned int cmdr;	// I2C Secondary Command
	volatile unsigned int br0;	// I2C Secondary Clock Prescale 0
	volatile unsigned int br1;	// I2C Secondary Clock Prescale 1 - only [1:0] used.
	volatile unsigned int txdr;	// I2C Secondary Transmit Data
	volatile unsigned int sr;	// I2C Secondary Status
	volatile unsigned int gcdr;	// I2C Secondary General Call
	volatile unsigned int rxdr;	// I2C Secondary Received Data
	volatile unsigned int irq;	// I2C Secondary Interrupts
	volatile unsigned int irqen;// I2C Secondary Interrupts Enable (0x53)
};

// SPI Controller Bit Masks:
// Control Register 0 "cr0":
#define SPI_TIDLE_XCNT	(0xC0)
#define SPI_TTRAIL_XCNT	(0x38)
#define SPI_TLEAD_XCNT	(0x07)
// Control Register 1 "cr1":
#define SPI_SPE			(0x80)
#define SPI_WKUPEN_USER (0x40)
#define SPI_WKUPEN_CFG  (0x20)
#define SPI_TXEDGE      (0x10) // Upper nibble not used.
// Control Register 2 "cr2":
#define SPI_MSTRSPI	(0x80)
#define SPI_MCSH    (0x40)
#define SPI_SDBRE   (0x20)
#define SPI_CPOL    (0x04)
#define SPI_CPHA    (0x02)
#define SPI_LSBF    (0x01) // Middle 2 bits not used.
// Clock Prescale Register "br":
#define SPI_DIVIDER (0x3F) // Upper 2 bits not used.
// Master Chip Selects "csr":
#define SPI_CSN_7  (0x80)
#define SPI_CSN_6  (0x40)
#define SPI_CSN_5  (0x20)
#define SPI_CSN_4  (0x10)
#define SPI_CSN_3  (0x08)
#define SPI_CSN_2  (0x04)
#define SPI_CSN_1  (0x02)
#define SPI_CSN_0  (0x01)
// Status Register "sr":
#define SPI_TIP	 (0x80)
#define SPI_TRDY (0x10)
#define SPI_RRDY (0x08)
#define SPI_ROE	 (0x02)
#define SPI_MDF	 (0x01) // Other bits not used.
// Interrupt Status Register "irq":
#define SPI_IRQTRDY	(0x10)
#define SPI_IRQRRDY	(0x08)
#define SPI_IRQROE	(0x02)
#define SPI_IRQMDF	(0x01) // Other bits not used.
// Interrupt Enable Register "irqen":
#define SPI_IRQTRDYEN	(0x10)
#define SPI_IRQRRDYEN	(0x08)
#define SPI_IRQROEEN	(0x02)
#define SPI_IRQMDFEN	(0x01) // Other bits not used.

// SPI Registers - Start at 0x54
struct efb_spi_dev {
	volatile unsigned int cr0;	// SPI Control Register 0 (0x54)
	volatile unsigned int cr1;	// SPI Control Register 1
	volatile unsigned int cr2;	// SPI Control Register 2
	volatile unsigned int br;	// SPI Clock Prescale
	volatile unsigned int csr;	// SPI Master Chip Select
	volatile unsigned int txdr;	// SPI Transmit Data
	volatile unsigned int sr;	// SPI Status Register
	volatile unsigned int rxdr;	// SPI Received Data
	volatile unsigned int irq;	// SPI Interrups
	volatile unsigned int irqen;// SPI Interrupt Enables (0x5D)
};

// Timer/Counter Register Bit Masks:
// Control Register 0 "cr0":
#define TCNT_RSTEN		(0x80)
#define TCNT_PRESCALE	(0x38)
#define TCNT_CLKEDGE	(0x04)
#define TCNT_CLKSEL		(0x02) // Other bits not used.
// Control Register 1 "cr1":
#define TCNT_SOVFEN (0x40)
#define TCNT_ICEN 	(0x20)
#define TCNT_TSEL 	(0x10)
#define TCNT_OCM 	(0x0C)
#define TCNT_TCM 	(0x03) // MSb not used.
// Control Register 2 "cr2":
#define TCNT_WBFORCE	(0x04)
#define TCNT_WBRESET	(0x02)
#define TCNT_WBPAUSE	(0x01) // Upper bits not used.
// Status Register "sr":
#define TCNT_BTF	(0x08)
#define TCNT_ICRF	(0x04)
#define TCNT_OCRF	(0x02)
#define TCNT_OVF	(0x01) // Upper nibble not used.
// Interrupt Status "irq":
#define TCNT_IRQICRF (0x04)
#define TCNT_IRQOCRF (0x02)
#define TCNT_IRQOVF  (0x01) // Upper bits not used.
// Interrupt Enable "irqen":
#define TCNT_IRQICRFEN (0x04)
#define TCNT_IRQOCRFEN (0x02)
#define TCNT_IRQOVFEN  (0x01) // Upper bits not used.

// TCNT (Timer/Counter) Registers - Start at 0x5E
struct efb_tcnt_dev {
	volatile unsigned int cr0;		// Timer/Counter Control 0 (0x5e) 
	volatile unsigned int cr1;		// Timer/Counter Control 1
	volatile unsigned int topset0;	// Timer/Counter Set Top Ctr. Value [7:0]
	volatile unsigned int topset1;	// Timer/Counter Set Top Ctr. Value [15:8]
	volatile unsigned int ocrset0;	// Timer/Counter Set Comp. Value [7:0]
	volatile unsigned int ocrset1;	// Timer/Counter Set Comp. Value [15:8]
	volatile unsigned int cr2;		// Timer/Counter Control 2
	volatile unsigned int cnt0;		// Timer/Counter Counter Value [7:0]
	volatile unsigned int cnt1;		// Timer/Counter Counter Value [15:8]
	volatile unsigned int top0;		// Timer/Counter Current Top Ctr. Value [7:0]
	volatile unsigned int top1;		// Timer/Counter Current Top Ctr. Value [15:8]
	volatile unsigned int ocr0;		// Timer/Counter Current Comp. Ctr. Value [7:0]
	volatile unsigned int ocr1;		// Timer/Counter Current Comp. Ctr. Value [15:8]
	volatile unsigned int icr0;		// Timer/Counter Current Capture Value [7:0]
	volatile unsigned int icr1;		// Timer/Counter Current Capture Value [15:8]
	volatile unsigned int sr;		// Timer/Counter Status Register
	volatile unsigned int irq;		// Timer/Counter Interrupts
	volatile unsigned int irqen;	// Timer/Counter Interrupt Enables (0x6F)
};

// UFM/Config Register Bit Masks:
// Control Register "cr":
#define UFMCFG_WBCE (0x80)
#define UFMCFG_RSTE (0x40) // Lower bits not used.
// Status Register "sr":
#define UFMCFG_WBCACT	(0x80)
#define UFMCFG_TXFE		(0x20)
#define UFMCFG_TXFF		(0x10)
#define UFMCFG_RXFE		(0x08)
#define UFMCFG_RXFF		(0x04)
#define UFMCFG_SSPIACT	(0x02)
#define UFMCFG_I2CACT	(0x01) // Bit [6] not used.
// Interrupt Status "irq":
#define UFMCFG_IRQTXFE 	(0x20)
#define UFMCFG_IRQTXFF 	(0x10)
#define UFMCFG_IRQRXFE 	(0x08)
#define UFMCFG_IRQRXFF 	(0x04)
#define UFMCFG_IRQSSPIACT 	(0x02)
#define UFMCFG_IRQI2CACT 	(0x01) // Upper bits not used.
// Interrupt Enable "irqen":
#define UFMCFG_IRQTXFEEN	(0x20)
#define UFMCFG_IRQTXFFEN	(0x10)
#define UFMCFG_IRQRXFEEN	(0x08)
#define UFMCFG_IRQRXFFEN	(0x04)
#define UFMCFG_IRQSSPIACTEN	(0x02)
#define UFMCFG_IRQI2CACTEN	(0x01) // Upper bits not used.

// UFMC (User Flash Memory and Configuration) Registers - Start at 0x70
struct efb_ufmcfg_dev {
	volatile unsigned int cr; 	// UFM/Config Control Register (0x70)
	volatile unsigned int txdr; // UFM/Config Transmit Data
	volatile unsigned int sr; 	// UFM/Config Status Register 
	volatile unsigned int rxdr; // UFM/Config Received Data
	volatile unsigned int irq; 	// UFM/Config Interrupts
	volatile unsigned int irqen;// UFM/Config Interrupt Enables (0x75)
};

// Interrupt Source Bit Flags:
#define UFMCFG_INT 	(0x10)
#define TC_INT		(0x08)
#define SPI_INT		(0x04)
#define I2C2_INT	(0x02)
#define I2C1_INT	(0x01)
// INTR (Interrupt Source) Registers - Begin at 0x76, Read Only.
struct efb_int_dev {
	volatile unsigned int reserved; // Do not use.
	volatile unsigned int source;   // [7:5] <= '000' : Interrupt Source Flags [4:0]
};

/*
** Device Instance(s)
*/
// EFB Instance Declaration (can only exist 1, as it's a hard silicon function)
struct efb_instance {
	const char *name;
	unsigned int base;
	unsigned int sys_clk;
	struct efb_i2c1_dev *i2c1;
	struct efb_i2c2_dev *i2c2;
	struct efb_pll0_dev *pll0;
	struct efb_pll1_dev *pll1;
	struct efb_spi_dev *spi;
	struct efb_tcnt_dev *tcnt;
	struct efb_ufmcfg_dev *tcnt;
	struct efb_int_dev *interrupt;
};

struct efb_spi_instance {
	unsigned int addr;
	unsigned int spi_clk;
}
/*
// Function Prototypes
*/
// EFB prototypes
unsigned char efb_init(
	struct efb_instance *this_efb,
	unsigned int base_addr,
	unsigned int sys_clk);

// SPI prototypes
unsigned char efb_spi_init(
	volatile unsigned int *efb_base,
	struct efb_spi_instance *this_spi,
	unsigned int spi_clk); // Assume SPI was pre-configured from IPExpress.
	
unsigned char efb_spi_putc(
	struct efb_spi_instance *this_spi,
	unsigned char data);
	
unsigned char efb_spi_getc(
	struct efb_spi_instance *this_spi,
	unsigned char *data);
	
unsigned char efb_spi_transfer(
	struct efb_spi_instance *this_spi,
	unsigned char *txBuffer,
	unsigned char *rxBuffer,
	unsigned int length,
	unsigned char readMode);

#endif /* EFB_H_ */