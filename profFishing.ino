/*
  Programa de control de motor stepper (Profundizador 1.0)
*/

// Include the AccelStepper library:
#include <AccelStepper.h>

/*
    Métodos librería AccelStepper
    Para ejecutar los movimientos, estos métodos serán suficientes para hacer funcionar nuestros motores paso a paso.

    - moveTo ( position ) – Definir el movimiento del motor hasta una posición en una referencia absoluta.
    - move( steps ) – Definir el movimiento del motor hasta una posición en una referencia relativa.
    - run() – Actualización y ejecución del movimiento en cada iteración del bucle.
    - stop() – Parada del motor.
    Mientras se ejecuta el movimiento podemos obtener información del sistema durante su transcurso con los siguientes métodos.

    - currentPosition() – posición absoluta en la que se encuentra el sistema actualmente.
    - targetPosition() – posición objetivo.
    - speed() – Velocidad actual del sistema.
    - distanceToGo() – Distancia o diferencia entre la posición objetivo y la posición actual

    Antes de definir el movimiento o si quisieramos modificarlo, podemos establecer otros parámetros como:

    - setMaxSpeed( vel ) – Velocidad máxima. El sistema no superará este valor de velocidad en ningún momento. Velocidades mayores de 1000 son impredecibles. [ steps/s ]
    - setAcceleration( acc ) – Aceleración del sistema en [ steps/s^2 ]
    - setSpeed( vel ) – Velocidad del sistema [ steps/s ]

*/

// Include the libraries for LCD screen
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

/*
   LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows)
   Función constructor, crea un objeto de la clase LiquidCrystal_I2C, con dirección, columnas y filas indicadas.

    - init()
    Inicializa el modulo adaptador LCD a I2C, esta función internamente configura e inicializa el I2C y el LCD.

    - clear()
    Borra la pantalla LCD y posiciona el cursor en la esquina superior izquierda (posición (0,0)).

    - setCursor(col, row)
    Posiciona el cursor del LCD en la posición indicada por col y row(x,y); es decir, establecer la ubicación en la que se mostrará posteriormente texto escrito para la pantalla LCD.

    - print()
    Se desplaza el contenido de la pantalla (texto y el cursor) un espacio a la derecha.

    - backlight();
    Enciende la Luz del Fondo del LCD

    - noBacklight();
    Apaga la Luz del Fondo del LCD

    - createChar (num, datos)
    Crea un carácter personalizado para su uso en la pantalla LCD. Se admiten hasta ocho caracteres de 5x8 píxeles (numeradas del 0 al 7). Dónde: num es el número de carácter y datos es una matriz que contienen los pixeles del carácter. Se verá un ejemplo de esto más adelante.
*/

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
const byte dirPin = 4;
const byte stepPin = 5;
#define motorInterfaceType 1 // stepper mostor controled with a driver

//constantes hardware
const byte buttonUP = 10;
const byte buttonDN = 11;
const byte buttonOSC = 12;
const byte ledPin   = 13;

// Las variables cambiarán para el boton Ocilador/Vaiven:
byte buttonState = 0; // variable para LECTURA del estado interruptor OSC
byte variable_buttonState = 0; // variable para ALMACENAR el estado del interriptor OSC
byte OSC_status = 0;

// Setting the LCD address to 0x27 for a 16 chars and 2 lines display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
// AccelStepper stepper = AccelStepper(1,2,3);


void setup() {

  // Ininiciar LCD
  lcd.init();

  // Encender retroiluminación
  lcd.backlight ();

  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 1, 0 );

  // Imprimir “Hola Mundo” en la primera linea
  lcd.print("PROFUNDIZADOR");

  // Mover el cursor a la segunda linea (1) primera columna
  lcd.setCursor ( 3, 1 );

  // Imprimir otra cadena en esta posicion
  lcd.print("PABLITO 1.0");

  // Esperar un segundo
  delay(1000);

  lcd.clear();

  // Set harware function
  pinMode(buttonUP, INPUT_PULLUP);
  pinMode(buttonDN, INPUT_PULLUP);
  pinMode(buttonOSC, INPUT);
  
  pinMode(ledPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(1000);
  // Set the aceleration in steps per second^2:
  stepper.setAcceleration(100);
  // Set the current position to 0:
  stepper.setCurrentPosition(0);

  Serial.begin(9600);
}

void loop() {
  frenoSeguridad();

  //Convertir la distancia a un valor de tres digitos, 000, 001 m, 002 ....

  /* char altura[100];
    sprintf(altura, "%3d,((stepper.currentPosition() / 200) * 6));
    serial.println(altura);
    delay(1000);
  */

  /* Guarda la posicion actual 200 pasos 1 vuelta y 1 vuelta = 15 cm/ 1,00 m = XX vueltas */
  
  // Imprimir encabezado
  lcd.setCursor(0, 0);
  lcd.print("Altura || UP&DWN");
  lcd.setCursor(11, 1);
  lcd.print((stepper.currentPosition() / 200) * 0.16); // conversion de vueltas a metros (1 vuelta es 200 pasos) [6 vueltas son 1 metro = 1 vuelta son 0.16 metros)
  lcd.setCursor(7, 1);
  lcd.print("||");
  

  if (variable_buttonState == 1) {
    lcd.setCursor(0, 3);
    lcd.print("On");
  }

  else {
    lcd.setCursor(0, 2);
    lcd.print("Off");
  }
  delay (500);

  if (digitalRead(buttonUP == LOW)) {
    Serial.println("Arriba");
    stepper.move(600);
    stepper.run();
  }
  else if (digitalRead(buttonDN == LOW)) {//200 pasos 1 vuelta (calcular cuantas vueltas para medio metro)
    Serial.println("Arriba");
    lcd.clear();
    stepper.move(-600);
    stepper.run();
  }
  osciladorAltura();


  delay(50);
  // Si la altura supera los 300 metros = 60000 pasos NEGATIVOS, parar el motor.
  // Y si llega a 0 igualmente parar el motor.
  /* else if (stepper.currentPosition() > 300*200){
    stepper.stop();
    }
  */

}

void frenoSeguridad() {

  if (stepper.currentPosition() == 0) {
    stepper.stop();
  }
  else (stepper.currentPosition() > 60000); {
    stepper.stop();
  }
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
    Serial.println("Vaiven activado");
    // turn led on & oscilate motor
    stepper.move(-1200);
    stepper.run();
    delay(500);
    stepper.move(1200);
    stepper.run();
    digitalWrite(ledPin, HIGH);
  }
  else {
    Serial.println("Vaiven desactivado");
    // turn LED off:
    digitalWrite(ledPin, LOW);
    stepper.stop();
  }
}
