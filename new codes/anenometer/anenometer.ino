 //============================================================================
volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;
int hallsensor = 2;    //The pin location of the sensor

unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;
 
void rpm ()     //This is the function that the interupt calls
{
    NbTopsFan++;  //This function measures the rising and falling edge of the
 
   // hall effect sensors signal
}
// The setup() method runs once, when the sketch starts
void setup() //
{
    pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
    Serial.begin(9600); //This is the setup function where the serial port is
 
   // initialised,
    attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}
// the loop() method runs over and over again,
// as long as the Arduino has power
void loop ()
{
   
  flowsensor();
   
 
}
void flowsensor(){
    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    cli();      //Disable interrupts
   // Calc = (NbTopsFan * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate
 // in L/hour
 Calc = (NbTopsFan/3);
    Serial.println (Calc, DEC); //Prints the number calculated above
   // Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
   previousMillis = currentMillis;
   NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
    sei();      //Enables interrupts
  }
  }
