#include <IRremote.h>

int pinReceptorIR = 2;
int pinZumbador = 6;
int pinLedRecepcion = 5;

String mensajeMorse = "";
const int tiempoBase = 150; // Tiempo base en milisegundos para un punto
const uint8_t REPEAT_REQUEST_COMMAND = 0x08; // Comando para solicitar repetición

void setup() {
  Serial.begin(9600);
  pinMode(pinZumbador, OUTPUT);
  pinMode(pinLedRecepcion, OUTPUT);
  IrReceiver.begin(pinReceptorIR, DISABLE_LED_FEEDBACK);
  Serial.println("Esperando el código Morse...");
}

void loop() {
  if (IrReceiver.decode()) {
    manejarSenalRecibida(IrReceiver.decodedIRData.command);
    postProcesarSenal();
  }
}

void postProcesarSenal() {
  delay(150);
  IrReceiver.resume(); // Recibir el siguiente valor
}

void manejarSenalRecibida(uint8_t comando) {
  digitalWrite(pinLedRecepcion, HIGH);
  switch (comando) {
    case 0x02:
      mensajeMorse += '.';
      break;
    case 0x03:
      mensajeMorse += '-';
      break;
    case 0x05: // Pausa letras
      mensajeMorse += ' ';
      break;
    case 0x06: // Pausa palabras
      mensajeMorse += " / ";
      break;
    case 0x07: // Fin de transmisión
      finalizarTransmision();
      break;
    case REPEAT_REQUEST_COMMAND:
      solicitarRepeticion();
      break;
  }
  digitalWrite(pinLedRecepcion, LOW);
}

void solicitarRepeticion() {
  Serial.println("Error en la transmisión. Solicitud de repetición.");
  // Aquí puedes añadir cualquier lógica para manejar la solicitud de repetición.
}

void finalizarTransmision() {
  digitalWrite(pinLedRecepcion, HIGH);
  Serial.println(mensajeMorse);
  String mensajeTraducido = traducirMorse(mensajeMorse);
  Serial.println(mensajeTraducido);

  if (mensajeTraducido == "SOS") {
    emitAlertSound();
  } else if (mensajeTraducido == "TIC") {
    emitCustomSound();
  } else {
    tone(pinZumbador, 500, tiempoBase * 2); // Indica fin de mensaje
  }
  
  digitalWrite(pinLedRecepcion, LOW);
  mensajeMorse = ""; // Limpia el mensaje Morse recibido para la próxima transmisión
}

String traducirMorse(String codigoMorse) {
  String resultado = "";
  String palabraMorse = "";
  for (int i = 0; i < codigoMorse.length(); i++) {
    if (codigoMorse.charAt(i) == ' ') {
      if (palabraMorse != "") {
        resultado += convertirMorse(palabraMorse);
        palabraMorse = "";
      }
    } else if (codigoMorse.charAt(i) == '/') {
      resultado += " ";
    } else {
      palabraMorse += codigoMorse.charAt(i);
    }
  }
  if (palabraMorse != "") {
    resultado += convertirMorse(palabraMorse);
  }
  return resultado;
}

char convertirMorse(String palabraMorse) {
  if (palabraMorse == ".-") return 'A';
  if (palabraMorse == "-...") return 'B';
  if (palabraMorse == "-.-.") return 'C';
  if (palabraMorse == "-..") return 'D';
  if (palabraMorse == ".") return 'E';
  if (palabraMorse == "..-.") return 'F';
  if (palabraMorse == "--.") return 'G';
  if (palabraMorse == "....") return 'H';
  if (palabraMorse == "..") return 'I';
  if (palabraMorse == ".---") return 'J';
  if (palabraMorse == "-.-") return 'K';
  if (palabraMorse == ".-..") return 'L';
  if (palabraMorse == "--") return 'M';
  if (palabraMorse == "-.") return 'N';
  if (palabraMorse == "---") return 'O';
  if (palabraMorse == ".--.") return 'P';
  if (palabraMorse == "--.-") return 'Q';
  if (palabraMorse == ".-.") return 'R';
  if (palabraMorse == "...") return 'S';
  if (palabraMorse == "-") return 'T';
  if (palabraMorse == "..-") return 'U';
  if (palabraMorse == "...-") return 'V';
  if (palabraMorse == ".--") return 'W';
  if (palabraMorse == "-..-") return 'X';
  if (palabraMorse == "-.--") return 'Y';
  if (palabraMorse == "--..") return 'Z';
  if (palabraMorse == ".----") return '1';
  if (palabraMorse == "..---") return '2';
  if (palabraMorse == "...--") return '3';
  if (palabraMorse == "....-") return '4';
  if (palabraMorse == ".....") return '5';
  if (palabraMorse == "-....") return '6';
  if (palabraMorse == "--...") return '7';
  if (palabraMorse == "---..") return '8';
  if (palabraMorse == "----.") return '9';
  if (palabraMorse == "-----") return '0';

  return '?'; // Carácter no reconocido
}

void emitAlertSound() {
  // Emitir un sonido de alerta para "SOS"
  tone(pinZumbador, 1000); // Emitir un tono de 1000 Hz
  delay(1000); // Mantener el tono por 1 segundo
  noTone(pinZumbador); // Apagar el buzzer
}

void emitCustomSound() {
  // Emitir un sonido personalizado para "TIC"
  tone(pinZumbador, 1500); // Emitir un tono de 1500 Hz
  delay(500); // Mantener el tono por 0.5 segundos
  noTone(pinZumbador);
  delay(200);
  tone(pinZumbador, 2000); // Emitir un tono de 2000 Hz
  delay(500); // Mantener el tono por 0.5 segundos
  noTone(pinZumbador);
}




