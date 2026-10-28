#include "MCAL/DIO/DIO_interface.h"
#include "HAL/LED/LED_interface.h"
#include "HAL/SEVSEG/SSD_interface.h"
#include "HAL/IR/IR_interface.h"
#include "HAL/DcMotor/DcMotor_Interface.h"
#include "avr/delay.h"

int main(void)
{
	u8 counter = 0;

	u8 SeeObj;
	u8 CheckSeeObj;
	u8 ExitSeeObj;
	u8 HeightSeeObj;
	
	IR_Init(DIO_u8PORTA, DIO_u8PIN1);
	IR_Init(DIO_u8PORTA, DIO_u8PIN5);
	IR_Init(DIO_u8PORTA, DIO_u8PIN6);
	IR_Init(DIO_u8PORTA, DIO_u8PIN7);
	
	DIO_u8SetPinDirection(DIO_u8PORTA, DIO_u8PIN0, DIO_u8PORT_OUTPUT);
	
	DCMOTOR_Init(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
	SEVSEG_Init(DIO_u8PORTC);

	while (1)
	{
		SEVSEG_DisplayNumber(DIO_u8PORTC, counter, SEVSEG_u8COMMON_CATHODE, DIO_u8PIN4, DIO_u8PORTA);
		
		if (counter == 4) {
			LED_u8TurnOn(DIO_u8PORTA, DIO_u8PIN0);
		}
		else if (counter <= 4 && counter > 0) {
			LED_u8TurnOff(DIO_u8PORTA, DIO_u8PIN0);
			}
		

		IR_GetState(DIO_u8PORTA, DIO_u8PIN1, &SeeObj);
		IR_GetState(DIO_u8PORTA, DIO_u8PIN5, &CheckSeeObj);
		IR_GetState(DIO_u8PORTA, DIO_u8PIN6, &ExitSeeObj);
		IR_GetState(DIO_u8PORTA, DIO_u8PIN7, &HeightSeeObj);
		
		if (SeeObj == IR_DETECTED && CheckSeeObj == IR_DETECTED)
		{
			DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
		}
		
		else if (HeightSeeObj == IR_DETECTED) {
			DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
		}
		
		else if (SeeObj != IR_DETECTED && CheckSeeObj == IR_DETECTED && counter < 4 && HeightSeeObj != IR_DETECTED)
		{
			DCMOTOR_RotateDirection2(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(2000);
			DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
			
			u8 WaitingObj = 1;
			while (WaitingObj == 1)
			{
				IR_GetState(DIO_u8PORTA, DIO_u8PIN6, &ExitSeeObj);
				
				if (ExitSeeObj == IR_DETECTED) {
					WaitingObj = 0;
					counter++;
					
					while (ExitSeeObj == IR_DETECTED) {
						IR_GetState(DIO_u8PORTA, DIO_u8PIN6, &ExitSeeObj);
					}
					
					DCMOTOR_RotateDirection1(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
					_delay_ms(2000);
					DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
				}
			}
		}
		
		else if (ExitSeeObj == IR_DETECTED && counter > 0 && counter <= 4)
		{
			DCMOTOR_RotateDirection2(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(2000);
			DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
			
			u8 WaitObj2 = 1;
			while (WaitObj2 == 1)
			{
				IR_GetState(DIO_u8PORTA, DIO_u8PIN5, &CheckSeeObj);
				
				if (CheckSeeObj == IR_DETECTED) {
					WaitObj2 = 0;
					counter--;  
					
					while (CheckSeeObj == IR_DETECTED) {
						IR_GetState(DIO_u8PORTA, DIO_u8PIN5, &CheckSeeObj);
					}
					
					DCMOTOR_RotateDirection1(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
					_delay_ms(2000);
					DCMOTOR_Stop(DIO_u8PORTA, DIO_u8PIN2, DIO_u8PIN3);
				}
			}
		}
	}
	return 0;
}