--Top level IP wrapper including wishbone to APB bridge.

library IEEE;
use IEEE.std_logic_1164.all;
-- synopsys translate_off
library MACHXO2;
use MACHXO2.components.all;
-- synopsys translate_on

entity efb_spi is
    generic (
        ADDR_WIDTH : integer := 32;
        SLAVE_DATA_WIDTH : integer := 8;
        HOST_DATA_WIDTH : integer := 32;
		SLAVE_ADDR_WIDTH : integer := 8
    );
    port (
		-- APB interface
        apb_pclk     : in  std_logic;
        apb_resetn   : in  std_logic;
        apb_addr     : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        apb_pselx    : in  std_logic;
        apb_penable  : in  std_logic;
        apb_pwrite   : in  std_logic;
        apb_pwdata   : in  std_logic_vector(HOST_DATA_WIDTH-1 downto 0);
        apb_pready   : out std_logic;
        apb_prdata   : out std_logic_vector(HOST_DATA_WIDTH-1 downto 0);
        apb_pslverr  : out std_logic;
		
		-- SPI pins
        spi_clk		: inout	std_logic; 
        spi_miso	: inout	std_logic; 
        spi_mosi	: inout	std_logic; 
        spi_scsn	: in  	std_logic; 
        spi_csn		: out  	std_logic_vector(0 downto 0); 
        spi_irq		: out  	std_logic);
end efb_spi;

architecture structure of efb_spi is
	
	-- Internal wiring
	signal wb_ack 		: std_logic;
	--signal apb_enable	: std_logic;
	signal wb_rst		: std_logic;
	
	-- Component Declarations	
	Component efb_spi_wb
    port(
		wb_clk_i: in  std_logic; 
		wb_rst_i: in  std_logic; 
		wb_cyc_i: in  std_logic; 
		wb_stb_i: in  std_logic; 
		wb_we_i : in  std_logic; 
		wb_adr_i: in  std_logic_vector(7 downto 0); 
		wb_dat_i: in  std_logic_vector(7 downto 0); 
		wb_dat_o: out  std_logic_vector(7 downto 0); 
		wb_ack_o: out  std_logic; 
		spi_clk : inout  std_logic; 
		spi_miso: inout  std_logic; 
		spi_mosi: inout  std_logic; 
		spi_scsn: in  std_logic; 
		spi_csn : out  std_logic_vector(0 downto 0); 
		spi_irq : out  std_logic);
	End Component;
	
	Component INV
	port(
		A 	: in std_logic;
		Z	: out std_logic);
	End Component;
	
	Component AND2
	port(
		A 	: in std_logic;
		B 	: in std_logic;
		Z	: out std_logic);
	End Component;
	
begin
	inv_inst: INV
	port map( A=>apb_resetn; Z=>wb_rst);
	
	and2_inst:	AND2
	port map( A=>wb_ack; B=>apb_penable; Z=>apb_pready);
	
	efb_inst: efb_spi_wb
	port map(
		wb_clk_i=>apb_pclk;
		wb_rst_i=>wb_rst;
		wb_cyc_i=>apb_pselx;
		wb_stb_i=>apb_pselx;
		wb_we_i=>apb_pwrite;
		wb_adr_i=>apb_addr(SLAVE_ADDR_WIDTH-1 downto 0);
		wb_dat_i=>apb_pwdata(SLAVE_DATA_WIDTH-1 downto 0);
		wb_dat_o=>apb_prdata(SLAVE_DATA_WIDTH-1 downto 0);
		wb_ack_o=>wb_ack;
		spi_clk=>spi_clk;
		spi_miso=>spi_miso;
		spi_mosi=>spi_mosi;
		spi_scsn=>spi_scsn;
		spi_csn=>spi_csn;
		spi_irq=>spi_irq);
		
	apb_prdata <= (others => '0');
	apb_pslverr <= '0';
end structure;
