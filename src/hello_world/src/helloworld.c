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

XGpio LedGpio;
XGpio BtnSwGpio;

uint32_t BTN_CHANN = 1;
uint32_t SW_CHANN = 2;

uint32_t SW_MASK = 0x0000000Fu;
uint32_t BTN_MASK = 0x0000000Fu;

int prevPressed;
int inverseFunction;

int main()
{
    init_platform();
    // Holds initialisation status
    int Status;
    // Holds value returned from the switches register
    uint32_t switches;
    // Holds value returned from the buttons register
    uint32_t buttons;

    Status = XGpio_Initialize(&LedGpio, XPAR_LED_GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	Status = XGpio_Initialize(&BtnSwGpio, XPAR_BTN_SW_GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("BtnSwGpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

    print("Hello World\n\r");
	print("Successfully started LEDs and switches application");


    while (1) {
		switches = XGpio_DiscreteRead(&BtnSwGpio, SW_CHANN) & SW_MASK;
		buttons = XGpio_DiscreteRead(&BtnSwGpio, BTN_CHANN) & BTN_MASK;

		if (buttons) {
			if (!prevPressed) {
				inverseFunction = !inverseFunction;
			}
			prevPressed = 1;
		} else {
			prevPressed = 0;
		}

		if (inverseFunction) {
			XGpio_DiscreteWrite(&LedGpio, 1, ~(switches | 0x0u));
		} else {
			XGpio_DiscreteWrite(&LedGpio, 1, switches | 0x0u);
		}
    }

    cleanup_platform();
    return 0;
}
