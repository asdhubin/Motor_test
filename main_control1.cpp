#include <unistd.h>   //_getch
#include <termios.h>  //_getch
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <ctime>//时间戳
#include <pthread.h>//多线程
//下面是自己写的头文件
#include "AQMD3620NS-A.h"
#include "serialport.h"
#include "odometer.h"
#include "stepper_controller.h"
//按键定义
#define KEY_LEFT		0x44
#define KEY_RIGHT		0x43
#define KEY_UP          0x41
#define KEY_DOWN        0x42
#define WHITE_SPACE     0x20
#define q               0x71

//Windows中的getch()在Linux的实现
char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

int main(){
    /*----------------------------连接步进电机-----------------------------------*/
    int stepper = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (stepper < 0)
    {
        printf("error opening the stepper\n");
    }

    if(set_interface_attribs(stepper, B9600, 0,1)!=0)//此处stopbit is 1 bit
    {
        printf("error set stepper interface\n");
    }
    else
    if(set_blocking(stepper, 0)!=0)
    {
        printf("error set stepper blocking\n");
    }
    else
        printf("stepper done\n");

    if( set_interface_attribs (stepper, B9600, 0,1) )
    { // then set_interface_attribs failed
        return -1;
    }
    if( set_blocking (stepper, 0) )                // set no blocking
    {
        return -1;
    }
    /*----------------------------结束连接步进电机-------------------------------*/
    /*-------------------设定步进电机参数-----------------------*/
    set_stepper_address(stepper,3);
    printf("press enter\n");std::cin.get();
    led_on(stepper);
    printf("press enter\n");std::cin.get();
    led_off(stepper);
    printf("press enter\n");std::cin.get();
    set_mode(stepper,0);
    printf("press enter\n");std::cin.get();
    set_stop_mode(stepper,2);
    printf("press enter\n");std::cin.get();
    set_return_home(stepper,0);
    printf("press enter\n");std::cin.get();
    set_subdivision_angle(stepper,8,180);
    printf("press enter\n");std::cin.get();
    set_distance(stepper,6000);
    printf("press enter\n");std::cin.get();
    set_direction_and_start_frequency(stepper,1,50);
    printf("press enter\n");std::cin.get();
    set_frequency_acceleration_and_speed(stepper,50,1000);
    printf("press enter\n");std::cin.get();
    /*-------------------设定步进电机参数结束-----------------------*/


    float direction=0;
    float speed=0;
    char temp=0;
    bool flag=true;
    printf("\033[4mpress white space to stop,press q to exit.\033[0m\n");
    do{
        temp=getch();
        system("clear");
        printf("\033[4mpress white space to stop,press q to exit.\033[0m\n");
        //printf("%x hahaha\n",temp);
            switch(temp){
            case KEY_LEFT:  if(direction<=60 and direction>=-50){
                                direction=direction-10;
                                set_direction_and_start_frequency(stepper,1,50);//设定方向
                                usleep(800000);
                                run_stepper(stepper);//前轮转向
                            }
                        else printf("\033[31;1m%s\033[0m \n","Left  limited");
                        break;
            case KEY_RIGHT:  if(direction<=50 and direction>=-60){
                                direction=direction+10;
                                set_direction_and_start_frequency(stepper,0,50);//设定方向
                                usleep(800000);
                                run_stepper(stepper);//前轮转向
                             }
                        else printf("\033[31m%s\033[0m \n","Right limited");
                        break;
            case KEY_UP:  if(speed<=290 and speed>=-300){speed+=10;}
                        else printf("\033[34m%s\033[0m \n","Speed limited");
                        break;
            case KEY_DOWN:  if(speed<=300 and speed>=-290){speed-=10;}
                        else printf("\033[34m%s\033[0m \n","Speed limited");
                        break;
            case WHITE_SPACE:speed=0;
                        printf("Stop.\n");
                        break;
            case q:speed=0;
                    flag=false;
                    printf("exit.");
                    break;
            }
        printf("\033[33;1m%s %f\033[0m \n","Direction:",direction);
        printf("\033[33;1m%s %f\033[0m \n","Speed:",speed);
    }while(flag);
    return 0;
 }
