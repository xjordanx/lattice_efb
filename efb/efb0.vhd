Library IEEE;
Use     IEEE.std_logic_1164.all;


Entity efb0 Is
  generic (
      ADDR_WIDTH : integer := 32;
      SLAVE_DATA_WIDTH : integer := 8;
      HOST_DATA_WIDTH : integer := 32
  );
  port
  (
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
      i2c1_scl     : inout  std_logic; 
      i2c1_sda     : inout  std_logic; 
      i2c1_irqo    : out  std_logic; 
      i2c2_scl     : inout  std_logic; 
      i2c2_sda     : inout  std_logic; 
      i2c2_irqo    : out  std_logic; 
      spi_clk      : inout  std_logic; 
      spi_miso     : inout  std_logic; 
      spi_mosi     : inout  std_logic; 
      spi_scsn     : in  std_logic; 
      spi_csn      : out  std_logic_vector(1 downto 0); 
      spi_irq      : out  std_logic; 
      ufm_sn       : in  std_logic; 
      wbc_ufm_irq  : out  std_logic
  );
End efb0;
------------------------------------------------------------

------------------------------------------------------------
Architecture Structure Of efb0 Is

    Signal wb_clk   : std_logic;
    Signal wb_rst  : std_logic;
    Signal wb_cyc  : std_logic;
    Signal wb_stb  : std_logic;
    Signal wb_we   : std_logic;
    Signal wb_adr  : std_logic_vector(ADDR_WIDTH-1 downto 0);
    Signal wb_dat_r  : std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0);
    Signal wb_dat_w  : std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0);
    Signal wb_ack  : std_logic;

   Component apb_wb_bridge                                        
    port (
        -- APB Signals
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

        -- Wishbone Signals
        wb_clk_o    : out std_logic;
        wb_rst_o    : out std_logic;
        wb_cyc_o    : out std_logic;
        wb_stb_o    : out std_logic;
        wb_we_o     : out std_logic;
        wb_adr_o    : out std_logic_vector(ADDR_WIDTH-1 downto 0);
        wb_dat_o    : out std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0);
        wb_dat_i    : in  std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0);
        wb_ack_i    : in  std_logic
    );
   End Component;
   Component efb
    port (
        wb_clk_i    : in  std_logic; 
        wb_rst_i    : in  std_logic; 
        wb_cyc_i    : in  std_logic; 
        wb_stb_i    : in  std_logic; 
        wb_we_i     : in  std_logic; 
        wb_adr_i    : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        wb_dat_i    : in  std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0);
        wb_dat_o    : out  std_logic_vector(SLAVE_DATA_WIDTH-1 downto 0); 
        wb_ack_o    : out  std_logic; 
        i2c1_scl    : inout  std_logic; 
        i2c1_sda    : inout  std_logic; 
        i2c1_irqo   : out  std_logic; 
        i2c2_scl    : inout  std_logic; 
        i2c2_sda    : inout  std_logic; 
        i2c2_irqo   : out  std_logic; 
        spi_clk     : inout  std_logic; 
        spi_miso    : inout  std_logic; 
        spi_mosi    : inout  std_logic; 
        spi_scsn    : in  std_logic; 
        spi_csn     : out  std_logic_vector(1 downto 0); 
        spi_irq     : out  std_logic; 
        ufm_sn      : in  std_logic; 
        wbc_ufm_irq : out  std_logic);
   End Component;

Begin
    u_apb_wb : apb_wb_bridge                            
      Port Map
      (
        apb_pclk    => apb_pclk,
        apb_resetn  => apb_resetn,
        apb_addr    => apb_addr,
        apb_pselx   => apb_pselx,
        apb_penable => apb_penable,
        apb_pwrite  => apb_pwrite,
        apb_pwdata  => apb_pwdata,
        apb_pready  => apb_pready,
        apb_prdata  => apb_prdata,
        apb_pslverr => apb_pslverr,
        wb_clk_o    => wb_clk,
        wb_rst_o    => wb_rst,
        wb_cyc_o    => wb_cyc,
        wb_stb_o    => wb_stb,
        wb_we_o     => wb_we, 
        wb_adr_o    => wb_adr,
        wb_dat_o    => wb_dat_w,
        wb_dat_i    => wb_dat_r,
        wb_ack_i    => wb_ack
      );

    u_efb : efb
      Port Map
      (
        wb_clk_i    => wb_clk,
        wb_rst_i    => wb_rst,
        wb_cyc_i    => wb_cyc,
        wb_stb_i    => wb_stb,
        wb_we_i     => wb_we, 
        wb_adr_i    => wb_adr,
        wb_dat_i    => wb_dat_w,
        wb_dat_o    => wb_dat_r,
        wb_ack_o    => wb_ack,
        i2c1_scl    => i2c1_scl, 
        i2c1_sda    => i2c1_sda,     
        i2c1_irqo   => i2c1_irqo,    
        i2c2_scl    => i2c2_scl,     
        i2c2_sda    => i2c2_sda,     
        i2c2_irqo   => i2c2_irqo,    
        spi_clk     => spi_clk,  
        spi_miso    => spi_miso,     
        spi_mosi    => spi_mosi,     
        spi_scsn    => spi_scsn,     
        spi_csn     => spi_csn,  
        spi_irq     => spi_irq,  
        ufm_sn      => ufm_sn,   
        wbc_ufm_irq => wbc_ufm_irq
      );

End Structure;
------------------------------------------------------------
