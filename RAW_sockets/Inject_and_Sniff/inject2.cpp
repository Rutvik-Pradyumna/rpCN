#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/ether.h>
#include<netinet/if_ether.h>        // for arp_hdr
#include<linux/ip.h>
#include<linux/udp.h>
#include<linux/tcp.h>
#include<linux/if_packet.h>         // for sockaddr_ll
#include<net/if.h>                  //for ifreq   
#include<sys/ioctl.h>
using namespace std;

struct _arphdr {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t opcode;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
};

struct PseudoHeader{
    unsigned long int sourceIP;
    unsigned long int destIP;
    unsigned char reserved;
    unsigned char protocol;
    unsigned short int protoLen;    // can be tcp/udp
};

int getRsfd(){
    int rsfd=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
    if(rsfd<0){
        perror("rsock");
        exit(0);
    }

    struct sockaddr_ll sll;
    struct ifreq ifr;

    bzero(&sll,sizeof(sll));
    bzero(&ifr,sizeof(ifr));

    strncpy((char*)ifr.ifr_name,"wlo1",IFNAMSIZ);
    if(ioctl(rsfd,SIOCGIFINDEX,&ifr)==-1){
        perror("ioctl");
        exit(0);
    }

    sll.sll_family=AF_PACKET;
    sll.sll_ifindex=ifr.ifr_ifindex;
    sll.sll_protocol=htons(ETH_P_IP);

    if(bind(rsfd,(struct sockaddr*)&sll,sizeof(sll))<0){
        perror("bind");
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
    ipHdr->daddr=inet_addr("127.8.8.8");
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
    udpHdr->dest=htons(8080);
    udpHdr->len=htons(8+100);
    udpHdr->check=0;

    return udpHdr;
}

struct ethhdr *createEthHdr(string proto){
    struct ethhdr *ethHdr;
    ethHdr=(struct ethhdr*)malloc(sizeof(struct ethhdr));

    memcpy(ethHdr->h_source,ether_aton("11:22:33:44:55:66"),6);
    memcpy(ethHdr->h_dest,ether_aton("00:00:00:00:00:00"),6);
    if(proto=="ip") ethHdr->h_proto=htons(ETH_P_IP);
    // else if(proto=="arp") ethHdr->h_proto=htons(ETH_P_ARP);

    return ethHdr;
}

struct ether_arp createArpPacket(int rsfd){
    struct ether_arp req;
    req.arp_hrd=htons(ARPHRD_ETHER);
    req.arp_pro=htons(ETH_P_IP);
    req.arp_hln=ETHER_ADDR_LEN;
    req.arp_pln=sizeof(in_addr_t);
    req.arp_op=htons(ARPOP_REQUEST);
    memset(&req.arp_tha,0,sizeof(req.arp_tha));

    struct in_addr target_ip_addr={0};
    if (!inet_aton("192.168.173.151",&target_ip_addr)) {
        perror("not a valid IP address");
        exit(1);
    }
    memcpy(&req.arp_tpa,&target_ip_addr.s_addr,sizeof(req.arp_tpa));
    
    struct ifreq ifr;
    bzero(&ifr,sizeof(ifr));
    strncpy((char*)ifr.ifr_name,"wlo1",IFNAMSIZ);
    // Obtain the source IP address, copy into ARP request
    if (ioctl(rsfd,SIOCGIFADDR,&ifr)==-1) {
        perror(0);
        close(rsfd);
        exit(1);
    }
    struct sockaddr_in* source_ip_addr = (struct sockaddr_in*)&ifr.ifr_addr;
    memcpy(&req.arp_spa,&source_ip_addr->sin_addr.s_addr,sizeof(req.arp_spa));

    // Obtain the source MAC address, copy into Ethernet header and ARP request.
    if (ioctl(rsfd,SIOCGIFHWADDR,&ifr)==-1) {
        perror(0);
        close(rsfd);
        exit(1);
    }
    if (ifr.ifr_hwaddr.sa_family!=ARPHRD_ETHER) {
        fprintf(stderr,"not an Ethernet interface");
        close(rsfd);
        exit(1);
    }
    const unsigned char* source_mac_addr=(unsigned char*)ifr.ifr_hwaddr.sa_data;
    memcpy(&req.arp_sha,source_mac_addr,sizeof(req.arp_sha));

    return req;
}

void sendARP(int rsfd){
    struct ether_header header;
    header.ether_type=htons(ETH_P_ARP);
    memset(header.ether_dhost,0xff,sizeof(header.ether_dhost));

    struct ether_arp arpPacket;
    arpPacket=createArpPacket(rsfd);

    memcpy(header.ether_shost,arpPacket.arp_sha,sizeof(header.ether_shost));

    // Combine the Ethernet header and ARP request into a contiguous block.
    unsigned char frame[sizeof(struct ether_header)+sizeof(struct ether_arp)];
    memcpy(frame,&header,sizeof(struct ether_header));
    memcpy(frame+sizeof(struct ether_header),&arpPacket,sizeof(struct ether_arp));

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_addr.s_addr=INADDR_ANY;
    int st=send(rsfd,frame,sizeof(frame),0);
    if(st<0) perror("send");
    cout<<"Sent arp"<<endl;
}

int main(){
    int rsfd=getRsfd();

    int flag=0;
    cout<<"Enter 1 to send arp packet : ";
    cin>>flag;
    if(flag){
        sendARP(rsfd);
        exit(EXIT_SUCCESS);
    }

    unsigned char *packet;

    const char *data = "From injector"; 
    unsigned char *dataCopy = (unsigned char*)malloc(strlen(data) + 1); // Allocate memory for a copy
    if (dataCopy) {
        memcpy(dataCopy, data, strlen(data) + 1); // Copy the data to the allocated memory
    }

    int proto;
    cout<<"Give protocol : ";
    cin>>proto;

    struct ethhdr *ethHdr=createEthHdr("ip");

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

    int pcktlen=sizeof(struct ethhdr)+ntohs(ipHdr->tot_len);
    packet=(unsigned char*)malloc(pcktlen);
    memcpy(packet,ethHdr,sizeof(ethhdr));

    // copy ip-header
    memcpy(packet+sizeof(struct ethhdr),ipHdr,ipHdr->ihl*4);
    
    // copy layer-4-header
    if(proto==6) memcpy(packet+sizeof(struct ethhdr)+ipHdr->ihl*4,tcpHdr,tcpHdr->doff*4);
    else memcpy(packet+sizeof(struct ethhdr)+ipHdr->ihl*4,udpHdr,ntohs(udpHdr->len));

    // copy data
    if(proto==6) memcpy((packet+sizeof(struct ethhdr)+ipHdr->ihl*4+tcpHdr->doff*4),data,100);
    else memcpy((packet+sizeof(struct ethhdr)+ipHdr->ihl*4+sizeof(struct udphdr)),data,100);

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_addr.s_addr=INADDR_ANY;
    int st=send(rsfd,packet,pcktlen,0);
    if(st<0) perror("send");

}