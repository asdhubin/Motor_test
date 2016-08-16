#include <unistd.h>   //_getch
#include <termios.h>  //_getch
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <ctime>//ʱ���
#include <pthread.h>//���߳�
//�������Լ�д��ͷ�ļ�
#include "AQMD3620NS-A.h"
#include "serialport.h"
#include "odometer.h"
#include "stepper_controller.h"
//��������
#define KEY_LEFT		0x44
#define KEY_RIGHT		0x43
#define KEY_UP          0x41
#define KEY_DOWN        0x42
#define WHITE_SPACE     0x20
#define q               0x71

//Windows�е�getch()��Linux��ʵ��
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

int initialize_stepper(int stepper){
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
}
int main(){
    /*----------------------------���Ӳ������-----------------------------------*/
    int stepper = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (stepper < 0){printf("error opening the stepper\n");}
    if(set_interface_attribs(stepper, B9600, 0,1)!=0){//�˴�stopbit is 1 bit
            printf("error set stepper interface\n");
    }
    else if(set_blocking(stepper, 0)!=0){
        printf("error set stepper blocking\n");
    }
    else   printf("stepper done\n");
    if( set_interface_attribs (stepper, B9600, 0,1) ){ // then set_interface_attribs failed
        return -1;
    }
    if( set_blocking (stepper, 0) ) {               // set no blocking
        return -1;
    }
    /*----------------------------�������Ӳ������-------------------------------*/
    /*----------------------------����ֱ�����--------------------------------------*/
    int motor = open ("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (motor < 0)
    {
        printf("error opening the DC motor.\n");
    }
    if(set_interface_attribs(motor, B9600, 0,2)!=0){//�˴����޸ģ�stopbit is 2bit
        printf("error set DC motor interface\n");
    }
    else if(set_blocking(motor, 0)!=0){
        printf("error set DC motor blocking\n");
    }
    else printf("DC motor done\n");
    if( set_interface_attribs (motor, B9600, 0,2) ){ // then set_interface_attribs failed
        return -1;
    }
    // implied else set_interface_attribs successful
    if( set_blocking (motor, 0) ) {// set no blocking
        return -1;
    }
    /*----------------------------��������ֱ�����-------------------------------*/
    /*-------------------�趨�����������-----------------------*/
    initialize_stepper(stepper);
    /*-------------------�趨���������������-----------------------*/


    float direction=0;
    float speed=0;
    char temp=0;
    char direction_temp=1;//�����־�������жϴ˴�ת�����ϴ�ת���Ƿ���ͬ�������ͬ�Ļ������������������ת������趨��������һ������ʹ����Ӧ�ٶ���������Ϊ1����Ϊ0.
    //dirention_temp�ĳ�ʼֵҪ��initialize_stepper()�������set_direction_and_start_frequency(stepper,1,50);�ϸ񱣳�һ�¡�
    bool flag=true;
    printf("\033[4mpress white space to stop,press q to exit.\033[0m\n");
    do{
        temp=getch();
        system("clear");
        printf("\033[4mpress white space to stop,press q to exit.\033[0m\n");
        //printf("%x hahaha\n",temp);
            switch(temp){
            case KEY_LEFT:  if(direction<=40 and direction>=-30){
                            if(direction_temp!=1){
                                    set_direction_and_start_frequency(stepper,1,50);//�趨����
                                    usleep(800000);//�������ԣ�����ָ��֮�����������0.8s�ļ����ԭ���в���ȷ��
                                    direction_temp=1;
                            }
                                if(0==run_stepper(stepper)){direction=direction-10;};//ǰ��ת��
                            }
                        else printf("\033[31;1;5m%s\033[0m \n","Left  limited");
                        break;
            case KEY_RIGHT:  if(direction<=30 and direction>=-40){
                                if(direction_temp!=0){
                                    set_direction_and_start_frequency(stepper,0,50);//�趨����
                                    usleep(800000);
                                    direction_temp=0;
                                }
                                if(0==run_stepper(stepper)){direction=direction+10;};//ǰ��ת��
                             }
                        else printf("\033[31;1;5m%s\033[0m \n","Right limited");
                        break;
            case KEY_UP:  if(speed<=190 and speed>=-200){
                        speed+=10;
                        set_speed(motor,-speed);
                        usleep(3000);
                        }
                        else printf("\033[34m%s\033[0m \n","Speed limited");
                        break;
            case KEY_DOWN:  if(speed<=200 and speed>=-190){
                        speed-=10;
                        set_speed(motor,-speed);
                        usleep(3000);
                        }
                        else printf("\033[34m%s\033[0m \n","Speed limited");
                        break;
            case WHITE_SPACE:
                        speed=0;
                        set_speed(motor,0);
                        printf("Stop.\n");
                        break;
            case q:
                        speed=0;
                        set_speed(motor,0);
                        flag=false;//�˳���־
                        printf("exit.\n");
                        break;
            }
        printf("\033[33;1m%s %f\033[0m \n","Direction:",direction);
        printf("\033[33;1m%s %f\033[0m \n","Speed:",speed);
    }while(flag);

    tcsetattr (motor, TCSANOW, &oldtty);
    tcsetattr (stepper, TCSANOW, &oldtty);
    return 0;
 }
