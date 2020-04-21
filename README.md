# Implementación y estudio de una red LoRaWAN

Trabajo final de grado para obtención de título en Ingeniero el Telecomunicaciones - Universidad Católica del Uruguay Dámaso Antonio Larrañaga

AUTORES: Juan Collazo, Mauro Gonzalez y Andrés Muracciole

El trabajo consiste en el estudio del protocolo de red LoRaWAN para luego implementar una red a partir de un gateway RAK 831 y módulos de comunicacion propios fabricados con Arduino Pro Mini y módulos LoRa RFM95W.
El servidor de red y aplicaciones utilizado en dicho trabajo fue TTN.

![LoRaWAN](https://github.com/amuracciole/LoRaWAN_Network/blob/master/LoRaWAN.jpg)

## NODO

Este se creó desde cero, realizando el diseño de la protoboard para posteriormente enviar a fabicarlas.
En cuanto a la configuración, esta se hizo de forma escalable de forma que el usuario pueda decidir los modos DEBUG y SLEEP.
Por otro lado, cuenta con diversos módulos para determinar el tipo de sensor o dato a enviar.

### Número Aleatorio

Envía un número aleatorio entre 0 y 255

### Conteo

Envía de forma periódica un número. Este comienza en 1 y va incrementando.

### Palabra Fija

Para mostrar que no solo se puede enviar números, esta función es capaz de enviar una palabra o frase corta.

### Humedad y Temperatura

Pensado para utilizarse con el sensor DHT11. Esta función envia la presión y temperatura ambiental.

### Luz

Pensado para utilizarse con el sensor BH1750. Esta función envia el nivel de luxes en el ambiente. Va entre 0 y 65535

## LIBRERIAS

Se adjuntan las librerías de Arduino utilizadas.

## PCB

![Nodo](https://github.com/amuracciole/LoRaWAN_Network/blob/master/Nodo.png)

Los archivos PCB_Sensor y PCB_Nodo son los diseños de las placas finales realizadas para el trabajo.

## DOCUMENTACIÓN

![RAK831](https://github.com/amuracciole/LoRaWAN_Network/blob/master/RAK831.png)

Se agrega una memoria final del trabajo con los estudios y mediociones realizadas así como también un archivo que indica los pasos a seguir para realizar la configuración del gatewar RAK831

