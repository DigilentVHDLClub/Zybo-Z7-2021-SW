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

XGpio LedGpio,SwBtnGpio; /* The instance for GPIO LED*/

int main()
{
    init_platform();
    XStatus Status, StatusSwBtn;
    print("Hello World\n\r");
    print("Successfully ran Hello World application");
   // *(uint32_t*)0x41200000 = 0x0u;  //putem aprinde ledurile asa
  //  *(uint32_t*)(0x41200000) = 0xFu;

    Status = XGpio_Initialize(&LedGpio,XPAR_LED_GPIO_DEVICE_ID);
    if(Status != XST_SUCCESS)
    {
    	xil_printf("GPIO initialization failed\r\n");
    	return XST_FAILURE;
    }

    StatusSwBtn = XGpio_Initialize(&SwBtnGpio,XPAR_BTN_SW_GPIO_DEVICE_ID);
    if(StatusSwBtn != XST_SUCCESS)
       {
       	xil_printf("GPIO initialization failed\r\n");
       	return XST_FAILURE;
       }

    XGpio_SetDataDirection(&SwBtnGpio, 1, 0x00); //setting switches to inputs
    XGpio_SetDataDirection(&SwBtnGpio, 2, 0x00); //setting buttons to inputs

    u32 switchesdata;
    u32 btnsdata;

    while (1)
    {
    switchesdata= XGpio_DiscreteRead(&SwBtnGpio,1);  //reads data from the switches

    btnsdata= XGpio_DiscreteRead(&SwBtnGpio,2); //reads data from the buttons

   // printf("\n\r %lx", btnsdata);


    if(btnsdata == 0 ){     //check if buttons are pressed
    	XGpio_DiscreteWrite(&LedGpio,1,switchesdata); // if not the switches work normally
    }
    else {  //if one button was pressed
    	switchesdata = ~switchesdata; // the switches will invert their function would be inverted
    	XGpio_DiscreteWrite(&LedGpio,1,switchesdata); //write the data to the leds
    }



    }


    cleanup_platform();
    return 0;
}
