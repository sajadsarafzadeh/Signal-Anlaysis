# include <TimerOne.h>
# include <MsTimer2.h>

#define PWMOUT 5   
#define ADC_CHANNEL A0

float DUTYCYCLE;
const int SINE_FREQ = 23;      // generated sine wave's frequency 
const int SINE_GEN_FREQ = 500;       //Frequency of the sine wave generation system
const float SINE_COEFF = SINE_FREQ*TWO_PI/SINE_GEN_FREQ, AMP = 2.5/5.00*255, OFFSET = 2.5/5.00*255;      


const int SAMPLING_FREQ = 120; // sampling frequency is 120Hz
const float TOT_SAMPLE_TIME = 1;   //Total  sampling time 
const int NUMBER_OF_SAMPLES = TOT_SAMPLE_TIME*SAMPLING_FREQ;
int SAMPLES[NUMBER_OF_SAMPLES];
long COUNTER, SAMPLING_COUNTER;
bool FINISHED = false;

void setup() {
 for (int i = 0; i < NUMBER_OF_SAMPLES; i++) { // building an array to put datas in it
   SAMPLES[i] = -1;   //equalizing all the cells in our array to -1 to make sure sampling goes appropriate
 }

 TCCR0B = TCCR0B & B11111000 | B00000001; //sets timer 0 frequency to 62.5kHz
 Serial.begin(57600);    // Serial is put to 57600 bits/s
 pinMode(PWMOUT,OUTPUT); 
 MsTimer2::set(1000/SINE_GEN_FREQ, PWM_DT);   // using timer2 to generate the signal
 MsTimer2::start();
 Timer1.initialize(1000000/SAMPLING_FREQ);  // usuing timer1 to sampling the signal
 Timer1.attachInterrupt(SAMPLING);
}

void loop() { 
 if (FINISHED == true) {
  for (int i = 0; i < NUMBER_OF_SAMPLES; i++) { // if FINISHED is equal to true we print the data
    Serial.println(SAMPLES[i]/1023.00*5);
 } 
 FINISHED =false;
 }
}

void PWM_DT() {
 DUTYCYCLE = AMP*sin(SINE_COEFF*COUNTER) + OFFSET; // generate the sinusoidal signal
 analogWrite(PWMOUT,DUTYCYCLE);
 COUNTER++; 
 
}
void SAMPLING() {
  if (SAMPLING_COUNTER == NUMBER_OF_SAMPLES) {    // if it gets N samples it would stop gathering more samples
    MsTimer2::stop();

    Timer1.detachInterrupt();
    FINISHED = true; //  FINISHED == true to print data points in Serial monitor
  }
  else {
    SAMPLES[SAMPLING_COUNTER] = analogRead(ADC_CHANNEL);   // if it hasnt reached N samples it continues 
    SAMPLING_COUNTER++;

  } 
}