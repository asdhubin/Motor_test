<AQMD3620NS-A.h>\n
  AQMD3620NS-A直流电机驱动器有多种控制方式，包括模拟信号输入和485控制。这里主要采用485通信接口。
  在使用中通过485——串口转换器连接电脑使用，转换器是一个看起来像USB，屁股这里有绿帽可以连485的AB线。
  第17行有一个 #define ADR 0x02 语句，这里ADR是控制器的地址，这个地址可以通过控制器上的拨码选择(注意：地址从1计数，不是从0开始。比如0000代表地址为1,0001代表地址为2)，也就是说通过485总线，电脑其实可以控制多台电机控制器（有点废话，毕竟叫做“总线”）。
  .h文件里面的函数的用途可以结合<AQMD3620NS-A直流电机驱动器文档>理解，当然注释也写得很完善了。
<serialport.h>
  用来建立串口通信的头文件，源代码是从stackoverflow上摘下来改编的，变动主要在停止位，后面提到的两个设备，每字节传输时停止位个数是不一样的。
  目前有两个设备用到串口，虽然设备本身不用串口，但是通过中间件映射成了串口，直接当串口来用就可以了。
  一是编码器计数器，设备选用欧姆龙的编码器作为里程计，中间加了一个FPGA用来计数。编码器在旋转时，三根引线会输出不同的电压，这个频率十分高，所以用了FPGA来计数，FPGA又通过串口连接电脑，与编码器通信时，波特率9600，停止位1位。
  二是直流电机控制器（兼驱动器），通过485转串口与电脑连接。与电机通信时，波特率9600，停止位两位。
<CRC16.h>
  用于AQMD3620NS-A直流电机控制器的通信，编码器的通信不需要这个。串口通信时尽管每个字节内部没有校验位，但是会在一条指令之后放两个字节专门用来校验。具体的算法有兴趣的可以研究一下，没兴趣的话直接拿来用就可以了，功能跟和校验一样，算法不一样。
<odometer.h>
  建了一个类，类名是odometer.很简单，看一下就知道了。调用类函数odo_print()可以将目前的前进、后退距离print出来。这个里程计是分别计算前进后退的，比如往前走了三米，后退了两米，它都记录下来，而不是返回+1米。
<encoder.cpp>
  编码器模块，主要用来测试编码器能否正常工作。如果能正常工作，就会每隔500ms输出当前的前进、后退距离。
  因为编码器的计数不是标准单位，比如计数器增加了100，实际是前进了1毫米。所以里面有一个换算系数int revolution，可以根据实际的测量修改。
<motor_test.cpp>
  直流电机模块。主要用来测试电机能否正常工作。
  打开后，输入数字-2000~2000使电机转动，数字越大转得越快。目前还没测试这个速度和输入的数字是不是线性的。
  输入j，速度+1。输入k，速度-1。输入a~z除了j、k，电机停止，当然输入0电机也会停止，这样主要是为了在停止的时候能够操作的更快速，以免发生意外。输入exit，推出程序。
<main.cpp>
  上面两个模块合并，开了双线程，以电机模块为主线程。用途时一边调节电机的转速，一边记录里程计的读数，同时用两时刻的差值计算瞬时速度，打上时间戳之后保存在文件中，文件名是“speed_data.txt”，格式有一个样例。
  

#Motor_test.cpp
键盘控制电机转速，配合里程计测试直流电机。
