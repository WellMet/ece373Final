

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "switch.h"
#define leftmotor OC4RS
#define rightmotor OC2RS
#define PRESSED 0
#define RELEASED 1


volatile unsigned int right, middle, left = 0;

typedef enum stateTypeEnum {
    forward, idle1, wait, backward, idle2, debouncePress, debounceRelease,
            turnRight, turnLeft, findTrack, scan, check

} stateType;
volatile stateType state, nextState;
//adc1 < 55 = on black (right)
//adc2 < 30 = on black (middle)
//adc < 55 = on black (left)

//adc1 >= 80 = white
//adc2 >= 40 = white
//adc3 >= 80 = white

int main(void) {
    SYSTEMConfigPerformance(40000000);


    initTimer2();
    initTimer3();
    initLCD();
    initPWM();
    enableInterrupts();
    initADC();
   // initSW1();
    char c[9];
    char adc[9];
    double v = 0;
    
    //led 1
    TRISDbits.TRISD0 = 0;
    LATDbits.LATD0 = 0;
    
    //led 3
    TRISDbits.TRISD2 = 0;
    LATDbits.LATD2 = 0;

    state = forward;

    
    while (1) {
        switch (state) {
            case forward:
                //both motors forward

                
                //set speeds
                OC2RS = 600;
                OC4RS = 600;
                
                LATDbits.LATD0 = 0;
                LATDbits.LATD2 = 0;
  
                state = scan;
                break;
                
            case scan:
                AD1CON1bits.SAMP = 1;
                while(AD1CON1bits.SAMP == 1); //wait for the adc to sample and convert 3 times
                    state = check;
                
                break;
                
            case check:
                if(right >= 80 && middle <= 80 && left >= 25)
                {
                    state = forward;
                }

                //if 3  blacks at T, turn right
                    else if(right <= 100 && middle <= 100 && left <= 15)
                {
                    state = turnRight;
                }

                //if 3 white, off track, circle to find where to go
                    else if(right >= 100 && middle >= 100 && left >= 25)
                {
                    state = findTrack;
                }

                //if left white, middle and right are black turn right
                    else if(right <= 100 && middle <=100 && left >= 25)
                {
                    state = turnRight;
                }

                //if left white, middle and right are black turn right
                    else if(right >= 100 && middle <= 100 && left <= 15)
                {
                    state = turnLeft;
                }
                break;
            case findTrack:
                AD1CON1bits.SAMP = 1;
                while(AD1CON1bits.SAMP == 1); //wait for the adc to sample and convert 3 times
                    if(right >= 80 && middle <= 80 && left >= 25){
                        state = forward;
                    }
                    else if(right <= 100 && middle <= 100 && left <= 15){ //turn right on T
                        OC2RS = 0;
                       OC4RS = 800;

                       LATDbits.LATD0 = 0;
                       LATDbits.LATD2 = 1;                        
                    }
                    else if(right <= 100 && middle <=100 && left >= 25){ //turn right
                        OC2RS = 0;
                       OC4RS = 800;

                       LATDbits.LATD0 = 0;
                       LATDbits.LATD2 = 1;  
                    }
                    else if(right >= 100 && middle <= 100 && left <= 15){
                        //set speeds
                        OC2RS = 0;
                        OC4RS = 800;

                        LATDbits.LATD0 = 0;
                        LATDbits.LATD2 = 1;                         
                    }
               
                break;
                
            case turnRight:
                
                //set speeds
                OC2RS = 0;
                OC4RS = 800;
                
                LATDbits.LATD0 = 0;
                LATDbits.LATD2 = 1; 
                state = scan;
                
                break;
                
            case turnLeft:
                
                //set speeds
                OC2RS = 800;
                OC4RS = 0;
                
                LATDbits.LATD0 = 1;
                LATDbits.LATD2 = 0;
                state = scan;
                
                break;
        }
    }

    return 0;
}

void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void) {

    IFS0bits.AD1IF = 0;
    AD1CON1bits.SAMP = 0;
    right = ADC1BUF0; // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
    middle = ADC1BUF1;
    left = ADC1BUF2;
}