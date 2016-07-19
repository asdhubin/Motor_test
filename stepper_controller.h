#ifndef STEPPER_CONTROLLER_H_INCLUDED
#define STEPPER_CONTROLLER_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include "serialport.h"
#include "iostream"

#define STEPPER_ADR 0x03//设备号
/**define stepper_wait_answer_duration 10000
*默认发送命令后到读取返回信息中间，等待1ms
*/
#define stepper_wait_answer_duration 600000
//int fd;//设备号

/*bool checksum(unsigned char rst[7]){
    unsigned char wrong[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77};
    for(int i=0;i<7;i++){
        if(rst[i]!=)
    }
}*/

/**int set_mode(int fd,unsigned char md)
*设定运行模式，自编程模式为00。
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0a
*/
int set_mode(int fd,unsigned char md){
    std::cout<<"setting running mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0a,md,0x00,0x00,0x00,0x00};
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==md){printf("set stepper running mode %x success\n",md);}//返回成功信息
    return 0;
}
/**int set_stop_mode(int fd,unsigned char mode)
*设定停止模式
*1：缓慢停止
*2：立即停止
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0b
*/
int set_stop_mode(int fd,unsigned char mode){
    std::cout<<"setting stop mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0b,mode,0x00,0x00,0x00,0x00};
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==mode){printf("set stop mode %x success\n",mode);}
    return 0;
}
/**int set_return_home(int fd,unsigned char rh)
*0:上电不回零  1：上电回零
*若上电回零功能开启，则每次上电后，自动找零点（此时需要给I4输入点一个低电平信号），找到零点后，才进入设定的运行模式中。
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int set_return_home(int fd,unsigned char rh){
    std::cout<<"setting return home mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,rh,0x00,0x00,0x00,0x00};
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==rh){printf("set return home mode %x success\n",rh);}
    return 0;
}

/**int set_run_mode4(int fd,unsigned char mode)
*设定模式4的运行模式
*0：原来模式（I1或I2有效就运行，无效不会停止运行）
*1：松开就停止模式（I1或I2一直有效就运行，无效就停止运行）
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0d
*/
int set_run_mode4(int fd,unsigned char mode){
    std::cout<<"setting running mode in mode4"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0d,mode,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==mode){printf("set running mode %x in mode4 success\n",mode);}/**/
    return 0;
}

/**int is_in_place(int fd)
*模式0状态下，读是否运行到位
*若到位，返回0；没有到位（即还在运动状态），返回-3
*通信失败返回-1，信号传输中校验错误返回-2。
*命令号 02
*/
int is_in_place(int fd){
    std::cout<<"setting running mode in mode4"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x02,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[5]==0x01){printf("In place,stepper has stopped.");return 0;}
    else if(result[5]==0x00){printf("Not in place,stepper is running.");return -3;}
}

/** set_subdivision_angle(int fd,unsigned short subd,unsigned char angle)
*设定细分和步距角
*subd为angle细分角，angle为步距角*100.例如步距角为1.8，输入的参数是180，十六进制为0xb4.
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 01
*/
int  set_subdivision_angle(int fd,unsigned short subd,unsigned char angle){
    std::cout<<"setting subdivision and step angle"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x01,subd,subd>>8,angle,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x01){printf("set subdivision %d and step angle %x  success\n",subd,angle);}/**/
    return 0;
}
/**int set_distance(int fd,int32_t distance)
*设定运行距离，单位是脉冲个数，虽然用了int数据，但是有效值为0~16777215，三个字节。
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 03
*/
int set_distance(int fd,int32_t distance){
    std::cout<<"setting distance"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x03,distance,distance>>8,distance>>16,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x03){printf("set distance %d success\n",distance);}/**/
    return 0;
}

/**int set_direction_and_start_frequency(int fd,unsigned char dir,unsigned short fre=50)
*设定运行方向和启动频率
*0反转，1正转。默认启动频率为50Hz。
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 04
*/
int set_direction_and_start_frequency(int fd,unsigned char dir,unsigned short fre=50){
    std::cout<<"setting direction and start frequency"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x04,dir,fre,fre>>8,0x00,0x00};
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x04){printf("set direction %x and frequency %d success\n",dir,fre);}/**/
    return 0;
}
/**int set_frequency_acceleration_and_speed(int fd,unsigned short acc=50,unsigned short speed=200)
*设定加减速频率和运行速度，运行速度的单位是RPM
*默认加减速频率50Hz。速度为200rpm。
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 05
*/
int set_frequency_acceleration_and_speed(int fd,unsigned short acc=50,unsigned short speed=200){
    std::cout<<"setting frequency acceleration and speed"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x05,acc,acc>>8,speed,speed>>8,0x00};
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x05){printf("set frequency acceleration %d and speed %d success\n",acc,speed);}/**/
    return 0;
}
/**int stop_stepper(int fd)
*停止命令
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 06
*/
int stop_stepper(int fd){
    std::cout<<"stopping"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x06,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x06){printf("Stepper has stopped.");}
    return 0;
}
/**int run_stepper(int fd)
*开始运行命令（按设定方向，速度，脉冲数运行1次）
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 09
*/
int run_stepper(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x09,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x09){printf("Stepper is running.\n");}
    return 0;
}
/**int keep_stepper_running(int fd)
*正向一直运行（以设定的速度）
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 07
*/
int keep_stepper_running_positive(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x07,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x07){printf("Stepper is keeping running positively.");}
    return 0;
}

/**
*
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 08
*/
int keep_stepper_running_negative(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x08,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x08){printf("Stepper is keeping running negatively.");}
    return 0;
}
/**int led_on(int fd)
*点亮LED灯
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int led_on(int fd){
    std::cout<<"Turning on led..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x00,0x0c,0x05,0x01,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x01){printf("Led is on!");}
    return 0;
}

/**int led_off(int fd)
*熄灭LED灯
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int led_off(int fd){
    std::cout<<"Turning off led..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x00,0x0c,0x05,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x00){printf("Led is off.");}
    return 0;
}
/**int OC1_ON(int fd)
*OC1输出（可外接继电器）ON
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC1_ON(int fd){
    std::cout<<"Turning on OC1 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x02,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x02){printf("OC1 output is on.");}
    return 0;
}
/**int OC1_OFF(int fd)
*OC1输出（可外接继电器）OFF
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC1_OFF(int fd){
    std::cout<<"Turning off OC1 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x03,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x03){printf("OC1 output is off.");}
    return 0;
}
/**int OC2_ON(int fd)
*OC2输出（可外接继电器）ON
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC2_ON(int fd){
    std::cout<<"Turning on OC2 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x04,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x04){printf("OC2 output is on.");}
    return 0;
}
/**int OC2_OFF(int fd)
*OC2输出（可外接继电器）OFF
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC2_OFF(int fd){
    std::cout<<"Turning off OC2 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x05,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x05){printf("OC2 output is off.");}
    return 0;
}
/**int OC3_ON(int fd)
*OC3输出（可外接继电器）ON
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC3_ON(int fd){
    std::cout<<"Turning on OC3 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x06,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x06){printf("OC3 output is on.");}
    return 0;
}

/**int OC3_OFF(int fd)
*OC3输出（可外接继电器）OFF
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int OC3_OFF(int fd){
    std::cout<<"Turning off OC3 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x07,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x07){printf("OC3 output is off.");}
    return 0;
}

/**int read_I3_I4(int fd)
*读取I3和I4二路输入信号命令，（在串口通信模式下：I3为正向限位输入点反馈，I4为反向限位输入点反馈）
*若I3有效，I4无效,返回2（二进制10）
*若I3无效，I4有效,返回1（二进制01）
*若I3有效，I4有效,返回3
*若I3无效，I4无效,返回0
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 0c
*/
int read_I3_I4(int fd){
    std::cout<<"Reading I3 and I4 input..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x08,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x08 ){
        if(result[6]==0x0f){printf("I3 true,I4 false.");return 2;}
        else if(result[6]==0xf0){printf("I3 false,I4 true.");return 1;}
        else if(result[6]==0xff){printf("I3 true,I4 true.");return 3;}
        else if(result[6]==0x00){printf("I3 false,I4 false.");return 0;}
    }
}
/**int set_stepper_address(int fd,unsigned char address=3)
*设置设备的地址
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号 bd
*/
int set_stepper_address(int fd,unsigned char address=0x03){
    std::cout<<"Setting stepper address..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,0xbd,address,0x00,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        //printf("%x %x %x %x",result[0],result[1],result[2],result[3]);
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[2]==0xbd and result[3]==address){printf("Address is set to be %x.",address);}
    return 0;
}

/**unsigned char stepper_config(int fd)
*读取步进电机地址
*通信失败返回-1，信号传输中校验错误返回-2，成功执行返回0。
*命令号
*/
unsigned char stepper_config(int fd){
    std::cout<<"Getting stepper address..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,0xbe,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//返回数据
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//校验和
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//停止1ms，等待返回数据
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[2]==0xbe){printf("Address is  %x.",result[3]);}
    return result[3];
}




#endif // STEPPER_CONTROLLER_H_INCLUDED
