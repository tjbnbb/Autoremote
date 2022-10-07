
//******* 程序包含文件 *************//
#include "main.H" 

//******* 全局变量定义 *************//
struct_ADC        ADCresult;                   //热敏、光敏测量AD值           
xdata unsigned char co[140];                 //存储红外线发射编码
unsigned char instruction[9]={0x40,0x00,0x20,0x50,0x02,0x00,0x21,0x00,0x00};       //存放空调指令
unsigned char start[2]={15,7};								//起始码
unsigned char connect[2]={1,33};						  //连接码
unsigned char finish[2]={1,0};              //结束码
unsigned char zero[2]={1,1};                 //“0”
unsigned char one[2]={1,3};									//“1”
unsigned char func=0;                       //空调的模式
unsigned char switchflag=0;                 //空调是否打开
unsigned char wind=0;                       //空调的风速大小
unsigned char sweep=0;                      //扫风模式
unsigned char temperature=16;               //设定的温度
unsigned char tempnow;                      //实时温度
unsigned char autocontrol=0;                //是否开启智能控制
unsigned char count=10;                     //智能控制检测的执行次数
unsigned int t=0;                          //执行温度的次数
unsigned long sumt=0;                       //温度AD的总和
//温度值对应表
int code tempdata[]={239,197,175,160,150,142,135,129,124,120,116,113,109,107,104,101, 
										  99, 97, 95, 93, 91, 90, 88, 86, 85, 84, 82, 81, 80, 78, 77, 76, 
										  75, 74, 73, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 63, 62, 
										  61, 61, 60, 59, 58, 58, 57, 57, 56, 55, 55, 54, 54, 53, 52, 52, 
										  51, 51, 50, 50, 49, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44, 
										  43, 43, 42, 42, 41, 41, 41, 40, 40, 39, 39, 38, 38, 38, 37, 37, 
										  36, 36, 36, 35, 35, 34, 34, 34, 33, 33, 32, 32, 32, 31, 31, 31, 
										  30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 26, 25, 25,
										  24, 24, 24, 23, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 20, 19, 
										  19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 14, 
										  13, 13, 13, 12, 12, 12, 11, 11, 11, 10, 10, 9, 9, 9, 8, 8, 8, 7, 
										  7, 7, 6, 6,5, 5, 54,4, 3, 3,3, 2, 2, 1, 1, 1, 0, 0, -1, -1, -1, 
										  -2, -2, -3, -3, -4, -4, -5, -5, -6, -6, -7, -7, -8, -8, -9, -9, 
										  -10, -10, -11, -11, -12, -13, -13, -14, -14, -15, -16, -16, -17, 
										  -18, -19, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28, -29, 
										  -30, -32, -33, -35, -36, -38, -40, -43, -46, -50, -55, -63, 361};


//******* 函数原型 *************//
#include "function.c" 

void my1mS_callback()                       //1ms事件回调函数
{ dealwithtemp();   //求实时温度
}
	
void my100mS_callback()			                //100mS事件回调函数
{ dealwithDisp();   //数码管及LED显示
}			 
							 
void my1S_callback()				               //1S事件回调函数
{ dealwithauto();   //智能控制
}

void myKN_callback()			               	          //导航按键事件回调函数
{ dealwithmyKN();		
}

void mykey_callback()		                            // 按键（Key1、Key2）事件回调函数
{	dealwithmykey();	  
}

//******* main()函数 *************//	
void main() {         //主函数 main() 开始 
	
//******* main()函数（初始化类程序） *************//		
//加载需要用的模块(由各模块提供加载函数)
		Key_Init();                                      //需要用到的模块及其函数、方法，必须对其初始化（让其准备、就绪和响应做后台服务）
	  DisplayerInit();                                 //显示模块初始化
	  BeepInit();	                                     //蜂鸣器模块初始化
	  AdcInit(ADCexpEXT);                              //ADC模块初始化，有参数，选择扩展接口EXT上P1.0、P1.1是否也做ADC功能
 		IrInit(NEC_R05d);                                //红外线模块初始化
//设置事件回调函数(由sys提供设置函数SetEventCallBack())	
		SetEventCallBack(enumEventKey, mykey_callback);            //设置按键回调函数
		SetEventCallBack(enumEventSys1mS, my1mS_callback);         //设置1ms回调函数
		SetEventCallBack(enumEventSys100mS, my100mS_callback);     //设置100ms回调函数
 		SetEventCallBack(enumEventSys1S, my1S_callback);           //设置1s回调函数
 		SetEventCallBack(enumEventNav, myKN_callback);             //设置导航按键回调函数

//状态初始化 	
		SetDisplayerArea(0,7);                           //设置数码管显示区域
		
/*****************  MySTC_OS 初始化与加载开始  ********************************/   
	  MySTC_Init();	    // MySTC_OS 初始化 
	while(1)            // 系统主循环 	
		{ MySTC_OS();     // MySTC_OS 加载
/*****************  MySTC_OS 初始化与加载结束  ********************************/  
			
		}	              //主循环while(1)结束
}                   //主函数 main() 结束 

