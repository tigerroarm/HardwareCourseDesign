# Copyright (C) 1991-2015 Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions 
# and other software and tools, and its AMPP partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License 
# Subscription Agreement, the Altera Quartus II License Agreement,
# the Altera MegaCore Function License Agreement, or other 
# applicable license agreement, including, without limitation, 
# that your use is for the sole purpose of programming logic 
# devices manufactured by Altera and sold by Altera or its 
# authorized distributors.  Please refer to the applicable 
# agreement for further details.

# Quartus II 64-Bit Version 15.0.0 Build 145 04/22/2015 SJ Full Version
# File: G:\altera_ED2_115\test1\DE2_115.tcl
# Generated on: Fri Apr 12 15:44:50 2019

package require ::quartus::project

#============================================================
# Build by Terasic System Builder
#============================================================

set_global_assignment -name FAMILY "Cyclone IV E"
set_global_assignment -name DEVICE EP4CE115F29C7
set_global_assignment -name TOP_LEVEL_ENTITY DE2_115
set_global_assignment -name DEVICE_FILTER_PACKAGE FBGA
set_global_assignment -name DEVICE_FILTER_PIN_COUNT 780
set_global_assignment -name DEVICE_FILTER_SPEED_GRADE 7
set_global_assignment -name STRATIX_DEVICE_IO_STANDARD "3.3-V LVTTL"

#============================================================
# CLOCK
#============================================================
set_location_assignment PIN_Y2 -to CLOCK_50
#复位键对应位置key0
set_location_assignment PIN_M23 -to reset_n 


#============================================================
# LED
#============================================================

#set_location_assignment PIN_G19 -to LEDR[0]
#set_location_assignment PIN_F19 -to LEDR[1]
#set_location_assignment PIN_E19 -to LEDR[2]
#set_location_assignment PIN_F21 -to LEDR[3]
#set_location_assignment PIN_F18 -to LEDR[4]
#set_location_assignment PIN_E18 -to LEDR[5]
#set_location_assignment PIN_J19 -to LEDR[6]
#set_location_assignment PIN_H19 -to LEDR[7]
#set_location_assignment PIN_J17 -to LEDR[8]
#set_location_assignment PIN_G17 -to LEDR[9]
#set_location_assignment PIN_J15 -to LEDR[10]
#set_location_assignment PIN_H16 -to LEDR[11]
#set_location_assignment PIN_J16 -to LEDR[12]
#set_location_assignment PIN_H17 -to LEDR[13]
#set_location_assignment PIN_F15 -to LEDR[14]
#set_location_assignment PIN_G15 -to LEDR[15]
#set_location_assignment PIN_G16 -to LEDR[16]
#set_location_assignment PIN_H15 -to LEDR[17]
#set_location_assignment PIN_E21 -to LEDG[0]
#set_location_assignment PIN_E22 -to LEDG[1]
#set_location_assignment PIN_E25 -to LEDG[2]
#set_location_assignment PIN_E24 -to LEDG[3]
#set_location_assignment PIN_H21 -to LEDG[4]
#set_location_assignment PIN_G20 -to LEDG[5]
#set_location_assignment PIN_G22 -to LEDG[6]
#set_location_assignment PIN_G21 -to LEDG[7]
#set_location_assignment PIN_F17 -to LEDG[8]

#============================================================
# KEY
#============================================================
#set_location_assignment PIN_M23 -to KEY[0]
#set_location_assignment PIN_M21 -to KEY[1]
#set_location_assignment PIN_N21 -to KEY[2]
#set_location_assignment PIN_R24 -to KEY[3]

#============================================================
# SW
#============================================================
#set_location_assignment PIN_AB28 -to SW[0]
#set_location_assignment PIN_AC28 -to SW[1]
#set_location_assignment PIN_AC27 -to SW[2]
#set_location_assignment PIN_AD27 -to SW[3]
#set_location_assignment PIN_AB27 -to SW[4]
#set_location_assignment PIN_AC26 -to SW[5]
#set_location_assignment PIN_AD26 -to SW[6]
#set_location_assignment PIN_AB26 -to SW[7]
#set_location_assignment PIN_AC25 -to SW[8]
#set_location_assignment PIN_AB25 -to SW[9]
#set_location_assignment PIN_AC24 -to SW[10]
#set_location_assignment PIN_AB24 -to SW[11]
#set_location_assignment PIN_AB23 -to SW[12]
#set_location_assignment PIN_AA24 -to SW[13]
#set_location_assignment PIN_AA23 -to SW[14]
#set_location_assignment PIN_AA22 -to SW[15]
#set_location_assignment PIN_Y24 -to SW[16]
#set_location_assignment PIN_Y23 -to SW[17]

#============================================================
# SEG7
#============================================================
#set_location_assignment PIN_G18 -to HEX0[0]
#set_location_assignment PIN_F22 -to HEX0[1]
#set_location_assignment PIN_E17 -to HEX0[2]
#set_location_assignment PIN_L26 -to HEX0[3]
#set_location_assignment PIN_L25 -to HEX0[4]
#set_location_assignment PIN_J22 -to HEX0[5]
#set_location_assignment PIN_H22 -to HEX0[6]
#set_location_assignment PIN_M24 -to HEX1[0]
#set_location_assignment PIN_Y22 -to HEX1[1]
#set_location_assignment PIN_W21 -to HEX1[2]
#set_location_assignment PIN_W22 -to HEX1[3]
#set_location_assignment PIN_W25 -to HEX1[4]
#set_location_assignment PIN_U23 -to HEX1[5]
#set_location_assignment PIN_U24 -to HEX1[6]
#set_location_assignment PIN_AA25 -to HEX2[0]
#set_location_assignment PIN_AA26 -to HEX2[1]
#set_location_assignment PIN_Y25 -to HEX2[2]
#set_location_assignment PIN_W26 -to HEX2[3]
#set_location_assignment PIN_Y26 -to HEX2[4]
#set_location_assignment PIN_W27 -to HEX2[5]
#set_location_assignment PIN_W28 -to HEX2[6]
#set_location_assignment PIN_V21 -to HEX3[0]
#set_location_assignment PIN_U21 -to HEX3[1]
#set_location_assignment PIN_AB20 -to HEX3[2]
#set_location_assignment PIN_AA21 -to HEX3[3]
#set_location_assignment PIN_AD24 -to HEX3[4]
#set_location_assignment PIN_AF23 -to HEX3[5]
#set_location_assignment PIN_Y19 -to HEX3[6]
#set_location_assignment PIN_AB19 -to HEX4[0]
#set_location_assignment PIN_AA19 -to HEX4[1]
#set_location_assignment PIN_AG21 -to HEX4[2]
#set_location_assignment PIN_AH21 -to HEX4[3]
#set_location_assignment PIN_AE19 -to HEX4[4]
#set_location_assignment PIN_AF19 -to HEX4[5]
#set_location_assignment PIN_AE18 -to HEX4[6]
#set_location_assignment PIN_AD18 -to HEX5[0]
#set_location_assignment PIN_AC18 -to HEX5[1]
#set_location_assignment PIN_AB18 -to HEX5[2]
#set_location_assignment PIN_AH19 -to HEX5[3]
#set_location_assignment PIN_AG19 -to HEX5[4]
#set_location_assignment PIN_AF18 -to HEX5[5]
#set_location_assignment PIN_AH18 -to HEX5[6]
#set_location_assignment PIN_AA17 -to HEX6[0]
#set_location_assignment PIN_AB16 -to HEX6[1]
#set_location_assignment PIN_AA16 -to HEX6[2]
#set_location_assignment PIN_AB17 -to HEX6[3]
#set_location_assignment PIN_AB15 -to HEX6[4]
#set_location_assignment PIN_AA15 -to HEX6[5]
#set_location_assignment PIN_AC17 -to HEX6[6]
#set_location_assignment PIN_AD17 -to HEX7[0]
#set_location_assignment PIN_AE17 -to HEX7[1]
#set_location_assignment PIN_AG17 -to HEX7[2]
#set_location_assignment PIN_AH17 -to HEX7[3]
#set_location_assignment PIN_AF17 -to HEX7[4]
#set_location_assignment PIN_AG18 -to HEX7[5]
#set_location_assignment PIN_AA14 -to HEX7[6]

#============================================================
# EPCS
#============================================================
set_location_assignment PIN_P3 -to epcs_dclk
set_location_assignment PIN_E2 -to epcs_sce
set_location_assignment PIN_F4 -to epcs_sdo
set_location_assignment PIN_N7 -to epcs_data0

#============================================================
# SDCARD
#============================================================
set_location_assignment PIN_AD14 -to SD_CMD
set_location_assignment PIN_AE13 -to SD_CLK
set_location_assignment PIN_AE14 -to SD_DAT0
set_location_assignment PIN_AC14 -to SD_DAT3

#============================================================
# SDRAM
#============================================================
set_location_assignment PIN_U7 -to SDRAM_BA[0]
set_location_assignment PIN_R4 -to SDRAM_BA[1]
set_location_assignment PIN_U2 -to SDRAM_DQM[0]
set_location_assignment PIN_W4 -to SDRAM_DQM[1]
set_location_assignment PIN_K8 -to SDRAM_DQM[2]
set_location_assignment PIN_N8 -to SDRAM_DQM[3]
set_location_assignment PIN_U6 -to SDRAM_RAS_N
set_location_assignment PIN_V7 -to SDRAM_CAS_N
set_location_assignment PIN_AA6 -to SDRAM_CKE
set_location_assignment PIN_AE5 -to SDRAM_CLK
set_location_assignment PIN_V6 -to SDRAM_WE_N
set_location_assignment PIN_T4 -to SDRAM_CS_N
set_location_assignment PIN_W3 -to SDRAM_DQ[0]
set_location_assignment PIN_W2 -to SDRAM_DQ[1]
set_location_assignment PIN_V4 -to SDRAM_DQ[2]
set_location_assignment PIN_W1 -to SDRAM_DQ[3]
set_location_assignment PIN_V3 -to SDRAM_DQ[4]
set_location_assignment PIN_V2 -to SDRAM_DQ[5]
set_location_assignment PIN_V1 -to SDRAM_DQ[6]
set_location_assignment PIN_U3 -to SDRAM_DQ[7]
set_location_assignment PIN_Y3 -to SDRAM_DQ[8]
set_location_assignment PIN_Y4 -to SDRAM_DQ[9]
set_location_assignment PIN_AB1 -to SDRAM_DQ[10]
set_location_assignment PIN_AA3 -to SDRAM_DQ[11]
set_location_assignment PIN_AB2 -to SDRAM_DQ[12]
set_location_assignment PIN_AC1 -to SDRAM_DQ[13]
set_location_assignment PIN_AB3 -to SDRAM_DQ[14]
set_location_assignment PIN_AC2 -to SDRAM_DQ[15]
set_location_assignment PIN_M8 -to SDRAM_DQ[16]
set_location_assignment PIN_L8 -to SDRAM_DQ[17]
set_location_assignment PIN_P2 -to SDRAM_DQ[18]
set_location_assignment PIN_N3 -to SDRAM_DQ[19]
set_location_assignment PIN_N4 -to SDRAM_DQ[20]
set_location_assignment PIN_M4 -to SDRAM_DQ[21]
set_location_assignment PIN_M7 -to SDRAM_DQ[22]
set_location_assignment PIN_L7 -to SDRAM_DQ[23]
set_location_assignment PIN_U5 -to SDRAM_DQ[24]
set_location_assignment PIN_R7 -to SDRAM_DQ[25]
set_location_assignment PIN_R1 -to SDRAM_DQ[26]
set_location_assignment PIN_R2 -to SDRAM_DQ[27]
set_location_assignment PIN_R3 -to SDRAM_DQ[28]
set_location_assignment PIN_T3 -to SDRAM_DQ[29]
set_location_assignment PIN_U4 -to SDRAM_DQ[30]
set_location_assignment PIN_U1 -to SDRAM_DQ[31]
set_location_assignment PIN_R6 -to SDRAM_ADDR[0]
set_location_assignment PIN_V8 -to SDRAM_ADDR[1]
set_location_assignment PIN_U8 -to SDRAM_ADDR[2]
set_location_assignment PIN_P1 -to SDRAM_ADDR[3]
set_location_assignment PIN_V5 -to SDRAM_ADDR[4]
set_location_assignment PIN_W8 -to SDRAM_ADDR[5]
set_location_assignment PIN_W7 -to SDRAM_ADDR[6]
set_location_assignment PIN_AA7 -to SDRAM_ADDR[7]
set_location_assignment PIN_Y5 -to SDRAM_ADDR[8]
set_location_assignment PIN_Y6 -to SDRAM_ADDR[9]
set_location_assignment PIN_R5 -to SDRAM_ADDR[10]
set_location_assignment PIN_AA5 -to SDRAM_ADDR[11]
set_location_assignment PIN_Y7 -to SDRAM_ADDR[12]

#============================================================
# SRAM
#============================================================
#set_location_assignment PIN_AB7 -to SRAM_ADDR[0]
#set_location_assignment PIN_AD7 -to SRAM_ADDR[1]
#set_location_assignment PIN_AE7 -to SRAM_ADDR[2]
#set_location_assignment PIN_AC7 -to SRAM_ADDR[3]
#set_location_assignment PIN_AB6 -to SRAM_ADDR[4]
#set_location_assignment PIN_AE6 -to SRAM_ADDR[5]
#set_location_assignment PIN_AB5 -to SRAM_ADDR[6]
#set_location_assignment PIN_AC5 -to SRAM_ADDR[7]
#set_location_assignment PIN_AF5 -to SRAM_ADDR[8]
#set_location_assignment PIN_T7 -to SRAM_ADDR[9]
#set_location_assignment PIN_AF2 -to SRAM_ADDR[10]
#set_location_assignment PIN_AD3 -to SRAM_ADDR[11]
#set_location_assignment PIN_AB4 -to SRAM_ADDR[12]
#set_location_assignment PIN_AC3 -to SRAM_ADDR[13]
#set_location_assignment PIN_AA4 -to SRAM_ADDR[14]
#set_location_assignment PIN_AB11 -to SRAM_ADDR[15]
#set_location_assignment PIN_AC11 -to SRAM_ADDR[16]
#set_location_assignment PIN_AB9 -to SRAM_ADDR[17]
#set_location_assignment PIN_AB8 -to SRAM_ADDR[18]
#set_location_assignment PIN_T8 -to SRAM_ADDR[19]
#set_location_assignment PIN_AH3 -to SRAM_DQ[0]
#set_location_assignment PIN_AF4 -to SRAM_DQ[1]
#set_location_assignment PIN_AG4 -to SRAM_DQ[2]
#set_location_assignment PIN_AH4 -to SRAM_DQ[3]
#set_location_assignment PIN_AF6 -to SRAM_DQ[4]
#set_location_assignment PIN_AG6 -to SRAM_DQ[5]
#set_location_assignment PIN_AH6 -to SRAM_DQ[6]
#set_location_assignment PIN_AF7 -to SRAM_DQ[7]
#set_location_assignment PIN_AD1 -to SRAM_DQ[8]
#set_location_assignment PIN_AD2 -to SRAM_DQ[9]
#set_location_assignment PIN_AE2 -to SRAM_DQ[10]
#set_location_assignment PIN_AE1 -to SRAM_DQ[11]
#set_location_assignment PIN_AE3 -to SRAM_DQ[12]
#set_location_assignment PIN_AE4 -to SRAM_DQ[13]
#set_location_assignment PIN_AF3 -to SRAM_DQ[14]
#set_location_assignment PIN_AG3 -to SRAM_DQ[15]
#set_location_assignment PIN_AC4 -to SRAM_UB_N
#set_location_assignment PIN_AD4 -to SRAM_LB_N
#set_location_assignment PIN_AF8 -to SRAM_CE_N
#set_location_assignment PIN_AD5 -to SRAM_OE_N
#set_location_assignment PIN_AE8 -to SRAM_WE_N

#============================================================
# pios added by LBL at 2019/4/16
#============================================================
set_location_assignment PIN_AB22 -to lcd_base_ctrl[0]
set_location_assignment PIN_AB21 -to lcd_cmd[0]
set_location_assignment PIN_AC21 -to lcd_base_ctrl[1]
set_location_assignment PIN_AD21 -to lcd_data[1]
set_location_assignment PIN_AD15 -to lcd_data[3]
set_location_assignment PIN_AC19 -to lcd_data[5]
set_location_assignment PIN_AD19 -to lcd_data[7]
set_location_assignment PIN_AF24 -to lcd_data[9]
set_location_assignment PIN_AF25 -to lcd_data[11]
set_location_assignment PIN_AE22 -to lcd_data[13]
set_location_assignment PIN_AF22 -to lcd_data[15]
set_location_assignment PIN_AG25 -to lcd_base_ctrl[2]
set_location_assignment PIN_AH25 -to touch_msg[0]
set_location_assignment PIN_AG22 -to touch_pen_intr
set_location_assignment PIN_AH22 -to touch_ctrl[0]

set_location_assignment PIN_AC15 -to lcd_cmd[1]
set_location_assignment PIN_Y17 -to lcd_cmd[2]
set_location_assignment PIN_Y16 -to lcd_data[0]
set_location_assignment PIN_AE16 -to lcd_data[2]
set_location_assignment PIN_AE15 -to lcd_data[4]
set_location_assignment PIN_AF16 -to lcd_data[6]
set_location_assignment PIN_AF15 -to lcd_data[8]
set_location_assignment PIN_AE21 -to lcd_data[10]
set_location_assignment PIN_AC22 -to lcd_data[12]
set_location_assignment PIN_AF21 -to lcd_data[14]
set_location_assignment PIN_AD22 -to touch_ctrl[1]
set_location_assignment PIN_AD25 -to touch_msg[1]
set_location_assignment PIN_AE25 -to touch_ctrl[2]

#============================================================
# End of pin assignments by Terasic System Builder
#============================================================


set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85
set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
set_global_assignment -name POWER_BOARD_THERMAL_MODEL "NONE (CONSERVATIVE)"
set_global_assignment -name POWER_PRESET_COOLING_SOLUTION "23 MM HEAT SINK WITH 200 LFPM AIRFLOW"
set_global_assignment -name PARTITION_NETLIST_TYPE SOURCE -section_id Top
set_global_assignment -name PARTITION_FITTER_PRESERVATION_LEVEL PLACEMENT_AND_ROUTING -section_id Top
set_global_assignment -name PARTITION_COLOR 16764057 -section_id Top
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to CLOCK_50
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to reset_n

#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[0]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[1]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[2]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[3]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[4]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[5]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[6]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[7]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[8]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[9]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[10]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[11]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[12]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[13]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[14]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[15]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[16]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDR[17]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[0]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[1]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[2]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[3]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[4]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[5]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[6]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[7]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to LEDG[8]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[7]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[8]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[9]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[10]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[11]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[12]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[13]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[14]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[15]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[16]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW[17]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to HEX0[0]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to HEX0[1]
#set_instance_assignment -name IO_STANDARD "2.5 V" -to HEX0[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX0[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX0[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX0[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX0[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX1[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX2[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX3[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX4[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX5[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX6[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to HEX7[6]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to epcs_dclk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to epcs_sce
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to epcs_sdo
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to epcs_data0

set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SD_CMD
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SD_CLK
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SD_DAT0
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SD_DAT3
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_BA[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_BA[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQM[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQM[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQM[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQM[3]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_RAS_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_CAS_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_CKE
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_CLK
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_WE_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_CS_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQ[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SDRAM_DQ[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[7]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[8]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[9]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[10]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[11]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[12]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[13]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[14]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[15]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[16]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[17]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[18]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_ADDR[19]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[0]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[1]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[2]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[3]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[4]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[5]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[6]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[7]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[8]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[9]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[10]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[11]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[12]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[13]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[14]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_DQ[15]
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_UB_N
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_LB_N
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_CE_N
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_OE_N
#set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SRAM_WE_N

#============================================================
# pios added by LBL at 2019/4/16
#============================================================
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[3]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[4]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[5]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[6]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[7]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[8]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[9]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[10]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[11]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[12]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[13]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[14]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_data[15]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_base_ctrl[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_base_ctrl[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_base_ctrl[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_cmd[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_cmd[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to lcd_cmd[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_ctrl[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_ctrl[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_ctrl[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_msg[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_msg[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to touch_pen_intr

set_global_assignment -name SDC_FILE DE2_115.sdc
set_global_assignment -name SOURCE_FILE DE2_115.qsf
set_global_assignment -name QIP_FILE ED2platform/synthesis/ED2platform.qip
set_instance_assignment -name PARTITION_HIERARCHY root_partition -to | -section_id Top
