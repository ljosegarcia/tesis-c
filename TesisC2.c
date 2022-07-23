#INCLUDE <16f88.h>
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#USE DELAY(CLOCK=4000000)
#use rs232(uart1,baud=48600) //Requerido para para que B5 y B2 necesitan para enviar
#DEFINE MOTOR PORTA,0
#DEFINE LED_VERDE PORTA,1
#DEFINE LED_AMARILLO PORTA,2
#DEFINE LED_ROJO PORTA,3
#BYTE PORTA= 5

char direccion_r;
int ban= 0;

#int_RDA
void RDA_isr()
{
  direccion_r=getc();
  ban = 1;
}
void main()
{     int estado=0;
      set_tris_a(0B11110000);
      enable_interrupts(INT_RDA);
      enable_interrupts(GLOBAL);
      bit_set(LED_AMARILLO);
      while(true)
      {     if(ban == 1)
            {     if(estado == 0)
                  {     bit_set(MOTOR);
                        bit_set(LED_VERDE);
                        bit_clear(LED_ROJO);
                        bit_clear(LED_AMARILLO);
                        estado = 1;
                        delay_ms(200);
                        ban = 0;
                  }
                  else
                  {     bit_clear(MOTOR);
                        bit_clear(LED_VERDE);
                        bit_set(LED_ROJO);
                        bit_clear(LED_AMARILLO);
                        estado = 0;
                        delay_ms(200);
                        ban = 0;
                  }
            } 
      }
}
