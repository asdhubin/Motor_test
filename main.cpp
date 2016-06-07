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
extern short speed=0;
extern int motor_address;
pthread_mutex_t   stop_all_program_mutex;//互斥锁，encoder每次循环时要判断stop_all_programe的值，为1时终止线程
pthread_mutex_t   speed_mutex;

void *encoder_thread(void* args ){
    float revolution=3.74587458;//every 3.7458 of encoder number is 1mm
    float T=0.5;//T的单位是秒，这里表示每500ms循环一次，读取编码器的计数器的数值。
    int encoder = open ("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_SYNC);
    int motor_address;
    if (encoder < 0)
    {
        printf("error opening the device1\n");
    }


    /*CHANGES*/
    if(set_interface_attribs(encoder, B9600, 0,1)!=0)
    {            printf("hello");
        printf("error set interface1\n");
    }

    else


    if(set_blocking(encoder, 0)!=0)
    {
        printf("error set blocking1\n");
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

    int old_data=0;int new_data=0;int32_t delta1;
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
    short protection_count;//protect the car if the odometer broke down.

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
        delta1=(new_data-old_data)*(-1);//our odometer is not properly  installed, it decrease when move forward.
        delta=delta1;
        //protection module begin
        if(speed!=0 and abs(delta1)<2 ){
            protection_count++;
        }
        else{protection_count=0;}//如果连续4次里程计显示速度不为0,但速度为0，可能是编码器皮带断裂或者小车撞上了，此时立刻停止电机并输出警报。
        if(protection_count>3){
            pthread_mutex_lock(&stop_all_program_mutex);
            stop_all_program=1;
            std::cout<<"speed data is abnormal!Stop the car and check out!!"<<std::endl;
            set_speed(motor_address,0);
            usleep(2*1000000);
            break;
        }

        //protection module end

        encoder001.odo_add_mm(delta/revolution);
        std::cout<<"Odometer show speed is"<<delta/revolution/T/1000<<"m/s"<<std::endl;//10ms=0.01s
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


/*-------------------------------motor programe-------------------------------------------*/
    int motor = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        std::cout<<"motor connecting"<<std::endl;
    int motor_address=motor;//用来把电机参数传递给里程计线程
    pthread_create(&odo,NULL,encoder_thread,NULL);//开启里程计线程
    if (motor < 0)
    {pthread_mutex_lock(&stop_all_program_mutex);
        printf("error opening the device0\n");
    }

    if(set_interface_attribs(motor, B9600, 0,2)!=0)//此处有修改，stopbit is 2bit
    {            printf("hello");
        printf("error set interface0\n");
    }
    else
    if(set_blocking(motor, 0)!=0)
    {
        printf("error set blocking0\n");
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

    //while(1){
    //std::cout<<"input an integer to set initial speed,input 'j' to speed up;'k' to speed down;\n \"exit\" to exit;any other input will stop."<<std::endl;
    //char adjust_in[10];
    //int speed=0;
    while(1){
        std::cin>>adjust_in;
        //printf("input is %s \n",adjust_in);
        if (adjust_in[0]=='j'){
            pthread_mutex_lock(&speed_mutex);
            speed++;
            pthread_mutex_unlock(&speed_mutex);
            printf("speed is %d\n",speed);
            }
        else if(adjust_in[0]=='k'){
            pthread_mutex_lock(&speed_mutex);
            speed--;
            pthread_mutex_unlock(&speed_mutex);
            printf("speed is %d\n",speed);}
        else if(0==strcmp(adjust_in,"exit")){break;}
        else if((adjust_in[0]>='0' and adjust_in[0]<='9' ) or adjust_in[0]=='-'){
            sscanf(adjust_in,"%d",&speed);
            printf("set speed %d\n",speed);
        }
        else{
            pthread_mutex_lock(&speed_mutex);
            speed=0;
            pthread_mutex_unlock(&speed_mutex);
            std::cout<<"the motor has stopped\n"<<std::endl;
        }
        set_speed(motor,-speed);
        usleep(3000);
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
//}
}
