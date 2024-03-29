#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<linux/ip.h>
#include<linux/udp.h>
#include<linux/tcp.h>
using namespace std;

int getRsfd(int proto){
    int rsfd=socket(AF_INET,SOCK_RAW,proto); // here protocol is for what protos we can hear
    if(rsfd<0){
        perror("rsock");
        exit(0);
    }

    return rsfd;
}

void printTCPHdr(char *packet,int sz){
    // ethhdr len is not needed to consider as packets are from layer-3
    unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)( packet );
	iphdrlen = iph->ihl*4;
	
	struct tcphdr *tcph=(struct tcphdr*)(packet + iphdrlen );
			
	int header_size =  iphdrlen + tcph->doff*4;

	printf("TCP Header\n");
	printf("   |-Source Port      : %u\n",ntohs(tcph->source));
	printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
	printf("   |-Sequence Number    : %u\n",ntohl(tcph->seq));
	printf("   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
 	printf("   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
	printf("   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
	printf("   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
	printf("   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
	printf("   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
	printf("   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
	printf("   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
	printf("   |-Window         : %d\n",ntohs(tcph->window));
	printf("   |-Checksum       : %d\n",ntohs(tcph->check));
	printf("   |-Urgent Pointer : %d\n",tcph->urg_ptr);

    // **correct way of printing tcp-data**
    if(sz>(iph->ihl * 4 + tcph->doff*4)){
        char *payload = packet + iph->ihl * 4 + tcph->doff*4;
        cout << "Data : " << payload << endl;
    }
} 

void printUDPHdr(char *packet,int sz){
    // ethhdr len is not needed to consider as packets are from layer-3
    unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)(packet);
	iphdrlen = iph->ihl*4;
	
	struct udphdr *udph = (struct udphdr*)(packet + iphdrlen);
	
	int header_size =  iphdrlen + sizeof udph;
	
	printf("\nUDP Header\n");
	printf("   |-Source Port      : %d\n" , ntohs(udph->source));
	printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
	printf("   |-UDP Length       : %d\n" , ntohs(udph->len));
	printf("   |-UDP Checksum     : %d\n" , ntohs(udph->check));

}

void printIPHdr(char *packet,int sz){
    
    struct iphdr *ipHeader=(struct iphdr*)packet;
    struct sockaddr_in source,dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ipHeader->saddr;
    
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ipHeader->daddr;
    
    // if(strcmp(inet_ntoa(source.sin_addr),"127.8.8.8")) return;

    cout<<"\nGot a Packet :"<<endl;
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

    if(ipHeader->protocol==6){
        printTCPHdr(packet,sz);
    }
    else if(ipHeader->protocol==17){
        printUDPHdr(packet,sz);
        char *payload = packet + ipHeader->ihl * 4 + sizeof(struct udphdr);
        cout << "Data : " << payload << endl;
    }

}

int main(){
    int proto;
    cout<<"Give protocol : ";
    cin>>proto;
    int rsfd=getRsfd(proto);

    while(1){
        char packet[6000]={'\0'};
        int sz=recvfrom(rsfd,packet,sizeof packet,0,NULL,NULL);
        if(sz<0){
            perror("recv");
            exit(0);
        }
        printIPHdr(packet,sz);
    }
}