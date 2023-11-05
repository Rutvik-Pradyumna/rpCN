#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<linux/if_ether.h> /* for ETH_IP_ALL or ETH_IP_IP */
#include<linux/ip.h>
using namespace std;
int main(){
    int rsfd=socket(AF_INET,SOCK_RAW,50);
    
    struct sockaddr_in cAddr,sAddr;

    cAddr.sin_family=AF_INET;
    if(inet_pton(AF_INET, "127.5.5.1", &cAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }

    // if(bind(rsfd,(struct sockaddr*)&cAddr,sizeof(cAddr))<0){
    //     perror("Error in bind()");
    //     return 0;
    // }
    int adrlen=sizeof sAddr;
    while(1){
        char buff[50]={'\0'};
        int st=recvfrom(rsfd,buff,sizeof buff,0,NULL,0);
        if(st<0) perror("recv");
        
        struct iphdr *ipHeader=(struct iphdr*)buff;
        struct sockaddr_in source,dest;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = ipHeader->saddr;
        
        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = ipHeader->daddr;
        
        printf("\n");
        printf("IP Header\n");
        printf("   |-IP Version        : %d\n",(unsigned int)ipHeader->version);
        printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)ipHeader->ihl,((unsigned int)(ipHeader->ihl))*4);
        printf("   |-Type Of Service   : %d\n",(unsigned int)ipHeader->tos);
        printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(ipHeader->tot_len));
        printf("   |-Identification    : %d\n",ntohs(ipHeader->id));
        printf("   |-TTL      : %d\n",(unsigned int)ipHeader->ttl);
        printf("   |-Protocol : %d\n",(unsigned int)ipHeader->protocol);
        printf("   |-Checksum : %d\n",ntohs(ipHeader->check));
        printf("   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
        printf("   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );

        char *payload = buff + (ipHeader->ihl * 4);
        cout << "Data : " << payload << endl;
    }
}