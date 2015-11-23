 /* File:   adc.h
 * Author: gvanhoy
 *
 * Created on October 1, 2015, 10:19 AM
 */

#include <xc.h>


void initADC(){
    //TRISBbits.TRISB0=1;// RB0(AN0)
  
    ANSELBbits.ANSB0 = 0; 
    ANSELBbits.ANSB2 = 0; 
    ANSELBbits.ANSB4 = 0; 
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; // Use board refernece voltages
    AD1CON2bits.CSCNA = 1; // Disable scanning
    AD1CON2bits.SMPI = 15; // 15 burritos
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 31; // 2 Tad per sample
    AD1CON3bits.ADCS = 0xFF; // 512 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0SA = 0; // Scan AN8 at least
    AD1CSSLbits.CSSL0=1;
    AD1CSSLbits.CSSL2=1;
    AD1CSSLbits.CSSL4=1;
    IFS0bits.AD1IF = 0;
    //IEC0bits.AD1IE = 1;
    //IPC5bits.AD1IP = 7;
    AD1CON1bits.ADON = 1; // turn on the ADC
}