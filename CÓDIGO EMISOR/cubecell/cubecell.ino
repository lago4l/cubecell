#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "HT_SH1107Wire.h"

// Pantalla SH1107 personalizada
SH1107Wire pantalla(0x3c, 500000, SDA, SCL, GEOMETRY_128_64, GPIO10);

// Configuración LoRa
#define RF_FREQUENCY        868000000 // FRECUENCIA EU (no 915 MHz)
#define TX_OUTPUT_POWER     14        // dBm
#define LORA_BANDWIDTH      0         // 125 kHz
#define LORA_SPREADING_FACTOR 7       // SF7
#define LORA_CODINGRATE     1         // 4/5
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

char txpacket[50];
float txNumber = 0;
bool lora_idle = true;

static RadioEvents_t RadioEvents;

void OnTxDone(void) {
  Serial.println("TX completado");
  lora_idle = true;
}

void OnTxTimeout(void) {
  Serial.println("TX timeout");
  Radio.Sleep();
  lora_idle = true;
}

void setup() {
  Serial.begin(115200);

  // Iniciar pantalla
  pantalla.init();
  pantalla.flipScreenVertically();
  pantalla.setFont(ArialMT_Plain_16);
  pantalla.clear();
  pantalla.drawString(0, 10, "Emisor iniciado");
  pantalla.display();

  // Configurar radio
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);

  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
}

void loop() {
  if (lora_idle) {
    delay(5000); // espera 5 segundos entre envíos

    txNumber += 1;
    sprintf(txpacket, "Paquete numero %.0f", txNumber);
    Serial.printf("Enviando paquete: \"%s\"\n", txpacket);

    // Mostrar en pantalla
    pantalla.clear();
    pantalla.drawString(0, 10, "Enviando paquete");
    pantalla.drawString(0, 30, "numero: " + String((int)txNumber));
    pantalla.display();

    Radio.Send((uint8_t *)txpacket, strlen(txpacket));
    lora_idle = false;
  }
}
