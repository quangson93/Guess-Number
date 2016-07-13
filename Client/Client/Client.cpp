// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include "conio.h"
#include "iostream"
#include "string"
#include "fstream" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


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
		CSocket client;
		char sAdd[1000];
		unsigned int port = 1234; //Cung port voi server
		AfxSocketInit(NULL);

		//1. Tao socket
		client.Create();

		// nhap dc IP cua server
		cout << "\n Nhap dia chi IP cua server: ";
		gets_s(sAdd);	
		
		if (client.Connect((CA2W) sAdd ,port) == 0)
			cout << "Khong the ket noi toi Server!!!" << endl;
		else
		{
			cout << "\nClient da ket noi toi server"<< endl;
			
			// tạo tên người chơi
			char Name_User[20];
			int len;
			int flag;
			// Nhap ten nguoi choi va nhan ket qua kiem tra tu server
			while(1)
			{
				cout << "Moi ban nhap ten nguoi choi: " ;
				cin.getline(Name_User,20);// nhap vao mot chuoi ki tu
				len = strlen(Name_User);// do do dai cua chuoi ki tu vua nhap
				
				client.Send((char*)&len, sizeof(len), 0);//gui do dai len cho server 
				client.Send(Name_User, len, 0); // gui chuoi ki tu vua nhap len server
				client.Receive((char*)&flag, sizeof(int), 0); // Nhan ket qua tra ve tu server, voi flag = 1 la ket qua tra ve SAI
				if(flag == 1)
				{	
					cout << "Ban da nhap sai quy dinh, vui long nhap lai!!! "<<endl;
					continue;
				}
				else 
					break;
			}
			cout << "Moi ban "<< Name_User << " bat dau choi!\n\n" <<endl;
			
			int len_str;
			char* mang_kitu;
			int numbers, diem;
			for(int i = 0; i < 3; i++)
			{
				cout << "\n\nGui so thu " << i + 1 << " cho server: ";
				cin >> numbers;
				client.Send((char*)&numbers, sizeof(int), 0);
				client.Receive((char*)&len_str, sizeof(int), 0); 
				//nhận kết quả từ server
				mang_kitu = new char[len_str +1]; // tao ra mot mang ki tu de chua ket qua tra ve tu server
				client.Receive((char*)mang_kitu, len_str, 0);
				mang_kitu[len_str] = 0;	
				cout << mang_kitu << endl;
				client.Receive((char*)&diem, sizeof(int), 0);
				cout << "So diem hien tai cua "<< Name_User<<" la: "<< diem <<endl;
				delete mang_kitu;
			}
			if(diem > 0)
				cout << "\nBan la nguoi chien thang!!!" << endl;
			else
				cout << "\nBan da thua roi!!!" << endl;
				
			
			client.Close();
				
			}
		
		_getch();
	}

	return nRetCode;
}
