//................................................................................................................................................................................//
//                                                        TESIS LORA WAN - Juan Collazo, Mauro Gonzalez, Andrés Muracciole                                                        //
//                                                                                                                                                                                //
//                                Este programa funciona a 915 MHz en la banda 2 de EU para transmisión de paquetes mediante protocolo LoRa.                                      //
//                      El nodo se asocia a TTN (The Things Network) y es por medio de esta aplicación que se ven los paquetes enviados y recibidos.                              //
//                                                  Para ello es necesario configurar las NWSKEY, APPSKEY y DEVADDR de antemano.                                                  //
//                                                                                                                                                                                //
//         A su vez este código es genérico para todos los nodos. Para habilitar la funcionalidad hay que sacar el comentario de la parte de menú y comentar los demás.           //
//               También se puede agregar o quitar las funcionalidades de DEBUG y SLEEP, como también da la posibilidad de elegir como asociar los nodos al gateway.              //
//                                                                                                                                                                                //
//                                                       El código está diseñado para poder correr los siguientes programas:                                                      //
//                                 - LUZ: Envía la cantidad de luxes que registra el sensor. Este valor está comprendido entre 0 y 65535                                          //
//                                 - ALEATORIO: Envía un número aleatorio comprendido entre 0 y 255                                                                               //
//                                 - FRASE: Envía una palabra o grase predeterminada de un máximo de 53 caracteres (incluyendo espacios)                                          //
//                                 - HUMEDADyTEMPERATURA: Envía los datos de humedad (%) y temperatura (C)                                                                        //
//                                 - CONTEO: Envía paquetes numerados ordenadamente. Comienza por 1, sigue por 2, hasta llegar a 65535. Sirve para estudiar pérdida de paquetes   //
//................................................................................................................................................................................//
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

//-------------------------------------------------------------------------------------DEBUG--------------------------------------------------------------------------------------//
//BORRAR EL COMENTADO PARA CORRER LOS PROGRAMAS DESEADOS
#define DEBUG
//#define SLEEP

#define SYNC_ABP
//#define SYNC_OTTA

//#define LUZ
//#define ALEATORIO
//#define FRASE
//#define HUMEDADyTEMPERATURA
//#define ULTRASONIDO
#define CONTEO

//------------------------------------------------------------------------LIBRERIAS SEGÚN LA APLICACIÓN---------------------------------------------------------------------------//
#ifdef LUZ
  #include <BH1750FVI.h> //Libredía del sensor de luz
#endif

#ifdef HUMEDADyTEMPERATURA
  #include <DHT.h>  //Libredía del sensor de luz
  #include <DHT_U.h>
  #include "DHT.h"
#endif

//------------------------------------------------------------------------------------CLAVES--------------------------------------------------------------------------------------//
#ifdef LUZ
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0xE4, 0xDD, 0xA2, 0x33, 0x34, 0xD1, 0xB5, 0xAA, 0xF7, 0xAC, 0x5B, 0x5F, 0xE5, 0x38, 0xA0, 0x17 };
    static const u1_t PROGMEM APPSKEY[16] = { 0xF6, 0x12, 0x58, 0x45, 0x72, 0x79, 0x31, 0x08, 0x93, 0x57, 0x56, 0xD8, 0x71, 0x1D, 0x8A, 0xC1 };
    static const u4_t DEVADDR = 0x26031FB3 ;
  #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={ 0xD8, 0xAF, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={ 0xA0, 0x70, 0xBC, 0x2B, 0x46, 0x01, 0xC6, 0x99 };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = { 0xD9, 0xC7, 0xF7, 0xDB, 0xEF, 0xF1, 0x73, 0xAB, 0x76, 0x96, 0x17, 0xBE, 0x5F, 0x2B, 0x59, 0xBA };
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef ALEATORIO
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0xB5, 0xFF, 0xAB, 0xC4, 0x82, 0xA3, 0x26, 0x9D, 0x01, 0x56, 0x1D, 0x51, 0xA0, 0x71, 0x2F, 0x4D };
    static const u1_t PROGMEM APPSKEY[16] = { 0xCD, 0xB2, 0xC1, 0xB2, 0xF7, 0x0D, 0x26, 0xB2, 0x35, 0x22, 0x92, 0x85, 0xED, 0xA0, 0x31, 0x6A };
    static const u4_t DEVADDR = 0x260315A9 ;
    #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={ 0x6B, 0xB6, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={ 0x5C, 0x18, 0x00, 0xB0, 0xDE, 0x28, 0xA0, 0x76 };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = { 0x20, 0xDC, 0xF4, 0xE4, 0x63, 0x7B, 0x44, 0x0E, 0x04, 0xFE, 0x1A, 0xA7, 0x4A, 0xB7, 0x2F, 0x96 };
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef FRASE
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0x71, 0x50, 0x2A, 0x66, 0x62, 0x95, 0x33, 0x61, 0x0B, 0xCE, 0x9A, 0x95, 0x87, 0xC6, 0x01, 0x54 };
    static const u1_t PROGMEM APPSKEY[16] = { 0x45, 0xAA, 0x3C, 0x87, 0xFF, 0xD3, 0xBD, 0xEE, 0xF6, 0xD4, 0xE4, 0xFD, 0x17, 0xDC, 0x5A, 0x91 };
    static const u4_t DEVADDR = 0x260313BA;
  #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={ 0x57, 0xBD, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={ 0x5C, 0x00, 0xFF, 0xA5, 0x21, 0x10, 0xB4, 0x66 };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = { 0xB6, 0xCE, 0x84, 0x51, 0x2A, 0xC7, 0xD9, 0xD3, 0xBC, 0xD6, 0xD2, 0xC3, 0x52, 0x43, 0x71, 0xD7 };
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef HUMEDADyTEMPERATURA
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0xA0, 0x95, 0x7A, 0xBA, 0xB8, 0x3A, 0x95, 0xD6, 0xF0, 0xA4, 0x1D, 0x04, 0x20, 0x3A, 0xF2, 0xDE };
    static const u1_t PROGMEM APPSKEY[16] = { 0x83, 0x43, 0x5A, 0x8E, 0x29, 0x3A, 0x4E, 0x5B, 0x4C, 0x8A, 0xD0, 0xB5, 0x05, 0x6F, 0x15, 0x14 };
    static const u4_t DEVADDR = 0x260310DA;
  #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={ 0x4E, 0xC6, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={ 0x2F, 0x0D, 0xAA, 0x99, 0x43, 0x71, 0x06, 0x88 };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = { 0xAF, 0x57, 0xB1, 0xD5, 0xEF, 0x5C, 0x48, 0x77, 0x2F, 0x68, 0x98, 0x31, 0x56, 0x27, 0x05, 0x73 };
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef ULTRASONIDO
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0xFD, 0x95, 0x5D, 0x0E, 0x71, 0x36, 0xC0, 0x8E, 0x05, 0xB3, 0x24, 0x47, 0x5A, 0x17, 0xAC, 0xA5 };
    static const u1_t PROGMEM APPSKEY[16] = { 0x0E, 0x3E, 0xF9, 0x0C, 0x56, 0x26, 0xB0, 0xB3, 0x24, 0x27, 0x65, 0x49, 0xE9, 0x40, 0x58, 0xA2 };
    static const u4_t DEVADDR = 0x26021AB3;
  #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={  };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={  };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = ;
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef CONTEO
  #ifdef SYNC_ABP
    static const PROGMEM u1_t NWKSKEY[16] = { 0xA7, 0x63, 0xB5, 0xFA, 0xDD, 0x00, 0xEC, 0xE9, 0xAE, 0x69, 0xAC, 0x37, 0x75, 0xCE, 0x56, 0x91 };
    static const u1_t PROGMEM APPSKEY[16] = { 0x3E, 0x7F, 0x49, 0x7B, 0x51, 0x88, 0xB6, 0x80, 0xE0, 0x77, 0x2F, 0xE0, 0x6A, 0x3B, 0xC0, 0xB2 };
    static const u4_t DEVADDR = 0x26011897;
  #endif

  #ifdef SYNC_OTTA
    static const u1_t PROGMEM APPEUI[8]={ 0x21, 0x62, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
    void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
    static const u1_t PROGMEM DEVEUI[8]={ 0x47, 0xE2, 0xCA, 0x14, 0x2B, 0x03, 0x75, 0xA4 };
    void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
    static const u1_t PROGMEM APPKEY[16] = { 0xAF, 0x57, 0xB1, 0xD5, 0xEF, 0x5C, 0x48, 0x77, 0x2F, 0x68, 0x98, 0x31, 0x56, 0x27, 0x05, 0x73 };
    void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
  #endif
#endif

#ifdef SYNC_ABP
  void os_getArtEui (u1_t* buf) { }
  void os_getDevEui (u1_t* buf) { }
  void os_getDevKey (u1_t* buf) { }
#endif

//-----------------------------------------------------------------------------------TEXTO FIJO-----------------------------------------------------------------------------------//
#ifdef LUZ
  static uint8_t mydata[] = "  ";
#endif

#ifdef ALEATORIO
  static uint8_t mydata[] = " ";
#endif

# ifdef FRASE
  static uint8_t mydata[] = "Tesis LoRaWAN 2018"; //Las letras deben ser MAYUSCULAS
#endif

#ifdef HUMEDADyTEMPERATURA
  static uint8_t mydata[] = "  ";
#endif

#ifdef ULTRASONIDO
  static uint8_t mydata[] = " ";
#endif

#ifdef CONTEO
  static uint8_t mydata[] = "  ";
#endif

#ifdef SLEEP
  #include "LowPower.h"
  bool next = false;
#endif

static osjob_t sendjob;

//--------------------------------------------------------------------------------INTERVALO DE TX---------------------------------------------------------------------------------//
const unsigned TX_INTERVAL = 25;

//---------------------------------------------------------------------------------MAPEO DE PINES---------------------------------------------------------------------------------//
#ifdef ULTRASONIDO
  const lmic_pinmap lmic_pins = {
    .nss = 9,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 2,
    .dio = {3, 5, 4},
  };
  #else
  const lmic_pinmap lmic_pins = {
    .nss = 6,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 5,
    .dio = {2, 3, 4},
  };
#endif

//----------------------------------------------------------------------------DECLARACIÓN DE VARIABLES----------------------------------------------------------------------------//
#ifdef ULTRASONIDO
  #define LED_TXRX 7
  #else
  #define LED_TXRX 8
#endif

#ifdef LUZ
  #define LED_ACCION 14 // LED de ACCION
  BH1750FVI LightSensor;
  int lux = 0;
#endif

#ifdef ALEATORIO
  #define LED_ACCION 9 // LED de ACCION
  int ran = 0;
#endif

#ifdef FRASE
  #define LED_ACCION 9 // LED de ACCION
#endif

#ifdef HUMEDADyTEMPERATURA
  #define LED_ACCION 9 // LED de ACCION
  #define DHTTYPE DHT11   // DHT 11
  const int DHTPin = 7;
  DHT dht(DHTPin, DHTTYPE);
  int hum = 0;
  int temp = 0;
#endif

#ifdef ULTRASONIDO
  #define LED_ACCION 6 // LED de ACCION
  int dist = 0;
  long tiempo = 0;
#endif

#ifdef CONTEO
  #define LED_ACCION 9 // LED de ACCION
  int cont = 0;
#endif

//------------------------------------------------------------------------------------EVENTOS-------------------------------------------------------------------------------------//
void onEvent (ev_t ev) {
  #ifdef DEBUG
    //Serial.print(os_getTime());
    //Serial.print(": ");
  #endif
  
  switch(ev) {
      case EV_SCAN_TIMEOUT:
          Serial.println(F("EV_SCAN_TIMEOUT"));
          break;
      case EV_BEACON_FOUND:
          Serial.println(F("EV_BEACON_FOUND"));
          break;
      case EV_BEACON_MISSED:
          Serial.println(F("EV_BEACON_MISSED"));
          break;
      case EV_BEACON_TRACKED:
          Serial.println(F("EV_BEACON_TRACKED"));
          break;
      case EV_JOINING:
          Serial.println(F("EV_JOINING"));
          break;
      case EV_JOINED:
          Serial.println(F("EV_JOINED"));
          break;
      case EV_RFU1:
          Serial.println(F("EV_RFU1"));
          break;
      case EV_JOIN_FAILED:
          Serial.println(F("EV_JOIN_FAILED"));
          break;
      case EV_REJOIN_FAILED:
          Serial.println(F("EV_REJOIN_FAILED"));
          break;
      case EV_TXCOMPLETE:
          #ifdef DEBUG
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));

          #endif
          if (LMIC.txrxFlags & TXRX_ACK)
            Serial.println(F("Received ack"));
          if (LMIC.dataLen) {
            Serial.println();
            //LedRX();
            Serial.println(F("DATOS RECIBIDOS"));
            LeerRecepcion();
            Accion();
            Serial.println();
          }
          // Para la proxima tramsinción...
          #ifndef SLEEP
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
          #else
            next = true;
          #endif
          break;
      case EV_LOST_TSYNC:
          Serial.println(F("EV_LOST_TSYNC"));
          break;
      case EV_RESET:
          Serial.println(F("EV_RESET"));
          break;
      case EV_RXCOMPLETE:
          Serial.println(F("EV_RXCOMPLETE"));
          break;
      case EV_LINK_DEAD:
          Serial.println(F("EV_LINK_DEAD"));
          break;
      case EV_LINK_ALIVE:
          Serial.println(F("EV_LINK_ALIVE"));
          break;
       default:
          Serial.println(F("Unknown event"));
          break;
  }
}

//---------------------------------------------------------------------------------FUNCIÓN ENVIAR---------------------------------------------------------------------------------//
void do_send(osjob_t* j){
    // Chequeo que no esté transmitiendo ni recibiendo datos
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
      #ifdef LUZ
        NivelLuz();
        Datos(lux);
      #endif

      #ifdef ALEATORIO
        Aleatorio();
        Datos(ran);
      #endif

      #ifdef HUMEDADyTEMPERATURA
        Humedad();
        Temperatura();
        Datos (hum, temp);
      #endif

      #ifdef ULTRASONIDO
        Distancia();
        Datos (dist);
      #endif

      #ifdef CONTEO
        Contar();
        Datos (cont);
      #endif
        //LedTX();
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        //Serial.println(F("Packet queued"));
        Serial.print(F("FRECUENCIA: "));
        Serial.println(LMIC.freq);

        #ifdef LUZ
          Serial.print(F("NIVEL DE LUZ ENVIADO: "));
          Serial.println(lux);
        #endif

        #ifdef ALEATORIO
          Serial.print(F("NUMERO ALEATORIO ENVIADO: "));
          Serial.println(ran);
        #endif

        #ifdef HUMEDADyTEMPERATURA
          Serial.print(F("HUMEDAD: "));
          Serial.print(hum);
          Serial.println(F(" %\t"));
          Serial.print(F("TEMPERATURA: "));
          Serial.print(temp);
          Serial.println(F(" C"));
        #endif

        #ifdef ULTRASONIDO
          Serial.print(F("DISTANCIA: "));
          Serial.print(dist);
          Serial.println(F(" cm"));
        #endif

        #ifdef CONTEO
          Serial.print(F("NUMERO: "));
          Serial.println(cont);
        #endif

        #ifdef FRASE
          LeerEnvio_ASCII();
        #endif

        #ifdef DEBUG
          LeerEnvio();
        #endif
        Serial.println();
    }
}

//-------------------------------------------------------------------------------------SETUP--------------------------------------------------------------------------------------//
void setup() {
  #ifdef ULTRASONIDO
    pinMode(8, INPUT);
  #else
    pinMode(8, OUTPUT);
  #endif
    pinMode(9, OUTPUT);
    pinMode(14, OUTPUT);
    
    Serial.begin(9600);
    Serial.println(F("Inicializando..."));

    #ifdef HUMEDADyTEMPERATURA
      dht.begin();
    #endif

    #ifdef VCC_ENABLE
    Serial.println(F("si1"));
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
    #endif
    // Inicio LMIC
    os_init();
    // Reseteo el estado de la MAC.
    LMIC_reset();
    #ifdef SYNC_ABP
      #ifdef PROGMEM
        // En AVR estos valores se guardan en la memoria flash y se copian en la RAM una unica vez.
        // Se guardan en el buffer y con la función LMIC_setSession se copian graban
        uint8_t appskey[sizeof(APPSKEY)];
        uint8_t nwkskey[sizeof(NWKSKEY)];
        memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
        memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
        LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
      #else
        // Si no está configurado para que corra AVR con PROGEM, se copian directamente
        LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
      #endif
  
      //Selecciono la segunda banda (empieza en 0) compuesta por los canales del 8 al 15
      //LMIC_selectSubBand(1);
  
      // Disable link check validation
      LMIC_setLinkCheckMode(0);
  
      // TTN uses SF9 for its RX2 window.
      LMIC.dn2Dr = DR_SF9;
  
      //Defino el data rate y la potenia de uplink. Para el data rate puedo usar SF7 a SF10. A mayor numero menor es el payload pero mayor la distancia de transimsión.
      //La potencia de transmisión no se termina usando en la función de la librería
      LMIC_setDrTxpow(DR_SF10,30);
    #endif
    // Envio...
    do_send(&sendjob);
}

//--------------------------------------------------------------------------------------LOOP--------------------------------------------------------------------------------------//
void loop() {
  #ifndef SLEEP
    os_runloop_once();
  #else
    extern volatile unsigned long timer0_overflow_count;
  
    if (next == false) {
      os_runloop_once();
    } 
    else {
      int sleepcycles = TX_INTERVAL / 8;  // Calcula la cantidad de sleepcicles. Cada uno es de 8 segundos
      #ifdef DEBUG
        Serial.print(F("Enter sleeping for "));
        Serial.print(sleepcycles);
        Serial.println(F(" cycles of 8 seconds"));
      #endif
      Serial.flush();
      for (int i=0; i<sleepcycles; i++) {

        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  
        cli();
        timer0_overflow_count+= 8 * 64 * clockCyclesPerMicrosecond();
        sei();
      }
      #ifdef DEBUG
        Serial.println(F("Sleep complete"));
      #endif
      next = false;
      // Envio...
      do_send(&sendjob);
    } 
  #endif
}


//-----------------------------------------------------------------------------------FUNCIONES------------------------------------------------------------------------------------//
void LedTX(){
  #ifdef LUZ
    analogWrite(LED_TXRX, 150); 
    delay (100);
    analogWrite(LED_TXRX, 0);
  #else
    digitalWrite(LED_TXRX, HIGH); 
    delay (100);
    digitalWrite(LED_TXRX, LOW);
  #endif
}

void LedRX(){
  #ifdef LUZ
    analogWrite(LED_TXRX, 150); 
    delay (50);
    analogWrite(LED_TXRX, 0);
    delay (50);
    analogWrite(LED_TXRX, 150); 
    delay (50);
    analogWrite(LED_TXRX, 0);
    delay (50);
    analogWrite(LED_TXRX, 150); 
    delay (50);
    analogWrite(LED_TXRX, 0);
  #else
    digitalWrite(LED_TXRX, HIGH); 
    delay (50);
    digitalWrite(LED_TXRX, LOW);
    delay (50);
    digitalWrite(LED_TXRX, HIGH); 
    delay (50);
    digitalWrite(LED_TXRX, LOW);
    delay (50);
    digitalWrite(LED_TXRX, HIGH); 
    delay (50);
    digitalWrite(LED_TXRX, LOW);
  #endif
}
#ifdef LUZ
  int NivelLuz(){
    lux = LightSensor.getAmbientLight();
    return lux;
  }

  void Datos(int lux){  
    mydata[0] = (lux >> 8);       
    mydata[1] = lux;
  }
#endif

#ifdef ALEATORIO
  int Aleatorio(){
    ran = random(0, 255);
    return ran;
  }

  void Datos(int ran){       
    mydata[0] = ran;
  }
#endif

#ifdef HUMEDADyTEMPERATURA
  int Humedad(){
    hum = dht.readHumidity();
    return hum;
  }
  int Temperatura(){
    temp = dht.readTemperature();
    return temp;
  }

  void Datos(int hum, int temp){  
    mydata[0] = hum;       
    mydata[1] = temp;
  }
#endif

#ifdef ULTRASONIDO
  int Distancia(){
    digitalWrite(9,LOW);
    delayMicroseconds(5);
    /* Se envia el pulso ultrasonico */
    digitalWrite(9, HIGH);
    delayMicroseconds(10);
    /* Mide el tiempo transcurrido entre la salida y la llegada del pulso ultrasonico */
    tiempo=pulseIn(8, HIGH);
    /* Se calcula la distancia on esta formila*/
    dist= int(0.017*tiempo);
    /* Se imprime la distancia en centimetros en el monitor serial */
    return dist;
  }
  void Datos(int dist){  
    mydata[0] = dist;       
  }
#endif

#ifdef CONTEO
  int Contar(){
    if (cont < 65536 )
      cont = cont + 1;
    else
      cont = 0;
    return cont;
  }

  void Datos(int cont){ 
    mydata[0] = (cont >> 8);       
    mydata[1] = cont;
  }
#endif


#ifdef FRASE
  void LeerEnvio_ASCII(){
  Serial.print(F("PALABRA FIJA: "));
  for (int w=0; w<sizeof(mydata)-1; w++){
      Serial.print((char)mydata[w]);
  }
  Serial.println();
}
#endif

void LeerEnvio(){
  Serial.print(F("Mensaje enviado: 0x"));
  for (int w=0; w<sizeof(mydata)-1; w++){
    if(mydata[w] < 0x10){
      Serial.print(F("0"));
      Serial.print(mydata[w], HEX);
    }
    else
      Serial.print(mydata[w], HEX);
  }
  Serial.println();
}

void LeerRecepcion(){
  #ifdef DEBUG
    Serial.print(F("Largo del payload recibido: "));
    Serial.print(LMIC.dataLen);
    Serial.println(F(" bytes"));
  #endif
  Serial.print(F("MENSAJE RECIBIDO: 0x"));
  for (int l = 0; l < LMIC.dataLen; l++) {
    if (LMIC.frame[LMIC.dataBeg + l] < 0x10) {
      Serial.print(F("0"));
      Serial.print(LMIC.frame[LMIC.dataBeg + l], HEX);
    }
    else
      Serial.print(LMIC.frame[LMIC.dataBeg + l], HEX);
  }
}

void Accion(){
  if (LMIC.frame[LMIC.dataBeg] == 0x01){
    Serial.println();
    Serial.println(F("Led Prendido"));
    analogWrite(LED_ACCION, 150); 
  }
  else if (LMIC.frame[LMIC.dataBeg] == 0x00){
    Serial.println();
    Serial.println(F("Led Apagado"));
    analogWrite(LED_ACCION, 0); 
  }
  else
    Serial.println(F("Comando no valido"));
    return;
}
