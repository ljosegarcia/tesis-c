#INCLUDE <18f2550.h>
#USE DELAY(CLOCK=4000000)
#INCLUDE <LCD.C>
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#use rs232(uart1,baud=48600) //Requerido para para que C6 y C7 necesitan para enviar
#DEFINE USE_PORTB_LCD TRUE
#BYTE port_a= 0xF80
#BYTE port_c= 0xF82
#BYTE port_b= 0xF81
#byte tris_a = 0xF92 
#byte tris_c = 0xF94 
#BIT F0 = 0xF80.0 //Definición de puertos para el manejo del teclado matricial
#BIT F1 = 0xF80.1 
#BIT F2= 0xF80.2
#BIT F3 = 0xF80.3 
#define C0 bit_test(port_a,4) 
#define C1 bit_test(port_a,5) 
#define C2 bit_test(port_c,0)
#define C3 bit_test(port_c,1) 

int numero, cursor;

void Inicio (void) //Función usada cuando se enciende por primera vez el PIC
{       cursor=0; 
         for (int x = 1; x <= 42 ; x++)
         {  WRITE_EEPROM(cursor, x);
            cursor++;
            for (int y = 0; y < 5; y++)
            {  WRITE_EEPROM(cursor, 0);
               cursor++;
            }   
         }
}

 
void antirebote(void) //Subrutina antirebote.
 {    while(C0 == 1) { } //No realiza nada hasta que el pulsador esté inactivo 
      while(C1 == 1) { }
      while(C2 == 1) { }
      while(C3 == 1) { }
 } 
 
int Barrido (void)
{     set_tris_a(0b11110000); //Configura los los puertos A0 hasta A3 como salida y A4 hasta A5 como entrada
      set_tris_c(0xff);
      F0=0;
      F1=0;
      F2=0;
      F3=0;
      while(TRUE)
      {     F0=1;
            if (C0 == 1)
            {     numero = 1;
                  antirebote();
                  return(numero);
            }
            if (C1 == 1) //Censa si la tecla de la columna 1 ha sido accionada.
            {     numero = 2;
                  antirebote();
                  return(numero);
            }
            if (C2 == 1) //Censa si la tecla de la columna 2 ha sido accionada.
            {     numero = 3;
                  antirebote();
                  return(numero);
            }
            F0=0;
            F1=1;
            if (C0 == 1)
            {     numero = 4;
                  antirebote();
                  return(numero);
            } 
            if (C1 == 1) //Censa si la tecla de la columna 1 ha sido accionada.
            {     numero = 5;
                  antirebote();
                  return(numero);
            }
            if (C2 == 1) //Censa si la tecla de la columna 2 ha sido accionada.
            {     numero = 6;
                  antirebote();
                  return(numero);
            }
            F1=0;
            F2=1;
            if (C0 == 1)
            {     numero = 7;
                  antirebote();
                  return(numero);
            } 
            if (C1 == 1) //Censa si la tecla de la columna 1 ha sido accionada.
            {     numero = 8;
                  antirebote();
                  return(numero);
            }
            if (C2 == 1) //Censa si la tecla de la columna 2 ha sido accionada.
            {     numero = 9;
                  antirebote();
                  return(numero);
            }
            F2=0;
            F3=1;
            if (C0 == 1)
            {     numero = 10;
                  antirebote();
                  return(numero);
            } 
            if (C1 == 1) //Censa si la tecla de la columna 1 ha sido accionada.
            {     numero = 0;
                  antirebote();
                  return(numero);
            }
            if (C2 == 1)
            {     numero = 11;
                  antirebote();
                  return(numero);
            }
            F3=0;
      }
}

int8 clave_cero (int clave_c[]) //Comprobar si la clave es 0000
{     int cero = 0;
      for (int i = 0; i < 4; i++)
      {  if (clave_c[i] == 0)
               cero++;
      }
      if (cero == 4)
           return(1);
      return(0);
}     
void nueva_clave (void)
{     int cursor2 = 0, direccion = 0, clave[4];
      int cero;
      char letra[2];
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese clave:");
      cursor=1;
      for (int x = 0; x < 4; x++)
      {     clave[x] = Barrido();
            if(clave[x] == 10 || clave[x] == 11) //Si el usuario pulsa * o #
                  return;
            LCD_GOTOXY(cursor,2);
            lcd_putc ('*'); 
            cursor++;
      }
      delay_ms(200);
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registra una clave 0000
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Clave invalida"); 
               while(TRUE)
               {   if(Barrido() >= 0)
                   {    LCD_PUTC("\f");
                        return;
                   }
               }
      }
      while (cursor2 < 253)
      {     direccion = READ_EEPROM(cursor2); //Lee la dirección de la memoria
            cero = 0;
            cursor2=cursor2 + 2;
            for (int y = 0; y < 4; y++)
            {  if (READ_EEPROM(cursor2) == 0)
                              cero++;
               cursor2++;
            }
            if (cero == 4)
            {     cursor2 = cursor2 - 4;
                  for (int z = 0; z < 4; z++)
                  {     WRITE_EEPROM (cursor2, clave[z]); //Registra la clave en la memoria
                        cursor2++;
                  }
                  LCD_PUTC("\f");
                  LCD_GOTOXY(1,1);
                  LCD_PUTC("Clave registrada");
                  LCD_GOTOXY(1,2);
                  LCD_PUTC("en:");
                  LCD_GOTOXY(5,2);
                  sprintf(letra, "%d", direccion);
                  lcd_putc (letra[0]);
                  LCD_GOTOXY(6,2);
                  lcd_putc (letra[1]);
                  while(TRUE)
                  {   if(Barrido() >= 0)
                      {     LCD_PUTC("\f"); //Si el usuario pulsa cualquier tecla
                            return;
                      }
                  }
            }
      }
      LCD_PUTC("\f");
      LCD_GOTOXY(1,1);
      LCD_PUTC("Memoria llena"); //Solo muestra este mensaje cuando no hay más espacios para las claves
      while(TRUE)
      {   if(Barrido() >= 0)
          {     LCD_PUTC("\f");
                return;
          }
      }
} 

void cambiar_clave (void)
{     int codigo[2], clave[4], clave2[4], ant, var, valido;
      cursor= 0;
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese codigo:");
      for (int x = 0; x < 2; x++)
      {     codigo[x] = Barrido();
            if(codigo[x] == 10 || codigo[x] == 11)
                  return;
            LCD_GOTOXY((x+1),2);
            char lcd_most = codigo[x] + '0';
            lcd_putc (lcd_most); 
      }
      delay_ms(200);
      if (codigo[0] == 0)
      {     if(codigo[0] == 1)
                 cursor = 0;
            else
                 cursor = (codigo[1] - 1) * 6;
            if(codigo[1] == 0)
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
      }
      if (codigo[0] > 0)
      {     cursor = codigo[1] + (codigo[0] * 10);
            if(cursor > 42) //Solo entra cuando intenta ingresar un codigo mayor a 43
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
            cursor = (cursor - 1) * 6;
      }
      ant = cursor;
      cursor = cursor + 2;
      for (int y = 0; y < 4; y++)
      {     clave[y] = READ_EEPROM(cursor);
            cursor++;
      }
      cursor = ant;
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registrar un codigo no registrado
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Codigo invalido"); 
               while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
      }
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese clave");
      LCD_GOTOXY(1,2);
      LCD_PUTC("vieja:");
      for (int x = 0; x < 4; x++)
      {     clave[x] = Barrido();
            if(clave[x] == 10 || clave[x] == 11)
                  return;
            LCD_GOTOXY((x+8),2);
            lcd_putc ('*'); 
      }
      delay_ms(200);
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registra una clave 0000
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Clave invalida"); 
               while(TRUE)
               {   if(Barrido() >= 0)
                   {    LCD_PUTC("\f");
                        return;
                   }
               }
      }
      valido = 0;
      cursor=cursor+2;
      ant = cursor;
      for (int y = 0; y < 4; y++)
      {  var=READ_EEPROM(cursor);
         if (var == clave[y])
                valido++;
         cursor++;
      }
      if (valido == 4)
      {     LCD_PUTC("\f");   //Borrar el contenido del LCD
            LCD_GOTOXY(1,1);
            LCD_PUTC("Ingrese clave");
            LCD_GOTOXY(1,2);
            LCD_PUTC("nueva:");
            for (int x = 0; x < 4; x++)
            {     clave2[x] = Barrido();
                  if(clave2[x] == 10 || clave2[x] == 11)
                       return;
                  LCD_GOTOXY((x+8),2);
                  lcd_putc ('*'); 
            }
            delay_ms(200);
            if (clave_cero (clave2) == 1) //Solo entra cuando el usuario intenta ingresar una clave 0000
            {        LCD_PUTC("\f");
                     LCD_GOTOXY(1,1);
                     LCD_PUTC("Clave invalida"); 
                     while(TRUE)
                      {   if(Barrido() >= 0)
                               {    LCD_PUTC("\f");
                                    return;
                               }
                      }
            }
            cursor = ant;
            for (int z = 0; z < 4; z++)
            {     WRITE_EEPROM (cursor, clave2[z]);
                        cursor++;
            }
            LCD_PUTC("\f");
            LCD_GOTOXY(1,1);
            LCD_PUTC("Clave cambiada");
            while(TRUE)
            {   if(Barrido() >= 0)
                        {    LCD_PUTC("\f");
                             return;
                        }
            }
      }
      else
      {     LCD_PUTC("\f");
            LCD_GOTOXY(1,1);
            LCD_PUTC("Clave incorrecta");
            while(TRUE)
            {   if(Barrido() >= 0)
                {     LCD_PUTC("\f");
                            return;
                }
            }
     }
} 

void verificar_clave (void)
{     int clave[4], codigo[2], valido, var = 0, direccion = 0, c_estado = 0, estado = 0, ant;
      char direccion_e;
      cursor=0;
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese codigo:");
      for (int x = 0; x < 2; x++)
      {     codigo[x] = Barrido();
            if(codigo[x] == 10 || codigo[x] == 11)
                  return;
            LCD_GOTOXY((x+1),2);
            char lcd_most = codigo[x] + '0';
            lcd_putc (lcd_most); 
      }
      delay_ms(200);
      if (codigo[0] == 0)
      {     if(codigo[0] == 1)
                 cursor = 0;
            else
                 cursor = (codigo[1] - 1) * 6;
            if(codigo[1] == 0)
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
      }
      if (codigo[0] > 0)
      {     cursor = codigo[1] + (codigo[0] * 10);
            if(cursor > 42) //Solo entra cuando intenta ingresar un codigo mayor a 43
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
            cursor = (cursor - 1) * 6;
      }
      ant = cursor;
      cursor = cursor + 2;
      for (int y = 0; y < 4; y++)
      {     clave[y] = READ_EEPROM(cursor);
            cursor++;
      }
      cursor = ant;
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registrar un codigo no registrado
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Codigo invalido"); 
               while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
      }
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese clave:");
      for (int x = 0; x < 4; x++)
      {     clave[x] = Barrido();
            if(clave[x] == 10 || clave[x] == 11)
                  return;
            LCD_GOTOXY((x+1),2);
            lcd_putc ('*'); 
      }
      delay_ms(200);
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registra una clave 0000
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Clave invalida"); 
               while(TRUE)
               {   if(Barrido() >= 0)
                   {    LCD_PUTC("\f");
                        return;
                   }
               }
      }
      while (cursor < 253)
      {     direccion = READ_EEPROM(cursor);
            valido = 0;
            cursor++;
            c_estado = cursor;
            cursor++;
            for (int y = 0; y < 4; y++)
            {  var=READ_EEPROM(cursor);
               if (var == clave[y])
                              valido++;
               cursor++;
            }
            if (valido == 4)
            {     LCD_PUTC("\f");
                  LCD_GOTOXY(1,1);
                  LCD_PUTC("Clave valida");
                  direccion_e = direccion + '0';
                  putc(direccion_e);
                  delay_ms(200);
                  estado = READ_EEPROM(c_estado);
                  if (estado == 0)
                      WRITE_EEPROM(c_estado,1);
                  if (estado == 1)
                      WRITE_EEPROM(c_estado,0);
                  while(TRUE)
                  {   if(Barrido() >= 0)
                      {     LCD_PUTC("\f");
                            return;
                      }
                  }
            }
            else
            {     LCD_PUTC("\f");
                  LCD_GOTOXY(1,1);
                  LCD_PUTC("Clave incorrecta");
                  while(TRUE)
                  {   if(Barrido() >= 0)
                      {     LCD_PUTC("\f");
                            return;
                      }
                  }
            }
      }
}
void verificar_estado (void)
{     int codigo[2], clave[4], estado = 0, direccion = 0;
      char letra[2], letra2;
      cursor = 0;
      LCD_PUTC("\f");   //Borrar el contenido del LCD
      LCD_GOTOXY(1,1);
      LCD_PUTC("Ingrese codigo:");
      for (int x = 0; x < 2; x++)
      {     codigo[x] = Barrido();
            if(codigo[x] == 10 || codigo[x] == 11)
                  return;
            LCD_GOTOXY((x+1),2);
            char lcd_most = codigo[x] + '0';
            lcd_putc (lcd_most); 
      }
      delay_ms(200);
      if (codigo[0] == 0)
      {     if(codigo[0] == 1)
                 cursor = 0;
            else
                 cursor = (codigo[1] - 1) * 6;
            if(codigo[1] == 0)
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
      }
      if (codigo[0] > 0)
      {     cursor = codigo[1] + (codigo[0] * 10);
            if(cursor > 42) //Solo entra cuando intenta ingresar un codigo mayor a 43
            {   LCD_PUTC("\f");
                LCD_GOTOXY(1,1);
                LCD_PUTC("Codigo invalido");
                while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
            }
            cursor = (cursor - 1) * 6;
      }
      direccion = READ_EEPROM(cursor);
      cursor++;
      estado = READ_EEPROM(cursor);
      cursor++;
      for (int y = 0; y < 4; y++)
      {     clave[y] = READ_EEPROM(cursor);
            cursor++;
      }
      if (clave_cero (clave) == 1) //Solo entra cuando el usuario intenta registrar un codigo no registrado
      {        LCD_PUTC("\f");
               LCD_GOTOXY(1,1);
               LCD_PUTC("Codigo invalido"); 
               while(TRUE)
                {   if(Barrido() >= 0)
                     {    LCD_PUTC("\f");
                        return;
                     }
                }
      }
      LCD_PUTC("\f");
      LCD_GOTOXY(1,1);
      LCD_PUTC("Estado de");
      LCD_GOTOXY(11,1);
      sprintf(letra, "%d", direccion);
      lcd_putc (letra[0]);
      LCD_GOTOXY(12,1);
      lcd_putc (letra[1]);
      letra2= estado + '0';
      LCD_GOTOXY(5,2);
      LCD_PUTC(letra2);
      if (estado == 0)
      {     LCD_GOTOXY(1,2);
            LCD_PUTC("Apagado");
      }
      if (estado == 1)
      {     LCD_GOTOXY(1,2);
            LCD_PUTC("Encendido");
      }
      while(TRUE)
      {   if(Barrido() >= 0)
          {    LCD_PUTC("\f");
                        return;
          }
      }
}

void main()
{
   if(READ_EEPROM(0) != 1)
         Inicio();
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);      
   LCD_INIT();       //Inicializa el LCD
   LCD_PUTC("\f");   //Borrar el contenido del LCD
   WHILE(TRUE)
   {     LCD_GOTOXY(1,1);        //Se ubica En la columna 1 fila 1
         LCD_PUTC("Ingrese numero");    
         LCD_GOTOXY(1,2);        //Ubicarse en la columna 1 fila 2
         LCD_PUTC("del 1 al 4");
         switch(Barrido())
         {     case 1:
                        nueva_clave();
                        break;
                     case 2:          
                        cambiar_clave();
                        break;
                     case 3:
                        verificar_clave();
                        break;
                     case 4:
                        verificar_estado();
                        break;
          }
   } 
   return;
}
