
module ED2platform (
	clk_clk,
	lcd_base_ctrl_export,
	lcd_cmd_export,
	lcd_data_export,
	reset_reset_n,
	sd_card_b_SD_cmd,
	sd_card_b_SD_dat,
	sd_card_b_SD_dat3,
	sd_card_o_SD_clock,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	touch_ctrl_export,
	touch_msg_export,
	touch_pen_intr_export,
	pen_smp_period_export);	

	input		clk_clk;
	output	[2:0]	lcd_base_ctrl_export;
	output	[2:0]	lcd_cmd_export;
	inout	[15:0]	lcd_data_export;
	input		reset_reset_n;
	inout		sd_card_b_SD_cmd;
	inout		sd_card_b_SD_dat;
	inout		sd_card_b_SD_dat3;
	output		sd_card_o_SD_clock;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output	[2:0]	touch_ctrl_export;
	input	[1:0]	touch_msg_export;
	input		touch_pen_intr_export;
	output	[15:0]	pen_smp_period_export;
endmodule
