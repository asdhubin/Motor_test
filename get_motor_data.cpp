#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <ctime>
#include<pthread.h>//多线程
#include<fstream>//speed data output


#include "AQMD3620NS-A.h"
#include "serialport.h"
#include "odometer.h"

extern int stop_all_program =0;
pthread_mutex_t   stop_all_program_mutex;//互斥锁，encoder每次循环时要判断stop_all_programe的值，为1时终止线程


void *encoder_thread(void* args ){
    int revolution=1000;//every 1000 of encoder number is 1mm
    float T=0.5;//T的单位是秒，这里表示每500ms循环一次，读取编码器的计数器的数值。
    int encoder = open ("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_SYNC);

    if (encoder < 0)
    {
        printf("error opening the device\n");
    }


    /*CHANGES*/
    if(set_interface_attribs(encoder, B9600, 0,1)!=0)
    {            printf("hello");
        printf("error set interface\n");
    }

    else


    if(set_blocking(encoder, 0)!=0)
    {
        printf("error set blocking\n");
    }

    else
        printf("done");

    if( set_interface_attribs (encoder, B9600, 0,1) )
    { // then set_interface_attribs failed
        return NULL;
    }

    // implied else set_interface_attribs successful

    if( set_blocking (encoder, 0) )                // set no blocking
    { // then set_blocking failed
        return NULL;   // might need to also restore oldtty attributes
    }

    // implied else, set_blocking successful


    char receivebuffer [20];



    // implied else, read successful
    //initialize parameter
    unsigned char A5A5[2];A5A5[0]=0xA5;A5A5[1]=0xA5;

    int old_data=0;int new_data=0;
    float delta;
    class odometer encoder001;
    unsigned char var[4];//for saving 4bytes raw data

    std::ofstream speed_data("speed_data.txt",std::ios::out);//open a file named"speed_record.txt" to record the encoder data.
    if(!speed_data){
        std::cout<<"Creat speed_data.txt error!"<<std::endl;
    }
    write (encoder,A5A5 , 2);
    usleep (3000);
    clock_t start;//用于后面循环的定时，使得刷新周期稳定保持在10ms
    double duration;

    pthread_mutex_lock(&stop_all_program_mutex);
    while(stop_all_program ==0){
        start = clock();//起始时间
        pthread_mutex_unlock(&stop_all_program_mutex);

        if( 6> read (encoder, receivebuffer, sizeof receivebuffer) ){ // then read failed
            return NULL;
        }
        /*printf("value of buffer is %2d %2d %2d %2d %2d %2d \n\n",
        receivebuffer[0],
        receivebuffer[1],
        receivebuffer[2],
        receivebuffer[3],
        receivebuffer[4],
        receivebuffer[5]
        );*/  //here is test output,output like E1 9B FE FF FF 79
        for(int i=0;i<4;i++){
            var[i]=receivebuffer[i+1];
        }
        write (encoder,A5A5 , 2);
        old_data=new_data;
        if(receivebuffer[5]!=receivebuffer[0]+receivebuffer[1]+receivebuffer[2]+receivebuffer[3]+receivebuffer[4]){
            continue;
        }
        new_data=(var[3]<<24)|(var[2]<<16)|(var[1]<<8)|(var[0]);
        //Following is the main part to record the distance the car moved in the duration T.
        delta=(new_data-old_data)*(-1);//our odometer is not properly  installed, it decrease when move forward.
        encoder001.odo_add_mm(delta/revolution);
        std::cout<<"Odometer show speed is"<<delta/revolution/T<<std::endl;//10ms=0.01s
        encoder001.odo_print();
        speed_data<<start<<"    "<<delta/revolution/T<<std::endl;


        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;//一次循环计算所花费的时间
        usleep(T*1000000-duration*1000000);//等待满10ms
        pthread_mutex_lock(&stop_all_program_mutex);
    }
    pthread_mutex_unlock(&stop_all_program_mutex);

    // cleanup
    tcsetattr (encoder, TCSANOW, &oldtty);
    speed_data.close();

}
int main()
{

    pthread_t odo;
    pthread_create(&odo,NULL,encoder_thread,NULL);//开启里程计线程

/*-------------------------------motor programe-------------------------------------------*/
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

    //Following are main part of the programe,type 'j' to speed up;type 'k' to speed down;type an integer between -2000~2000 to set speed straightly.
    //To make the motor stop, you can type in any word except ‘j' 'k' and number. For example, 'p' or '$%*&gkjfd' or only a whitespace.
    //You can type in "exit" to exit.

    std::cout<<"input an integer to set initial speed,input 'j' to speed up;'k' to speed down;\n \"exit\" to exit;any other input will stop."<<std::endl;
    char adjust_in[10];//get user's input and analysis it
    short speed=0;
    while(1){
 std::cout<<"input an integer to set initial speed,input 'j' to speed up;'k' to speed down;\n \"exit\" to exit;any other input will stop."<<std::endl;
    char adjust_in[10];
    int speed=0;
   /* while(1){
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
    }*/
    for(int i=0;i<=20;i++){

        speed=i*20;
        set_speed(motor,speed);
        usleep(10*1000000);
    }
    //clean up

    set_speed(motor,0);
    pthread_mutex_lock(&stop_all_program_mutex);
    stop_all_program=1;
    pthread_mutex_unlock(&stop_all_program_mutex);
    pthread_join(odo,NULL);//wait odometer to close the serial port

    //close the serial port of motor
    tcsetattr (motor, TCSANOW, &oldtty);
    return 0;
}
}
