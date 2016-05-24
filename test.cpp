#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
int main(){
    std::cout<<"input an integer to set initial speed,input 'j' to speed up;'k' to speed down;\n \"exit\" to exit;any other input will stop."<<std::endl;
    char adjust_in[10];
    short speed=0;

    while(1){

        std::cin>>adjust_in;
        //printf("input is %s \n",adjust_in);
        if     (adjust_in[0]=='j'){speed++;printf("speed is %d\n",speed);}
        else if(adjust_in[0]=='k'){speed--;printf("speed is %d\n",speed);}
        else if(0==strcmp(adjust_in,"exit")){break;}
        else if(adjust_in[0]>='0' and adjust_in[0]<='9'){
            sscanf(adjust_in,"%d",&speed);
            printf("set speed %d\n",speed);
        }
        else{
            speed=0;
            std::cout<<"the motor has stopped\n"<<std::endl;
        }
    }
}
