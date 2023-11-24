#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<linux/if_ether.h>      // for ETH_P_IP | ETH_P_ALL
#include<linux/ip.h>
#include<linux/udp.h>
#include<linux/tcp.h>
using namespace std;

int getRsfd(){
    int rsfd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL)); // here protocol is for what protos we can hear
    if(rsfd<0){
        perror("rsock");
        exit(0);
    }

    return rsfd;
}

void printTCPHdr(char *packet,int sz){
    // ethhdr len is not needed to consider as packets are from layer-3
    unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)( packet + sizeof(struct ethhdr) );
	iphdrlen = iph->ihl*4;
	
	struct tcphdr *tcph=(struct tcphdr*)(packet + sizeof(struct ethhdr) + iphdrlen );
			
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
        char *payload = packet + sizeof(struct ethhdr) + iph->ihl * 4 + tcph->doff*4;
        cout << "Data : " << payload << endl;
    }
} 

void printUDPHdr(char *packet,int sz){
    // ethhdr len is not needed to consider as packets are from layer-3
    unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
	iphdrlen = iph->ihl*4;
	
	struct udphdr *udph = (struct udphdr*)(packet + sizeof(struct ethhdr) + iphdrlen);
	
	int header_size =  iphdrlen + sizeof udph;
	
	printf("\nUDP Header\n");
	printf("   |-Source Port      : %d\n" , ntohs(udph->source));
	printf("   |-Destination Port : %d\n" , ntohs(udph->dest));
	printf("   |-UDP Length       : %d\n" , ntohs(udph->len));
	printf("   |-UDP Checksum     : %d\n" , ntohs(udph->check));

}

void printIPHdr(char *packet,int sz){
    
    struct iphdr *ipHeader=(struct iphdr*)(packet+sizeof(struct ethhdr));
    struct sockaddr_in source,dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ipHeader->saddr;
    
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ipHeader->daddr;

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
        char *payload = packet + sizeof(struct ethhdr) + ipHeader->ihl * 4 + sizeof(struct udphdr);
        cout << "Data : " << payload << endl;
    }

}

void printARPHdr(char *packet,int sz){
    struct ether_arp *arpHdr=(struct ether_arp*)(packet+sizeof(struct ethhdr));
    printf("\nARP Header\n");
    printf("\t|-HW Type : %d\n",ntohs(arpHdr->arp_hrd));
    printf("\t|-Protocol : %d\n",ntohs(arpHdr->arp_pro));
    printf("\t|-HW Len : %d\n",arpHdr->arp_hln);
    printf("\t|-Protocol Len : %d\n",arpHdr->arp_pln);
    printf("\t|-OpCode : %d\n",ntohs(arpHdr->arp_op));
    printf("\t|-Source MAC : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",arpHdr->arp_sha[0],arpHdr->arp_sha[1],arpHdr->arp_sha[2],arpHdr->arp_sha[3],arpHdr->arp_sha[4],arpHdr->arp_sha[5]);
    printf("\t|-Destination MAC : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",arpHdr->arp_tha[0],arpHdr->arp_tha[1],arpHdr->arp_tha[2],arpHdr->arp_tha[3],arpHdr->arp_tha[4],arpHdr->arp_tha[5]);
    printf("\t|-Sender IP: %u.%u.%u.%u\n",arpHdr->arp_spa[0], arpHdr->arp_spa[1], arpHdr->arp_spa[2], arpHdr->arp_spa[3]);
    printf("\t|-Target IP: %u.%u.%u.%u\n", arpHdr->arp_tpa[0], arpHdr->arp_tpa[1], arpHdr->arp_tpa[2], arpHdr->arp_tpa[3]);
}

void printEthHdr(char *packet,int sz){
    if(sz>sizeof(struct ethhdr)){

        struct iphdr *ipHeader=(struct iphdr*)(packet+sizeof(struct ethhdr));
        // struct sockaddr_in source,dest;
        // memset(&source, 0, sizeof(source));
        // source.sin_addr.s_addr = ipHeader->saddr;
        
        // memset(&dest, 0, sizeof(dest));
        // dest.sin_addr.s_addr = ipHeader->daddr;
        // if(strcmp(inet_ntoa(source.sin_addr),"10.42.0.151")&&strcmp(inet_ntoa(source.sin_addr),"10.42.0.151")) return;
        
        struct ethhdr *ethHdr=(struct ethhdr*)packet;
        if(ethHdr->h_proto!=htons(ETH_P_ARP)) return;
        struct ether_arp *arpHdr=(struct ether_arp*)(packet+sizeof(struct ethhdr));
        string sip="",tip="";
        for(int i=0;i<4;i++){
            sip += to_string(arpHdr->arp_spa[i])+".";
            tip += to_string(arpHdr->arp_tpa[i])+".";
        }
        sip.pop_back(); tip.pop_back();
        if(sip!="192.168.173.134" && tip!="192.168.173.134") return;

        cout<<"\nGot a Packet :"<<endl;
        printf("\nEthernet Header\n");
        printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",ethHdr->h_source[0],ethHdr->h_source[1],ethHdr->h_source[2],ethHdr->h_source[3],ethHdr->h_source[4],ethHdr->h_source[5]);
        printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",ethHdr->h_dest[0],ethHdr->h_dest[1],ethHdr->h_dest[2],ethHdr->h_dest[3],ethHdr->h_dest[4],ethHdr->h_dest[5]);
        printf("\t|-Protocol : %d\n",ntohs(ethHdr->h_proto));
        if(ethHdr->h_proto==htons(ETH_P_ARP)) printARPHdr(packet,sz);
        else if(ethHdr->h_proto==htons(ETH_P_IP)) printIPHdr(packet,sz);
    }
}

int main(){
    int rsfd=getRsfd();

    while(1){
        char packet[6000]={'\0'};
        int sz=recvfrom(rsfd,packet,sizeof packet,0,NULL,NULL);
        if(sz<0){
            perror("recv");
            exit(0);
        }
        printEthHdr(packet,sz);
    }
}