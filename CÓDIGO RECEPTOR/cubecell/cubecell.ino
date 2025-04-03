#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "HT_SH1107Wire.h"

// Pantalla SH1107 personalizada
SH1107Wire pantalla(0x3c, 500000, SDA, SCL, GEOMETRY_128_64, GPIO10);

// LoRa configuración
#define RF_FREQUENCY        868000000 // cambia a 915000000 si estás en América
#define LORA_BANDWIDTH      0         // 125 kHz
#define LORA_SPREADING_FACTOR 7       // SF7
#define LORA_CODINGRATE     1         // 4/5
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define BUFFER_SIZE 50
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

int16_t paquetesRecibidos = 0;
int16_t ultimoRSSI = 0;
bool lora_idle = true;

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  Radio.Sleep();
  lora_idle = true;

  // Copiar mensaje
  memcpy(rxpacket, payload, size);
  rxpacket[size] = '\0';  // asegurar fin de cadena
  ultimoRSSI = rssi;
  paquetesRecibidos++;

  Serial.printf("\r\nMensaje recibido: \"%s\" | RSSI: %d dBm | tamaño: %d\r\n", rxpacket, rssi, size);

  // Mostrar en pantalla
  pantalla.clear();
  pantalla.setFont(ArialMT_Plain_10);
  pantalla.drawString(0, 0, "Paquetes recibidos: " + String(paquetesRecibidos));
  pantalla.drawString(0, 20, "Ultimo RSSI: " + String(ultimoRSSI) + " dBm");
  pantalla.setFont(ArialMT_Plain_10);
  pantalla.drawString(0, 40, String(rxpacket));
  pantalla.display();
}

void setup() {
  Serial.begin(115200);

  // Iniciar pantalla
  pantalla.init();
  pantalla.flipScreenVertically();
  pantalla.setFont(ArialMT_Plain_16);
  pantalla.drawString(0, 20, "Receptor iniciado");
  pantalla.display();
  delay(1000);

  // Configurar radio
  RadioEvents.RxDone = OnRxDone;
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
}

void loop() {
  if (lora_idle) {
    lora_idle = false;
    Serial.println("Esperando paquetes...");
    Radio.Rx(0);  // recibir sin límite de tiempo
  }
}
