
//******* ��������ļ� *************//
#include "main.H" 

//******* ȫ�ֱ������� *************//
struct_ADC        ADCresult;                   //��������������ADֵ           
xdata unsigned char co[140];                 //�洢�����߷������
unsigned char instruction[9]={0x40,0x00,0x20,0x50,0x02,0x00,0x21,0x00,0x00};       //��ſյ�ָ��
unsigned char start[2]={15,7};								//��ʼ��
unsigned char connect[2]={1,33};						  //������
unsigned char finish[2]={1,0};              //������
unsigned char zero[2]={1,1};                 //��0��
unsigned char one[2]={1,3};									//��1��
unsigned char func=0;                       //�յ���ģʽ
unsigned char switchflag=0;                 //�յ��Ƿ��
unsigned char wind=0;                       //�յ��ķ��ٴ�С
unsigned char sweep=0;                      //ɨ��ģʽ
unsigned char temperature=16;               //�趨���¶�
unsigned char tempnow;                      //ʵʱ�¶�
unsigned char autocontrol=0;                //�Ƿ������ܿ���
unsigned char count=10;                     //���ܿ��Ƽ���ִ�д���
unsigned int t=0;                          //ִ���¶ȵĴ���
unsigned long sumt=0;                       //�¶�AD���ܺ�
//�¶�ֵ��Ӧ��
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


//******* ����ԭ�� *************//
#include "function.c" 

void my1mS_callback()                       //1ms�¼��ص�����
{ dealwithtemp();   //��ʵʱ�¶�
}
	
void my100mS_callback()			                //100mS�¼��ص�����
{ dealwithDisp();   //����ܼ�LED��ʾ
}			 
							 
void my1S_callback()				               //1S�¼��ص�����
{ dealwithauto();   //���ܿ���
}

void myKN_callback()			               	          //���������¼��ص�����
{ dealwithmyKN();		
}

void mykey_callback()		                            // ������Key1��Key2���¼��ص�����
{	dealwithmykey();	  
}

//******* main()���� *************//	
void main() {         //������ main() ��ʼ 
	
//******* main()��������ʼ������� *************//		
//������Ҫ�õ�ģ��(�ɸ�ģ���ṩ���غ���)
		Key_Init();                                      //��Ҫ�õ���ģ�鼰�亯������������������ʼ��������׼������������Ӧ����̨����
	  DisplayerInit();                                 //��ʾģ���ʼ��
	  BeepInit();	                                     //������ģ���ʼ��
	  AdcInit(ADCexpEXT);                              //ADCģ���ʼ�����в�����ѡ����չ�ӿ�EXT��P1.0��P1.1�Ƿ�Ҳ��ADC����
 		IrInit(NEC_R05d);                                //������ģ���ʼ��
//�����¼��ص�����(��sys�ṩ���ú���SetEventCallBack())	
		SetEventCallBack(enumEventKey, mykey_callback);            //���ð����ص�����
		SetEventCallBack(enumEventSys1mS, my1mS_callback);         //����1ms�ص�����
		SetEventCallBack(enumEventSys100mS, my100mS_callback);     //����100ms�ص�����
 		SetEventCallBack(enumEventSys1S, my1S_callback);           //����1s�ص�����
 		SetEventCallBack(enumEventNav, myKN_callback);             //���õ��������ص�����

//״̬��ʼ�� 	
		SetDisplayerArea(0,7);                           //�����������ʾ����
		
/*****************  MySTC_OS ��ʼ������ؿ�ʼ  ********************************/   
	  MySTC_Init();	    // MySTC_OS ��ʼ�� 
	while(1)            // ϵͳ��ѭ�� 	
		{ MySTC_OS();     // MySTC_OS ����
/*****************  MySTC_OS ��ʼ������ؽ���  ********************************/  
			
		}	              //��ѭ��while(1)����
}                   //������ main() ���� 

