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

#define RED_DIV 0x43C00004
#define RED_DTY 0x43C00008

#define GRN_DIV 0x43C10004
#define GRN_DTY 0x43C10008

#define BLU_DIV 0x43C20004
#define BLU_DTY 0x43C20008

typedef enum {
	RED,
	GRN,
	BLU
} Color;

XGpio LedGpio;
XGpio BtnSwGpio;

float red_cnt = 0;
float blu_cnt = 0;
float grn_cnt = 0;

int prevPressed;
int inverseFunction;

void setDivider(Color color, uint32_t value) {
	switch(color) {
	case RED:
		*(uint32_t*)(RED_DIV) = value;
		break;
	case BLU:
		*(uint32_t*)(GRN_DIV) = value;
		break;
	case GRN:
		*(uint32_t*)(BLU_DIV) = value;
		break;
	}
}

void setDtyFactor(Color color, uint32_t value) {
	switch(color) {
	case RED:
		*(uint32_t*)(RED_DTY) = value;
		break;
	case BLU:
		*(uint32_t*)(GRN_DTY) = value;
		break;
	case GRN:
		*(uint32_t*)(BLU_DTY) = value;
		break;
	}
}

int main()
{
    init_platform();


    // ----------------------- BUTTONS & SWITCHES -----------------------


	// ----------------------- LED PWM -----------------------------

	// Output
	setDivider(RED, 255);
	setDivider(GRN, 255);
	setDivider(BLU, 255);

	// ----------------------- LED PWM - END -----------------------

	print("Hello World\n\r");
	print("Successfully started LEDs and switches application");


    while (1) {

	// ----------------------- LED PWM -----------------------------
	setDtyFactor(RED, (int)(red_cnt) % 255);
	red_cnt = red_cnt + 0.001;
	setDtyFactor(GRN, (int)(grn_cnt) % 255);
	grn_cnt = grn_cnt + 0.0007;
	setDtyFactor(BLU, (int)(blu_cnt) % 255);
	blu_cnt = blu_cnt + 0.0001;

	if (red_cnt > 255) {
		red_cnt = 0;
	} else if (grn_cnt > 255) {
		grn_cnt = 0;
	} else if (blu_cnt > 255) {
		blu_cnt = 0;
	}



	// ----------------------- LED PWM - END -----------------------


    }

    cleanup_platform();
    return 0;
}
