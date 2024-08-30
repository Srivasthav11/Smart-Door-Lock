
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#define MAX 4
#define BLINK 1000
#define HIGH 2000

#define LED_ON 1u
#define LED_OFF 0u

#define MAX_TRIES 1

const char* PASS = "0406";
unsigned int access_count = 0;
char* received_pass;

void LED_Controller(int dynamic_state)
{
    if(dynamic_state == HIGH)
    {
        LED_Write(LED_ON);
    }
    if(dynamic_state == BLINK)
    {
        while(1)
        {   
            LED_Write(LED_ON);
            CyDelay(500);
            LED_Write(LED_OFF);
        }
    }
}


int process_pass()
{
    if(++access_count > MAX_TRIES)
    {
        return -1;
    }
    else
    {
        for (int i = 0; i < MAX; ++i) {
            if (received_pass[i] != PASS[i]) {
                return 0;
            }
        }
        return 1;
    }
}

void display_state(int state)
{
        LCD_Position(0,0);
        
        if(state == -1)
        {
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("ACCESS DENIED");
            LED_Controller(BLINK);
        }
        
        if(state == 0)
        {
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("INVALID TRY");
        }
        if(state == 1)
        {
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("ACCESS GRANTED");
            LED_Controller(HIGH);
        }
}

int main(void)
{
    CyGlobalIntEnable;
    USBUART_Start(0, USBUART_5V_OPERATION);
    LCD_Start();  


    for(;;)
    {
        if (0u != USBUART_IsConfigurationChanged())
        {
            if (0u != USBUART_GetConfiguration())
            {
                USBUART_CDC_Init();
            }
        }

        if (0u != USBUART_GetConfiguration())
        {
            if (0u != USBUART_DataIsReady())
            {
                   char count = USBUART_GetChar();
                if (0u != count)
                {}
                LCD_Position(0,0);
                LCD_PrintNumber(count);
                
                for(int i = 0; i < MAX; i++)
                {
                    received_pass[i] = count;
                }
                received_pass[MAX] = '\0';
                
                int state = process_pass();
                
                display_state(state);
            }
        }
    }
}
