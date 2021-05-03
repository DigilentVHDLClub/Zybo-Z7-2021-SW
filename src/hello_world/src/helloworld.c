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
#include <unistd.h>

#define redDutyAddress 0x43C00008
#define greenDutyAddress 0x43C10008
#define blueDutyAddress 0x43C200008
#define redClockDivisionAddress 0x43C00004
#define greenClockDivisionAddress 0x43C10004
#define blueClockDivisionAddress 0x43C200004

XGpio LedGpio; /* The Instance of the GPIO Driver */
XGpio BtnSwGpio; /* The Instance of the GPIO Driver */

void setDutyRed(u32 value){
	*((uint32_t*)redDutyAddress) = value & 0xFF;
}

void setDutyGreen(u32 value){
	*((uint32_t*)greenDutyAddress) = value & 0xFF;
}

void setDutyBlue(u32 value){
	*((uint32_t*)blueDutyAddress) = value & 0xFF;
}

void setClockDivisionRed(u32 value){
	*((uint32_t*)redClockDivisionAddress) = value & 0xFF;
}

void setClockDivisionGreen(u32 value){
	*((uint32_t*)greenClockDivisionAddress) = value & 0xFF;
}

void setClockDivisionBlue(u32 value){
	*((uint32_t*)blueClockDivisionAddress) = value & 0xFF;
}

int main()
{
    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application");

   // XStatus Status;

   // *((uint32_t*)0x41200000) = 0x0u;
   // *((uint32_t*)0x41200000) = 0xFu;

    int Status;

	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(&LedGpio, XPAR_LED_GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	Status = XGpio_Initialize(&BtnSwGpio, XPAR_BTN_SW_GPIO_DEVICE_ID);
		if (Status != XST_SUCCESS) {
			xil_printf("Gpio Initialization Failed\r\n");
			return XST_FAILURE;
		}

	int buttons, switches;
	int red = 1;
	int green = 1;
	int blue = 1;
	setClockDivisionRed(128);
	setClockDivisionGreen(128);
	setClockDivisionBlue(128);

	while(1){
		buttons = XGpio_DiscreteRead(&BtnSwGpio, 1);
		if(!buttons){
			switches = XGpio_DiscreteRead(&BtnSwGpio, 2);
			XGpio_DiscreteWrite(&LedGpio, 1, switches);
		}
		else{
			switches = XGpio_DiscreteRead(&BtnSwGpio, 2);
			XGpio_DiscreteWrite(&LedGpio, 1, ~switches);
		}
		red *= 3;
		green *= 5;
		blue *= 7;
		red = red % 256;
		green = green % 256;
		blue = blue % 256;
		setDutyRed(red);
		setDutyGreen(green);
		setDutyBlue(blue);
		usleep(500000);

	}



    cleanup_platform();
    return 0;
}
