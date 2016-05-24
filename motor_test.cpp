#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <ctime>

#include "AQMD3620NS-A.h"
#include "serialport.h"

#define portname  "/dev/ttyUSB0"
int main()
{
    int motor = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        std::cout<<"hello01"<<std::endl;
    if (motor < 0)
    {
        printf("error opening the device\n");
    }

    if(set_interface_attribs(motor, B9600, 0,2)!=0)//此处有修改，stopbit is 2bit
    {            printf("hello");
        printf("error set interface\n");
    }
    else
    if(set_blocking(motor, 0)!=0)
    {
        printf("error set blocking\n");
    }
    else
        printf("done\n");

    if( set_interface_attribs (motor, B9600, 0,2) )
    { // then set_interface_attribs failed
        return -1;
    }
    // implied else set_interface_attribs successful
    if( set_blocking (motor, 0) )                // set no blocking
    { // then set_blocking failed
        return -1;   // might need to also restore oldtty attributes
    }
    // implied else, set_blocking successful

    //main part of the programe,type 'j' to speed up;type 'k' to speed down;type an integer between -2000~2000 to set speed straightly.
    //To make the motor stop, you can type in any word except ‘j' 'k' and number. For example, 'p' or '$%*&gkjfd' or only a whitespace.
    //You can type in "exit" to exit.
    std::cout<<"input an integer to set initial speed,input 'j' to speed up;'k' to speed down;\n \"exit\" to exit;any other input will stop."<<std::endl;
    char adjust_in[10];
    int speed=0;

    while(1){

        std::cin>>adjust_in;
        //printf("input is %s \n",adjust_in);
        if     (adjust_in[0]=='j'){speed++;printf("speed is %d\n",speed);}
        else if(adjust_in[0]=='k'){speed--;printf("speed is %d\n",speed);}
        else if(0==strcmp(adjust_in,"exit")){break;}
        else if((adjust_in[0]>='0' and adjust_in[0]<='9' ) or adjust_in[0]=='-'){
            sscanf(adjust_in,"%d",&speed);
            printf("set speed %d\n",speed);
        }
        else{
            speed=0;
            std::cout<<"the motor has stopped\n"<<std::endl;
        }
        set_speed(motor,speed);
        usleep(3000);
    }
    //close the serial port
    tcsetattr (motor, TCSANOW, &oldtty);

    std::cout<<"Disconnected.Exit successfully."<<std::endl;
    return 0;
}
