	ED2platform u0 (
		.clk_clk               (<connected-to-clk_clk>),               //            clk.clk
		.epcs_external_dclk    (<connected-to-epcs_external_dclk>),    //  epcs_external.dclk
		.epcs_external_sce     (<connected-to-epcs_external_sce>),     //               .sce
		.epcs_external_sdo     (<connected-to-epcs_external_sdo>),     //               .sdo
		.epcs_external_data0   (<connected-to-epcs_external_data0>),   //               .data0
		.lcd_base_ctrl_export  (<connected-to-lcd_base_ctrl_export>),  //  lcd_base_ctrl.export
		.lcd_cmd_export        (<connected-to-lcd_cmd_export>),        //        lcd_cmd.export
		.lcd_data_export       (<connected-to-lcd_data_export>),       //       lcd_data.export
		.pen_smp_period_export (<connected-to-pen_smp_period_export>), // pen_smp_period.export
		.reset_reset_n         (<connected-to-reset_reset_n>),         //          reset.reset_n
		.sd_card_b_SD_cmd      (<connected-to-sd_card_b_SD_cmd>),      //        sd_card.b_SD_cmd
		.sd_card_b_SD_dat      (<connected-to-sd_card_b_SD_dat>),      //               .b_SD_dat
		.sd_card_b_SD_dat3     (<connected-to-sd_card_b_SD_dat3>),     //               .b_SD_dat3
		.sd_card_o_SD_clock    (<connected-to-sd_card_o_SD_clock>),    //               .o_SD_clock
		.sdram_wire_addr       (<connected-to-sdram_wire_addr>),       //     sdram_wire.addr
		.sdram_wire_ba         (<connected-to-sdram_wire_ba>),         //               .ba
		.sdram_wire_cas_n      (<connected-to-sdram_wire_cas_n>),      //               .cas_n
		.sdram_wire_cke        (<connected-to-sdram_wire_cke>),        //               .cke
		.sdram_wire_cs_n       (<connected-to-sdram_wire_cs_n>),       //               .cs_n
		.sdram_wire_dq         (<connected-to-sdram_wire_dq>),         //               .dq
		.sdram_wire_dqm        (<connected-to-sdram_wire_dqm>),        //               .dqm
		.sdram_wire_ras_n      (<connected-to-sdram_wire_ras_n>),      //               .ras_n
		.sdram_wire_we_n       (<connected-to-sdram_wire_we_n>),       //               .we_n
		.touch_ctrl_export     (<connected-to-touch_ctrl_export>),     //     touch_ctrl.export
		.touch_msg_export      (<connected-to-touch_msg_export>),      //      touch_msg.export
		.touch_pen_intr_export (<connected-to-touch_pen_intr_export>)  // touch_pen_intr.export
	);

