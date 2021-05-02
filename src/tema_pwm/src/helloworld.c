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

#define BASE_RED 0x43C00000
#define BASE_GREEN 0x43C10000
#define BASE_BLUE 0x43C20000
#define CK_DIV_OFFSET 0X04
#define CHANNEL a
#define DUTY_FAC_OFFSET 0x08


void set_duty_factor(int BASEADDRESS , int duty_factor);
void set_divider_factor(int BASEADDRESS , int divider_factor);
void glitter(int BASEADDRESS1,int BASEADDRESS2);

void set_duty_factor(int BASEADDRESS , int duty_factor)
{
	*(uint32_t*)(BASEADDRESS+DUTY_FAC_OFFSET) = duty_factor;

}

void set_divider_factor(int BASEADDRESS , int divider_factor)
{
	*(uint32_t*)(BASEADDRESS+CK_DIV_OFFSET) = divider_factor;

}

/*Control the brightness of the RGB */

void glitter(int BASEADDRESS1,int BASEADDRESS2){
	int i;
	for(i=0;i<=255;i++)
				{
					set_duty_factor(BASEADDRESS1,i);
					set_duty_factor(BASEADDRESS2,255-i);
					usleep(5000);
				}
				for(i=255;i>=0;i--)
				{
					set_duty_factor(BASEADDRESS1,255-i);
					set_duty_factor(BASEADDRESS2,i);
					usleep(5000);
				}
}



int main()
{
    init_platform();

	set_divider_factor(BASE_RED,250);
	set_divider_factor(BASE_GREEN,250);
	set_divider_factor(BASE_BLUE,250);
		while(1)
		{

			glitter(BASE_RED,BASE_BLUE);
			glitter(BASE_BLUE,BASE_GREEN);
			glitter(BASE_GREEN,BASE_RED);

		}




	 cleanup_platform();
    return 0;
}

