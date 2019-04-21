	component ED2platform is
		port (
			clk_clk               : in    std_logic                     := 'X';             -- clk
			key_export            : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			lcd_base_ctrl_export  : out   std_logic_vector(2 downto 0);                     -- export
			lcd_cmd_export        : out   std_logic_vector(2 downto 0);                     -- export
			lcd_data_export       : inout std_logic_vector(15 downto 0) := (others => 'X'); -- export
			ledgreen_export       : out   std_logic_vector(8 downto 0);                     -- export
			ledred_export         : out   std_logic_vector(17 downto 0);                    -- export
			reset_reset_n         : in    std_logic                     := 'X';             -- reset_n
			sd_card_b_SD_cmd      : inout std_logic                     := 'X';             -- b_SD_cmd
			sd_card_b_SD_dat      : inout std_logic                     := 'X';             -- b_SD_dat
			sd_card_b_SD_dat3     : inout std_logic                     := 'X';             -- b_SD_dat3
			sd_card_o_SD_clock    : out   std_logic;                                        -- o_SD_clock
			sdram_clk_clk         : out   std_logic;                                        -- clk
			sdram_wire_addr       : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba         : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n      : out   std_logic;                                        -- cas_n
			sdram_wire_cke        : out   std_logic;                                        -- cke
			sdram_wire_cs_n       : out   std_logic;                                        -- cs_n
			sdram_wire_dq         : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm        : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n      : out   std_logic;                                        -- ras_n
			sdram_wire_we_n       : out   std_logic;                                        -- we_n
			seg7l_HEX4            : out   std_logic_vector(6 downto 0);                     -- HEX4
			seg7l_HEX5            : out   std_logic_vector(6 downto 0);                     -- HEX5
			seg7l_HEX6            : out   std_logic_vector(6 downto 0);                     -- HEX6
			seg7l_HEX7            : out   std_logic_vector(6 downto 0);                     -- HEX7
			sge7r_HEX0            : out   std_logic_vector(6 downto 0);                     -- HEX0
			sge7r_HEX1            : out   std_logic_vector(6 downto 0);                     -- HEX1
			sge7r_HEX2            : out   std_logic_vector(6 downto 0);                     -- HEX2
			sge7r_HEX3            : out   std_logic_vector(6 downto 0);                     -- HEX3
			sram_DQ               : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
			sram_ADDR             : out   std_logic_vector(19 downto 0);                    -- ADDR
			sram_LB_N             : out   std_logic;                                        -- LB_N
			sram_UB_N             : out   std_logic;                                        -- UB_N
			sram_CE_N             : out   std_logic;                                        -- CE_N
			sram_OE_N             : out   std_logic;                                        -- OE_N
			sram_WE_N             : out   std_logic;                                        -- WE_N
			sw_export             : in    std_logic_vector(17 downto 0) := (others => 'X'); -- export
			touch_ctrl_export     : out   std_logic_vector(2 downto 0);                     -- export
			touch_msg_export      : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			touch_pen_intr_export : in    std_logic                     := 'X'              -- export
		);
	end component ED2platform;

	u0 : component ED2platform
		port map (
			clk_clk               => CONNECTED_TO_clk_clk,               --            clk.clk
			key_export            => CONNECTED_TO_key_export,            --            key.export
			lcd_base_ctrl_export  => CONNECTED_TO_lcd_base_ctrl_export,  --  lcd_base_ctrl.export
			lcd_cmd_export        => CONNECTED_TO_lcd_cmd_export,        --        lcd_cmd.export
			lcd_data_export       => CONNECTED_TO_lcd_data_export,       --       lcd_data.export
			ledgreen_export       => CONNECTED_TO_ledgreen_export,       --       ledgreen.export
			ledred_export         => CONNECTED_TO_ledred_export,         --         ledred.export
			reset_reset_n         => CONNECTED_TO_reset_reset_n,         --          reset.reset_n
			sd_card_b_SD_cmd      => CONNECTED_TO_sd_card_b_SD_cmd,      --        sd_card.b_SD_cmd
			sd_card_b_SD_dat      => CONNECTED_TO_sd_card_b_SD_dat,      --               .b_SD_dat
			sd_card_b_SD_dat3     => CONNECTED_TO_sd_card_b_SD_dat3,     --               .b_SD_dat3
			sd_card_o_SD_clock    => CONNECTED_TO_sd_card_o_SD_clock,    --               .o_SD_clock
			sdram_clk_clk         => CONNECTED_TO_sdram_clk_clk,         --      sdram_clk.clk
			sdram_wire_addr       => CONNECTED_TO_sdram_wire_addr,       --     sdram_wire.addr
			sdram_wire_ba         => CONNECTED_TO_sdram_wire_ba,         --               .ba
			sdram_wire_cas_n      => CONNECTED_TO_sdram_wire_cas_n,      --               .cas_n
			sdram_wire_cke        => CONNECTED_TO_sdram_wire_cke,        --               .cke
			sdram_wire_cs_n       => CONNECTED_TO_sdram_wire_cs_n,       --               .cs_n
			sdram_wire_dq         => CONNECTED_TO_sdram_wire_dq,         --               .dq
			sdram_wire_dqm        => CONNECTED_TO_sdram_wire_dqm,        --               .dqm
			sdram_wire_ras_n      => CONNECTED_TO_sdram_wire_ras_n,      --               .ras_n
			sdram_wire_we_n       => CONNECTED_TO_sdram_wire_we_n,       --               .we_n
			seg7l_HEX4            => CONNECTED_TO_seg7l_HEX4,            --          seg7l.HEX4
			seg7l_HEX5            => CONNECTED_TO_seg7l_HEX5,            --               .HEX5
			seg7l_HEX6            => CONNECTED_TO_seg7l_HEX6,            --               .HEX6
			seg7l_HEX7            => CONNECTED_TO_seg7l_HEX7,            --               .HEX7
			sge7r_HEX0            => CONNECTED_TO_sge7r_HEX0,            --          sge7r.HEX0
			sge7r_HEX1            => CONNECTED_TO_sge7r_HEX1,            --               .HEX1
			sge7r_HEX2            => CONNECTED_TO_sge7r_HEX2,            --               .HEX2
			sge7r_HEX3            => CONNECTED_TO_sge7r_HEX3,            --               .HEX3
			sram_DQ               => CONNECTED_TO_sram_DQ,               --           sram.DQ
			sram_ADDR             => CONNECTED_TO_sram_ADDR,             --               .ADDR
			sram_LB_N             => CONNECTED_TO_sram_LB_N,             --               .LB_N
			sram_UB_N             => CONNECTED_TO_sram_UB_N,             --               .UB_N
			sram_CE_N             => CONNECTED_TO_sram_CE_N,             --               .CE_N
			sram_OE_N             => CONNECTED_TO_sram_OE_N,             --               .OE_N
			sram_WE_N             => CONNECTED_TO_sram_WE_N,             --               .WE_N
			sw_export             => CONNECTED_TO_sw_export,             --             sw.export
			touch_ctrl_export     => CONNECTED_TO_touch_ctrl_export,     --     touch_ctrl.export
			touch_msg_export      => CONNECTED_TO_touch_msg_export,      --      touch_msg.export
			touch_pen_intr_export => CONNECTED_TO_touch_pen_intr_export  -- touch_pen_intr.export
		);

