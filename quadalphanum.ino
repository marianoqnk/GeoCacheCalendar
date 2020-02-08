// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "LowPower.h"



#define INTENTOS 5
#define T_RESPUESTA 8000


#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha5 = Adafruit_AlphaNum4();
//const int interruptPin = 2;
const int selectPin = 2;
const int enterPin = 3;
const char semanario[][9] = {"DOMINGO", "LUNES", "MARTES", "MIERCOLE", "JUEVES", "VIERNES", "SABADO"};
int despertado = 0;

const char bb[] = "TIENES QUE ACERTAR 3 DE 5 AVERIGUA QUE DIA DE LA SEMANA FUE...";
int jugador = 0;
void writeMensaje(char displaybuffer[]);

void setup() {
  Serial.begin(9600);
  alpha4.begin(0x70);  // pass in the address
  alpha5.begin(0x71);
  alpha4.setBrightness(4);
  alpha4.clear();
  alpha4.writeDisplay();
  alpha5.setBrightness(4);
  alpha5.clear();
  alpha5.writeDisplay();
  pinMode(selectPin, INPUT);
  pinMode(enterPin, INPUT);
  scroll("Hola");
  delay(1000);

  Serial.println("Start typing to display!");
}
void escribeNumero(int n)
{
  char cadena[6];
  sprintf(cadena, "%i", n);
  scroll(cadena);
}
void ruleta(char bb[])
{
  byte n, k, t;
  char displaybuffer[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  for (n = 0; n < 8; n++)
  {
    for (k = 0; k < 9; k++)
    {
      for (t = n; t < 8; t++)displaybuffer[t] = random('0', '9');
      writeMensaje(displaybuffer);
      delay(50);
    }
    displaybuffer[n] = bb[n];
  }
  writeMensaje(displaybuffer);
}
void scroll(char bb[])
{
  byte n;
  char displaybuffer[8] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  for (n = 0; bb[n] != '\0'; n++)
  {
    displaybuffer[0] = displaybuffer[1];
    displaybuffer[1] = displaybuffer[2];
    displaybuffer[2] = displaybuffer[3];
    displaybuffer[3] = displaybuffer[4];
    displaybuffer[4] = displaybuffer[5];
    displaybuffer[5] = displaybuffer[6];
    displaybuffer[6] = displaybuffer[7];
    displaybuffer[7] = bb[n];
    writeMensaje(displaybuffer);
    delay(200);

  }
}

void writeMensaje(char displaybuffer[])
{
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);
  alpha5.writeDigitAscii(0, displaybuffer[4]);
  alpha5.writeDigitAscii(1, displaybuffer[5]);
  alpha5.writeDigitAscii(2, displaybuffer[6]);
  alpha5.writeDigitAscii(3, displaybuffer[7]);
  alpha4.writeDisplay();
  alpha5.writeDisplay();

}

//char displaybuffer[8] = {' ', ' ', ' ', ' '};

void loop() {
  char fecha[9];
  byte diaSemana = 0, intentos = 0;
  bool cambia_frecuencia1 = false;
  bool cambia_frecuencia2 = false;
  scroll(bb);
  //escribeNumero(despertado);
  //delay(2000);
  for (byte k = 0; k < INTENTOS; k++)
  {
    int mes = random(1, 12);
    int dia = random(1, 28);
    int anno = random(1492, 2020);
    sprintf(fecha, "%02i%02i%4i", dia, mes, anno);
    ruleta(fecha);
    //blink(HT16K33_BLINK_HALFHZ);
    unsigned long desde = millis();
    attachInterrupt(digitalPinToInterrupt(enterPin), wakeUp, RISING );
    attachInterrupt(digitalPinToInterrupt(selectPin), wakeUp, RISING );
    while (1)
    {
      // while (!digitalRead(enterPin) && !digitalRead(selectPin) && T_RESPUESTA > (millis() - desde))blink(3);
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      /*{//9000/3  6000  3000
        if(!cambia_frecuencia2 && (2*T_RESPUESTA/3) >(millis() - desde))
        {
          alpha4.blinkRate(HT16K33_BLINK_2HZ);
          alpha5.blinkRate(HT16K33_BLINK_2HZ);
          cambia_frecuencia2=true;
        }
        else if(!cambia_frecuencia1 && (T_RESPUESTA/3) >(millis() - desde))
        {
          alpha4.blinkRate(HT16K33_BLINK_1HZ);
          alpha5.blinkRate(HT16K33_BLINK_1HZ);
          cambia_frecuencia1=true;
        }


        }*/ //poner tiempo
      /*alpha4.blinkRate(HT16K33_BLINK_OFF);
        alpha5.blinkRate(HT16K33_BLINK_OFF);*/
      if (digitalRead(selectPin))(diaSemana < 6) ? diaSemana++ : diaSemana = 0;
      else break; //pulsado enter
      scroll(semanario[diaSemana]);
      desde = millis();
    };

    if (DiaSemana(anno, mes, dia) == diaSemana) {
      scroll("Correcto");
      intentos++;

    } else {
      scroll("Error fue");
      scroll(semanario[DiaSemana(anno, mes, dia)]);
    }
    //delay(2000);
    sprintf(fecha, "%02i/%02i", intentos, INTENTOS);
    scroll(fecha);
    delay(2000);

  }
  if (intentos >= 3)scroll("La clave es 149");
  else scroll("PRUEBA OTRA VEZ");
  delay(2000);

  alpha4.sleep();
  alpha5.sleep();
  attachInterrupt(digitalPinToInterrupt(enterPin), wakeUp, HIGH);
  attachInterrupt(digitalPinToInterrupt(selectPin), wakeUp, HIGH);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(enterPin));
  detachInterrupt(digitalPinToInterrupt(selectPin));
  despertado++;
  //delay(4000);
  alpha4.normal();
  alpha5.normal();
  //delay(2000);
}
void wakeUp(void)
{

}


int DiaSemana (int anyo, int mes, int dia) {

  int a, y, m, diaSemana;

  a = (14 - mes) / 12;
  y = anyo - a;
  m = mes + (12 * a) - 2;
  diaSemana = (dia + y + (y / 4) - (y / 100) + (y / 400) + (31 * m) / 12) % 7;


  return diaSemana;
}

void blink(byte periodo)
{
  unsigned static long timer1 = millis();
  static bool blink_on = true;
  unsigned long retardo;

  switch (periodo)
  {
    case 0:
      alpha4.normal();
      alpha5.normal();
      return;
    case 1:
      retardo = 500;
      break;
    case 2:
      retardo = 1000;
      break;
    case 3:
      retardo = 2000;
      break;
    default:
      alpha4.normal();
      alpha5.normal();
      return;
  }
  if (retardo > ( millis() - timer1))
  {
    if (blink_on)
    {
      blink_on = false;
      alpha4.sleep();
      alpha5.sleep();
    } else
    {
      blink_on = true;
      alpha4.normal();
      alpha5.normal();
    }
    timer1 = millis();
  }
}
