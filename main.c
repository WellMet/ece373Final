//
//
//#include <xc.h>
//#include <sys/attribs.h>
//#include "config.h"
//#include "interrupt.h"
//#include "timer.h"
//#include "lcd.h"
//#include "adc.h"
//#include "pwm.h"
//#include "switch.h"
//#define leftmotor OC4RS
//#define rightmotor OC2RS
//#define PRESSED 0
//#define RELEASED 1
//
//
//volatile unsigned int val = 0;
//
//typedef enum stateTypeEnum {
//    forward, turnLeft, turnRight, T_forward, T_backward, TurnAround
//
//} stateType;
//volatile stateType state, nextState;
//volatile int count = 0;
//
//
//int main(void) {
//    SYSTEMConfigPerformance(40000000);
//
//    
//
//    initTimer2();
//    initTimer3();
//    initLCD();
//    initPWM();
//    enableInterrupts();
//    initADC();
//    //initSW1();
//      
//    state = forward;
//
//    while (1) {
//
//        testLCD();
//        switch (state) {
//            case forward:
//                                
//                PINRD1 = 11; //0b1011;
//                PINRB14 = 0;
//                PINRC4 = 11; // 0b1011
//                PINRF5 = 0;
//            
//                 OC2RS = 800;
//                 OC4RS = 800;
//                  
//                break;
//            
//            case turnLeft:
//     
//                 OC2RS = 800;
//                 OC4RS = 300;
//                
//                break;
//                
//            case turnRight:
//                
//                 OC2RS = 300;
//                 OC4RS = 800;
//                
//                break;
//            
//            case T_forward:
//              
//                 OC2RS = 800;
//                 OC4RS = 800;
//                
//                break;
//                
//            case T_backward:
//                
//                PINRD1 = 0; //0b1011;
//                PINRB14 = 11;
//                PINRC4 = 0; // 0b1011
//                PINRF5 = 11;
//                
//                OC2RS = 500;
//                OC4RS = 500;
//                
//                delayUs(5000);
//                
//                PINRD1 = 11; //0b1011;
//                PINRB14 = 0;
//                PINRC4 = 11; // 0b1011
//                PINRF5 = 0;
//                
//                OC2RS = 500;
//                OC4RS = 500;
//                
//                state = turnRight;
//                
//                break;
//                
//            case TurnAround:
//                
//                PINRD1 = 0; //0b1011;
//                PINRB14 = 11;
//                PINRC4 = 11; // 0b1011
//                PINRF5 = 0;
//                
//                OC2RS = 500;
//                OC4RS = 500;
//                
//                delayUs(5000);
//                
//                state = forward;
//            
//                  break;
//                       
//        }
//
//    }
//
//    return 0;
//}

//void __ISR(_ADC_VECTOR, IPL6AUTO) _ADCInterrupt(void) {
//
//    IFS0bits.AD1IF = 0;
//    val = ADC1BUF0; // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
//    
//    if(state == T_forward)
//    {
//        OC2RS = 800;
//        OC4RS = 800;
//    }
//        
//    if('3 blacks (T))')
//    {
//        state = T_forward;
//    }
//
//    if('3 what spaces')
//    {
//        if(count == 2)
//        {
//            state = TurnAround;
//        }
//        else
//        {
//            state = T_backward;
//        }
//    }
//
//    if('middle  = black , left, right are white')
//    {
//        count = count +1;
//
//        state = forward;
//    }
//
//    if (val > 460 && val < 562) { //full speed in the middle
//        OC2RS = 1023;
//        OC4RS = 1023;
//    } else if (val <= 460) {
//        OC2RS = 1023;
//        OC4RS = 2 * val;
//    } else if (val >= 562) {
//        OC2RS = 2 * (1023 - val);
//        OC4RS = 1023;
//    }
//}

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


volatile unsigned int val = 0;

typedef enum stateTypeEnum {
    forward, idle1, wait, backward, idle2, debouncePress, debounceRelease

} stateType;
volatile stateType state, nextState;
volatile double val1, val2, val3 = 0;


int main(void) {
    SYSTEMConfigPerformance(40000000);


    initTimer2();
    initTimer3();
    initLCD();
    initPWM();
    enableInterrupts();
    initADC();
    initSW1();
    char c[9];
    char adc[9];
    double v = 0;
    


    state = forward;

    while (1) {
     
        if(IFS0bits.AD1IF == 1){
            
            IFS0bits.AD1IF = 0;
            int adc1 = ADC1BUF0;
            int adc2 = ADC1BUF2;
            int adc3 = ADC1BUF4;
            
            val1 = 3.3*adc1/1023;
            val2 = 3.3*adc2/1023;
            val3 = 3.3*adc3/1023;
            int dummy = 0;
//            val = 500; // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
//
//            if (val > 460 && val < 562) { //full speed in the middle
//                OC2RS = 1023;
//                OC4RS = 1023;
//            } else if (val <= 460) {
//                OC2RS = 1023;
//                OC4RS = 2 * val;
//            } else if (val >= 562) {
//                OC2RS = 2 * (1023 - val);
//                OC4RS = 1023;
//            }
       }
        //OC2RS = 500;
        //OC4RS = 500;

        switch (state) {
            case forward:
                PINRD1 = 11; //0b1011;
                PINRB14 = 0;
                PINRC4 = 11; // 0b1011
                PINRF5 = 0;
                
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD("forward");
                
                
                break;
            
            case idle1:
                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD("idle1");
                
                break;
                
            case backward:
                PINRD1 = 0; //0b1011;
                PINRB14 = 11;
                PINRC4 = 0; // 0b1011
                PINRF5 = 11;
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD("backward");
                
                break;
            
            case idle2:
                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;

                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD("idle2");
                break;
                
            case debounceRelease:
                delayUs(5000);
                state = nextState;
                break;
        }
    }

    return 0;
}



void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
    IFS1bits.CNDIF = 0; // Put Interrupt flag down
    int dummysw1 = PORTDbits.RD6;
    
    if (SW1 == RELEASED && state == forward) {
        nextState = idle1;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == idle1) {
        nextState = backward;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == backward) {
        nextState = idle2;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == idle2) {
        nextState = forward;
        state = debounceRelease;
    }
}