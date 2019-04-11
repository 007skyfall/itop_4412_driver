/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include "spidev.h"
#include "spidev_test.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	
	abort();
}

static const char *device = "/dev/rc522";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 400 * 1000;//500000;
static uint16_t delay;

int g_SPI_Fd = 0;

unsigned char UID[5], Temp[4];

/*******************************************
函数名称：tochar
功    能：处理16进制函数
参    数：id
返回值  ：无
********************************************/
#if 0
void tochar(unsigned char id)
{
    switch(id) 
    {
    case 0x00:printf("00");break;
    case 0x01:printf("01");break;
    case 0x02:printf("02");break;
    case 0x03:printf("03");break;
    case 0x04:printf("04");break;
    case 0x05:printf("05");break;
    case 0x06:printf("06");break;
    case 0x07:printf("07");break;
    case 0x08:printf("08");break;
    case 0x09:printf("09");break;
    case 0x0a:printf("0a");break;
    case 0x0b:printf("0b");break;
    case 0x0c:printf("0c");break;
    case 0x0d:printf("0d");break;
    case 0x0e:printf("0e");break;
    case 0x0f:printf("0f");break;
    case 0x10:printf("10");break;
    case 0x11:printf("11");break;
    case 0x12:printf("12");break;
    case 0x13:printf("13");break;
    case 0x14:printf("14");break;
    case 0x15:printf("15");break;
    case 0x16:printf("16");break;
    case 0x17:printf("17");break;
    case 0x18:printf("18");break;
    case 0x19:printf("19");break;
    case 0x1a:printf("1a");break;
    case 0x1b:printf("1b");break;
    case 0x1c:printf("1c");break;
    case 0x1d:printf("1d");break;
    case 0x1e:printf("1e");break;
    case 0x1f:printf("1f");break;
    case 0x20:printf("20");break;
    case 0x21:printf("21");break;
    case 0x22:printf("22");break;
    case 0x23:printf("23");break;
    case 0x24:printf("24");break;
    case 0x25:printf("25");break;
    case 0x26:printf("26");break;
    case 0x27:printf("27");break;
    case 0x28:printf("28");break;
    case 0x29:printf("29");break;
    case 0x2a:printf("2a");break;
    case 0x2b:printf("2b");break;
    case 0x2c:printf("2c");break;
    case 0x2d:printf("2d");break;
    case 0x2e:printf("2e");break;
    case 0x2f:printf("2f");break;
    case 0x30:printf("30");break;
    case 0x31:printf("31");break;
    case 0x32:printf("32");break;
    case 0x33:printf("33");break;
    case 0x34:printf("34");break;
    case 0x35:printf("35");break;
    case 0x36:printf("36");break;
    case 0x37:printf("37");break;
    case 0x38:printf("38");break;
    case 0x39:printf("39");break;
    case 0x3a:printf("3a");break;
    case 0x3b:printf("3b");break;
    case 0x3c:printf("3c");break;
    case 0x3d:printf("3d");break;
    case 0x3e:printf("3e");break;
    case 0x3f:printf("3f");break;
    case 0x40:printf("40");break;
    case 0x41:printf("41");break;
    case 0x42:printf("42");break;
    case 0x43:printf("43");break;
    case 0x44:printf("44");break;
    case 0x45:printf("45");break;
    case 0x46:printf("46");break;
    case 0x47:printf("47");break;
    case 0x48:printf("48");break;
    case 0x49:printf("49");break;
    case 0x4a:printf("4a");break;
    case 0x4b:printf("4b");break;
    case 0x4c:printf("4c");break;
    case 0x4d:printf("4d");break;
    case 0x4e:printf("4e");break;
    case 0x4f:printf("4f");break;
    case 0x50:printf("50");break;
    case 0x51:printf("51");break;
    case 0x52:printf("52");break;
    case 0x53:printf("53");break;
    case 0x54:printf("54");break;
    case 0x55:printf("55");break;
    case 0x56:printf("56");break;
    case 0x57:printf("57");break;
    case 0x58:printf("58");break;
    case 0x59:printf("59");break;
    case 0x5a:printf("5a");break;
    case 0x5b:printf("5b");break;
    case 0x5c:printf("5c");break;
    case 0x5d:printf("5d");break;
    case 0x5e:printf("5e");break;
    case 0x5f:printf("5f");break;
    case 0x60:printf("60");break;
    case 0x61:printf("61");break;
    case 0x62:printf("62");break;
    case 0x63:printf("63");break;
    case 0x64:printf("64");break;
    case 0x65:printf("65");break;
    case 0x66:printf("66");break;
    case 0x67:printf("67");break;
    case 0x68:printf("68");break;
    case 0x69:printf("69");break;
    case 0x6a:printf("6a");break;
    case 0x6b:printf("6b");break;
    case 0x6c:printf("6c");break;
    case 0x6d:printf("6d");break;
    case 0x6e:printf("6e");break;
    case 0x6f:printf("6f");break;
    case 0x70:printf("70");break;
    case 0x71:printf("71");break;
    case 0x72:printf("72");break;
    case 0x73:printf("73");break;
    case 0x74:printf("74");break;
    case 0x75:printf("75");break;
    case 0x76:printf("76");break;
    case 0x77:printf("77");break;
    case 0x78:printf("78");break;
    case 0x79:printf("79");break;
    case 0x7a:printf("7a");break;
    case 0x7b:printf("7b");break;
    case 0x7c:printf("7c");break;
    case 0x7d:printf("7d");break;
    case 0x7e:printf("7e");break;
    case 0x7f:printf("7f");break;
    case 0x80:printf("80");break;
    case 0x81:printf("81");break;
    case 0x82:printf("82");break;
    case 0x83:printf("83");break;
    case 0x84:printf("84");break;
    case 0x85:printf("85");break;
    case 0x86:printf("86");break;
    case 0x87:printf("87");break;
    case 0x88:printf("88");break;
    case 0x89:printf("89");break;
    case 0x8a:printf("8a");break;
    case 0x8b:printf("8b");break;
    case 0x8c:printf("8c");break;
    case 0x8d:printf("8d");break;
    case 0x8e:printf("8e");break;
    case 0x8f:printf("8f");break;
    case 0x90:printf("90");break;
    case 0x91:printf("91");break;
    case 0x92:printf("92");break;
    case 0x93:printf("93");break;
    case 0x94:printf("94");break;
    case 0x95:printf("95");break;
    case 0x96:printf("96");break;
    case 0x97:printf("97");break;
    case 0x98:printf("98");break;
    case 0x99:printf("99");break;
    case 0x9a:printf("9a");break;
    case 0x9b:printf("9b");break;
    case 0x9c:printf("9c");break;
    case 0x9d:printf("9d");break;
    case 0x9e:printf("9e");break;
    case 0x9f:printf("9f");break;
    case 0xa0:printf("a0");break;
    case 0xa1:printf("a1");break;
    case 0xa2:printf("a2");break;
    case 0xa3:printf("a3");break;
    case 0xa4:printf("a4");break;
    case 0xa5:printf("a5");break;
    case 0xa6:printf("a6");break;
    case 0xa7:printf("a7");break;
    case 0xa8:printf("a8");break;
    case 0xa9:printf("a9");break;
    case 0xaa:printf("aa");break;
    case 0xab:printf("ab");break;
    case 0xac:printf("ac");break;
    case 0xad:printf("ad");break;
    case 0xae:printf("ae");break;
    case 0xaf:printf("af");break;
    case 0xb0:printf("b0");break;
    case 0xb1:printf("b1");break;
    case 0xb2:printf("b2");break;
    case 0xb3:printf("b3");break;
    case 0xb4:printf("b4");break;
    case 0xb5:printf("b5");break;
    case 0xb6:printf("b6");break;
    case 0xb7:printf("b7");break;
    case 0xb8:printf("b8");break;
    case 0xb9:printf("b9");break;
    case 0xba:printf("ba");break;
    case 0xbb:printf("bb");break;
    case 0xbc:printf("bc");break;
    case 0xbd:printf("bd");break;
    case 0xbe:printf("be");break;
    case 0xbf:printf("bf");break;
    case 0xc0:printf("c0");break;
    case 0xc1:printf("c1");break;
    case 0xc2:printf("c2");break;
    case 0xc3:printf("c3");break;
    case 0xc4:printf("c4");break;
    case 0xc5:printf("c5");break;
    case 0xc6:printf("c6");break;
    case 0xc7:printf("c7");break;
    case 0xc8:printf("c8");break;
    case 0xc9:printf("c9");break;
    case 0xca:printf("ca");break;
    case 0xcb:printf("cb");break;
    case 0xcc:printf("cc");break;
    case 0xcd:printf("cd");break;
    case 0xce:printf("ce");break;
    case 0xcf:printf("cf");break;
    case 0xd0:printf("d0");break;
    case 0xd1:printf("d1");break;
    case 0xd2:printf("d2");break;
    case 0xd3:printf("d3");break;
    case 0xd4:printf("d4");break;
    case 0xd5:printf("d5");break;
    case 0xd6:printf("d6");break;
    case 0xd7:printf("d7");break;
    case 0xd8:printf("d8");break;
    case 0xd9:printf("d9");break;
    case 0xda:printf("da");break;
    case 0xdb:printf("db");break;
    case 0xdc:printf("dc");break;
    case 0xdd:printf("dd");break;
    case 0xde:printf("de");break;
    case 0xdf:printf("df");break;
    case 0xe0:printf("e0");break;
    case 0xe1:printf("e1");break;
    case 0xe2:printf("e2");break;
    case 0xe3:printf("e3");break;
    case 0xe4:printf("e4");break;
    case 0xe5:printf("e5");break;
    case 0xe6:printf("e6");break;
    case 0xe7:printf("e7");break;
    case 0xe8:printf("e8");break;
    case 0xe9:printf("e9");break;
    case 0xea:printf("ea");break;
    case 0xeb:printf("eb");break;
    case 0xec:printf("ec");break;
    case 0xed:printf("ed");break;
    case 0xee:printf("ee");break;
    case 0xef:printf("ef");break;
    case 0xf0:printf("f0");break;
    case 0xf1:printf("f1");break;
    case 0xf2:printf("f2");break;
    case 0xf3:printf("f3");break;
    case 0xf4:printf("f4");break;
    case 0xf5:printf("f5");break;
    case 0xf6:printf("f6");break;
    case 0xf7:printf("f7");break;
    case 0xf8:printf("f8");break;
    case 0xf9:printf("f9");break;
    case 0xfa:printf("fa");break;
    case 0xfb:printf("fb");break;
    case 0xfc:printf("fc");break;
    case 0xfd:printf("fd");break;
    case 0xfe:printf("fe");break;
    case 0xff:printf("ff");break;
    default:
              ;  
    }
}
#endif

int WriteRawRC(int addr, int data)
{
	int ret;
	int fd = g_SPI_Fd;
	unsigned char  TxBuf[2];

	//bit7:MSB=0,bit6~1:addr,bit0:RFU=0
	TxBuf[0] = ((unsigned char)addr << 1)&0x7E;
	//TxBuf[0] &= 0x7E;

	TxBuf[1] = (unsigned char)data;
	
	ret = write(fd, TxBuf, 2);
	if (ret < 0)
		printf("spi:SPI Write error\n");

	usleep(10);

	return ret;
}

unsigned char ReadRawRC(int addr)
{
	int ret;
	int fd = g_SPI_Fd;
	unsigned char  ReData;
	unsigned char  Address;
	
	Address  = (unsigned char)addr << 1;
	Address |= (1 << 7);
	Address &= ~(1 << 0);
	
	ret = write(fd, &Address, 1);
	if (ret < 0)
		printf("spi:SPI Write error\n");

	usleep(100);

	ret = read(fd, &ReData, 1);
	if (ret < 0)
		printf("spi:SPI Read error\n");

	return ReData;
}

void SetBitMask(unsigned char reg,unsigned char mask)  
{
  	char tmp = 0x0;
	
  	tmp = ReadRawRC(reg) | mask;
	
  	WriteRawRC(reg,tmp | mask);
}

//******************************************************************/
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
//******************************************************************/
void ClearBitMask(unsigned char reg, unsigned char mask)  
{
	char tmp = 0x0;
	
	tmp = ReadRawRC(reg)&(~mask);
	
	WriteRawRC(reg, tmp);  // clear bit mask
}

int rc522_init()
{
	int ret;
	char version = 0;

	//reset
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	usleep(10);
	WriteRawRC(ModeReg, 0x3D);
	WriteRawRC(TReloadRegL, 30);
	WriteRawRC(TReloadRegH, 0);
	WriteRawRC(TModeReg, 0x8D);
	WriteRawRC(TPrescalerReg, 0x3E);

	version = ReadRawRC(VersionReg);
	printf("Chip Version: 0x%x\n", version);
	usleep(50000);

	return 0;
}

void PcdAntennaOn()
{
	unsigned char i;
  
	WriteRawRC(TxASKReg, 0x40);
  	usleep(20);
  
	i = ReadRawRC(TxControlReg);
  	if(!(i&0x03))
    		SetBitMask(TxControlReg, 0x03);
  	
	i = ReadRawRC(TxASKReg);
}

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

//******************************************************************/
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
//******************************************************************/
char PcdComMF522(unsigned char Command, unsigned char *pInData, 
                 				unsigned char InLenByte, unsigned char *pOutData, 
                 				unsigned int  *pOutLenBit)
{
	char status = MI_ERR;
	unsigned char irqEn  = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int  i;
	
	switch (Command)
	{
		case PCD_AUTHENT:
	  		irqEn   = 0x12;
			  waitFor = 0x10;
			  break;
		case PCD_TRANSCEIVE:
	  		irqEn   = 0x77;
	  		waitFor = 0x30;
	  		break;
		default:
	  		break;
	}
	
	WriteRawRC(ComIEnReg, irqEn|0x80);
	ClearBitMask(ComIrqReg, 0x80);
	WriteRawRC(CommandReg, PCD_IDLE);
	SetBitMask(FIFOLevelReg, 0x80); // 清空FIFO 
	for(i=0; i<InLenByte; i++)
		WriteRawRC(FIFODataReg, pInData[i]); // 数据写入FIFO 
		
	WriteRawRC(CommandReg, Command); // 命令写入命令寄存器

	if(Command == PCD_TRANSCEIVE)
		SetBitMask(BitFramingReg,0x80); // 开始发送     

	i = 6000; //根据时钟频率调整，操作M1卡最大等待时间25ms
  	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	}
	while((i!=0)&&!(n&0x01)&&!(n&waitFor));
	
	ClearBitMask(BitFramingReg, 0x80);

	if(i!=0)
	{
		if(!(ReadRawRC(ErrorReg) & 0x1B))
		{
			status = MI_OK;
			if (n&irqEn&0x01)
				status = MI_NOTAGERR;
			if(Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if(lastBits)
					*pOutLenBit = (n-1)*8 + lastBits;
				else
					*pOutLenBit = n*8;

				if(n == 0)
					n = 1;
				if(n>MAXRLEN)
					n = MAXRLEN;

				for (i=0; i<n; i++)
					pOutData[i] = ReadRawRC(FIFODataReg); 
			}
		}
		else
		{
			status = MI_ERR;
		}
	}

	SetBitMask(ControlReg, 0x80);// stop timer now
	WriteRawRC(CommandReg, PCD_IDLE); 

	return status;
}

char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
	char status;  
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x07);
	SetBitMask(TxControlReg, 0x03);
 
  	ucComMF522Buf[0] = req_code;

  	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf,
                       				1, ucComMF522Buf, &unLen);

  	if ((status == MI_OK) && (unLen == 0x10))
  	{    
    		*pTagType     = ucComMF522Buf[0];
    		*(pTagType+1) = ucComMF522Buf[1];
  	}
  	else
	{
		status = MI_ERR;
  	}

  	return status;
}

//******************************************************************/
//功    能：防冲撞                                                  /
//参数说明: pSnr[OUT]:卡片序列号，4字节                             /
//返    回: 成功返回MI_OK                                           /
//******************************************************************/
char PcdAnticoll(unsigned char *pSnr)
{
	char status;
	unsigned char i, snr_check = 0;
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
    
	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x00);
	ClearBitMask(CollReg, 0x80);
 
	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

    	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf,
								2, ucComMF522Buf, &unLen);

    	if(status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			*(pSnr+i)  = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
		{
			status = MI_ERR;
		}
	}
    
	SetBitMask(CollReg,0x80);
	
	return status;
}

void Find_Card(void)
{
	if(PcdRequest(0x52, Temp) == MI_OK)
	{
	  	if(Temp[0]==0x04 && Temp[1]==0x00)  
	      		printf("MFOne-S50\n");
	    	else if(Temp[0]==0x02 && Temp[1] == 0x00)
	      		printf("MFOne-S70\n");
	    	else if(Temp[0]==0x44 && Temp[1]==0x00)
	      		printf("MF-UltraLight\n");
	    	else if(Temp[0]==0x08 && Temp[1]==0x00)
	      		printf("MF-Pro\n");
	    	else if(Temp[0]==0x44 && Temp[1]==0x03)
	      		printf("MF Desire\n");
	    	else
	      		printf("Unknown\n");
			
	    	printf("SUCCESS!\n");
	}
	else
	{
		printf("No card!\n");
	}
}

void Auto_Reader(void)
{
	int i = 0;
	unsigned long num = 0;
	
  //	while(1)
	//{
		if(PcdRequest(0x52,Temp) == MI_OK)
		{
			if(Temp[0]==0x04 && Temp[1]==0x00)  
				printf("MFOne-S50\n");
			else if(Temp[0]==0x02 && Temp[1]==0x00)
				printf("MFOne-S70\n");
			else if(Temp[0]==0x44 && Temp[1]==0x00)
				printf("MF-UltraLight\n");
			else if(Temp[0]==0x08 && Temp[1]==0x00)
				printf("MF-Pro\n");
			else if(Temp[0]==0x44 && Temp[1]==0x03)
				printf("MF Desire\n");
			else
				printf("Unknown\n");
			
			if(PcdAnticoll(UID) == MI_OK)
			{ 
				printf("Card Id is(%d):", num++);
#if 1
				for(i=0; i<4; i++)
					printf("%x", UID[i]);
#else			
				tochar(UID[0]);
				tochar(UID[1]);
				tochar(UID[2]);
				tochar(UID[3]);
#endif
				printf("\n");
				
				PcdRequest(0x52,Temp);//clear
			}
			else
			{
				printf("no serial num read\n");
			}
		}
		else
		{
			printf("No Card!\n");
		}

		usleep(300000);
//	} 
}

void HandleConfigMenu(unsigned char inputvalue)
{
#if 0
    switch(toupper(inputvalue)) 
    {
    case 'A':
              Auto_Reader();
              break;
    case 'F':
              Find_Card();
              break;
    default:
              DisplayConfigMenu();
    }
#endif

	//Find_Card();

	Auto_Reader();
}

int main(int argc, char *argv[])
{
	unsigned char i;
	
	int ret = 0;
	int fd;

	parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	g_SPI_Fd = fd;

#if 0
	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
#endif

	rc522_init();

	PcdAntennaOn();

	HandleConfigMenu(i);

	close(fd);

	return ret;
}
