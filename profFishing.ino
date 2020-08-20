/* Programa de control de motor stepper (Profundizador 1.0)
*
*/

// Include the AccelStepper library:
#include <AccelStepper.h>
// Include the libraries for LCD screen
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
const byte dirPin = 4;
const byte stepPin = 5;
#define motorInterfaceType 1 // stepper mostor controled with a driver
const int stepCount = 0;    // variable para contar pasos del motor

//constantes hardware
const byte buttonUP = 6;
const byte buttonDN = 7;
const byte buttonOSC= 8;
const byte ledPin   = 12;

// Las variables cambiarán:
byte buttonState = 0; // variable para LECTURA del estado interruptor OSC
byte variable_buttonState = 0; // variable para ALMACENAR el estado del interriptor OSC
byte OSC_status = 0;


// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
// AccelStepper stepper = AccelStepper(1,2,3);
void setup() {
  
  // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.begin(16, 2);
  // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.home ();
  // Imprimir “Hola Mundo” en la primera linea
  lcd.print(“PROFUNDIZADOR”);
  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 0, 1 );
  // Imprimir otra cadena en esta posicion
  lcd.print(“PABLITO 1.0”);
  // Esperar un segundo
  delay(1000);
    
  // Set harware function
  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(buttonDN, INPUT_PULLUP);
  pinMode(buttonOSC, INPUT);
  
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(2000);
  // Set the current position to 0:
  stepper.setCurrentPosition(0);
}
void loop() { 
    stepCount = currentPosition(); //guarda la posicion actual 200 pasos 1 vuelta y 1 vuelta = 15 cm/ 1,00 m = XX vueltas
    
  // Actualizar la pantalla completa cada segundo
  lcd.clear();
  // Imprimir encabezado

  lcd.print(“Altura || Oscila”);
  lcd.setCursor(0, 1);
  lcd.print(currentPosition()/3000);
  lcd.setCursor(0, 8);
  lcd.print(“||”);
  lcd.setCursor(0, 12);
  if (OSC_status == 0){
      lcd.print(“ON”);
      else(){
          lcd.print(“OFF”);
      }
      
  }
  delay (500);

    if(buttonUP == LOW){
      stepper.move(6000);
    }
    else if(buttonDN == LOW) //200 pasos 1 vuelta (calcular cuantas vueltas para medio metro) 
      steper.move(-6000);
    }  
  delay(50);
}

void osciladorAltura(){


// read the state of the pushbutton value:
buttonState = digitalRead(buttonOSC);

// check if the pushbutton is pressed.
// if it is, the buttonState is HIGH:
if (buttonState == HIGH) {
// turn LED on:
variable_buttonState = !variable_buttonState;
//waiting time so we’re not changing state several times with only push
delay(300);
}

    if (variable_buttonState == HIGH) {
        // turn led on & oscilate motor
        OSC_status = 1;
        stepper.move(-5000);
        delay(500);
        stepper.move(5000);
        digitalWrite(ledPin, HIGH);
    }
    else {
        // turn LED off:
        OSC_status = 0
        digitalWrite(ledPin, LOW);
    }

