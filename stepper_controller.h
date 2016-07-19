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

#define STEPPER_ADR 0x03//�豸��
/**define stepper_wait_answer_duration 10000
*Ĭ�Ϸ�������󵽶�ȡ������Ϣ�м䣬�ȴ�1ms
*/
#define stepper_wait_answer_duration 600000
//int fd;//�豸��

/*bool checksum(unsigned char rst[7]){
    unsigned char wrong[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77};
    for(int i=0;i<7;i++){
        if(rst[i]!=)
    }
}*/

/**int set_mode(int fd,unsigned char md)
*�趨����ģʽ���Ա��ģʽΪ00��
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0a
*/
int set_mode(int fd,unsigned char md){
    std::cout<<"setting running mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0a,md,0x00,0x00,0x00,0x00};
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==md){printf("set stepper running mode %x success\n",md);}//���سɹ���Ϣ
    return 0;
}
/**int set_stop_mode(int fd,unsigned char mode)
*�趨ֹͣģʽ
*1������ֹͣ
*2������ֹͣ
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0b
*/
int set_stop_mode(int fd,unsigned char mode){
    std::cout<<"setting stop mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0b,mode,0x00,0x00,0x00,0x00};
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==mode){printf("set stop mode %x success\n",mode);}
    return 0;
}
/**int set_return_home(int fd,unsigned char rh)
*0:�ϵ粻����  1���ϵ����
*���ϵ���㹦�ܿ�������ÿ���ϵ���Զ�����㣨��ʱ��Ҫ��I4�����һ���͵�ƽ�źţ����ҵ����󣬲Ž����趨������ģʽ�С�
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int set_return_home(int fd,unsigned char rh){
    std::cout<<"setting return home mode"<<std::endl;
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,rh,0x00,0x00,0x00,0x00};
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==rh){printf("set return home mode %x success\n",rh);}
    return 0;
}

/**int set_run_mode4(int fd,unsigned char mode)
*�趨ģʽ4������ģʽ
*0��ԭ��ģʽ��I1��I2��Ч�����У���Ч����ֹͣ���У�
*1���ɿ���ֹͣģʽ��I1��I2һֱ��Ч�����У���Ч��ֹͣ���У�
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0d
*/
int set_run_mode4(int fd,unsigned char mode){
    std::cout<<"setting running mode in mode4"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0d,mode,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[6]==mode){printf("set running mode %x in mode4 success\n",mode);}/**/
    return 0;
}

/**int is_in_place(int fd)
*ģʽ0״̬�£����Ƿ����е�λ
*����λ������0��û�е�λ���������˶�״̬��������-3
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2��
*����� 02
*/
int is_in_place(int fd){
    std::cout<<"setting running mode in mode4"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x02,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[5]==0x01){printf("In place,stepper has stopped.");return 0;}
    else if(result[5]==0x00){printf("Not in place,stepper is running.");return -3;}
}

/** set_subdivision_angle(int fd,unsigned short subd,unsigned char angle)
*�趨ϸ�ֺͲ����
*subdΪangleϸ�ֽǣ�angleΪ�����*100.���粽���Ϊ1.8������Ĳ�����180��ʮ������Ϊ0xb4.
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 01
*/
int  set_subdivision_angle(int fd,unsigned short subd,unsigned char angle){
    std::cout<<"setting subdivision and step angle"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x01,subd,subd>>8,angle,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x01){printf("set subdivision %d and step angle %x  success\n",subd,angle);}/**/
    return 0;
}
/**int set_distance(int fd,int32_t distance)
*�趨���о��룬��λ�������������Ȼ����int���ݣ�������ЧֵΪ0~16777215�������ֽڡ�
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 03
*/
int set_distance(int fd,int32_t distance){
    std::cout<<"setting distance"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x03,distance,distance>>8,distance>>16,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x03){printf("set distance %d success\n",distance);}/**/
    return 0;
}

/**int set_direction_and_start_frequency(int fd,unsigned char dir,unsigned short fre=50)
*�趨���з��������Ƶ��
*0��ת��1��ת��Ĭ������Ƶ��Ϊ50Hz��
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 04
*/
int set_direction_and_start_frequency(int fd,unsigned char dir,unsigned short fre=50){
    std::cout<<"setting direction and start frequency"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x04,dir,fre,fre>>8,0x00,0x00};
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x04){printf("set direction %x and frequency %d success\n",dir,fre);}/**/
    return 0;
}
/**int set_frequency_acceleration_and_speed(int fd,unsigned short acc=50,unsigned short speed=200)
*�趨�Ӽ���Ƶ�ʺ������ٶȣ������ٶȵĵ�λ��RPM
*Ĭ�ϼӼ���Ƶ��50Hz���ٶ�Ϊ200rpm��
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 05
*/
int set_frequency_acceleration_and_speed(int fd,unsigned short acc=50,unsigned short speed=200){
    std::cout<<"setting frequency acceleration and speed"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x05,acc,acc>>8,speed,speed>>8,0x00};
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    if(result[4]==0x05){printf("set frequency acceleration %d and speed %d success\n",acc,speed);}/**/
    return 0;
}
/**int stop_stepper(int fd)
*ֹͣ����
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 06
*/
int stop_stepper(int fd){
    std::cout<<"stopping"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x06,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x06){printf("Stepper has stopped.");}
    return 0;
}
/**int run_stepper(int fd)
*��ʼ����������趨�����ٶȣ�����������1�Σ�
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 09
*/
int run_stepper(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x09,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x09){printf("Stepper is running.\n");}
    return 0;
}
/**int keep_stepper_running(int fd)
*����һֱ���У����趨���ٶȣ�
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 07
*/
int keep_stepper_running_positive(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x07,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
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
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 08
*/
int keep_stepper_running_negative(int fd){
    std::cout<<"Loading"<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x08,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x08){printf("Stepper is keeping running negatively.");}
    return 0;
}
/**int led_on(int fd)
*����LED��
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int led_on(int fd){
    std::cout<<"Turning on led..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x00,0x0c,0x05,0x01,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x01){printf("Led is on!");}
    return 0;
}

/**int led_off(int fd)
*Ϩ��LED��
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int led_off(int fd){
    std::cout<<"Turning off led..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x00,0x0c,0x05,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x00){printf("Led is off.");}
    return 0;
}
/**int OC1_ON(int fd)
*OC1���������Ӽ̵�����ON
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC1_ON(int fd){
    std::cout<<"Turning on OC1 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x02,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x02){printf("OC1 output is on.");}
    return 0;
}
/**int OC1_OFF(int fd)
*OC1���������Ӽ̵�����OFF
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC1_OFF(int fd){
    std::cout<<"Turning off OC1 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x03,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x03){printf("OC1 output is off.");}
    return 0;
}
/**int OC2_ON(int fd)
*OC2���������Ӽ̵�����ON
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC2_ON(int fd){
    std::cout<<"Turning on OC2 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x04,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x04){printf("OC2 output is on.");}
    return 0;
}
/**int OC2_OFF(int fd)
*OC2���������Ӽ̵�����OFF
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC2_OFF(int fd){
    std::cout<<"Turning off OC2 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x05,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x05){printf("OC2 output is off.");}
    return 0;
}
/**int OC3_ON(int fd)
*OC3���������Ӽ̵�����ON
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC3_ON(int fd){
    std::cout<<"Turning on OC3 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x06,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x06){printf("OC3 output is on.");}
    return 0;
}

/**int OC3_OFF(int fd)
*OC3���������Ӽ̵�����OFF
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int OC3_OFF(int fd){
    std::cout<<"Turning off OC3 output..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x07,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[4]==0x0c and result[5]==0x07){printf("OC3 output is off.");}
    return 0;
}

/**int read_I3_I4(int fd)
*��ȡI3��I4��·�����ź�������ڴ���ͨ��ģʽ�£�I3Ϊ������λ����㷴����I4Ϊ������λ����㷴����
*��I3��Ч��I4��Ч,����2��������10��
*��I3��Ч��I4��Ч,����1��������01��
*��I3��Ч��I4��Ч,����3
*��I3��Ч��I4��Ч,����0
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� 0c
*/
int read_I3_I4(int fd){
    std::cout<<"Reading I3 and I4 input..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,STEPPER_ADR,0x03,0x0c,0x05,0x08,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
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
*�����豸�ĵ�ַ
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*����� bd
*/
int set_stepper_address(int fd,unsigned char address=0x03){
    std::cout<<"Setting stepper address..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,0xbd,address,0x00,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
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
*��ȡ���������ַ
*ͨ��ʧ�ܷ���-1���źŴ�����У����󷵻�-2���ɹ�ִ�з���0��
*�����
*/
unsigned char stepper_config(int fd){
    std::cout<<"Getting stepper address..."<<std::endl;/**/
    unsigned char order[]={0xff,0xaa,0xbe,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/**/
    unsigned char result[7];//��������
    for(int i=0;i<9;i++){order[9]=order[9]+order[i];}//У���
    write(fd,order,10);
    usleep(stepper_wait_answer_duration);//ֹͣ1ms���ȴ���������
    if( 5> read (fd, result, sizeof result) ){ // then read failed
        std::cout<<"read stepper failed"<<std::endl;
        return -1;
    }
    if(result[0]==0x11 and result[6]==0x77){std::cout<<"data transmission wrong, try again"<<std::endl;return -2;}
    else if(result[2]==0xbe){printf("Address is  %x.",result[3]);}
    return result[3];
}




#endif // STEPPER_CONTROLLER_H_INCLUDED
