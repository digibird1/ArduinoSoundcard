char sine[] = {1,2,3};

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

int bytesInBuffer=0;

struct fifo_t {
     char * buf;
     int head;
     int tail;
     int size;
};


char Buffer[1024];
struct fifo_t myFIFO;
  
//This initializes the FIFO structure with the given buffer and size
void fifo_init(struct fifo_t *f, char * buf, int size){
     f->head = 0;
     f->tail = 0;
     f->size = size;
     f->buf = buf;
}


//Read one byte from fifo
int fifo_read(struct fifo_t * f, char &a){

      if( f->tail != f->head ){ //see if any data is available
           a = f->buf[f->tail];  //grab a byte from the buffer
           f->tail++;  //increment the tail
           if( f->tail == f->size ){  //check for wrap-around
                f->tail = 0;
           }
      } else {
           return 0; //number of bytes read 
      }
      return 1;
      
      //returns 0 when no byte was read
      //returns 1 when byte was read
    
}

//This writes up to nbytes bytes to the FIFO
//If the head runs in to the tail, not all bytes are written
//The number of bytes written is returned
int fifo_write(struct fifo_t * f, const char * buf, int nbytes){
     int i;
     const char * p;
     p = buf;
     for(i=0; i < nbytes; i++){
           //first check to see if there is space in the buffer
           if( (f->head + 1 == f->tail) ||
                ( (f->head + 1 == f->size) && (f->tail == 0) )){
                 return i; //no more room
           } else {
               f->buf[f->head] = *p++;
               f->head++;  //increment the head
               if( f->head == f->size ){  //check for wrap-around
                    f->head = 0;
               }
           }
     }
     return nbytes;
}

int i=0;


void setup()
{
 // pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(115200);        // connect to the serial port
  //Serial.println("DDS Test");

  pinMode(6, OUTPUT);      // sets the digital pin as output
  pinMode(7, OUTPUT);      // sets the digital pin as output
  pinMode(11, OUTPUT);     // pin11= PWM  output / frequency output

  Setup_timer2();

  // disable interrupts to avoid timing distortion
 // cbi (TIMSK0,TOIE0);              // disable Timer0 !!! delay() is now not available
  sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt
  
  
  fifo_init(&myFIFO, Buffer, 1024);
  
  
  //Parallel port
  DDRD = DDRD | B11111100;  // this is safer as it sets pins 2 to 7 as outputs
	                  // without changing the value of pins 0 & 1, which are RX & TX 
  PORTD = B00000000; // sets digital pins 7,5,3 HIGH
  
  
  
  delay(1000);

}
void loop()
{
 /* while(1) {
     if (c4ms > 250) {                 // timer / wait fou a full second
      c4ms=0;
      dfreq=analogRead(0);             // read Poti on analog pin 0 to adjust output frequency from 0..1023 Hz

      cbi (TIMSK2,TOIE2);              // disble Timer2 Interrupt
      tword_m=pow(2,32)*dfreq/refclk;  // calulate DDS new tuning word
      sbi (TIMSK2,TOIE2);              // enable Timer2 Interrupt 

      Serial.print(dfreq);
      Serial.print("  ");
      Serial.println(tword_m);
    }

   sbi(PORTD,6); // Test / set PORTD,7 high to observe timing with a scope
   cbi(PORTD,6); // Test /reset PORTD,7 high to observe timing with a scope
  }*/
  
    while (Serial.available () > 0){
      char inByte = Serial.read ();
      fifo_write(&myFIFO,&inByte,1); 
      bytesInBuffer++;
      
      //Serial.println(inByte);
    }
    
    if(bytesInBuffer<200){
      Serial.println("OK");//send Signal for new data
      //Serial.print(bytesInBuffer);
      //Serial.flush();
      //delayMicroseconds(100);
    }
    
    //delay(1000);
  
 }
//******************************************************************
// timer2 setup
// set prscaler to 1, PWM mode to phase correct PWM,  16000000/510 = 31372.55 Hz clock
void Setup_timer2() {

// Timer2 Clock Prescaler to : 1
  sbi (TCCR2B, CS20);
  cbi (TCCR2B, CS21);
  cbi (TCCR2B, CS22);

  // Timer2 PWM Mode set to Phase Correct PWM
  cbi (TCCR2A, COM2A0);  // clear Compare Match
  sbi (TCCR2A, COM2A1);

  sbi (TCCR2A, WGM20);  // Mode 1  / Phase Correct PWM
  cbi (TCCR2A, WGM21);
  cbi (TCCR2B, WGM22);
}

//******************************************************************
// Timer2 Interrupt Service at 31372,550 KHz = 32uSec
// this is the timebase REFCLOCK for the DDS generator
// FOUT = (M (REFCLK)) / (2 exp 32)
// runtime : 8 microseconds ( inclusive push and pop)
ISR(TIMER2_OVF_vect) {

  

  //OCR2A=sine[i/4];

    

  if(i++>=4){
    bytesInBuffer--;
    i=0; //4*1600 
    char a;
    fifo_read(&myFIFO, a);
    OCR2A=a;
    
    a=a*(64./256.);
    a=a<<2;
    //Parallel port
    PORTD=a;
    
  }

}
