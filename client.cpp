#include <iostream>
#include <winsock.h>

using namespace std;
#define IP "127.0.0.1"
#define PORT 3000

int main(){

    // Initialize the wsa variable in windows (apis to enable socket communication in Windows)
    WSADATA ws;
    if(WSAStartup(MAKEWORD(2,2), &ws)<0){
        cout<<"wsa not initialized"<<endl;
        return -1;
    }else{
        cout<<"wsa initialized successfully"<<endl;
    }

    //create socket

    int nsock = socket(AF_INET, SOCK_STREAM, 0);

     if(nsock<0) {
         cout<<"cant open socket"<<endl;
         return -1;
     }
     else cout<<"socket created successfully"<<endl;

     
    // initialize variables for address

    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT); 
    serverAddress.sin_addr.s_addr = inet_addr(IP);

    //connect to server

    int connServ = connect(nsock, (sockaddr*)&serverAddress, sizeof(serverAddress));

    if(connServ<0) {
         cout<<"cant connect to server"<<endl;
         return -1;
     }
     else cout<<"connected to server successfully"<<endl;

     //send receive data

     char buff[4096];
     string userInput;

     do{
         memset(buff, 0, sizeof(buff));

         //prompt user for input message
         cout<<"\nclient > ";
         getline(cin,userInput);
         
         //send to server
         if(userInput.size()>=0){
                int snd = send(nsock,userInput.c_str(), sizeof(userInput)+1,0);
                if(snd<0) {
                cout<<"cant send message to server"<<endl;
                return -1;
                }
                else{
                    int rec = recv(nsock,buff,4096,0);
                    if(rec<0) {
                        cout<<"cant recv message from server"<<endl;
                        return -1;
                    }else{
                        
                        cout<<"\nServer > "<<buff;
                    }
                }
         }else{
             cout<<"please enter message"<<endl;
         }
         
     }while(userInput.size()>=0);

     //close the socket

     closesocket(nsock);
     return 0;
}