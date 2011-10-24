/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this 
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in 
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting 
  documentation, and that the name of the author not be used in 
  advertising or publicity pertaining to distribution of the 
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the Arduino-usbserial project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "sparkfun_USBtoSerial.h"

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config = 
			{
				.ControlInterfaceNumber         = 0,

				.DataINEndpointNumber           = CDC_TX_EPNUM,
				.DataINEndpointSize             = CDC_TXRX_EPSIZE,
				.DataINEndpointDoubleBank       = false,

				.DataOUTEndpointNumber          = CDC_RX_EPNUM,
				.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
				.DataOUTEndpointDoubleBank      = false,

				.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
				.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
				.NotificationEndpointDoubleBank = false,
			},
	};

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	
	sei();

	for (;;)
	{
		int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		if (!(ReceivedByte < 0)) {
			switch (ReceivedByte) {
				case '0':
					cbi(PORTB, 5);
					cbi(PORTB, 6);
					cbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '0');
				break;
				case '1':
				case 'g':
				case 'G':
					cbi(PORTB, 5);
					cbi(PORTB, 6);
					sbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '1');
				break;
				case '2':
				case 'o':
				case 'O':
					cbi(PORTB, 5);
					sbi(PORTB, 6);
					cbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '2');
				break;
				case '3':
					cbi(PORTB, 5);
					sbi(PORTB, 6);
					sbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '3');
				break;
				case '4':
				case 'r':
				case 'R':
				case 's':
				case 'S':
					sbi(PORTB, 5);
					cbi(PORTB, 6);
					cbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '4');
				break;
				case '5':
					sbi(PORTB, 5);
					cbi(PORTB, 6);
					sbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '5');
				break;
				case '6':
					sbi(PORTB, 5);
					sbi(PORTB, 6);
					cbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '6');
				break;
				case '7':
				case 'a':
				case 'A':
					sbi(PORTB, 5);
					sbi(PORTB, 6);
					sbi(PORTB, 7);
					CDC_Device_SendByte(&VirtualSerial_CDC_Interface, '7');
				break;
			}
		}

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Hardware Initialization */
	USB_Init();

	/* Pull target /RESET line high */
	AVR_RESET_LINE_PORT |= AVR_RESET_LINE_MASK;
	AVR_RESET_LINE_DDR  |= AVR_RESET_LINE_MASK;
	
	//turn on status LED
	DDRB = 0xFF;
	cbi(PORTB, 4);

	// turn on all traffic light segments
	cbi(PORTB, 5);
	cbi(PORTB, 6);
	cbi(PORTB, 7);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Unhandled Control Request event. */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
}

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART1_RX_vect, ISR_BLOCK)
{
}

/** Event handler for the CDC Class driver Host-to-Device Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
}
