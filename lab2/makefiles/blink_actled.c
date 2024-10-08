// The blue ACT LED is controlled by GPIO PD18
// From the D-1 manual, we can obtain the address of
// the peripheral registers for the PD group

void main(void) {
    volatile unsigned int *PD_CFG2 =  (unsigned int *)0x2000098;
    volatile unsigned int *PD_DATA  = (unsigned int *)0x20000a0;

   *PD_CFG2 = (*PD_CFG2 & ~(0xf << 8)) | (1 << 8);  // configure PD18 for output

    while (1) {
        *PD_DATA ^= (1 << 18);    // invert PD18
        for (int c = 0x2f00000; c != 0; c--) ;  // countdown loop to wait
   }
}
