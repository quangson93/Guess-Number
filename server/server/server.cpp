// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "math.h"
#include "string.h"
#include "fstream" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


// tạo một struct lưu tên người chơi và điểm của người chơi đó
struct User
{
	char Name_user[6];
	int Diem;
};



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		CSocket server;
		unsigned int port = 1234;
		AfxSocketInit(NULL);

		server.Create(port);
		server.Listen();

		//Nhap so luong client
		printf("\n Nhap so luong client: ");
		int num_client;
		scanf_s("%d", &num_client);
		
		//Tao mang chua cac socket client
		CSocket *sockClients = new CSocket[num_client];

		//Tạo môt mảng chứa các User
		User * mang = new User[num_client];
		
		
		ofstream GHI ("Diem.txt");
		// 
		for (int i = 0; i < num_client; i++)
		{

			server.Accept(sockClients[i]);
			
			printf("Da tiep nhan client %d/%d\n", i+1, num_client);
			//Bao cho client biet minh la client thu may
			sockClients[i].Send((char*)&i, sizeof(int), 0);
		}
		//tạo một mảng chứa 3 số được chọn ngẫu nhiên
		srand((unsigned)time(NULL));
		int num_test[3];
		num_test[0]=(int)(rand() % 10);
		num_test[1]=(int)(rand() % 10);
		num_test[2]=(int)(rand() % 10);

		//Tao mang chua cac so client gui
		int * numbers = new int[num_client*3];
		int len_str;
		char chuoi_tb[100];
		char* temp;

		// nhận và lưu user vào mảng

		for(int i = 0; i < num_client; i++)
		{
			while(1)
			{
				int flag_out_while = 1;
				sockClients[i].Receive((char*)&len_str,sizeof(int),0); // nhan do dai chuoi ki tu tu client 
				temp = new char[len_str +1]; // tao ra mot mang ki tu vua du de luu chuoi ki tu
				sockClients[i].Receive((char*)temp, len_str, 0);// nhan chuoi ki tu tu client
				if(len_str > 5 )
				{
					int flag = 1;// gui flag ve cho client de bao cho client nhap sai
					sockClients[i].Send((char*)&flag, sizeof(int), 0);	
					//break;
					continue;
				}
				else
				{
					int flag = 0; // khai bao co 
					// tao vong lap de kiem tra tung ki tu
					for(int j = 0; j < len_str; j++)
					{
						if(temp[j] < 65 || temp[j] > 122 || ( temp[j] > 90 && temp[j] < 97))
						{
							flag = 1;
							flag_out_while = 6; // cờ dùng để ko thoát khỏi vòng lặp while
							sockClients[i].Send((char*)&flag, sizeof(int), 0); // gửi cờ về cho client xác định nhập tên user đúng hay sai
							break;
							
						}
						else
						{
							// kiem tra het cac ki tu
							if(j < len_str - 1)
								continue;

							// kiểm tra trùng user 
							if(i != 0)//
							{
								int h;
								temp[len_str] = 0; // tạo kí tự kết thúc chuỗi ở mảng kí tự temp
								for(h = 0; h < i; h++)
								{
									if(strcmp(mang[h].Name_user,temp) == 0) // so sánh user được lưu ở trong mảng với user client vừa nhập
										break;
								}
								if(h != i)
								{
									flag = 1;
									flag_out_while = 6; // cờ dùng để thoát khỏi vòng lặp while
									sockClients[i].Send((char*)&flag, sizeof(int), 0); // gửi cờ về cho client xác định nhập tên user đúng hay sai
									break;
								}
							}

							sockClients[i].Send((char*)&flag, sizeof(int), 0);//gửi flag != 1 về cho client
							temp[len_str] = 0;

						
							// ghi vao file
							strcpy_s(mang[i].Name_user,temp);
							GHI << mang[i].Name_user << endl;
							

							// nhận và kiểm tra kết quả 
							mang[i].Diem = 0;
							for (int k = 0; k < 3; k++)
							{
								sockClients[i].Receive((char*)&numbers[k+num_client], sizeof(int), 0);
								printf("Da nhan tu client %d lan %d, so %d\n", i+1, k+1, numbers[k+num_client]);
								// CHECK VÀ GỬI VỀ CLIENT
								if(numbers[k+num_client]==num_test[0])
								{
									strcpy_s(chuoi_tb, "Ban doan dung roi, ban duoc cong 2 diem!!!");
									mang[i].Diem += 2;
								}
								else
								{
									strcpy_s(chuoi_tb, "Ban doan sai roi, ban bi tru 1 diem!!!") ;
									mang[i].Diem -= 1;
								}
								int diem = mang[i].Diem;

								int leng = strlen(chuoi_tb);
								sockClients[i].Send((char*)&leng, sizeof(int), 0);// gửi độ dài chuỗi về cho client
								sockClients[i].Send(chuoi_tb, leng, 0); //gửi thông báo kết quả cho client
								sockClients[i].Send((char*)&diem, sizeof(int), 0); // gửi điểm về cho client
							}
							//delete temp;
						}		
					}
					if(flag_out_while == 1)
						break;
				}
						
			}
		}
		

		
		//Gui ket qua cho tung client
		for (int i = 0; i < num_client; i++)
		{
			/*printf("Gui ket qua cho client %d\n", i+1);
			sockClients[i].Send((char*)&count,sizeof(int),0);*/
	
			//Ngat ket noi voi client do
			 //Ngat ca chieu Gui va Nhan
			sockClients[i].Close();
		}
		system("pause");

		server.Close();
		
	}

	return nRetCode;
}





















































































































