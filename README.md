# CubeCell LoRa – Emisor y Receptor

Este proyecto demuestra cómo comunicar dos placas **CubeCell HTCC-AB02** mediante **LoRa**, con visualización en una pantalla tanto para el **emisor** como para el **receptor**.

## Requisitos

- 2 placas Heltec **CubeCell HTCC-AB02**
- 2 antenas LoRa (una por placa)
- Arduino IDE
- Pantalla SH1107 conectada a cada placa (I2C)
- Librerías necesarias:
  - `HT_SH1107Wire`
  - `LoRaWan_APP` (incluida con soporte oficial de CubeCell)

## Contenido del repositorio

- `CÓDIGO EMISOR` – Código que envía paquetes LoRa cada 5 segundos.
- `CÓDIGO RECEPTOR` – Código que recibe paquetes LoRa y muestra cuántos han llegado.
- `CUBECELL_ARDUINOIDE.pdf` – Guía paso a paso para configurar el entorno, instalar dependencias y entender el funcionamiento.

## Funcionalidad

### Emisor:
- Envía un mensaje con contador por LoRa cada 5 segundos
- Muestra por pantalla: `Enviando paquete número: X`

### Receptor:
- Escucha mensajes por LoRa de forma continua
- Muestra en pantalla:
  - Último mensaje recibido
  - RSSI (potencia de la señal)
  - Contador de paquetes recibidos
 
### Para más detalles sobre la configuración consulta la documentación.
