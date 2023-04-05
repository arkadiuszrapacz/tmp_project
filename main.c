/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - projekt
					Gra Rzut Kością do gry
					autor: Arkadiusz Rapacz
					20.12.2022
----------------------------------------------------------------------------*/
	
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "DAC.h"
#include "tsi.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "klaw.h"

volatile uint8_t S1_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
volatile uint8_t S2_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
volatile uint8_t S3_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
volatile uint8_t S4_press=0;	// "1" - klawisz zostal wcisniety "0" - klawisz "skonsumowany"
int S2_press_limit=0;					// limit wciśnięć klawiszi odpowiadającym rzutom kostką
int S3_press_limit=0;					// limit wciśnięć klawiszi odpowiadającym rzutom kostką
int S4_press_limit=0;					// limit wciśnięć klawiszi odpowiadającym rzutom kostką

void PORTA_IRQHandler(void)	// Podprogram obslugi przerwania od klawiszy S1, S2, S3 i S4
{
	uint32_t buf;
	buf=PORTA->ISFR & (S1_MASK | S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{
		case S1_MASK: DELAY(10)
									if(!(PTA->PDIR&S1_MASK))
									{
										if(!(PTA->PDIR&S1_MASK))
										{
											if(!S1_press)
											{
												S1_press=1;
											}
										}
									}
									break;
		case S2_MASK:	DELAY(10)
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S2_press)
											{
												S2_press=1;
												S2_press_limit++;			// inkrementacja liczby wciśnięć przycisku S2
												
											}
										}
									}
									break;
		case S3_MASK:	DELAY(10)
									if(!(PTA->PDIR&S3_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S3_press)
											{
												S3_press=1;
												S3_press_limit++;			// inkrementacja liczby wciśnięć przycisku S3
											}
										}
									}
									break;
		case S4_MASK:	DELAY(10)
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgan zestyków
									{
										if(!(PTA->PDIR&S4_MASK))	// Minimalizacja drgan zestyków (c.d.)
										{
											if(!S4_press)
											{
												S4_press=1;
												S4_press_limit++;			// inkrementacja liczby wciśnięć przycisku S4
											}
										}
									}
									break;
		default:			break;
	}	
	PORTA->ISFR |=  S1_MASK | S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}


int main(void)
{
	uint8_t w=0;
	int rzut_1;
	int rzut_2;
	int rzut_3;
	char display[] = {0x20,0x20,0x20,0x20,0x20, 0x20,0x20, 0x20,0x20, 0x20,0x20, 0x20,0x20, 0x20,0x20, 0x20};
	bool game_over = false;											// zmienna typu bool kontrolująca koniec działania programu
	uint8_t reszta;
	
	Klaw_Init();
	Klaw_S2_4_Int();
	TSI_Init();
	LCD1602_Init();
	LCD1602_Backlight(TRUE);
	
	LCD1602_ClearAll();
	LCD1602_SetCursor(2,0);
	LCD1602_Print("Witam w grze");
	LCD1602_SetCursor(2,1);
	LCD1602_Print("rzut koscia!");
	
	while(!game_over)
	{
		if(S2_press)
		{
			if (S2_press_limit <= 3)
			{
				S2_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(3,0);
				LCD1602_Print("Rzut nr 1!");
				LCD1602_SetCursor(0,1);
				LCD1602_Print("dotknij slidera!");
				while(0 == (w=TSI_ReadSlider()));
				LCD1602_SetCursor(0,0);
				if(w!=0)
				{
					LCD1602_ClearAll();
					sprintf(display,"Wynik rzutu = %d", rzut_1 = w%6+1);
					LCD1602_Print(display);
				}
			}
			else
			{
				S2_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(2,0);
				LCD1602_Print("Wyczerpales");
				LCD1602_SetCursor(1,1);
				LCD1602_Print("proby rzutu 1!");
			}
		}
		if(S3_press)
		{
			if (S3_press_limit <= 3)
			{
				S3_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(3,0);
				LCD1602_Print("Rzut nr 2!");
				LCD1602_SetCursor(0,1);
				LCD1602_Print("dotknij slidera");
				while(0 == (w=TSI_ReadSlider()));
				LCD1602_SetCursor(0,0);
				if(w!=0)
				{
					LCD1602_ClearAll();
					sprintf(display,"Wynik rzutu = %d", rzut_2 = w%6+1);
					LCD1602_Print(display);
				}
			}
			else
			{
				S3_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(2,0);
				LCD1602_Print("Wyczerpales");
				LCD1602_SetCursor(1,1);
				LCD1602_Print("proby rzutu 2!");
			}
		}
		if(S4_press)
		{
			if (S4_press_limit <= 3)
			{
				S4_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(3,0);
				LCD1602_Print("Rzut nr 3!");
				LCD1602_SetCursor(0,1);
				LCD1602_Print("dotknij slidera");
				while(0 == (w=TSI_ReadSlider()));
				LCD1602_SetCursor(0,0);
				if(w!=0)
				{
					LCD1602_ClearAll();
					sprintf(display,"Wynik rzutu = %d", rzut_3 = w%6+1);
					LCD1602_Print(display);
				}
			}
			else
			{
				S4_press = 0;
				LCD1602_ClearAll();
				LCD1602_SetCursor(2,0);
				LCD1602_Print("Wyczerpales");
				LCD1602_SetCursor(1,1);
				LCD1602_Print("proby rzutu 3!");
			}
		}
		if(!(PTA->PDIR&S1_MASK))
		{
			S1_press = 0;
			reszta = 13 - (rzut_1 + rzut_2 + rzut_3);
			if (S2_press_limit >= 3 && S3_press_limit >= 3 && S4_press_limit >= 3)
			{
				LCD1602_ClearAll();
				LCD1602_Print("Przegrales! :(");
				LCD1602_SetCursor(0,1);
				LCD1602_Print("Koniec rzutow");
				game_over = true;
			}
			else
			{
				if (reszta == 0)
				{
					LCD1602_ClearAll();
					LCD1602_SetCursor(1,0);
					LCD1602_Print("Gratulacje! :)");
					LCD1602_SetCursor(4,1);
					LCD1602_Print("Wygrales");
					game_over = true;
				}
				else
				{
					LCD1602_ClearAll();
					LCD1602_Print("Rzut");
					LCD1602_SetCursor(7,0);
					sprintf(display, "%d", rzut_1);
					LCD1602_Print(display);
					LCD1602_SetCursor(11,0);
					sprintf(display, "%d", rzut_2);
					LCD1602_Print(display);
					LCD1602_SetCursor(15,0);
					sprintf(display, "%d", rzut_3);
					LCD1602_Print(display);
				}
			}
		}
	}
}