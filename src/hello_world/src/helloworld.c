/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
<<<<<<< HEAD
#include "xparameters.h" //address definition header
#include "xgpio.h"
#include "xil_printf.h"

XGpio LedGpio;
XGpio Btn_Sw_Gpio;
int main()
{
	u32 btn_data, sw_data;
=======
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

XGpio LedGpio; /* The Instance of the GPIO Driver */

int main()
{
>>>>>>> elodg/main
	XStatus Status;
    init_platform();

    print("Hello World\n\r");
<<<<<<< HEAD
    print("Successfully ran Hello World application\n");


//    *(uint32_t*)0x41200000=0x0u; // turn ON the LED
//    *(uint32_t*)0x41200000=0xFu; //turn OFF the LED

    /* Initialize the LED'S GPIO driver */
    	Status = XGpio_Initialize(&LedGpio, XPAR_LED_GPIO_DEVICE_ID);
    	if (Status != XST_SUCCESS) {
    		xil_printf("Gpio Initialization Failed\r\n");
    		return XST_FAILURE;
    	}

    	/* Initialize the BUTTONS and SWITCHES GPIO driver */
    	Status = XGpio_Initialize(&Btn_Sw_Gpio, XPAR_BTN_SW_GPIO_DEVICE_ID);
    	if (Status != XST_SUCCESS) {
    		xil_printf("Gpio Initialization Failed\r\n");
    		return XST_FAILURE;
    	}

    	while(1){

    		//2 CHANNELS BTN_SW_GPIO
    		btn_data = XGpio_DiscreteRead(&Btn_Sw_Gpio, 1); //reading data from buttons
    		sw_data = XGpio_DiscreteRead(&Btn_Sw_Gpio, 2); //reading data from switches

    		if(btn_data){ //if button is pressed
    			 XGpio_DiscreteWrite(&LedGpio, 1, ~sw_data); //one CHANNEL LED_GPIO
    		}else{
    			 XGpio_DiscreteWrite(&LedGpio, 1, sw_data); //write switches state
    		}
    	}

    	/* Set the LED to High */
       //XGpio_DiscreteWrite(&LedGpio, 1, 0x0F);


=======
    print("Successfully ran Hello World application");

//    *(uint32_t*)0x41200000 = 0x0u;
//    *(uint32_t*)(0x41200000) = 0xFu;

	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&LedGpio, XPAR_LED_GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set the LED to High */
	XGpio_DiscreteWrite(&LedGpio, 1, 0xFF);
>>>>>>> elodg/main

    cleanup_platform();
    return 0;
}
