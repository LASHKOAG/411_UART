/**********************************************************************************
* @file    main.cpp
* @author  Vaclav Moravcik
* @version V1.0
* @date    3-May-2015
* @brief   Morse code beacon controlled through UART.
***********************************************************************************/

/* Includes ----------------------------------------------------------------------*/
#include "mbed.h"

/* Defines -----------------------------------------------------------------------*/

/* Function prototypes -----------------------------------------------------------*/
void enqueue(int x);
int dequeue();
void Rx_interrupt();
void broadcastChar(int charIndex);
void pressed();

/* Structures --------------------------------------------------------------------*/
struct Node {
    int data;
    struct Node* next;
};

/* Variables ---------------------------------------------------------------------*/ 
float unitDuration = 0.3;  //Duration of one unit affects the speed of the transmission 
int morseTable[26][4] = {0,1,2,0,
                      1,0,0,0,
                      1,0,1,0,
                      1,0,0,2,
                      0,2,0,0,
                      0,0,1,0,
                      1,1,0,2,
                      0,0,0,0,
                      0,0,2,0,
                      0,1,1,1,
                      1,0,1,2,
                      0,1,0,0,
                      1,1,2,0,
                      1,0,2,0,
                      1,1,1,2,
                      0,1,1,0,
                      1,1,0,1,
                      0,1,0,2,
                      0,0,0,2,
                      1,2,0,0,
                      0,0,1,2,
                      0,0,0,1,
                      0,1,1,2,
                      1,0,0,1,
                      1,0,1,1,
                      1,1,0,0}; //Morse code - 0 is dot, 1 is dash, 2 is end of letter
int durationTable[5] = {1,3,5,7,9};  //dot, dash, pause btw signs, pause btw letters, pause bwt words and sentences
bool interrupted = false;
bool paused = true; 
struct Node* front = NULL;  // Two glboal variables to store address of front and rear nodes.
struct Node* rear = NULL;
int queueSize = 0;

//mbed - initialization of peripherals
Serial pc(SERIAL_TX, SERIAL_RX);
DigitalOut led(LED1);
InterruptIn mybutton(USER_BUTTON);

/* Functions----------------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : enqueue.
* Description    : Enqueues an integer.
* Input          : x: integer value of character that will be enqueued.
* Output         : none.
* Return         : none.
*******************************************************************************/
void enqueue(int x) {
    struct Node* temp = 
        (struct Node*)malloc(sizeof(struct Node));
    temp->data = x; 
    temp->next = NULL;
    if(front == NULL && rear == NULL){
        front = rear = temp;
        queueSize++;
        return;
    }
    rear->next = temp;
    rear = temp;
    queueSize++;
}

/*******************************************************************************
* Function Name  : dequeue.
* Description    : Dequeues an integer.
* Input          : none.
* Output         : charIndex: Index of a char that is stored in queue.
* Return         : none.
*******************************************************************************/ 
int dequeue() {
    int charIndex;
    struct Node* temp = front;
    if(front == NULL) {
        return NULL;
    }
    if(front == rear) {
        front = rear = NULL;
    }
    else {
        front = front->next;
    }
    charIndex = temp->data;
    free(temp);
    queueSize--;
    return charIndex;
}

/*******************************************************************************
* Function Name  : rx_interrupt.
* Description    : Interupt Routine to read in data from serial port.
* Input          : none.
* Output         : charIndex: Index of a char that is stored in queue.
* Return         : none.
*******************************************************************************/
void rx_interrupt() {
    char ch;
    // Inform user and stop enqueueing if buffer is full
    if(queueSize > 512){
        pc.printf("Queue is full! Unpause broadcasting if paused and wait a moment before sending another message.\n");
        return;
    }
    interrupted = true;
    //pc.printf("readable %d\n", pc.readable());   
    if(pc.readable()){  //Determine if there is a character available to read
        ch = pc.getc(); //Read char from terminal
        int charIndex = (ch - '0') - 49;  //Conversion from char to int where letter 'a' is 0;
        if((charIndex > -1 && charIndex < 26) || charIndex == -51 || charIndex == -65){ //If its not a new line or carriage return character
            //Uncomment to echo back written characters
            //pc.printf("Enqueued char: %c\n", ch);
            enqueue(charIndex);        
        }
    }
    interrupted = false;
    return;
}

/*******************************************************************************
* Function Name  : broadcastChar.
* Description    : Takes chararcter and broadcasts it by turning LED on and off.
* Input          : charIndex: Index of a char that is stored in queue.
* Output         : none.
* Return         : none.
*******************************************************************************/
void broadcastChar(int charIndex){
    int sign;
    if(charIndex > -1 && charIndex < 26){    
        for(int i = 0; i < 4; i++){
            sign = morseTable[charIndex][i];
            if(sign != 2){
                led = 1;
                wait(unitDuration*durationTable[sign]);
                led = 0;
                wait(unitDuration*1);   //Pause between signs in a letter.
            }
            else{
                break;
            }
        }
        led = 0;
        wait(unitDuration*durationTable[2]);   //Pause at the end of a letter.
    }
    if(charIndex == -51){
        led = 0;
        wait(unitDuration*durationTable[4]);   //Pause at the end of a sentence.        
    }
    if(charIndex == -65){
        led = 0;
        wait(unitDuration*durationTable[3]);   //Pause at the end of a word.        
    }    
}

/*******************************************************************************
* Function Name  : pressed.
* Description    : Interupt Routine to pause broadcast when button is pressed.
* Input          : none.
* Output         : none.
* Return         : none.
*******************************************************************************/
void pressed()
{
    paused = !paused;
}

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main() {
    
    // Setup a serial interrupt function to receive data
    pc.attach(&rx_interrupt, Serial::RxIrq);
    // Setup an interrupt function to detect pressed button
    mybutton.fall(&pressed);
    
    //Initialization
    led = 1;
    wait(5);
    led = 0;
    int charIndex;
    pc.printf("Type message to broadcast it with LED in morse code. Allowed characters are [a - z], 'SPACCE' and '.' (Other characters will be ignored).\n");
    pc.printf("Morse code broadcasting is now paused. Press button on STM Nucleo to start the broadcasting of the message you sent.\n");
    
    //While loop that broadcast messages
    while(1) {
        if(!paused && !interrupted && queueSize > 0){
            charIndex = dequeue();
            pc.printf("%c", (char)(charIndex+97));  //Echoing of letter that is being currently broadcasted
            broadcastChar(charIndex);
        }
    }
}
 