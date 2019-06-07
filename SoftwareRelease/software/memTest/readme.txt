/**************************************************************************
 * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 *************************************************************************/
/**************************************************************************
 *
 * 
 * Description
 *************** 
 * This is a test program which tests RAM and flash memory. 
 *
 * 
 * Requirements
 ****************
 * This is a "Hosted" application. According to the ANSI C standard, hosted 
 * applications can rely on numerous system-services (including properly-
 * initialized device drivers and, in this case, STDOUT).  
 * 
 * When this program is compiled, code is added before main(), so that all 
 * devices are properly-initialized and all system-services (e.g. the <stdio>
 * library) are ready-to-use. In this hosted environment, all standard C 
 * programs will run.
 * 
 * A hosted application (like this example) does not need to concern itself 
 * with initializing devices. As long as it only calls C Standard Library 
 * functions, a hosted application can run "as if on a workstation."
 * 
 * An application runs in a hosted environment if it declares the function 
 * main(), which this application does.
 * 
 * This software example requires a STDOUT component such as a UART or 
 * JTAG UART, a CFI flash component, and 2 RAM components (one for running
 * the program, and one for testing)
 *
 * Note: This example will not run on the Nios II Instruction Set Simulator
 * 
 * Peripherals Exercised by SW
 *******************************
 * The example's purpose is to test RAM and flash, as well as demonstrate the 
 * use of the DMA controller and flash API in NiosII.
 * 
 * The RAM test routine performs the following operations:
 * 1.) Tests the address and data lines for shorts and opens. 
 * 2.) Tests byte and half-word access.
 * 3.) Tests every bit in the memory to store both '1' and '0'. 
 * 4.) Tests DMA access to the memory.
 *
 * IMPORTANT: The RAM test is destructive to the contents of the RAM.  For this
 * reason, you MUST assure that none of the software sections are located in 
 * the RAM being tested.  This requires that code, data, and exception 
 * locations must all be in a memory seperate from the one being tested.
 * These locations can be adjusted in Nios II Software Build Tools and SOPC Builder.
 *
 *  
 * The flash tests demonstrate the use of the flash programming API.  After the
 * flash device specified is opened, the test routine searches for a block in 
 * the device that is already erased.  This prevents any overwriting of 
 * important data that may be programmed in flash.  When an erased block is 
 * found, the routine performs a test of the flash API calls on that block.
 *
 * The following API functions are then run to test the flash interface:
 * 
 * - alt_get_flash_info
 *    This function queries the flash device and collects various information 
 *    about it.  In the example, the results of this query are compared to what
 *    is expected, and an error is reported in the event of a mismatch.
 * - alt_write_flash
 *    This function writes a specified number of bytes to the flash device.  
 *    In the example, this function is called repeatedly in a loop to write a 
 *    lengthy amount of data.
 * - alt_read_flash
 *    This function reads a specified number of bytes of data from the flash 
 *    device.  In the example, alt_read_flash is used to read back and test 
 *    all of the writing routines.
 * - alt_erase_flash_block
 *    This function performs a block erase on the flash device. 
 * - alt_write_flash_block
 *    This function writes an erase block of data to the flash device.  
 * 
 * During the test, status and error information is passed to the user via 
 * printf's.
 * 
 * Software Files
 ******************
 * memtest.c - Main C file that contains all memory testing code in this 
 *             example.
 * 
 **************************************************************************/
