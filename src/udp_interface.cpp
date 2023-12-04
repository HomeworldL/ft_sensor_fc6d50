#include "udp_interface.h"
#include <stdio.h>
#include <math.h>
#include <ctime>

UdpInterface::UdpInterface(_uint8 Addr)
{  
  Addr_ = Addr;
}

UdpInterface::~UdpInterface()
{  
}

void UdpInterface::Initialize(const char *remote_ip, int remote_port, int sockmode)
{
  is_run_ = true;
  p_udp_adapter_ = new UdpAdapter();
  p_udp_adapter_->Initialize(remote_ip, remote_port, sockmode);
  read_tcp_ = false;
}

bool UdpInterface::ReadData(int* data)
{
	bool	ret;
	_uint8  size;
	_uint8	Len;
	_uint16 Crc;					// crc
	_uint8	in[1024];
	_int32  buf[32];
	unsigned char out[255];

	out[0] = Addr_;					// 0x01
	out[1] = 0x03;					//
	out[2] = 0x02;					//
	out[3] = 0x00;					//
	out[4] = 0x12;					//
	Len = 5;

	ret = p_udp_adapter_->Send((char*)out, Len);						//

	if (!ret) return false;
	//------------------------------- ---------------------------
	//Sleep(1);
	//------------------------------- ---------------------------
	memset(in, 0, sizeof(in));


	Len = p_udp_adapter_->Read((char*)in, (out[4] + 5));

	//--------------------- --------------------
	int m_z = 0;
	for (int i = 3; i < (out[4] + 4); i += 3)
	{
		if (0x00 == (((_uint8)in[i]) & 0x80))
		{
			data[m_z] = (_uint8)in[i];
			data[m_z] <<= 8;
			data[m_z] += (_uint8)in[i + 1];
			data[m_z] <<= 8;
			data[m_z] += (_uint8)in[i + 2];
		}
		else
		{
			data[m_z] = ((_uint8)in[i] & 0x7F);
			data[m_z] <<= 8;
			data[m_z] += (_uint8)in[i + 1];
			data[m_z] <<= 8;
			data[m_z] += (_uint8)in[i + 2];
			data[m_z] *= -1;
		}
		m_z++;
	}
	return true;
}

void UdpInterface::Run()
{

  if(is_run_){
    p_udp_adapter_->Send(data_tran_.c_str(),data_tran_.length());
  }
}

void UdpInterface::Stop()
{
  is_run_ = false;
}


void UdpInterface::PthreadCreate(void * (*pthreadfunc)(void *))
{
  if(pthread_create(&tid_, nullptr, pthreadfunc, this))
  {
      perror("pthread_create() error");
      exit(0);
  }



}

// void UdpInterface::PthreadStart()
// {

//   ssize_t recvbytes;

//   while (1) {

//       recvbytes = p_udp_adapter_->Read(rcv_buff);

//       if(recvbytes == 0){

//         cout << "recvbytes == 0" << endl;

//       }

//       else if (recvbytes == -1)
//       {
//         if(errno!=EAGAIN&&errno != EWOULDBLOCK&&errno!=EINTR){
//           cout << "socket receive error!!!!" << endl;
//         }
//         else{

//         }
        
//       }
      

//       else{
//         read_tcp_ = true;
//         // ParsePacket(rcv_buff, static_cast<int>(recvbytes));
//       }

//       // usleep(10000);

//   }

// }



