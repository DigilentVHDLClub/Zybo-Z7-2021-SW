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
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"


int main()
{
    init_platform();


    int sw_data=0,btn_data=0,flag=0,btn_old=0,btn_new;
    XGpio LedGPIO,Switch,Button; /*instance of GPIO driver */

    XGpio_Initialize(&LedGPIO, XPAR_LED_GPIO_1_DEVICE_ID); /* Initialising of LED */
	XGpio_Initialize(&Switch,XPAR_BTN_GPIO_0_DEVICE_ID); /* Initialising of switches */
	XGpio_Initialize(&Button,XPAR_BTN_GPIO_0_DEVICE_ID); /* Initialising of Button */

	while (1){


				sw_data= XGpio_DiscreteRead(&Switch, 2); //Read data from Switch
				btn_data= XGpio_DiscreteRead(&Button, 1);// Read data from button
				btn_data= XGpio_DiscreteRead(&Switch, 1);// Read data from button
				btn_new=btn_data;

				if (btn_old==0b0000 && btn_new!=0b0000) //Check if there is any transition
				{
					flag=~flag; // If any button is pressed we change the value of the flag from 0=>1 or 1=>0
				}
				btn_old=btn_new;

				if (flag!=0)
				XGpio_DiscreteWrite(&LedGPIO,1,~(sw_data));
				else
				XGpio_DiscreteWrite(&LedGPIO,1,sw_data);

				//usleep(500000);
	}
	 cleanup_platform();
    return 0;
}

