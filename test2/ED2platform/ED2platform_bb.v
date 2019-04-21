
module ED2platform (
	clk_clk,
	key_export,
	lcd_base_ctrl_export,
	lcd_cmd_export,
	lcd_data_export,
	ledgreen_export,
	ledred_export,
	reset_reset_n,
	sd_card_b_SD_cmd,
	sd_card_b_SD_dat,
	sd_card_b_SD_dat3,
	sd_card_o_SD_clock,
	sdram_clk_clk,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	seg7l_HEX4,
	seg7l_HEX5,
	seg7l_HEX6,
	seg7l_HEX7,
	sge7r_HEX0,
	sge7r_HEX1,
	sge7r_HEX2,
	sge7r_HEX3,
	sram_DQ,
	sram_ADDR,
	sram_LB_N,
	sram_UB_N,
	sram_CE_N,
	sram_OE_N,
	sram_WE_N,
	sw_export,
	touch_ctrl_export,
	touch_msg_export,
	touch_pen_intr_export);	

	input		clk_clk;
	input	[3:0]	key_export;
	output	[2:0]	lcd_base_ctrl_export;
	output	[2:0]	lcd_cmd_export;
	inout	[15:0]	lcd_data_export;
	output	[8:0]	ledgreen_export;
	output	[17:0]	ledred_export;
	input		reset_reset_n;
	inout		sd_card_b_SD_cmd;
	inout		sd_card_b_SD_dat;
	inout		sd_card_b_SD_dat3;
	output		sd_card_o_SD_clock;
	output		sdram_clk_clk;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output	[6:0]	seg7l_HEX4;
	output	[6:0]	seg7l_HEX5;
	output	[6:0]	seg7l_HEX6;
	output	[6:0]	seg7l_HEX7;
	output	[6:0]	sge7r_HEX0;
	output	[6:0]	sge7r_HEX1;
	output	[6:0]	sge7r_HEX2;
	output	[6:0]	sge7r_HEX3;
	inout	[15:0]	sram_DQ;
	output	[19:0]	sram_ADDR;
	output		sram_LB_N;
	output		sram_UB_N;
	output		sram_CE_N;
	output		sram_OE_N;
	output		sram_WE_N;
	input	[17:0]	sw_export;
	output	[2:0]	touch_ctrl_export;
	input	[1:0]	touch_msg_export;
	input		touch_pen_intr_export;
endmodule
