#ifndef ODOMETER_H_INCLUDED
#define ODOMETER_H_INCLUDED
/**
*class odometer
*km,m,mm
*/
#include <iostream>
#include<math.h>
class odometer{
private:
    int positive_kilometer=0;
    int positive_meter=0;
    float positive_milimeter=0;
    int negative_kilometer=0;
    int negative_meter=0;
    float negative_milimeter=0;
public:
    /*void odo_add_mm(int mm);
    void odo_add_m(int m);
    void odo_add_km(int km);
    void odo_print();
    void odo_print_positive();
    void odo_print_negative();*/

/**
*void odo_add_mm()
*add XX milimeters to odometer
*/
void odo_add_mm(float mm){
    if(mm>=0){
        positive_milimeter+=mm;
        positive_meter    +=positive_milimeter/1000;
        positive_kilometer =positive_meter/1000;
        positive_meter     =positive_meter%1000;
        positive_milimeter =fmod(positive_milimeter,1000);
    }
    else if(mm<0){
        negative_milimeter+=mm;
        negative_meter    +=negative_milimeter/1000;
        negative_kilometer =negative_meter/1000;
        negative_meter     =negative_meter%1000;
        negative_milimeter =fmod(negative_milimeter,1000);
    }
}
/**
*void odo_add_m()
*add XX meters to odometer
*/
void odo_add_m(float m){
    int a=m;//È¡Õû
    if(m>=0){
        positive_milimeter+=(m-a)*1000;
        positive_meter     = positive_meter + a +(positive_milimeter/1000);
        positive_kilometer = positive_meter/1000;
        positive_meter     = positive_meter%1000;
        positive_milimeter =fmod(positive_milimeter,1000);
    }
    else if(m<0){
        negative_milimeter+=(m-a)*1000;
        negative_meter     = negative_meter + a +(negative_milimeter/1000);
        negative_kilometer = negative_meter/1000;
        negative_meter     = negative_meter%1000;
        negative_milimeter =fmod(negative_milimeter,1000);
    }
}
/**
*void odo_print()
*print both forward distance and backward distance.
*/
void odo_print(){
    std::cout<<"move forward "<<positive_kilometer<<"km "<<positive_meter<<"m "
    <<positive_milimeter<<"mm"<<std::endl;
    std::cout<<"move back    "<<negative_kilometer<<"km "<<negative_meter<<"m "
    <<negative_milimeter<<"mm"<<std::endl;
}
/**
*void odo_print_positive()
*only print forward distance.
*/
void odo_print_positive(){
    std::cout<<"move forward "<<positive_kilometer<<"km "<<positive_meter<<"m "
    <<positive_milimeter<<"mm"<<std::endl;
}
/**
*void odo_print_negative()
*only print backward distance.
*/
void odo_print_negative(){
    std::cout<<"move back    "<<negative_kilometer<<"km "<<negative_meter<<"m "
    <<negative_milimeter<<"mm"<<std::endl;
}
};
#endif // ODOMETER_H_INCLUDED
