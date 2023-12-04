// force_6axis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

//#include "comm.h"
#include "sock.h"

#include <chrono>
using namespace std;
using namespace std::chrono;

#define _uint8   unsigned char
#define _int8	 char
#define _uint16  unsigned short
#define _int16   short
#define _uint32  unsigned int
#define _int32   int


_uint8 Addr;     //        ַ
int buto;
bool Acq_Run = false;
int delay_time = 2; //  ʱʱ  

/*******************************************************************************
* Name			 : CRCTAB.
* Description    : CRCУ    .
*******************************************************************************/
_uint16 CRCTAB[256] =
{
	0x0000, 0xC1C0, 0x81C1, 0x4001, 0x01C3, 0xC003, 0x8002, 0x41C2,
	0x01C6, 0xC006, 0x8007, 0x41C7, 0x0005, 0xC1C5, 0x81C4, 0x4004,
	0x01CC, 0xC00C, 0x800D, 0x41CD, 0x000F, 0xC1CF, 0x81CE, 0x400E,
	0x000A, 0xC1CA, 0x81CB, 0x400B, 0x01C9, 0xC009, 0x8008, 0x41C8,
	0x01D8, 0xC018, 0x8019, 0x41D9, 0x001B, 0xC1DB, 0x81DA, 0x401A,
	0x001E, 0xC1DE, 0x81DF, 0x401F, 0x01DD, 0xC01D, 0x801C, 0x41DC,
	0x0014, 0xC1D4, 0x81D5, 0x4015, 0x01D7, 0xC017, 0x8016, 0x41D6,
	0x01D2, 0xC012, 0x8013, 0x41D3, 0x0011, 0xC1D1, 0x81D0, 0x4010,
	0x01F0, 0xC030, 0x8031, 0x41F1, 0x0033, 0xC1F3, 0x81F2, 0x4032,
	0x0036, 0xC1F6, 0x81F7, 0x4037, 0x01F5, 0xC035, 0x8034, 0x41F4,
	0x003C, 0xC1FC, 0x81FD, 0x403D, 0x01FF, 0xC03F, 0x803E, 0x41FE,
	0x01FA, 0xC03A, 0x803B, 0x41FB, 0x0039, 0xC1F9, 0x81F8, 0x4038,
	0x0028, 0xC1E8, 0x81E9, 0x4029, 0x01EB, 0xC02B, 0x802A, 0x41EA,
	0x01EE, 0xC02E, 0x802F, 0x41EF, 0x002D, 0xC1ED, 0x81EC, 0x402C,
	0x01E4, 0xC024, 0x8025, 0x41E5, 0x0027, 0xC1E7, 0x81E6, 0x4026,
	0x0022, 0xC1E2, 0x81E3, 0x4023, 0x01E1, 0xC021, 0x8020, 0x41E0,
	0x01A0, 0xC060, 0x8061, 0x41A1, 0x0063, 0xC1A3, 0x81A2, 0x4062,
	0x0066, 0xC1A6, 0x81A7, 0x4067, 0x01A5, 0xC065, 0x8064, 0x41A4,
	0x006C, 0xC1AC, 0x81AD, 0x406D, 0x01AF, 0xC06F, 0x806E, 0x41AE,
	0x01AA, 0xC06A, 0x806B, 0x41AB, 0x0069, 0xC1A9, 0x81A8, 0x4068,
	0x0078, 0xC1B8, 0x81B9, 0x4079, 0x01BB, 0xC07B, 0x807A, 0x41BA,
	0x01BE, 0xC07E, 0x807F, 0x41BF, 0x007D, 0xC1BD, 0x81BC, 0x407C,
	0x01B4, 0xC074, 0x8075, 0x41B5, 0x0077, 0xC1B7, 0x81B6, 0x4076,
	0x0072, 0xC1B2, 0x81B3, 0x4073, 0x01B1, 0xC071, 0x8070, 0x41B0,
	0x0050, 0xC190, 0x8191, 0x4051, 0x0193, 0xC053, 0x8052, 0x4192,
	0x0196, 0xC056, 0x8057, 0x4197, 0x0055, 0xC195, 0x8194, 0x4054,
	0x019C, 0xC05C, 0x805D, 0x419D, 0x005F, 0xC19F, 0x819E, 0x405E,
	0x005A, 0xC19A, 0x819B, 0x405B, 0x0199, 0xC059, 0x8058, 0x4198,
	0x0188, 0xC048, 0x8049, 0x4189, 0x004B, 0xC18B, 0x818A, 0x404A,
	0x004E, 0xC18E, 0x818F, 0x404F, 0x018D, 0xC04D, 0x804C, 0x418C,
	0x0044, 0xC184, 0x8185, 0x4045, 0x0187, 0xC047, 0x8046, 0x4186,
	0x0182, 0xC042, 0x8043, 0x4183, 0x0041, 0xC181, 0x8180, 0x4040
};
#define NSTDAQmxData_BASE	 0x02D0					//            ׵ ַ		   0x02D0 ~ 0x0383

HANDLE hcom;     //   ھ  
Msock  sock;
CString Ser_IP = _T("192.168.1.1");
CString Ser_Port = _T("1024");


class Timer
{
	public:
	Timer() : m_begin(high_resolution_clock::now()) {}
	void reset() { m_begin = high_resolution_clock::now(); }
	//默认输出毫秒
	int64_t elapsed() const
	{
		return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();
	}
	//微秒
	int64_t elapsed_micro() const
	{
		return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();
	}
	//纳秒
	int64_t elapsed_nano() const
	{
		return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();
	}
	//秒
	int64_t elapsed_seconds() const
	{
		return duration_cast<chrono::seconds>(high_resolution_clock::now() - m_begin).count();
	}
	//分
	int64_t elapsed_minutes() const
	{
		return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();
	}
	//时
	int64_t elapsed_hours() const
	{
		return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();
	}
	private:
	time_point<high_resolution_clock> m_begin;
};

void Connect()
{

}

_uint16 CRC16(_uint8* pMsg, _uint8 DataLen)
{
	_uint16 uCRC;
	_uint16 pCRC;
	_uint8 uIndex;

	//----------------------------   ʼ  CRC  Ĵ   FCS -------------------------
	uCRC = 0xFFFF;
	//----------------------------     CRCУ     -------------------------------
	while (DataLen--)
	{
		uIndex = (uCRC >> 8) ^ *pMsg++;
		pCRC = CRCTAB[uIndex];
		uCRC = ((pCRC & 0xFF00) ^ (uCRC << 8)) | (pCRC & 0x00FF);
	}
	//----------------------------    ؼ      --------------------------------
	return (uCRC); //^0xFFFF);
}



//   ô     IP

BOOL UDP_write_Ip(_uint8 Addr, Msock* P, char* ip)
{
	BOOL	ret;
	_uint8  size;
	_uint8	Len;
	_uint16 Crc;					// crc   㻺  
	_uint8	in[200];
	_int32  buf[32];
	unsigned char out[255];
	/*char str[8] = { 0 };
	sprintf(str, "%08X", buto);
	for (int i = 0; i < 8; i++)
	{
		if (str[i] >= 48 && str[i] < 58) { str[i] = str[i] - 48; }
		else if (str[i] >= 65 && str[i] <= 90) { str[i] = str[i] - 65 + 10; }
		else { str[i] = str[i] - 97 + 10; }
	}*/
	out[0] = Addr;					//         ַ
	out[1] = 0x10;					//       
	out[2] = 0x02;					//  Ĵ     ַ  
	out[3] = 0x3E;					//  Ĵ    ص 
	out[4] = 0x04;					//   ݳ   
	out[5] = ip[0];	//         115200 -->    00  01  c2  00 
	out[6] = ip[1];
	out[7] = ip[2];
	out[8] = ip[3];
	Len = 9;

	//----------------------------           ݳ    ----------------------

	ret = udpSendClient(P, (char*)out, Len);						//     Э      

	if (!ret) return FALSE;
	//-------------------------------       ʱ ---------------------------
	//   ʱ100ms
	memset(out, 0, sizeof(out));
	//-------------------------------          ---------------------------
	Len = udpRecvClient(P, (char*)in, 3);

	if (Len < 3)
	{
		return false;
	}
	return TRUE;//

}

//   ô      ˿ں 

BOOL UDP_write_Port(_uint8 Addr, Msock* P, int port)
{
	BOOL	ret;
	_uint8  size;
	_uint8	Len;
	_uint16 Crc;					// crc   㻺  
	_uint8	in[200];
	_int32  buf[32];
	unsigned char out[255];
	/*char str[8] = { 0 };
	sprintf(str, "%08X", buto);
	for (int i = 0; i < 8; i++)
	{
	if (str[i] >= 48 && str[i] < 58) { str[i] = str[i] - 48; }
	else if (str[i] >= 65 && str[i] <= 90) { str[i] = str[i] - 65 + 10; }
	else { str[i] = str[i] - 97 + 10; }
	}*/
	out[0] = Addr;					//         ַ
	out[1] = 0x10;					//       
	out[2] = 0x02;					//  Ĵ     ַ  
	out[3] = 0x42;					//  Ĵ    ص 
	out[4] = 0x02;					//   ݳ   
	out[5] = port / 256;					// 
	out[6] = port % 256;

	Len = 7;

	//----------------------------           ݳ    ----------------------

	ret = udpSendClient(P, (char*)out, Len);						//     Э      

	if (!ret) return FALSE;
	//-------------------------------       ʱ ---------------------------
	//   ʱ100ms
	memset(out, 0, sizeof(out));
	//-------------------------------          ---------------------------
	Len = udpRecvClient(P, (char*)in, 3);

	if (Len < 3)
	{
		return false;
	}
	return TRUE;//

}


BOOL Udp_read_Data(_uint8 Addr, Msock* P, int* data)
{
	BOOL	ret;
	_uint8  size;
	_uint8	Len;
	_uint16 Crc;					// crc   㻺  
	_uint8	in[1024];
	_int32  buf[32];
	USHORT		s_CRC;						//     CRCУ    
	USHORT		r_CRC;						//     CRCУ    
	unsigned char out[255];

	out[0] = Addr;					//         ַ
	out[1] = 0x03;					//       
	out[2] = 0x02;					//  Ĵ     ַ  
	out[3] = 0x00;					//  Ĵ    ص 
	out[4] = 0x12;					//    ݳ   
	Len = 5;


	ret = udpSendClient(P, (char*)out, Len);						//     Э      

	if (!ret) return FALSE;
	//-------------------------------       ʱ ---------------------------
	//Sleep(1);
	//-------------------------------          ---------------------------
	memset(in, 0, sizeof(in));


	Len = udpRecvClient(P, (char*)in, (out[4] + 5));

	//---------------------       ֵ     --------------------
	int m_z = 0;
	for (int i = 3; i < (out[4] + 4); i += 3)
	{
		if (0x00 == (((UCHAR)in[i]) & 0x80))
		{
			data[m_z] = (UCHAR)in[i];
			data[m_z] <<= 8;
			data[m_z] += (UCHAR)in[i + 1];
			data[m_z] <<= 8;
			data[m_z] += (UCHAR)in[i + 2];
		}
		else
		{
			data[m_z] = ((UCHAR)in[i] & 0x7F);
			data[m_z] <<= 8;
			data[m_z] += (UCHAR)in[i + 1];
			data[m_z] <<= 8;
			data[m_z] += (UCHAR)in[i + 2];
			data[m_z] *= -1;
		}
		m_z++;
	}
	return TRUE;
}


int main()
{
	std::cout << "Start!" << std::endl;

	bool ret;
	ret = InitSocket();
	if (ret == false)
	{
		std::cout << "Error: init socket" << std::endl;
	}
	else
	{
		std::cout << "init socket" << std::endl;
	}

	sock.gseriveIP = Ser_IP;
	sock.gserveport = _ttoi(Ser_Port);
	sock.gsockmode = 1;

	ret = udpInitClient(&sock);
	if (ret == false)
	{
		std::cout << "Error: init client" << std::endl;
	}
	else
	{
		std::cout << "init client" << std::endl;
	}

	//CString addr;
	int data[5] = { 0 };

	while (1)
	{

		Timer t; //开始计时

		//memset(data, 0, sizeof(data));
		Udp_read_Data(0x01, &sock, data);
		//cout << t.elapsed() << endl;

		if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0 && data[4] == 0 && data[5] == 0)
		{
		}
		else
		{
			printf("Fx:%lf, fy:%lf fz:%lf mx:%lf my:%lf mz:%lf\n",
				data[0] * 1.0 / 1000, data[1] * 1.0 / 1000, data[2] * 1.0 / 1000, data[3] * 1.0 / 1000, data[4] * 1.0 / 1000, data[5] * 1.0 / 1000);
		}

		//cout << delay_time - t.elapsed() << endl;
		//cout << delay_time<< endl;
		//cout << t.elapsed() << endl;
		double dt = delay_time - t.elapsed();
		//cout << dt << endl;
		if (dt > 0)
		{
			Sleep(dt);
		}
		
		//Sleep(delay_time);
		//auto start = system_clock::now();
		//// do something...
		//auto end = system_clock::now();
		//auto duration = duration_cast<microseconds>(end - start);
		//cout << "using"
		//	<< double(duration.count()) * microseconds::period::num / microseconds::period::den
		//	<< "s" << endl;

	}

	ret = udpClientClose(&sock);




}