#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<linux/ip.h>
#include<linux/udp.h>
#include<linux/tcp.h>
using namespace std;

struct PseudoHeader{
    unsigned long int sourceIP;
    unsigned long int destIP;
    unsigned char reserved;
    unsigned char protocol;
    unsigned short int protoLen;    // can be tcp/udp
};

int getRsfd(){
    int rsfd=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
    if(rsfd<0){
        perror("rsock");
        exit(0);
    }
    return rsfd;
}

unsigned short csum(unsigned char *data,int len) 
{
	long sum;
	unsigned short *temp=(unsigned short*)data;

    while(len>1){
        sum += *temp++;
        if(sum & 0x80000000)
            sum = sum&0xFFFF + sum>>16;
        len -=2 ;
    }
    if(len)
        sum += (unsigned short) *((unsigned char*)temp);

    while(sum>>16){
        sum = sum&0xFFFF + sum>>16;
    }
    return ~sum;
}

void csumTCP(struct tcphdr *tcpHdr,struct iphdr *ipHdr,unsigned char *data){
    // calculated over pseudoHdr+tcpHdr+data
    // size of tcpHdr+data
    int segmentLen=ntohs(ipHdr->tot_len)-ipHdr->ihl*4;

    int headerLen=sizeof(struct PseudoHeader)+segmentLen;

    unsigned char *hdr=(unsigned char*)malloc(headerLen);

    PseudoHeader *pseudoHdr=(PseudoHeader*)hdr;

    pseudoHdr->sourceIP=ipHdr->saddr;
    pseudoHdr->destIP=ipHdr->daddr;
    pseudoHdr->reserved=0;
    pseudoHdr->protocol=ipHdr->protocol;
    pseudoHdr->protoLen=htons(segmentLen);
    // copy tcpHdr after pseudoHdr
    memcpy((hdr+sizeof(PseudoHeader)), (void*)tcpHdr,tcpHdr->doff*4);
    // copy data after tcpHdr
    memcpy((hdr+sizeof(PseudoHeader)+tcpHdr->doff*4),data,100);
    // calculate csum
    tcpHdr->check=csum(hdr,headerLen);
    return;
}

struct iphdr *createIPHdr(int proto){

    struct iphdr *ipHdr;
    ipHdr=(struct iphdr*)malloc(sizeof(struct iphdr));

    ipHdr->version=4;
    ipHdr->ihl=(sizeof(struct iphdr))/4;        // no.of double words
    ipHdr->tos=0;                               // type of service
    ipHdr->tot_len=htons(sizeof(struct iphdr)+sizeof(struct tcphdr)+100);
    // tot_len : len of iphdr+what it consists(ex:tcphdr/udphdr)
    ipHdr->id=htons(30);
    ipHdr->ttl=255;
    ipHdr->protocol=proto;
    ipHdr->saddr=inet_addr("198.168.192.11");
    ipHdr->daddr=inet_addr("198.12.16.71");
    ipHdr->check=0;
    // can send without giving csum for now but better to do this
    ipHdr->check=csum((unsigned char*)ipHdr,ipHdr->ihl*4);

    return ipHdr;
}

struct tcphdr *createTCPHdr(){
    struct tcphdr *tcpHdr;
    tcpHdr=(struct tcphdr*)malloc(sizeof(struct tcphdr));

    tcpHdr->source=htons(5000);
    tcpHdr->dest=htons(6000);
    tcpHdr->seq=htonl(111);
    tcpHdr->ack_seq=htonl(111);
    tcpHdr->res1=0;
    tcpHdr->doff=(sizeof(struct tcphdr))/4;     // dataoffset | where tcphdr ends
    tcpHdr->syn=1;
    tcpHdr->window=htons(100);
    tcpHdr->check=0;
    tcpHdr->urg_ptr=0;

    return tcpHdr;
}

struct udphdr *createUDPHdr(){
    struct udphdr *udpHdr;
    udpHdr=(struct udphdr*)malloc(sizeof(struct udphdr));

    udpHdr->source=htons(5550);
    udpHdr->dest=htons(6660);
    udpHdr->len=htons(8+100);
    udpHdr->check=0;

    return udpHdr;
}

int main(){
    int rsfd=getRsfd();

    unsigned char *packet;

    const char *data = "From injector"; 
    unsigned char *dataCopy = (unsigned char*)malloc(strlen(data) + 1); // Allocate memory for a copy
    if (dataCopy) {
        memcpy(dataCopy, data, strlen(data) + 1); // Copy the data to the allocated memory
    }

    int proto;
    cout<<"Give protocol : ";
    cin>>proto;

    struct iphdr *ipHdr;
    ipHdr=createIPHdr(proto);

    struct tcphdr *tcpHdr;
    struct udphdr *udpHdr;
    if(proto==6){
        tcpHdr=createTCPHdr();
        // can send without giving csum for now but better to do this
        csumTCP(tcpHdr,ipHdr,dataCopy);
    }else{
        udpHdr=createUDPHdr();
    }

    int pcktlen=ntohs(ipHdr->tot_len);
    packet=(unsigned char*)malloc(pcktlen);

    // copy ip-header
    memcpy(packet,ipHdr,ipHdr->ihl*4);
    
    // copy layer-4-header
    if(proto==6) memcpy(packet+ipHdr->ihl*4,tcpHdr,tcpHdr->doff*4);
    else memcpy(packet+ipHdr->ihl*4,udpHdr,ntohs(udpHdr->len));

    // copy data
    if(proto==6) memcpy((packet+ipHdr->ihl*4+tcpHdr->doff*4),data,100);
    else memcpy((packet+ipHdr->ihl*4+sizeof(struct udphdr)),data,100);

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_addr.s_addr=INADDR_ANY;
    int st=sendto(rsfd,packet,pcktlen,0,(struct sockaddr*)&caddr,sizeof caddr);
    if(st<0) perror("send");

}