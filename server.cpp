#include <iostream>
#include <winsock.h>
using namespace std;


int main(int argc,char **argv){

    if(argc<2){
        cout<<"usages : "<<argv[0]<<" <port number>";
        return -1;
    }
    //initilise wsa api for windows compatiblity
    WSADATA ws;

    if(WSAStartup(MAKEWORD(2,2),&ws)<0){
        cout<<"WSAStartup failed to initlise";
        return -1;
    }else{
        cout<<"WSAStartup is initialized successfully."<<endl;
    }

    //create socket

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock<0){
        cout<<"unable to create socket";
        return -1;
    }else{
        cout<<"socket is initialized."<<endl;
    }

    //initialize address values 
    int PORT = atoi(argv[1]);
    cout<<PORT;
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    memset(&servaddr.sin_zero,0,sizeof(servaddr.sin_zero));

    //bind address and port to socket

    int bindval = bind(sock,(sockaddr*)&servaddr,sizeof(servaddr));
    if(bindval<0){
        cout<<"unable to bind port to socket";
        return -1;
    }else{
        cout<<"address bound to socket successfully"<<endl;
    }

    //listen for incoming connections

    int lis = listen(sock,1);
    if(lis<0){
        cout<<"unable to listen for incoming connections";
        return -1;
    }else{
        cout<<"started to listen for incoming connections"<<endl;
    }

    

     // get incoming clients address

    sockaddr_in client;
    memset(&client,0,sizeof(client));
    int addrlen = sizeof(client);
    
    
    // accept connection
    int acpt = accept(sock,(sockaddr*)&client,&addrlen);
    if(acpt<0){
        cout<<"unable to accept new connectio";
        return -1;
    }
    
    string client_addr = inet_ntoa(client.sin_addr);
    int client_port = ntohs(client.sin_port);

    cout<<"Server connected to ip : "<<client_addr<<" and port : "<<client_port<<endl;


    closesocket(sock);
    cout<<"main socket is closed"<<endl;
    //receive incoming data
    char buff[4096];
    while(true){
        memset(buff,0,4096);

        //receive bytes

        int receive_bytes = recv(acpt,buff,sizeof(buff)-1,0);
        if(receive_bytes==0){
            cout<<"client disconnected";
            return -1;
        }else if(receive_bytes<0){
            cout<<"error receiving data from client";
            break;
        }else{
            cout<<"\nclient > "<<buff<<endl;
            //send message to client
            string s;
            cout<<"server > ";
            getline(cin,s);
            int sdmsg = send(acpt,s.c_str(),s.length(),0);
            if(sdmsg<0){
                cout<<"error sending message back to client";
            }
        }

    }

    closesocket(sock);
    return 0;
}
