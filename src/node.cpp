// force_6axis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

//#include "comm.h"
#include "udp_interface.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

int delay_time = 2; //

#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 1024

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


int main()
{
	std::cout << "Start!" << std::endl;

	UdpInterface udp;
	udp.Initialize(SERVER_IP, SERVER_PORT, 0);

	int data[5] = { 0 };

	while (1)
	{
		Timer t; //开始计时

		//memset(data, 0, sizeof(data));
		udp.ReadData(data);
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
			// Sleep(dt);
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




	// bool ret;
	// ret = InitSocket();
	// if (ret == false)
	// {
	// 	std::cout << "Error: init socket" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "init socket" << std::endl;
	// }

	// sock.gseriveIP = Ser_IP;
	// sock.gserveport = _ttoi(Ser_Port);
	// sock.gsockmode = 1;

	// ret = udpInitClient(&sock);
	// if (ret == false)
	// {
	// 	std::cout << "Error: init client" << std::endl;
	// }
	// else
	// {
	// 	std::cout << "init client" << std::endl;
	// }

	// //CString addr;
	// int data[5] = { 0 };

	// while (1)
	// {

	// 	Timer t; //开始计时

	// 	//memset(data, 0, sizeof(data));
	// 	Udp_read_Data(0x01, &sock, data);
	// 	//cout << t.elapsed() << endl;

	// 	if (data[0] == 0 && data[1] == 0 && data[2] == 0 && data[3] == 0 && data[4] == 0 && data[5] == 0)
	// 	{
	// 	}
	// 	else
	// 	{
	// 		printf("Fx:%lf, fy:%lf fz:%lf mx:%lf my:%lf mz:%lf\n",
	// 			data[0] * 1.0 / 1000, data[1] * 1.0 / 1000, data[2] * 1.0 / 1000, data[3] * 1.0 / 1000, data[4] * 1.0 / 1000, data[5] * 1.0 / 1000);
	// 	}

	// 	//cout << delay_time - t.elapsed() << endl;
	// 	//cout << delay_time<< endl;
	// 	//cout << t.elapsed() << endl;
	// 	double dt = delay_time - t.elapsed();
	// 	//cout << dt << endl;
	// 	if (dt > 0)
	// 	{
	// 		Sleep(dt);
	// 	}
		
	// 	//Sleep(delay_time);
	// 	//auto start = system_clock::now();
	// 	//// do something...
	// 	//auto end = system_clock::now();
	// 	//auto duration = duration_cast<microseconds>(end - start);
	// 	//cout << "using"
	// 	//	<< double(duration.count()) * microseconds::period::num / microseconds::period::den
	// 	//	<< "s" << endl;

	// }

	// ret = udpClientClose(&sock);




}