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
#include "xuartps_hw.h"
#include "xuartps.h"

XGpio LedGpio,SwBtnGpio; /* The instance for GPIO LED*/

#define Duty_Cycle_Offset 8
#define Clock_Divider_Offset 4
#define RED_LED_PWM_BASEADDR XPAR_AXI_PWM_GENERATOR_0_S00_AXI_BASEADDR
#define BLUE_LED_PWM_BASEADDR XPAR_AXI_PWM_GENERATOR_1_S00_AXI_BASEADDR
#define GREEN_LED_PWM_BASEADDR XPAR_AXI_PWM_GENERATOR_2_S00_AXI_BASEADDR

void set_duty(u32 dutyF, uint32_t BaseAddr )
{

	uint32_t *Adress=BaseAddr+Duty_Cycle_Offset;

	*Adress = dutyF;

}
void set_ncfkpwm (u32 pwm,uint32_t BaseAddr)
{

	uint32_t *Adress=BaseAddr+Clock_Divider_Offset;

		*Adress = pwm;
}

void Set_Two_Colors_RGB_LED(uint32_t BaseAddr1, uint32_t BaseAddr2, int ClockDiv)
{
	 	set_ncfkpwm(ClockDiv,BaseAddr1);
	    set_ncfkpwm(ClockDiv,BaseAddr2);
	    int delay;

	    for(int i =0; i<256; i++)
	    {
	    	delay=0;
	       while(delay < 40000)
	    	{set_duty(255-i,BaseAddr1);
	    	set_duty(i,BaseAddr2);
	    	delay++;}

	    }
}

int main()
{
    init_platform();
    XStatus Status, StatusSwBtn;
   // print("Hello World\n\r");
  //  print("Successfully ran Hello World application");
   // *(uint32_t*)0x41200000 = 0x0u;  //putem aprinde ledurile asa
  //  *(uint32_t*)(0x41200000) = 0xFu;




  // ------------------- Led that turn on or off if we press the buttons -------//
  //--------------- and if we turn on a switch they will invert their behavior---//
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

    //XGpio_SetDataDirection(&SwBtnGpio, 1, 0xFF); //setting switches to inputs
  //  XGpio_SetDataDirection(&SwBtnGpio, 2, 0xFF); //setting buttons to inputs

    u32 switchesdata;
    u32 btnsdata;

   /* while (1)
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

    }// end of while(1)*/

    //--------------------------end of Led_btn_switches app--------------------------------//



   //------------------- Pwm application, where dutycycle and nfckpwm are set using functions------//

    //XStatus StatusPwm;
       u32 data_duty;
       u8 data_duty_Dec1;
       u8 data_duty_Dec2;
       u8 data_duty_Dec3;

       u32 data_ncfkpwm;
       u8 data_ncfkpwm_Dec1;
       u8 data_ncfkpwm_Dec2;
       u8 data_ncfkpwm_Dec3;


       int k=1,ok;
    while( k == 1)
       {
    do {ok=1;
    	printf("\rInput a value between 1 and 255 for the duty factor: \n\r");
    	data_duty_Dec1 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
    	data_duty_Dec1-=48;
    	printf("First dec %d \n",data_duty_Dec1);
    	data_duty_Dec2 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
    	data_duty_Dec2-=48;
    	printf("Second dec %d \n",data_duty_Dec2);
    	data_duty_Dec3 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
    	data_duty_Dec3-=48;
    	printf("Third dec %d \n",data_duty_Dec3);


    	data_duty = data_duty_Dec1*100 + data_duty_Dec2*10 + data_duty_Dec3;

    	if(data_duty > 255 )
    	{
    		printf("Error number is too big. ");
    		ok =0;
    	}
    	printf("Data duty is %ld \n\r", data_duty);

    }while( ok == 0);

    	//scanf("%ld",&data_duty);
       set_duty(data_duty,RED_LED_PWM_BASEADDR);


      do{
    	  ok =1;
    	  print("Input a value between 1 and 255 for the clock divider: \n\r");
       data_ncfkpwm_Dec1 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
       data_ncfkpwm_Dec1-=48;
       printf("First dec %d \n",data_ncfkpwm_Dec1);
       data_ncfkpwm_Dec2 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
       data_ncfkpwm_Dec2-=48;
       printf("Second dec %d \n",data_ncfkpwm_Dec2);
       data_ncfkpwm_Dec3 = XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
       data_ncfkpwm_Dec3-=48;
       printf("Third dec %d \n",data_ncfkpwm_Dec3);


       data_ncfkpwm = data_ncfkpwm_Dec1* 100 + data_ncfkpwm_Dec2*10 +data_ncfkpwm_Dec3 ;

       if (data_ncfkpwm > 255)
       {
    	   printf("Error number is too big \n ");
    	   ok =0;
       }
       printf("Clock divider is %ld \n\r", data_ncfkpwm);
      }while(ok==0);

     set_ncfkpwm (data_ncfkpwm,RED_LED_PWM_BASEADDR);


      print("Want to input other values ? \n");
      print("y/n\n\r");
      char x;
       x= XUartPs_RecvByte(XPAR_PS7_UART_1_BASEADDR);
      if(x == 'y' || x == 'Y')
    	  k=1;
      else if(x == 'n' || x=='N')
    	   k=0;
      else {
    	  print("INCORRECT INPUT. Exit application \n\r");
    	  k=0;
      }



    } // end of while(k==0)


//--------------------------end of Pwm_fcts app--------------------------------//




  //--------------------------- RGB led that changes color ---------------------------------//

    int ClockDiv = 64;
    while(1)
    {

    	Set_Two_Colors_RGB_LED(RED_LED_PWM_BASEADDR,GREEN_LED_PWM_BASEADDR,ClockDiv);
    	Set_Two_Colors_RGB_LED(GREEN_LED_PWM_BASEADDR,BLUE_LED_PWM_BASEADDR,ClockDiv);
    	Set_Two_Colors_RGB_LED(BLUE_LED_PWM_BASEADDR,RED_LED_PWM_BASEADDR,ClockDiv);

    }








 //------------------------------end of RGB_Color app -------------------------------------//
    cleanup_platform();
    return 0;
}
