// BIBLIOTECAS //

#include <Servo.h>
#include <DHT.h>

// ETIQUETAS //

#define pinServo 3
#define pinBotonEmergencia 5
#define pinBotonPastillero 6
#define pinBotonDHT 7
#define pinLEDRojo 9
#define pinLEDAzul 10
#define pinLEDVerde 11
#define pinSensorDHT 2

#define DHTTYPE DHT11   // DHT 11

#define TIEMPO_TOMA_PASTILLA 200

#define POS_ABIERTA 90
#define POS_CERRADA 0

DHT dht(pinSensorDHT, DHTTYPE);
//CONECTAR -:gnd // S:DHTPIN //Medio:5V
 
Servo miServomotor;

// VARIABLES // 

int posicionServo = POS_CERRADA ;
 
int contadorTiempoPastillero = TIEMPO_TOMA_PASTILLA;
 
int valorBotonDHT = 0;
int valorBotonPastillero = 0;
int valorBotonEmergencia = 0;
int estadoLedTitila = LOW;
 
 
void setup()

{ 
  
// INICIO DE SERIE //

  Serial.begin(9600);
 
// INICIO DE ENTRADAS-SALIDAS //
  
  dht.begin();
  miServomotor.attach(pinServo);
  
  pinMode (pinBotonEmergencia, INPUT);
  pinMode (pinBotonPastillero, INPUT);
  pinMode (pinBotonDHT, INPUT);
  pinMode (pinLEDRojo, OUTPUT);
  pinMode (pinLEDVerde, OUTPUT);
  pinMode (pinLEDAzul, OUTPUT);

// ESTADOS INICIALES //

  digitalWrite(pinLEDRojo,LOW);
  digitalWrite(pinLEDVerde,LOW);
  digitalWrite(pinLEDAzul,LOW);
  
}
 
 
void loop()
{
  // ENTRADAS (Botones-Sensores)//
  
  valorBotonEmergencia = digitalRead(pinBotonEmergencia);  
  valorBotonPastillero = digitalRead(pinBotonPastillero);
  valorBotonDHT = digitalRead(pinBotonDHT);
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
 
//------------------------------------------------------------//
  
  // CÓDIGO EMERGENCIA //
  
  if (valorBotonEmergencia == HIGH) // Si el botón de emergencia se presiona,
  {
    //digitalWrite(pinLEDRojo,HIGH);
    digitalWrite(pinLEDRojo,estadoLedTitila); // empieza a titilar el led rojo 
    Serial.println("Botón de Emergencia Presionado: Se requiere asistencia"); // y se envía un mensaje por serie.
  }
  else
  {
    digitalWrite(pinLEDRojo,LOW); // Si el botón de emergencia no se presiona, el led queda apagado.
  }
 
//------------------------------------------------------------//

  // CÓDIGO PASTILLERO //
  
  if (contadorTiempoPastillero == 1) // Si la cuenta llega a uno, 
  {
    //digitalWrite(pinLEDAzul,HIGH); 
    posicionServo = POS_ABIERTA;    // el servo se abre
    Serial.println("Tiempo de tomar medicamento!"); // y se envía un mensaje por serie.
 
  }
 
  if (contadorTiempoPastillero == 0) // Si la cuenta llega a cero, 
  { 
    digitalWrite(pinLEDAzul,estadoLedTitila); // empieza a titilar el led azul.
    
    if (valorBotonPastillero == HIGH) // Si el botón se presiona,
    {
      contadorTiempoPastillero = TIEMPO_TOMA_PASTILLA; // el tiempo del contador se reinicia,
      digitalWrite(pinLEDAzul,LOW); //  el led azul se apaga,
      posicionServo = POS_CERRADA; // el servo se cierra 
      Serial.println("MENSAJE: Medicamento tomado."); // y se envía un mensaje por serie.
    }
  }
 
 
//------------------------------------------------------------//
  
  // CÓDIGO SENSOR AMBIENTAL //
  
if (valorBotonDHT == HIGH) // Si el botón se presiona, 

{
    digitalWrite(pinLEDVerde,HIGH); // se enciende el led verde,
    Serial.println("MIDIENDO... "); // se envía un mensaje serie,
    delay(1000);                    // se espera un segundo 
    Serial.print("Humedad: ");
    Serial.print(h);                 // se envía la humedad por un mensaje serie
    Serial.print("%"); 
    Serial.print(" - Temperatura: ");
    Serial.print(t);                 // y se envía la temperatura por un mensaje serie.
    Serial.println("°C ");
  }
  
else //Si el botón no está presionado,
{
  
digitalWrite(pinLEDVerde,LOW); // el led verde se apaga.

}

//------------------------------------------------------------//

  //SALIDA-ACTUADORES (SERVO)
  
  miServomotor.write(posicionServo);
 
//------------------------------------------------------------//

// TEMPORALES //

  delay(100);// Se esperan 100 milisegundos
  
  if (contadorTiempoPastillero > 0) // Si el contador es mayor a 0 se decrementa la cuenta,
  {
    contadorTiempoPastillero = contadorTiempoPastillero - 1; // restando un segundo.
  }
  
// LED TITILA //

  if (estadoLedTitila == HIGH)
  {
      estadoLedTitila = LOW;
  }
  else
  {
      estadoLedTitila = HIGH;
  }

}
