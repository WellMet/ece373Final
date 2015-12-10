#include <xc.h>




void initADC(){
    
//    TRISBbits.TRISB0 = 1;
//    TRISBbits.TRISB2 = 1;
//    TRISBbits.TRISB4 = 1;
        
    ANSELBbits.ANSB0 = 1;
    ANSELBbits.ANSB2 = 1;
    ANSELBbits.ANSB4 = 1;
    
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0; // Use board refernece voltages
    AD1CON2bits.CSCNA = 0; // disable scanning
    AD1CON2bits.SMPI = 15; // 1 burrito
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 31; // 2 Tad per sample
    AD1CON3bits.ADCS = 4; // 4 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0SA = 0; // Scan AN0 at least
    IFS0bits.AD1IF = 0; // Put down ADC flag
    
   // AD1CSSLbits.CSSL0 = 1;
   // AD1CSSLbits.CSSL2 = 1;
  //  AD1CSSLbits.CSSL4 = 1;
    
//    IPC5bits.AD1IP = 7;
//    IEC0bits.AD1IE = 1;
    AD1CON1bits.ADON = 1; // turn on the ADC  
}

int scanADC(int s){
    IFS0bits.AD1IF = 0;
    AD1CON1bits.ADON = 0;
    AD1CHSbits.CH0SA = s; //select an0
    AD1CON1bits.ADON = 1;
    while(IFS0bits.AD1IF == 0);
    int dummy = ADC1BUF0;
    IFS0bits.AD1IF = 0;
    AD1CON1bits.ADON = 0;
    
    return dummy;
}