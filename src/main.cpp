#include "mbed.h"

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial pc(SERIAL_TX, SERIAL_RX,9600);


DigitalOut myled(LED1);
typedef struct{
    float testFloat;
} sAns;

void callback_ex() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    // char ab = 44;
//    float tFloat=540.07;
//    char *TempMSV2= new char[sizeof(float)];
//    memcpy(&TempMSV2[0], &tFloat, sizeof(float));
pc.putc(pc.getc()+1);

    int tInt=500;
    char *TempMSV2= new char[sizeof(int)];
    memcpy(&TempMSV2[0], &tInt, sizeof(int));
    
    for(int i=0; i<6; ++i){ //request INA
        myled = !myled;
        wait(0.2);
    }
    char bc[]={80,88,55};
//    pc.puts(bc);
    pc.puts(&TempMSV2[0]);
delete[] TempMSV2;
//    pc.putc(ab);
    // pc.putc(pc.getc()+1);
//    myled = !myled;
}

int main()
{
    pc.format(8, SerialBase::None, 1);
    pc.attach(&callback_ex);
    int i = 0;
    
    int tInt3=600;
    char *TempMSV3= new char[sizeof(int)];
    memcpy(&TempMSV3[0], &tInt3, sizeof(int));
    
    
    // pc.printf("Hello World !\n");

    // int tInt3=500;
    // char *TempMSV3= new char[sizeof(int)];
    // memcpy(&TempMSV3[0], &tInt3, sizeof(int));
    // pc.puts(&TempMSV3[0]);
    // pc.printf("TempMSV3[0] = %x\n", TempMSV3[0]);
    // pc.printf("TempMSV3[1] = %x\n", TempMSV3[1]);
    // pc.printf("TempMSV3[2] = %x\n", TempMSV3[2]);
    // pc.printf("TempMSV3[3] = %x\n", TempMSV3[3]);

    char arr[]={0,1,2};
    // char* TempMSV = "";
    // sprintf(TempMSV, "%d", arr[1]);
    char MSV[10]={0, };
    while(false){}
    while(false) {
//        wait(0.1);
        //pc.printf("This program runs since %d seconds.\n", i++);
//        myled = !myled;
        myled=0;
        int count =0;
        if(pc.readable()) {
            MSV[0] = pc.getc();
            if(MSV[0]==120){
                for(int i=0; i<6; ++i){ //request INA
                    myled = !myled;
                    wait(0.2);
                }
            }
//            if(MSV[1]==100){
//                for(int i=0; i<6; ++i){ //request INA
//                    myled = !myled;
//                    wait(1);
//                }
//            }
//            pc.puts(&TempMSV3[0]);

//            pc.printf("test\n\r"); 
//            while(pc.readable()){
//                MSV[count++] = pc.getc();
//                if(count==1){
//                    pc.putc('100');
//                }else if(count==2){
//                     pc.putc('102');
//                }else if(count==3){
//                     pc.putc('103');
//                }
                pc.puts(&TempMSV3[0]);
//                wait(0.3);
////                pc.printf("test5\n\r");   
//             }
//             pc.puts(&MSV[0]);
//            
//            pc.printf("strlen(MSV)%d", strlen(MSV));
//            pc.printf(":");
//            pc.printf("%d,", MSV[0]);
//            pc.printf("%d,", MSV[1]);
//            pc.printf("%d,", MSV[2]);
            for(int i=0; i<6; ++i){
                myled = !myled;
                wait(0.2);
            }
//            count=0;
         
//            myled=1;
//            pc.printf("%d", pc.getc());
////            while(!'\n' && !'\0'){
//                MSV[count]=pc.getc();
////                wait(0.1);
////                pc.printf("%d-%d", i, MSV[i]);
//                pc.printf("%d", MSV[i]);
//                count++;
//            }
            
        }

            
//            pc.printf("e7\n\r");
//            myled=0;
//            pc.printf("strlen(MSV)%d", strlen(MSV));
//            for(int k=0; k<strlen(MSV); ++k){
//                myled=1;
//                pc.printf("p%d", MSV[k]);
//                myled=0;
//            }

//        pc.printf("%d\n", (int)700);

//        if(pc.readable()) {
//            pc.printf("%d", pc.getc());
//            while(!'\n' && !'\0'){
//                MSV[i]=pc.getc();
//                i++;
//            }
//            i=0;
////            if(pc.getc() == 0x32){
//            if(MSV[0] == 0x32){
//                //pc.puts(arr);
////                pc.putc((char)arr[1]);
////                pc.printf("%d\n", (int)700);
////                pc.printf("%s\n", TempMSV);
////                pc.write(arr, 3,NULL, 1);
//                pc.printf("%d", (char)60);
//            }
//            
//            for(int i=0; i<6; ++i){
//                myled = !myled;
//                wait(0.2);
//            }
//            myled=1;
//
////            pc.putc(pc.getc()+1);
//            pc.printf("Hello World !\n");
//            pc.printf("%d\n", (int)500);
////            pc.putc(pc.getc()+1);
//            //device.putc(pc.getc());
//            //printf("= %s =", pc.getc());
////            pc.putc(pc.getc());
////            pc.printf("= %c =", pc.getc());
//        }else{myled=0;}
    }
}
//
//
//Serial device(MBED_CONF_APP_UART1_TX, MBED_CONF_APP_UART1_RX);  // tx, rx
//
//int main() {
//    while(1) {
//        if(pc.readable()) {
//            device.putc(pc.getc());
//        }
//        if(device.readable()) {
//            pc.putc(device.getc());
//        }
//    }
//}