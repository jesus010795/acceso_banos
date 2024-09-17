#include <SPI.h>
#include <MFRC522.h>

#include "RTClib.h"
#include <SD.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PINPIXELS 6  // 
#define NUMPIXELS 2  
#define PIN_SD 4     
#define RST_PIN 5    
#define SS_PIN 53  
#define BUZZER 9
#define BTN 38

byte lock = 2;

// RESET
// #define RESET_BTN 40
// #define RESTART asm("jmp 0x0000")

// PIXELS
Adafruit_NeoPixel pixels(NUMPIXELS, PINPIXELS, NEO_GRB + NEO_KHZ800);

// ----- CLOCK
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };

// ----- File register
File myFile;

// ----- RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance


// ---- RFID variables
String first_name = ""; // 
String last_name = "";
String id_employee = "";
// String card_to_hex = "";
// String card_to_hex_comp = "";
String compare_employee = "";

MFRC522::MIFARE_Key key;

byte block;
byte len;

MFRC522::StatusCode status;


void setup() {
  Serial.begin(9600);
  
  pinMode(BUZZER, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(BTN, INPUT);
  
  digitalWrite(lock, HIGH);
  digitalWrite(BUZZER, HIGH);  //PNP

  // RESET
  // pinMode(RESET_BTN, INPUT);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();
  SPI.begin();
  mfrc522.PCD_Init();
  // mfrc522.PCD_DumpVersionToSerial();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    buzzer_error();
    Serial.flush();
    while (1) delay(10);
  }

  if (!SD.begin(PIN_SD)) {
    Serial.println("initialization failed!");
    buzzer_error();
    while (1)
      ;
  }
  // set_time();
  Serial.println("initialization done.");

//------------------------------------------------------------------
  //Guardando datos despues de reset, reconexion o reinicio
  DateTime time = rtc.now();
  myFile = SD.open("registro.txt", FILE_WRITE);

  if (myFile) {
    myFile.print(time.timestamp(DateTime::TIMESTAMP_TIME));
    myFile.print("/");
    myFile.println("RESET - REINICIO");
    myFile.close();
    delay(100);
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening testRegistro.txt");
  }

//------------------------------------------------------------------
}

void loop() {
  digitalWrite(BUZZER, HIGH);  //PNP

  // -----------                ---------------------------
  // --- COMPROBAR REINICIO DE VALORES DESPUES DE RESET ---
  // Serial.println(id_employee);
  // Serial.println(last_name);
  // Serial.println(first_name);
  // -----------                ---------------------------

  // --------------------          -----------------
  //RESET DE PROGRAMA
  //Se ejecuta la funcion que almacena el evento del reset
  //se almacena fecha y hora de la ejecucion
  //Los valores se resetean a su estado incial
  // --------------------         ------------------

  //********************
  // if (digitalRead(RESET_BTN) == HIGH) {
  //   reset_button();
  //   mfrc522.PICC_HaltA();
  //   mfrc522.PCD_StopCrypto1();
  //   Serial.println("Reseteando....");
  //   delay(1000);
  //   RESTART;
  // }
  //********************

  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.show();  // Send the updated pixel colors to the hardware.


  bool occupied_bathroom = true; 

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // if (!mfrc522.PICC_IsNewCardPresent()) {
  //   return;
  // }

  // if (!mfrc522.PICC_ReadCardSerial()) {
  //   return;
  // }

  // ---- Clock instance
  DateTime time = rtc.now();

  if (mfrc522.PICC_IsNewCardPresent() & mfrc522.PICC_ReadCardSerial()) {
    // Serial.println(F("**Card Detected:**"));

    // GET DATAT FROM TAG
    id_employee = get_hex_id();

    // Serial.println(id_employee);

    first_name = get_first_name();

    // Serial.println(first_name);

    last_name = get_last_name();

    // Serial.println(last_name);

    if (id_employee != "") {
      pixels.clear();  // Set all pixel colors to 'off'
      pixels.setPixelColor(0, pixels.Color(150, 0, 0));
      pixels.setPixelColor(1, pixels.Color(150, 0, 0));
      // pixels.setPixelColor(2, pixels.Color(150, 0, 0));
      // pixels.setPixelColor(3, pixels.Color(150, 0, 0));
      pixels.show();  // Send the updated pixel colors to the hardware.

      buzzer_ok();
      open_door();
      register_entry();

      // digitalWrite(GREEN_LED, LOW);
      // digitalWrite(RED_LED, HIGH);

      while (occupied_bathroom) {

        // -----------      -------
        // RESET DE PROGRAMA
        //Crear logica de boton reset
        //Al presionar, llamar a la funcon registrar salida
        //Ejecutar instruccion para reiniciar el programa
        //Todos los valores deben regresar a su estado inicial
        // -----------      -------

        //********************
        // if (digitalRead(RESET_BTN) == HIGH) {
        //   check_out();
        //   mfrc522.PICC_HaltA();
        //   mfrc522.PCD_StopCrypto1();
        //   Serial.println("Reseteando....");
        //   delay(1000);
        //   RESTART;
        // }
        //********************


        if (digitalRead(BTN) == HIGH) {
          mfrc522.PICC_HaltA();
          mfrc522.PCD_StopCrypto1();

          bool compare = true;
          Serial.println("bonton Activado");

          buzzer_ok();
          open_door();

          while (compare) {
            // Serial.println("Coloca de nuevo el tag");
            bool card_comparation = true;

            while (card_comparation) {
              // check_tag_led();
              pixels_checkout();
              buzzer_alert();

              // -----------      -------
              //Crear logica de boton reset
              //Al presionar, llamar a la funcon registrar salida
              //Ejecutar instruccion para reiniciar el programa
              //Todos los valores deben regresar a su estado inicial
              // -----------      -------

              //********************
              // if (digitalRead(RESET_BTN) == HIGH) {
              //   check_out();
              //   mfrc522.PICC_HaltA();
              //   mfrc522.PCD_StopCrypto1();
              //   Serial.println("Reseteando....");
              //   delay(1000);
              //   RESTART;
              // }
              //********************

              if (mfrc522.PICC_IsNewCardPresent() & mfrc522.PICC_ReadCardSerial()) {

                to_string(mfrc522.uid.uidByte, mfrc522.uid.size, compare_employee);
                // Serial.print("Comprobando : ");
                // Serial.println(compare_employee);

                if (compare_employee == id_employee) {

                  buzzer_ok();
                  check_out();

                  mfrc522.PICC_HaltA();
                  mfrc522.PCD_StopCrypto1();

                  card_comparation = false;
                  delay(3000);
                } else {
                  delay(100);
                  buzzer_error();
                  compare_employee = "";
                }
              }
            }
            compare = false;
          }
          occupied_bathroom = false;
        }
      }
    }
    // read_data();

    //Resetting variables for a new reading
    id_employee = "";
    first_name = "";
    last_name = "";
    compare_employee = "";
    pixels.clear();

    // mfrc522.PICC_HaltA();
    // mfrc522.PCD_StopCrypto1();
  } else if (!mfrc522.PICC_IsNewCardPresent()) {
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return;
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void to_string(byte *buffer, byte bufferSize, String &hexValues) {
  for (byte i = 0; i < bufferSize; i++) {
    hexValues += (buffer[i] < 0x10 ? "0" : "");  // Añade un cero inicial si es necesario
    hexValues += String(buffer[i], HEX);         // Concatena el valor hexadecimal
  }
}
