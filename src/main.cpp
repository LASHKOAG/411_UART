#include "mbed.h"
 
DigitalIn       btn(USER_BUTTON);
DigitalOut      led(LED1);
RawSerial           pc(SERIAL_TX, SERIAL_RX,9600);
volatile char   c = '\0'; // Initialized to the NULL character

bool flag = false;
#define FLAG_TEST (1U << 0)
EventFlags event_flags;
char MSV[100]={10, };
char MSVsend[100]={10, };
//char MSV[10]=" ";
//int counters = 0;
char counters = 0;
bool interrupted = false;
// void onCharReceived()
// {
//     c = pc.getc();
// }

//void onCharReceived()
//{
//    event_flags.set(FLAG_TEST);
////    pc.putc(pc.getc()+1);
//    c = pc.getc();
////        MSV[0] = c;
////    while(pc.readable ()){
////          MSV[counters++] = pc.getc();  
////    }
//    MSV[counters++] = c;
////        MSV = c;
////    pc.putc(c);
////    flag=true;
////counters=0;
//}

void rx_interrupt() {
    char ch;
    if(pc.readable()){  //Determine if there is a character available to read
        ch = pc.getc(); //Read char from terminal
            //int charIndex = (ch - '0') - 49;  //Conversion from char to int where letter 'a' is 0;
        MSV[counters++] = ch;
    }
    event_flags.set(FLAG_TEST);
    return;
}

int main()
{
    for(int i=0; i<6; ++i){
        led = !led;
        wait(0.2); 
    }
    pc.format(8, SerialBase::None, 1);
        // pc.attach(callback(rx_interrupt), Serial::RxIrq);
    pc.attach(&rx_interrupt, Serial::RxIrq);
    // pc.attach(&onCharReceived);
 
    while (true)
    {
        event_flags.wait_all(FLAG_TEST);
        ThisThread::sleep_for(10);              //необходим таймаут

        for(int i=0; i <counters; ++i){
            pc.putc(MSV[i]);
        }  
        memset( &MSV[0], 0xFF, 100 );
        counters=0;
        

        
        if(MSV[1]==0x20){
            led=1;
        }

        if(MSV[0]==0x41){
            led=0;
        }
        // if(flag){
        //     led=1;
        // }

        if (c == '1')
        {
            c = '\0';  // To avoid execution of this block until a '1' is received again.
            led = 1;
        }
        
        if (c == '01')
        {
            c = '\0';  // To avoid execution of this block until a '1' is received again.
            led = 1;
        }
 
        if (c == '0')
        {
            c = '\0';  // To avoid execution of this block until a '0' is received again.
            led = 0;
        }
 
        if (btn == 0)
        {
            pc.printf("b");
        }
    }
}