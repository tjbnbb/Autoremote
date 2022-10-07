void sendinstruction()                                 //���Ϳյ�ָ��
{
	unsigned char i,j,k,check,temp;
	check=(func-1)+(temperature-16)+5+((instruction[5]>>4)&0x01)%16;   //���ɿյ�����ʱ��У����
	if(switchflag==0) check=(check+8)%16;                              //���ɿյ��ر�ʱ��У����
	instruction[8]=(instruction[8]&0x0f)|(check<<4);                   //����У����
	co[0]=start[0];                                                    //������ʼ��
	co[1]=start[1];
	k=1;
	for(i=0;i<5;i++)                                                   //����ǰ��������
	{
		temp=instruction[i];
		for(j=0;j<8;j++)
		{
			if(i==4&&j==3) break;
			if(temp&0x01==1)
			{
				co[k+1]=one[0];
				co[k+2]=one[1];
				k+=2;
			}
			else
			{
				co[k+1]=zero[0];
				co[k+2]=zero[1];
				k+=2;
			}
		  temp=temp>>1;
		}
	}
	co[k+1]=connect[0];                                               //����������
	co[k+2]=connect[1];
	k+=2;
	for(i=5;i<9;i++)                                                  //����󲿷�����
	{
		temp=instruction[i];
		for(j=0;j<8;j++)
		{
			if(temp&0x01==1)
			{
				co[k+1]=one[0];
				co[k+2]=one[1];
				k+=2;
			}
			else
			{
				co[k+1]=zero[0];
				co[k+2]=zero[1];
				k+=2;
			}
		  temp=temp>>1;
		}
	}
	co[k+1]=finish[0];                                                //���������
	co[k+2]=finish[1];
	k+=2;
	IrTxdSet(&co,140);                                                //����
}

void dealwithDisp()			                                            //�������LED������ʾ              
{ 
	unsigned char d0,d1,d2,d3,d4,d5,d6,d7,LED;
	d0=switchflag;
	d1=autocontrol;
	d2=10;
	d3=tempnow/10;
	d4=tempnow%10;
	d5=10;
	d6=temperature/10;
	d7=temperature%10;
	LED=func|(wind<<6)|(sweep<<4);
	Seg7Print(d0,d1,d2,d3,d4,d5,d6,d7);
	LedPrint(LED);
}			 
							 
void dealwithmyKN()			               	                      
{ 
	if(GetAdcNavAct(enumAdcNavKeyUp) == enumKeyPress)                          //�����趨�¶�
	{
		temperature++;
		if(temperature>30) temperature=30;
		instruction[1]=(temperature-16);
		sendinstruction();
		SetBeep(1000,20);
	}
	if(GetAdcNavAct(enumAdcNavKeyDown) == enumKeyPress)                       //��С�趨�¶�
	{
		temperature--;
		if(temperature<16) temperature=16;
		instruction[1]=(temperature-16);
		sendinstruction();
		SetBeep(1000,20);
	}
	
	if(GetAdcNavAct(enumAdcNavKeyCenter) == enumKeyPress)                          //ɨ���л�
	{
		sweep++;
		if(sweep>3) sweep=0;
		switch(sweep)
		{
			case 0: instruction[5]=0x00;break;                                         //��ɨ��
			case 1: instruction[5]=0x01;break;                                         //����ɨ��
			case 2: instruction[5]=0x10;break;                                         //����ɨ��
			case 3: instruction[5]=0x11;break;                                         //��������ɨ��
			default: break;
		}
		sendinstruction();
		SetBeep(1000,20);
	}
	
	if(GetAdcNavAct(enumAdcNavKeyLeft) == enumKeyPress)                          //��С����
	{
		wind--;
		if(wind<0) wind=0;
		switch(wind)
		{
			case 0: instruction[0]=instruction[0]&0xcf|0x00;break;                   //�Զ�
			case 1: instruction[0]=instruction[0]&0xcf|0x10;break;                   //�ͷ���
			case 2: instruction[0]=instruction[0]&0xcf|0x20;break;                   //�з���
			case 3: instruction[0]=instruction[0]&0xcf|0x30;break;                   //�߷���
			default: break;
		}
		sendinstruction();
		SetBeep(1000,20);
	}
	if(GetAdcNavAct(enumAdcNavKeyRight) == enumKeyPress)                          //�������
	{
		wind++;
		if(wind>3) wind=3;
		switch(wind)
		{
			case 0: instruction[0]=instruction[0]&0xcf|0x00;break;                    //�Զ�
			case 1: instruction[0]=instruction[0]&0xcf|0x10;break;                    //�ͷ���
			case 2: instruction[0]=instruction[0]&0xcf|0x20;break;                    //�з���
			case 3: instruction[0]=instruction[0]&0xcf|0x30;break;                    //�߷���
			default: break;
		}
		sendinstruction();
		SetBeep(1000,20);
	}
	
	if(GetAdcNavAct(enumAdcNavKey3) == enumKeyPress)                          //���°���3���л��յ���ģʽ
		{ 
			func++;
			if(func==5) func=0;
			switch(func)
			{
				case 0: instruction[0]=instruction[0]&0xf8|0x00;break;        //�Զ�ģʽ
				case 1: instruction[0]=instruction[0]&0xf8|0x01;break;        //����ģʽ
				case 2: instruction[0]=instruction[0]&0xf8|0x02;break;        //��ʪģʽ
				case 3: instruction[0]=instruction[0]&0xf8|0x03;break;        //�ͷ�ģʽ
				case 4: instruction[0]=instruction[0]&0xf8|0x04;break;        //����ģʽ
				default: break;
			}
			sendinstruction();
			SetBeep(1000,20);
		}			
}

void dealwithmykey()		                  
{	  	  
	  if (GetKeyAct(enumKey2)== enumKeyPress)    		                                        //�������ܿ���ģʽ      	  
			{
 				if(autocontrol==0) autocontrol=1;
 				else autocontrol=0;
				SetBeep(1000,20);			
			}
			
	  if (GetKeyAct(enumKey1)== enumKeyPress) 	                                            //�յ��Ŀ��ػ� 	
			{	
				if(switchflag==0) switchflag=1;
				else switchflag=0;
				if(switchflag==0) instruction[0]=instruction[0]&0xf7;
				else instruction[0]=instruction[0]|0x08;
				sendinstruction();
        SetBeep(1000,20);        
			}
}

void dealwithauto()                 //���ܿ��ƣ�10s�Զ����һ��
{
	if(count==10)
	{
		if(autocontrol==1)
		{
			if(tempnow>=20&&tempnow<=27)
			{
				if(switchflag==1)
				{
					switchflag=0;
					instruction[0]=instruction[0]&0xf7;
					sendinstruction();
				}
			}
			else
			{
				if(tempnow>27)
				{
					if(switchflag==0||func!=1||temperature!=25||sweep!=3||wind!=2)
					{
						switchflag=1;
						instruction[0]=instruction[0]|0x08;
						func=1;
						instruction[0]=instruction[0]&0xf8|0x01;
						temperature=25;
						instruction[1]=(temperature-16);
						sweep=3;
						instruction[5]=0x11;
						wind=2;
						instruction[0]=instruction[0]&0xcf|0x20;
						sendinstruction();
					}
				}
				else
				{
					if(switchflag==0||func!=4||temperature!=25||sweep!=3||wind!=2)
					{
						switchflag=1;
						instruction[0]=instruction[0]|0x08;
						func=4;
						instruction[0]=instruction[0]&0xf8|0x04;
						temperature=23;
						instruction[1]=(temperature-16);
						sweep=3;
						instruction[5]=0x11;
						wind=2;
						instruction[0]=instruction[0]&0xcf|0x20;
						sendinstruction();
					}
				}
			}
		}
		count=0;
	}
	count++;
}

void dealwithtemp()                    //����ʵʱ�¶�
{
	unsigned int date_temp;
	if(t==50)
	{
		date_temp=(sumt+t/2)/t;			//��������
	  tempnow=tempdata[date_temp-1]; //���Ҷ�Ӧ��AD���¶�ֵ
	  sumt=0;
		t=0;
	}
	ADCresult=GetADC();
	t++;
	date_temp=ADCresult.Rt/4;//��10λADֵת��Ϊ8λADֵ
	sumt+=date_temp;						//��t��ADֵ�ĺ�
}