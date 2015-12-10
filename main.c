#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "switch.h"
#include "i2c.h"
#include "prox.h"
#define leftmotor OC4RS
#define rightmotor OC2RS
#define PRESSED 0
#define RELEASED 1


volatile unsigned int right, middle, left, proxInt = 0;
volatile char proxChar;

typedef enum stateTypeEnum {
    forward, aggForward, turnRight, turnLeft, findTrack, scan, check

} stateType;
volatile stateType state, nextState;

int main(void) {
    SYSTEMConfigPerformance(40000000);


    initTimer2();
    initTimer3();
    initLCD();
    initPWM();
    enableInterrupts();
    initADC();
    initSW1();
    initI2C2();
    int wait =0;
    char c[9];
    char adc[9];
    double v = 0;

    state = forward;
    int i = 0;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD2 = 0; //enable LEDs
   
    while (1) {
        //fake interrupt
        
        right = scanADC(0);
        middle = scanADC(2);
        left = scanADC(4);
        proxChar = i2cRead(PROX_I2C_ADD, PROX_DIST_ADD);
        proxInt = (int)(proxChar);
            
        if(right >= 100 && middle < 150 && left >= 200 && (proxInt > 40 && proxInt < 1000))
        {
            state = forward;
        }
        
        else if(right >= 100 && middle < 150 && left >= 200 && (proxInt < 40 && proxInt > 0)) //speed up
        {
            state = aggForward;
        }

        //if 3  blacks at T, turn right
            else if(right < 100 && middle < 150 && left < 200)
        {
            state = turnRight;
        }

        //if 3 white, off track, circle to find where to go
            else if(right >= 100 && middle >= 150 && left >= 200)
        {
            state = findTrack;
        }

        //if left white, middle and right are black turn right
            else if(right < 100 && middle < 150 && left >= 200)
        {
            state = turnRight;
        }

        //if left white, middle and right are black turn right
            else if(right >= 100 && middle < 150 && left < 200)
        {
            state = turnLeft;
        }

        switch (state) {
            case forward:
                //both motors forward
 
                //set speeds
                OC2RS = 800;
                OC4RS = 800;
                
                LATDbits.LATD0 = 0;
                LATDbits.LATD2 = 0;
                
                break;
                
            case aggForward:
                //both motors forward
 
                //set speeds
                OC2RS = 1023;
                OC4RS = 1023;
                
                LATDbits.LATD0 = 1;
                LATDbits.LATD2 = 1;
                
                break;
                
            case findTrack:
                OC2RS = 600;
                OC4RS = 200;
                
                
                LATDbits.LATD0 = 1;
                LATDbits.LATD2 = 0;
                break;
                
            case turnRight:
                
                //set speeds
                OC2RS = 200;
                OC4RS = 600; 
                
                LATDbits.LATD0 = 0;
                LATDbits.LATD2 = 1;
                
                
                break;
                
            case turnLeft:
                
                //set speeds
                OC2RS = 600;
                OC4RS = 200;
                
                LATDbits.LATD0 = 1;
                LATDbits.LATD2 = 0;
                
                
                break;
        }
    }

    return 0;
}