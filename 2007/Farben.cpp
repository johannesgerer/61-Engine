#include "Farben.h"

CFarben::CFarben()
{
	farbe=new float*[51];
	Top=0;

	this->hinzu(0xFFFFFF);//0 Weiß
	this->hinzu(0x000000);//1
	this->hinzu(0x800000);//2
	this->hinzu(0x008000);//3
	this->hinzu(0x808000);//4
	this->hinzu(0x000080);//5
	this->hinzu(0x800080);//6
	this->hinzu(0x008080);//7
	this->hinzu(0x808080);//8
	this->hinzu(0xC0C0C0);//9
	this->hinzu(0xFF0000);//10
	this->hinzu(0x00FF00);//11
	this->hinzu(0xFFFF00);//12
	this->hinzu(0x0000FF);//13
	this->hinzu(0xFF00FF);//14
	this->hinzu(0x00FFFF);//15
	this->hinzu(0xFFFFFF);//16
	this->hinzu(0xF0F8FF);//17
	this->hinzu(0xFAEBD7);//18
	this->hinzu(0x7FFFD4);//19
	this->hinzu(0xF0FFFF);//20
	this->hinzu(0xF5F5DC);//21
	this->hinzu(0x8A2BE2);//22
	this->hinzu(0xA52A2A);//23
	this->hinzu(0xDEB887);//24
	this->hinzu(0x5F9EA0);//25
	this->hinzu(0x7FFF00);//26
	this->hinzu(0xD2691E);//27
	this->hinzu(0xFF7F50);//28
	this->hinzu(0x6495ED);//29
	this->hinzu(0xFFF8DC);//30
	this->hinzu(0xDC143C);//31
	this->hinzu(0x00008B);//32
	this->hinzu(0x008B8B);//33
	this->hinzu(0xB8860B);//34
	this->hinzu(0xA9A9A9);//35
	this->hinzu(0x006400);//36
	this->hinzu(0xBDB76B);//37
	this->hinzu(0x8B008B);//38
	this->hinzu(0x556B2F);//39
	this->hinzu(0xFF8C00);//40
	this->hinzu(0x9932CC);//41
	this->hinzu(0x8B0000);//42
	this->hinzu(0xE9967A);//43
	this->hinzu(0x8FBC8F);//44
	this->hinzu(0x483D8B);//45
	this->hinzu(0x2F4F4F);//46
	this->hinzu(0x00CED1);//47
	this->hinzu(0x9400D3);//48
	this->hinzu(0xFF1493);//49
	this->hinzu(0x00BFFF);//50
}

float* CFarben::operator ()(int i)
{
	return farbe[i];		
}
CVektor CFarben::operator [](int i)
{
	return CVektor(farbe[i][0],farbe[i][1],farbe[i][2],farbe[i][3]);
}
void CFarben::hinzu(int f)
{
	int k[3];
	k[0]=f/16/16/16/16;
	k[1]=(f/16/16-k[0]*16*16);
	k[2]=(f-k[0]*16*16*16*16-k[1]*16*16);

	farbe[Top]=new float[4];
	for(short i=0;i<3;i++)
		farbe[Top][i]=(float)k[i]/255;

	farbe[Top++][3]=255;
}


