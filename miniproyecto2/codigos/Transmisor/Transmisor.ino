#include <IRremote.h>

const int pin_emisor = 3;
const int pin_zumbador = 6;
const int PIN_LED_EMISION = 5;
const int duracion = 150; // Tiempo base en milisegundos para un punto

IRsend irsend;

void setup() {
  Serial.begin(9600);
  pinMode(pin_emisor, OUTPUT);
  pinMode(pin_zumbador, OUTPUT);
  pinMode(PIN_LED_EMISION, OUTPUT);
  IrSender.begin(pin_emisor, DISABLE_LED_FEEDBACK, 0);
  Serial.println("Ingrese su mensaje");
}

void loop() {
  if (Serial.available() > 0) {
    String texto = obtenerTextoDeSerial();
    imprimirTexto(texto);
    Serial.println("Iniciando Morse:");
    enviarMorse(texto);
  }
}

String obtenerTextoDeSerial() {
  String texto = Serial.readString(); // mensaje que ingresamos en el monitor serial
  return texto;
}

void imprimirTexto(const String& texto) {
  for (int i = 0; i < texto.length(); i++) {
    char c = texto.charAt(i);
    Serial.print(c);
  }
  Serial.println();
}

void enviarMorse(String texto) {
  iniciarTransmision();

  for (int i = 0; i < texto.length(); i++) {
    char c = texto.charAt(i);
    procesarCaracter(c);
  }

  finalizarTransmision();
}

void iniciarTransmision() {
  digitalWrite(PIN_LED_EMISION, HIGH); // LED que se prende cuando se envía mensaje
  IrSender.sendNEC(0x1234, 0x01, 1);
  delay(duracion);
}

void procesarCaracter(char c) {
  if (c == ' ') {
    enviarEspacio();
  } else {
    convertirMorse(c);
    enviarPausaLetra();
  }
}

void enviarEspacio() {
  IrSender.sendNEC(0x1234, 0x06, 1);
  delay(duracion * 7); // Pausa palabras
}

void enviarPausaLetra() {
  IrSender.sendNEC(0x1234, 0x05, 1);
  delay(duracion * 3); // Pausa letras
}

void finalizarTransmision() {
  IrSender.sendNEC(0x1234, 0x07, 1);
  delay(duracion);
  digitalWrite(PIN_LED_EMISION, LOW);
}

void convertirMorse(char c) {
  String codigoMorse = obtenerCodigoMorse(c);
  for (int j = 0; j < codigoMorse.length(); j++) {
    char m = codigoMorse.charAt(j);
    if (m == '.') {
      enviarPunto();
    } else if (m == '-') {
      enviarRaya();
    }
    delay(duracion);
  }
}

String obtenerCodigoMorse(char c) {
  switch (c) {
    case 'a': case 'A': return ".-";
    case 'b': case 'B': return "-...";
    case 'c': case 'C': return "-.-.";
    case 'd': case 'D': return "-..";
    case 'e': case 'E': return ".";
    case 'f': case 'F': return "..-.";
    case 'g': case 'G': return "--.";
    case 'h': case 'H': return "....";
    case 'i': case 'I': return "..";
    case 'j': case 'J': return ".---";
    case 'k': case 'K': return "-.-";
    case 'l': case 'L': return ".-..";
    case 'm': case 'M': return "--";
    case 'n': case 'N': return "-.";
    case 'o': case 'O': return "---";
    case 'p': case 'P': return ".--.";
    case 'q': case 'Q': return "--.-";
    case 'r': case 'R': return ".-.";
    case 's': case 'S': return "...";
    case 't': case 'T': return "-";
    case 'u': case 'U': return "..-";
    case 'v': case 'V': return "...-";
    case 'w': case 'W': return ".--";
    case 'x': case 'X': return "-..-";
    case 'y': case 'Y': return "-.--";
    case 'z': case 'Z': return "--..";
    case '1': return ".----";
    case '2': return "..---";
    case '3': return "...--";
    case '4': return "....-";
    case '5': return ".....";
    case '6': return "-....";
    case '7': return "--...";
    case '8': return "---..";
    case '9': return "----.";
    case '0': return "-----";
    default: return "";
  }
}

void enviarPunto() {
  IrSender.sendNEC(0x1234, 0x02, 1);
  tone(pin_zumbador, 1000, duracion); // Emitir un tono de 1000 Hz
  delay(duracion);
}

void enviarRaya() {
  IrSender.sendNEC(0x1234, 0x03, 1);
  tone(pin_zumbador, 500, duracion * 3); // Emitir un tono de 500 Hz
  delay(duracion * 3);
}

