/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu' in SOPC Builder design 'ED2platform'
 * SOPC Builder design path: ../../ED2platform.sopcinfo
 *
 * Generated: Wed May 15 04:47:59 CST 2019
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * Altera_UP_SD_Card_Avalon_Interface_0 configuration
 *
 */

#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_BASE 0x8201000
#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_IRQ -1
#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME "/dev/Altera_UP_SD_Card_Avalon_Interface_0"
#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_SPAN 1024
#define ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_TYPE "Altera_UP_SD_Card_Avalon_Interface"
#define ALT_MODULE_CLASS_Altera_UP_SD_Card_Avalon_Interface_0 Altera_UP_SD_Card_Avalon_Interface


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x08200820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1c
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x00000020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1c
#define ALT_CPU_NAME "cpu"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00000000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x08200820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1c
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x00000020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1c
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00000000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_GEN2
#define __ALTERA_UP_SD_CARD_AVALON_INTERFACE


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x82014c8
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x82014c8
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x82014c8
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "ED2platform"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER_1S
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x82014c8
#define JTAG_UART_IRQ 0
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * pen_smp_speed configuration
 *
 */

#define ALT_MODULE_CLASS_pen_smp_speed altera_avalon_pio
#define PEN_SMP_SPEED_BASE 0x8000000
#define PEN_SMP_SPEED_BIT_CLEARING_EDGE_REGISTER 0
#define PEN_SMP_SPEED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PEN_SMP_SPEED_CAPTURE 0
#define PEN_SMP_SPEED_DATA_WIDTH 16
#define PEN_SMP_SPEED_DO_TEST_BENCH_WIRING 0
#define PEN_SMP_SPEED_DRIVEN_SIM_VALUE 0
#define PEN_SMP_SPEED_EDGE_TYPE "NONE"
#define PEN_SMP_SPEED_FREQ 100000000
#define PEN_SMP_SPEED_HAS_IN 0
#define PEN_SMP_SPEED_HAS_OUT 1
#define PEN_SMP_SPEED_HAS_TRI 0
#define PEN_SMP_SPEED_IRQ -1
#define PEN_SMP_SPEED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PEN_SMP_SPEED_IRQ_TYPE "NONE"
#define PEN_SMP_SPEED_NAME "/dev/pen_smp_speed"
#define PEN_SMP_SPEED_RESET_VALUE 1
#define PEN_SMP_SPEED_SPAN 16
#define PEN_SMP_SPEED_TYPE "altera_avalon_pio"


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x0
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x19
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 10
#define SDRAM_SDRAM_DATA_WIDTH 32
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 13
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 1
#define SDRAM_SPAN 134217728
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * sysid0 configuration
 *
 */

#define ALT_MODULE_CLASS_sysid0 altera_avalon_sysid_qsys
#define SYSID0_BASE 0x82014c0
#define SYSID0_ID 305419896
#define SYSID0_IRQ -1
#define SYSID0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID0_NAME "/dev/sysid0"
#define SYSID0_SPAN 8
#define SYSID0_TIMESTAMP 1557864254
#define SYSID0_TYPE "altera_avalon_sysid_qsys"


/*
 * tftlcd_base_ctrl configuration
 *
 */

#define ALT_MODULE_CLASS_tftlcd_base_ctrl altera_avalon_pio
#define TFTLCD_BASE_CTRL_BASE 0x8201420
#define TFTLCD_BASE_CTRL_BIT_CLEARING_EDGE_REGISTER 0
#define TFTLCD_BASE_CTRL_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TFTLCD_BASE_CTRL_CAPTURE 0
#define TFTLCD_BASE_CTRL_DATA_WIDTH 3
#define TFTLCD_BASE_CTRL_DO_TEST_BENCH_WIRING 0
#define TFTLCD_BASE_CTRL_DRIVEN_SIM_VALUE 0
#define TFTLCD_BASE_CTRL_EDGE_TYPE "NONE"
#define TFTLCD_BASE_CTRL_FREQ 100000000
#define TFTLCD_BASE_CTRL_HAS_IN 0
#define TFTLCD_BASE_CTRL_HAS_OUT 1
#define TFTLCD_BASE_CTRL_HAS_TRI 0
#define TFTLCD_BASE_CTRL_IRQ -1
#define TFTLCD_BASE_CTRL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TFTLCD_BASE_CTRL_IRQ_TYPE "NONE"
#define TFTLCD_BASE_CTRL_NAME "/dev/tftlcd_base_ctrl"
#define TFTLCD_BASE_CTRL_RESET_VALUE 0
#define TFTLCD_BASE_CTRL_SPAN 16
#define TFTLCD_BASE_CTRL_TYPE "altera_avalon_pio"


/*
 * tftlcd_cmd configuration
 *
 */

#define ALT_MODULE_CLASS_tftlcd_cmd altera_avalon_pio
#define TFTLCD_CMD_BASE 0x8201430
#define TFTLCD_CMD_BIT_CLEARING_EDGE_REGISTER 0
#define TFTLCD_CMD_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TFTLCD_CMD_CAPTURE 0
#define TFTLCD_CMD_DATA_WIDTH 3
#define TFTLCD_CMD_DO_TEST_BENCH_WIRING 0
#define TFTLCD_CMD_DRIVEN_SIM_VALUE 0
#define TFTLCD_CMD_EDGE_TYPE "NONE"
#define TFTLCD_CMD_FREQ 100000000
#define TFTLCD_CMD_HAS_IN 0
#define TFTLCD_CMD_HAS_OUT 1
#define TFTLCD_CMD_HAS_TRI 0
#define TFTLCD_CMD_IRQ -1
#define TFTLCD_CMD_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TFTLCD_CMD_IRQ_TYPE "NONE"
#define TFTLCD_CMD_NAME "/dev/tftlcd_cmd"
#define TFTLCD_CMD_RESET_VALUE 0
#define TFTLCD_CMD_SPAN 16
#define TFTLCD_CMD_TYPE "altera_avalon_pio"


/*
 * tftlcd_data configuration
 *
 */

#define ALT_MODULE_CLASS_tftlcd_data altera_avalon_pio
#define TFTLCD_DATA_BASE 0x8201450
#define TFTLCD_DATA_BIT_CLEARING_EDGE_REGISTER 0
#define TFTLCD_DATA_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TFTLCD_DATA_CAPTURE 0
#define TFTLCD_DATA_DATA_WIDTH 16
#define TFTLCD_DATA_DO_TEST_BENCH_WIRING 0
#define TFTLCD_DATA_DRIVEN_SIM_VALUE 0
#define TFTLCD_DATA_EDGE_TYPE "NONE"
#define TFTLCD_DATA_FREQ 100000000
#define TFTLCD_DATA_HAS_IN 0
#define TFTLCD_DATA_HAS_OUT 0
#define TFTLCD_DATA_HAS_TRI 1
#define TFTLCD_DATA_IRQ -1
#define TFTLCD_DATA_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TFTLCD_DATA_IRQ_TYPE "NONE"
#define TFTLCD_DATA_NAME "/dev/tftlcd_data"
#define TFTLCD_DATA_RESET_VALUE 0
#define TFTLCD_DATA_SPAN 16
#define TFTLCD_DATA_TYPE "altera_avalon_pio"


/*
 * timer_1s configuration
 *
 */

#define ALT_MODULE_CLASS_timer_1s altera_avalon_timer
#define TIMER_1S_ALWAYS_RUN 0
#define TIMER_1S_BASE 0x8200040
#define TIMER_1S_COUNTER_SIZE 32
#define TIMER_1S_FIXED_PERIOD 1
#define TIMER_1S_FREQ 100000000
#define TIMER_1S_IRQ 1
#define TIMER_1S_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_1S_LOAD_VALUE 99999999
#define TIMER_1S_MULT 1.0
#define TIMER_1S_NAME "/dev/timer_1s"
#define TIMER_1S_PERIOD 1
#define TIMER_1S_PERIOD_UNITS "s"
#define TIMER_1S_RESET_OUTPUT 0
#define TIMER_1S_SNAPSHOT 1
#define TIMER_1S_SPAN 32
#define TIMER_1S_TICKS_PER_SEC 1
#define TIMER_1S_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_1S_TYPE "altera_avalon_timer"


/*
 * timer_scrollX configuration
 *
 */

#define ALT_MODULE_CLASS_timer_scrollX altera_avalon_timer
#define TIMER_SCROLLX_ALWAYS_RUN 0
#define TIMER_SCROLLX_BASE 0x8200020
#define TIMER_SCROLLX_COUNTER_SIZE 32
#define TIMER_SCROLLX_FIXED_PERIOD 0
#define TIMER_SCROLLX_FREQ 100000000
#define TIMER_SCROLLX_IRQ 4
#define TIMER_SCROLLX_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_SCROLLX_LOAD_VALUE 99999
#define TIMER_SCROLLX_MULT 0.001
#define TIMER_SCROLLX_NAME "/dev/timer_scrollX"
#define TIMER_SCROLLX_PERIOD 1
#define TIMER_SCROLLX_PERIOD_UNITS "ms"
#define TIMER_SCROLLX_RESET_OUTPUT 0
#define TIMER_SCROLLX_SNAPSHOT 1
#define TIMER_SCROLLX_SPAN 32
#define TIMER_SCROLLX_TICKS_PER_SEC 1000
#define TIMER_SCROLLX_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_SCROLLX_TYPE "altera_avalon_timer"


/*
 * timer_touch configuration
 *
 */

#define ALT_MODULE_CLASS_timer_touch altera_avalon_timer
#define TIMER_TOUCH_ALWAYS_RUN 0
#define TIMER_TOUCH_BASE 0x8200000
#define TIMER_TOUCH_COUNTER_SIZE 32
#define TIMER_TOUCH_FIXED_PERIOD 0
#define TIMER_TOUCH_FREQ 100000000
#define TIMER_TOUCH_IRQ 3
#define TIMER_TOUCH_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_TOUCH_LOAD_VALUE 99999
#define TIMER_TOUCH_MULT 0.001
#define TIMER_TOUCH_NAME "/dev/timer_touch"
#define TIMER_TOUCH_PERIOD 1
#define TIMER_TOUCH_PERIOD_UNITS "ms"
#define TIMER_TOUCH_RESET_OUTPUT 0
#define TIMER_TOUCH_SNAPSHOT 1
#define TIMER_TOUCH_SPAN 32
#define TIMER_TOUCH_TICKS_PER_SEC 1000
#define TIMER_TOUCH_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TOUCH_TYPE "altera_avalon_timer"


/*
 * touch_ctrl configuration
 *
 */

#define ALT_MODULE_CLASS_touch_ctrl altera_avalon_pio
#define TOUCH_CTRL_BASE 0x8200060
#define TOUCH_CTRL_BIT_CLEARING_EDGE_REGISTER 0
#define TOUCH_CTRL_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TOUCH_CTRL_CAPTURE 0
#define TOUCH_CTRL_DATA_WIDTH 3
#define TOUCH_CTRL_DO_TEST_BENCH_WIRING 0
#define TOUCH_CTRL_DRIVEN_SIM_VALUE 0
#define TOUCH_CTRL_EDGE_TYPE "NONE"
#define TOUCH_CTRL_FREQ 100000000
#define TOUCH_CTRL_HAS_IN 0
#define TOUCH_CTRL_HAS_OUT 1
#define TOUCH_CTRL_HAS_TRI 0
#define TOUCH_CTRL_IRQ -1
#define TOUCH_CTRL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TOUCH_CTRL_IRQ_TYPE "NONE"
#define TOUCH_CTRL_NAME "/dev/touch_ctrl"
#define TOUCH_CTRL_RESET_VALUE 0
#define TOUCH_CTRL_SPAN 16
#define TOUCH_CTRL_TYPE "altera_avalon_pio"


/*
 * touch_msg configuration
 *
 */

#define ALT_MODULE_CLASS_touch_msg altera_avalon_pio
#define TOUCH_MSG_BASE 0x8201410
#define TOUCH_MSG_BIT_CLEARING_EDGE_REGISTER 0
#define TOUCH_MSG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TOUCH_MSG_CAPTURE 0
#define TOUCH_MSG_DATA_WIDTH 2
#define TOUCH_MSG_DO_TEST_BENCH_WIRING 0
#define TOUCH_MSG_DRIVEN_SIM_VALUE 0
#define TOUCH_MSG_EDGE_TYPE "NONE"
#define TOUCH_MSG_FREQ 100000000
#define TOUCH_MSG_HAS_IN 1
#define TOUCH_MSG_HAS_OUT 0
#define TOUCH_MSG_HAS_TRI 0
#define TOUCH_MSG_IRQ -1
#define TOUCH_MSG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TOUCH_MSG_IRQ_TYPE "NONE"
#define TOUCH_MSG_NAME "/dev/touch_msg"
#define TOUCH_MSG_RESET_VALUE 0
#define TOUCH_MSG_SPAN 16
#define TOUCH_MSG_TYPE "altera_avalon_pio"


/*
 * touch_pen_intr configuration
 *
 */

#define ALT_MODULE_CLASS_touch_pen_intr altera_avalon_pio
#define TOUCH_PEN_INTR_BASE 0x8201440
#define TOUCH_PEN_INTR_BIT_CLEARING_EDGE_REGISTER 0
#define TOUCH_PEN_INTR_BIT_MODIFYING_OUTPUT_REGISTER 0
#define TOUCH_PEN_INTR_CAPTURE 1
#define TOUCH_PEN_INTR_DATA_WIDTH 1
#define TOUCH_PEN_INTR_DO_TEST_BENCH_WIRING 0
#define TOUCH_PEN_INTR_DRIVEN_SIM_VALUE 0
#define TOUCH_PEN_INTR_EDGE_TYPE "FALLING"
#define TOUCH_PEN_INTR_FREQ 100000000
#define TOUCH_PEN_INTR_HAS_IN 1
#define TOUCH_PEN_INTR_HAS_OUT 0
#define TOUCH_PEN_INTR_HAS_TRI 0
#define TOUCH_PEN_INTR_IRQ 2
#define TOUCH_PEN_INTR_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TOUCH_PEN_INTR_IRQ_TYPE "EDGE"
#define TOUCH_PEN_INTR_NAME "/dev/touch_pen_intr"
#define TOUCH_PEN_INTR_RESET_VALUE 0
#define TOUCH_PEN_INTR_SPAN 16
#define TOUCH_PEN_INTR_TYPE "altera_avalon_pio"

#endif /* __SYSTEM_H_ */
