/***************************************************************************
                          TcpDumpPacketProvider.h  -  description
                             -------------------
    begin                : Sun Apr 27 2003
    copyright            : (C) 2003 by Vic Hargrave & Morteza Amini
    email                : vhargrave@attbi.com & MortezaAmini@yahoo.com
 ***************************************************************************/

#ifndef TCPDUMPPACKETPROVIDER_H
#define TCPDUMPPACKETPROVIDER_H

#undef _POSIX_SOURCE
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE
#undef _XOPEN_SOURCE_EXTENDED 
#undef _GNU_SOURCE 
#undef _SVID_SOURCE
#ifndef _BSD_SOURCE
    #define _BSD_SOURCE
#endif

#define OFFLINE 0
#define ONLINE 1

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <time.h>


struct Session
{
    int id;
    struct tm ts;
    int dur;
    char protocol[10];
    short int sport;
    short int dport;
    char srcip[16];
    char dstip[16];
    float score;
    char type[32];
};

void ClosePacketSourceActive(int);
void ParsePacketActive(u_char *, struct pcap_pkthdr *, u_char *);
void ParsePacketPassive(u_char *, struct pcap_pkthdr *, u_char *);

class TcpDumpPacketProvider
{
    friend void ParsePacketActive(u_char *, struct pcap_pkthdr *, u_char *);
    friend void ParsePacketPassive(u_char *, struct pcap_pkthdr *, u_char *);
    friend void ClosePacketSourceActive(int);
    
    private:
        pcap_t * pd;
        int linkhdrlen;
        char Interface[256],
             Filter[256],	     
             PacketRecord[512],
             OffLineTrafficSourceName[512];
        uint32_t  NetMask, NetAddr;
        FILE * SessionListPointer;
        Session CurrentSession;
        timeval PrePacketTS;

        pcap_t * Open_pcap_Online(char* , const char* );
        int DetermineDataLinkLength(void);
        int ReadPackets(pcap_t * , int , int , pcap_handler );
        void ParsePacket(struct pcap_pkthdr *,  u_char *, bool =false);
        char * GetPacketSessionType(time_t ts, char *protocol, char *srcip, char *dstip, short int sport, short int dport);
        int ReadNextSession (FILE *, struct Session &);
        void StrToTime(char * , struct tm &);
        int OpenPacketSourceActive (bool RealTime);
        FILE* Open_pcap_Offline(char * NetTrafficFileName);


    public:
        TcpDumpPacketProvider();
        ~TcpDumpPacketProvider();
        void GetWorkSpaceParameters(char*);
        void SetWorkSpaceParameters(char*);
        int StartSniffer(bool);
        int StopSniffer();
        void ClosePacketSourcePassive(void);
        FILE* OpenPacketSourcePassive(char * NetTrafficFileName, char * SessionListName);
        int ReadOfflinePacket(char *);
};

#endif

