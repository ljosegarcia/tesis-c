# Introducción

En este repositorio esta mi tesis que tiene como título principal: Sistema de control inteligente para el accionamiento del encendido/apagado de aires acondicionados que contribuya al ahorro energético en el Instituto Universitario Politécnico “Santiago Mariño”, Extensión Maturín, Estado Monagas. Fue un trabajo de grado presentado para optar al título de ingeniero electrónico, que tenía por objetivo desarrollar un sistema para el control de encendido y apagado de los aires acondicionados que estaban presentes en la edificación de la institución, con el propósito de minimizar hora de trabajos de equipos, ya que estos trabajaban constantemente sin un control alguno, por lo que se planteo este trabajo de grado.

## Partes del circuito

El circuito consta de tres partes principales: la alimentación, que suministra una tensión de 5 voltios (V) al circuito, ya que las siguientes etapas requieren esa diferencia de potencial; el maestro, donde el microcontrolador PIC 18F2550 es el que controla la infertaz de entrada y salida del usuario, la memoria, además de la última etapa; el esclavo, mediante un microcontrolador PIC 16F88, al ser activado o desactivado mediante una orden del maestro, enciende o apaga el equipo, representado por un motor DC y tres luces led (esto último para observar el circuito en simulación y en físico).

<center> <img alt="Circuito.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Circuito.png" width="500" heigh="500"> </center>

## Funcionamiento

### Códigos

Los dos microcontroladores PIC están escritos en lenguaje C, en el entorno de promagramación PIC C, donde [TesisC.c](TesisC.c) corresponde al PIC 18F2550 (maestro) y [TesisC2.c](TesisC2.c) corresponde al PIC 16F88 (esclavo); además del uso de las librerias [18F2550.h](18F2550.h), [16F88.h](16F88.h) y [lcd.c](lcd.c), incluidas en el entorno de programación. El funcionamiento del código se puede observar en el siguiente diagrama de flujo:

<center> <img alt="Diagrama.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Diagrama.png" width="500" heigh="300"> </center>

Para entender y observar el funcionamiento del código se dividen en 6 etapas principales:

En su primera etapa, al iniciar por primera vez guarda en la memoria EEPROM del PIC 18F2550 una serie de identifcadores que corresponden la dirección, su estado de encendido y apagado, y finalmente su contraseña por defecto; debido a que esta memoria es no volátil y sigue manteniendo la información aunque no este alimentado, algo importante para mantener registros continuos para las siguientes etapas.

La segunda etapa nos dirige al menú, donde el LCD (Liquid Cristal Display, por sus siglas en inglés) pide al usuario que ingrese un número de 1 al 4 mediante el teclado matricial 4x4, el cual permitirá entrar a las 4 funciones: registrar un equipo, cambiar contraseña de un equipo registrado, encender o apagar un equipo y observar el estado (encendido/apagado) de un equipo.

En la tercera etapa, al introducir el número 1 entra en la instrucción de registro de contraseña, en cual pide la introducción de una clave y posteriormente a mostrar en qué dirección se guardó.

<center> <img alt="Etapa3.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Etapa3.png" width="500" heigh="500"> </center>

En la cuarta etapa, al introducir el número 2, el cambio de clave, donde pide el código de la dirección de dos dígitos (registrada anteriormente), seguidamente de la clave vieja, para finalizar con la clave nueva que se requiera.

<center> <img alt="Etapa4.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Etapa4.png" width="500" heigh="500"> </center>

En la quinta etapa, al introducir el número 3, el dispositivo pide al usuario la introducción del código y después de la clave, el cual al confirmar este envía al esclavo mediante los pines de comunicación el código de confirmación para su activación, el cual pone en marcha el motor, enciende el motor y el led verde, o en caso contrario, apaga el motor y enciende el led rojo.

<center> <img alt="Etapa5.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Etapa5.png" width="500" heigh="500"> </center>

En la sexta y última etapa, al introducir el número 4, el dispositivo pide al usuario introducir el código de la dirección, donde al colocar ese mismo, se puede observar que muestra el encendido o apagado en el LCD. 

<center> <img alt="Etapa6.png" src="https://github.com/ljosegarcia/maestro-esclavo-c/blob/main/images/Etapa6.png" width="500" heigh="500"> </center>
