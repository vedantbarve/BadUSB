// Build using the command => g++ server.cpp -o server.exe -lws2_32 -static-libstdc++ -static-libgcc -static
// g++ server.cpp -o server.exe -lws2_32

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

using namespace std;

#define BUFFER_SIZE 2048
#define PORT 55555
#define IP_ADDR "10.10.15.177"

int main(int argc, char const *argv[]){

    SOCKET serverSocket, acceptSocket;
    WSADATA wsaData;
    
    if( WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        cout<< " Failed at WSAStartup !" <<endl;
        return -1;
    }
    cout<< "[*] WSAStartup successful "<<endl;

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(serverSocket == INVALID_SOCKET){
        cout << "Error at socket()"<<endl;
        WSACleanup();
        return -1;
    }
    cout<< "[*] Socket successful "<<endl;

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(IP_ADDR);
    service.sin_port = htons(PORT);
    memset(&(service.sin_zero),0,8);

    if(bind(serverSocket,(SOCKADDR *)&service,sizeof(service)) == SOCKET_ERROR){
        cout << "Bind failed"<<endl;
        WSACleanup();
        return -1;
    }
    cout<< "[*] Bind successful "<<endl;

    if(listen(serverSocket,1) == SOCKET_ERROR){
        cout << "Listen failed"<<endl;
        WSACleanup();
        return -1;
    }

    cout<< "[*] Listening for incoming connections ..."<<endl;

    acceptSocket = accept(serverSocket,NULL,NULL);
    if(acceptSocket == INVALID_SOCKET){
        cout << "Accept failed"<<endl;
        WSACleanup();
        return -1;
    }

    int response;
    
    char receivedData[BUFFER_SIZE];
    char sendData[BUFFER_SIZE];
    char cwd[BUFFER_SIZE];

    strcpy(receivedData,"cd");
    strcpy(sendData,"\0");

    bool isCWD = true;

    while (true){
        response = recv(acceptSocket,receivedData,BUFFER_SIZE,0);
        if(isCWD){
            strcpy(cwd,receivedData);
            strcpy(receivedData,"\0");
            isCWD = false;
        }
        if(receivedData != "\0"){
            cout<<receivedData<<endl;
            strcpy(receivedData,"\0");
        }

        cout<<cwd<<">";
        cin.getline(sendData,BUFFER_SIZE);

        string command(sendData);
        
        if(strcmp(command.substr(0,2).c_str(),"cd") == 0){
            isCWD = true;
        }
        response = send(acceptSocket,sendData,BUFFER_SIZE,0);
        if(strcmp(command.c_str(),"exit") == 0){
            return -1;
        }
    }

    WSACleanup();

    cout<<endl;
    return 0;
}
