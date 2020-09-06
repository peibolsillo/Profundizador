/*
  Programa de control de motor stepper (Profundizador 1.0)
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

//constantes hardware
const byte buttonUP = 6;
const byte buttonDN = 7;
const byte buttonOSC = 8;
const byte ledPin   = 12;




// Las variables cambiarán:

/*
   En este punto estás declarando una varriable función, pero nunca la inicializas.
   Si es una variable --> no debe de tener () y la debes de asignarle un valor antes de usarla o hacer una comprobacion de null 
   Si es una función --> debes de indicar que quieres que haga cuando se invoque ( antes de invocarlo)
*/
//const int currentPosition();

byte buttonState = 0; // variable para LECTURA del estado interruptor OSC
byte variable_buttonState = 0; // variable para ALMACENAR el estado del interriptor OSC
byte OSC_status = 0;

//setting the LCD address to 0x27 for a 16 chars and 2 lines display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
// AccelStepper stepper = AccelStepper(1,2,3);
void setup() {

  // Indicar a la libreria que tenemos conectada una pantalla de 16x2
  lcd.begin(16, 2);
  // Mover el cursor a la primera posición de la pantalla (0, 0)
  lcd.home ();
  // Imprimir “Hola Mundo” en la primera linea
  lcd.print("PROFUNDIZADOR");
  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 0, 1 );
  // Imprimir otra cadena en esta posicion
  lcd.print("PABLITO 1.0");
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
  
  //guarda la posicion actual 200 pasos 1 vuelta y 1 vuelta = 15 cm/ 1,00 m = XX vueltas

  // Actualizar la pantalla completa cada segundo
  lcd.clear();

  // Imprimir encabezado

  lcd.print("Altura || Oscila");
  lcd.setCursor(0, 1);
  /*
    Aquí es donde me ha saltado el error.
    "undefined reference to `currentPosition()'"
    El error es claro, estás utilizando un método/función que no cotiene nada, el valor de el es "undefined".
    El simple hecho de comentar esta línea hace que compile sin problemas.
    Si quieres trazar la posición actual deberás de utilizar un método o variable y asignarle valor o función antes de invocarla.
  */
  //lcd.print((currentPosition() / 200) * 6);
  lcd.setCursor(0, 8);
  lcd.print(" || ");
  lcd.setCursor(0, 12);

  if (variable_buttonState == 1) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("On");
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Off");
  }
  delay (500);

  if (buttonUP == LOW) {
    stepper.move(600);
  }
  else if (buttonDN == LOW) //200 pasos 1 vuelta (calcular cuantas vueltas para medio metro)
    stepper.move(-600);

  osciladorAltura();
  
  delay(50);

}

void osciladorAltura() {


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
    stepper.move(-1200);
    delay(500);
    stepper.move(1200);
    digitalWrite(ledPin, HIGH);
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
