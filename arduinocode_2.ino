#include <TimerOne.h>
#include <MsTimer2.h>

#define PWMOUT 5
#define ADC_CHANNEL A0


float DUTYCYCLE, SAMPLE;
float frequency_low = 4;        //calculated with student number = 810698254so the low frequancy is 4Hz
float frequency_high = 61;      // The high frequancy is 61 Hz
const int  SINE_GEN_FREQ = 1000 ;              //Frequency of the sine wave generation system
long TIMESTAMP, counter=0;
float SINE_COEFF1 = frequency_low*TWO_PI/1000.0, Amp=1.25/5.0*255, offset=2.5/5.0*255;
float SINE_COEFF2 = frequency_high*TWO_PI/1000.0;

//sampling
const int SAMPLING_FREQ = 200  ;  // sampling frequency    -> f_sampling >= 2*f_maximum =2*61 = 122 Hz 
const float TOTAL_SAMPLING_TIME = 4;  //Total  sampling time       
const int NUMBER_OF_SAMPLES = TOTAL_SAMPLING_TIME*SAMPLING_FREQ;     //limitaion of 800 points maximum. 200 (Hz)*4(seconds)= 800 
int  SAMPLES[NUMBER_OF_SAMPLES];
long  SAMPLING_COUNTER ;
bool FINISHED=false;

void setup() {
  for (int i = 0 ; i < NUMBER_OF_SAMPLES ; i++){  // making an array to put datas in it
    SAMPLES[i]=-1;   // equalize all the cells in  array to -1 to make  sure sampling goes appropriate
  }
  
TCCR0B = TCCR0B & B11111000 | B00000001; //sets timer 0 frequency to 62.5kHz
pinMode(PWMOUT,OUTPUT);
Serial.begin(57600);
MsTimer2::set(1000/SINE_GEN_FREQ, PWM_DT);  // usuing timer2 to change dutycycle
MsTimer2::start();
Timer1.initialize(1000000/SAMPLING_FREQ);   // using timer1 to sampling the signal
Timer1.attachInterrupt(SAMPLING);
}

void loop() {
 if (FINISHED == true) {   // if FINISHED == true  print the data
 for (int i = 0; i < NUMBER_OF_SAMPLES; i++) {
 Serial.println(SAMPLES[i]/1023.00*5);
 }
 FINISHED = false;
 }
}

void PWM_DT() {
DUTYCYCLE = Amp*(sin(SINE_COEFF1*counter)+sin(SINE_COEFF2*counter)) + offset; //generating sinusoidal signal
analogWrite(PWMOUT,DUTYCYCLE);
counter++;
}
void SAMPLING(){
  if (SAMPLING_COUNTER == NUMBER_OF_SAMPLES){     // if it gets N samples it stops 
     MsTimer2::stop();
     Timer1.detachInterrupt();
     FINISHED = true;    // FINISHED == true to print data points in Serial monitor
  }
  else{
    SAMPLES[SAMPLING_COUNTER]=analogRead(ADC_CHANNEL);   // if it hasnt reached N samples it continues 
    SAMPLING_COUNTER++;

  }
}
