/***************************************************************************
                          TcpDumpPacketProvider.cpp  -  description
                             -------------------
    begin                : Sun Apr 27 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include "TcpDumpPacketProvider.h"
#include "PreProcessor.h"

uint32_t preNetMask, preNetAddr;
extern PreProcessor PreProcessorObj;

//#################        FUNCTION SNIFFER CONSTRUCTOR       ####################
TcpDumpPacketProvider::TcpDumpPacketProvider()
{
}


//#################        FUNCTION SNIFFER DESTRUCTOR       ####################
TcpDumpPacketProvider::~TcpDumpPacketProvider()
{
}


//#################        FUNCTION SET OWRK SPACE PARAMETERS       ####################
void TcpDumpPacketProvider::SetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format: <NetAddr> , <NetMask> , <Interface> , <Filter> , <OffLineTrafficSourceName>
    int i,j;
    char StrTemp[50];

    strcpy(Interface,"");
    strcpy(Filter,"");
    strcpy(OffLineTrafficSourceName,"");
    
    if (!strncasecmp(ParamString, "null", 4)) return;

    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    NetAddr=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=','  && ParamString[i]!='\0' && ParamString[i]!='\n' &&ParamString[i]!='\r'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    NetMask=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' &&ParamString[i]!='\r'; i++, j++) Interface[j]=ParamString[i];
    Interface[j]='\0';
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' &&ParamString[i]!='\r'; i++, j++) Filter[j]=ParamString[i];
    Filter[j]='\0';
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' &&ParamString[i]!='\r'; i++, j++) OffLineTrafficSourceName[j]=ParamString[i];
    OffLineTrafficSourceName[j]='\0';
    
    if ( !*Filter ) strcpy(Filter, "ip");
}//End of function TcpDumpPacketProvider::SetWorkSpaceParameters


//#################        FUNCTION GET WORK SPACE PARAMETERS       ####################
void TcpDumpPacketProvider::GetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format: <NetAddr> , <NetMask> , <Interface> , <Filter> , <OffLineTrafficSourceName>
    sprintf (ParamString, "%d,%d,%s,%s,%s", 
	     NetAddr,
	     NetMask,
	     Interface, Filter, OffLineTrafficSourceName);
}//End of function TcpDumpPacketProvider::GetWorkSpaceParameters


//#################        FUNCTION START SNIFFER       ####################
int TcpDumpPacketProvider::StartSniffer (bool RealTime)
{
    if ( OpenPacketSourceActive(RealTime) )
    {
        signal(SIGINT,  ClosePacketSourceActive);
        signal(SIGTERM, ClosePacketSourceActive);
        signal(SIGQUIT, ClosePacketSourceActive);
	
        if (RealTime)
        {
            ReadPackets(pd, ONLINE, 0, (pcap_handler)ParsePacketActive);
            ClosePacketSourceActive(0);
        }
        else 
        {
            ReadPackets(pd, OFFLINE, 0, (pcap_handler)ParsePacketActive);
            ClosePacketSourceActive(-1);
        }
    }
    else return 0;
    
    return 1;
}//End of function TcpDumpPacketProvider::StartSniffer


//#################        FUNCTION STOP SNIFFER       ####################
int TcpDumpPacketProvider::StopSniffer ()
{
    //Load user defined NatMask and NetAddr
    NetMask=preNetMask;
    NetAddr=preNetAddr;

    return 1;
}//End of function TcpDumpPacketProvider::StopSniffer


//#################        FUNCTION PACKET SOURCE ACTIVE       ####################
int TcpDumpPacketProvider::OpenPacketSourceActive (bool RealTime)
{
    if (RealTime)
    {
        if (! Open_pcap_Online(Interface, Filter) ) 
            return 0;
    }
    else 
    {
        if (! Open_pcap_Offline(OffLineTrafficSourceName) ) 
            return 0;
    }
    return 1;
}//End of function TcpDumpPacketProvider:OpenPacketSourceActive


//#################        FUNCTION OPEN PCAP ON-LINE       ####################
pcap_t* TcpDumpPacketProvider::Open_pcap_Online(char* device, const char* Filter)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program  bpf;

    //Obtain Network Device based on system information & configuration
    if ( !*device && !(device = pcap_lookupdev(errbuf)) )
    {
        printf("pcap_lookupdev(): %s\n", errbuf);
        return NULL;
    }

    //Open pcap entry    
    if ( (pd = pcap_open_live(device, BUFSIZ, 1, 0, errbuf)) == NULL )
    {
        printf("pcap_open_live(): %s\n", errbuf);
        return NULL;
    }

    preNetMask=NetMask; //Save User defined NetMask for loading after exiting from online IDS mode
    preNetAddr=NetAddr; //Save User defined NetAddr for loading after exiting from online IDS mode
    
    //Obtain NetMask and NetAddr based on network device configuration
    if ( pcap_lookupnet(device, &NetAddr, &NetMask, errbuf) < 0 )
    {
        printf("pcap_lookupnet: %s\n", errbuf);
	
        //Load user defined NatMask and NetAddr
        NetMask=preNetMask;
        NetAddr=preNetAddr;
        return NULL;
    }

    //Compile packet filtering string 
    if ( pcap_compile(pd, &bpf, (char*)Filter, 0, NetMask ) )
    {
        printf("pcap_compile(): %s\n", pcap_geterr(pd));
	
        //Load user defined NatMask and NetAddr
        NetMask=preNetMask;
        NetAddr=preNetAddr;
        return NULL;
    }

    //Set Packet Filter 
    if ( pcap_setfilter(pd, &bpf) < 0 )
    {
        printf("pcap_setfilter(): %s\n", pcap_geterr(pd));
	
        //Load user defined NatMask and NetAddr
        NetMask=preNetMask;
        NetAddr=preNetAddr;
        return NULL;
    }

    //Determine Data Link Header Length for determining first point of IP header in packet
    if (!DetermineDataLinkLength()) 
    {
        //Load user defined NatMask and NetAddr
        NetMask=preNetMask;
        NetAddr=preNetAddr;
        return 0;
    }
    
    //Initialize PrePacketTS variable, that's using for saving previouse packet time stamp
    PrePacketTS.tv_sec=0;    
    PrePacketTS.tv_usec=0;
    
    return pd;
}//End of function TcpDumpPacketProvider::Open_pcap_socket


//#################        FUNCTION OPEN PACKET SOURCE PASSIVE       ####################
FILE* TcpDumpPacketProvider::OpenPacketSourcePassive(char * NetTrafficFileName, char * SessionListName)
{
    FILE * fd;
    
    fd=Open_pcap_Offline(NetTrafficFileName);
    if (strcmp(SessionListName,""))
    {
        if ( !(SessionListPointer=fopen(SessionListName, "r")) )
        {
            perror ("Error: TcpDumpPacketProvider.Open_pcap_Offline.fopen: Session List File Not Found !");
            return 0;
        }
        StrToTime("1/1/1900 00:00:00", CurrentSession.ts);
    }//end of if
    else SessionListPointer=NULL;
	
    return fd;
}//End of function TcpDumpPacketProvider::OpenPacketSourcePassive

//#################        FUNCTION OPEN PCAP OFF-LINE       ####################
FILE* TcpDumpPacketProvider::Open_pcap_Offline(char * NetTrafficFileName)
{
    FILE* fd;
    char errbuf[PCAP_ERRBUF_SIZE];

    pd = pcap_open_offline(NetTrafficFileName, errbuf);
    if (pd == NULL)
    {
        printf("Error: TcpDumpPacketProvider.Open_pcap_Offline.pcap_open_offline: %s\n", errbuf);
        return NULL;
    }
    
    fd = pcap_file(pd);
    
    if (!DetermineDataLinkLength()) 
        return NULL;
    
    //Initialize PrePacketTS variable, that's using for saving previouse packet time stamp
    PrePacketTS.tv_sec=0;    
    PrePacketTS.tv_usec=0;
    
    return fd;
}//End of function TcpDumpPacketProvider::Open_pcap_Offline

//#################        FUNCTION READ OFF-LINE PACKET       ####################
int TcpDumpPacketProvider::ReadOfflinePacket(char *NetPacketRecord)
{
    if (ReadPackets(pd, OFFLINE, 1, (pcap_handler)ParsePacketPassive)>0)
        strcpy(NetPacketRecord, PacketRecord);
    else 
        return 0;
    
    return 1;
}//End of function TcpDumpPacketProvider::ReadOfflinePacket


//#################        FUNCTION DETERMINE DATA LINK LENGTH       ####################
int TcpDumpPacketProvider::DetermineDataLinkLength(void)
{
    int linktype;
 
    if ( (linktype = pcap_datalink(pd)) < 0 )
    {
        printf("pcap_datalink(): %s\n", pcap_geterr(pd));
        return 0;
    }

    switch ( linktype )
    {
    case DLT_NULL:
        linkhdrlen = 4;
        break;

    case DLT_EN10MB:
        linkhdrlen = 14;
        break;

    case DLT_SLIP:
    case DLT_PPP:
        linkhdrlen = 24;
        break;

    default:
        printf("Unsupported datalink (%d)\n", linktype);
        return 0;
    }
    return 1;
}//End of function TcpDumpPacketProvider::DetermineDataLinkLength


//#################        FUNCTION READ PACKETS       ####################
int TcpDumpPacketProvider::ReadPackets(pcap_t* pd, int Mode, int packets, pcap_handler func)
{
    int retval;
    if (Mode==ONLINE) 
        retval=pcap_loop(pd, packets, func, 0);
    else 
        retval=pcap_dispatch(pd, packets, func, 0);
    
    if (retval<0)
        printf("pcap_loop failed: %s\n", pcap_geterr(pd));
    return retval;
}//End of function TcpDumpPacketProvider::CaptureLoop


//#################        FUNCTION ON LINE PARSE PACKET       ####################
void ParsePacketActive(u_char *user,struct pcap_pkthdr *packethdr,u_char* packetptr)
{
    extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
    TcpDumpPacketProviderObj.ParsePacket(packethdr, packetptr, false);
    PreProcessorObj.GivePacket(TcpDumpPacketProviderObj.PacketRecord);
}//End of funcyion OnLineParsePacket


//#################        FUNCTION OFF LINE PARSE PACKET       ####################
void ParsePacketPassive(u_char *user,struct pcap_pkthdr *packethdr,u_char* packetptr)
{
    extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
    TcpDumpPacketProviderObj.ParsePacket(packethdr, packetptr, true);
}//End of funcyion OffLineParsePacket


//#################        FUNCTION PARSE PACKET       ####################
void TcpDumpPacketProvider::ParsePacket(struct pcap_pkthdr *packethdr, u_char* packetptr, bool WithSessionType)
{
    struct ip *iphdr;
    struct icmphdr* icmphdr;
    struct tcphdr* tcphdr;
    struct udphdr* udphdr;
    unsigned short id, seq;
    char iphdrInfo[256], 
           SrcIP[32], DstIP[32],
           TimeStamp[20],
           IsLand, IsSrcIpInHome, IsDstIpInHome;
    long int PacketsIntervalTime;

    /**********************  Extract IP header from packet  ***********************/
    packetptr += linkhdrlen; //Point to first of IP header in packet 
    iphdr = (struct ip*)packetptr;
    strcpy(SrcIP, inet_ntoa(iphdr->ip_src));
    strcpy(DstIP, inet_ntoa(iphdr->ip_dst));
    
    if ( (NetMask & NetAddr) == (NetMask & iphdr->ip_src.s_addr) ) IsSrcIpInHome='1'; //Home Net IP Address
    else IsSrcIpInHome='0'; //External Net IP Address
    if ((NetMask & NetAddr) == (NetMask & iphdr->ip_dst.s_addr) )  IsDstIpInHome='1'; //Home Net IP Address
    else IsDstIpInHome='0'; //External Net IP Address
    if ( iphdr->ip_src.s_addr== iphdr->ip_dst.s_addr) IsLand='1'; //Land : Same Source & Destination IP Address
    else IsLand='0';
    
    PacketsIntervalTime = (packethdr->ts.tv_sec - PrePacketTS.tv_sec)*1000000+(packethdr->ts.tv_usec - PrePacketTS.tv_usec);
    PacketsIntervalTime /= 100;
    sprintf (TimeStamp, "%ld", PacketsIntervalTime);
    //strftime(TimeStamp, sizeof TimeStamp, "%H", localtime(&packethdr->ts.tv_sec));
    
    PrePacketTS.tv_sec=packethdr->ts.tv_sec;
    PrePacketTS.tv_usec=packethdr->ts.tv_usec;

    sprintf(iphdrInfo, "%s,%d,%d,%d,%d,%d,%c,%c,%c,%d",
	TimeStamp, //Packet arriving time stamp
	ntohs(iphdr->ip_id), //IP Id
	iphdr->ip_tos, iphdr->ip_ttl, //IP TTL
	4*iphdr->ip_hl, //IP Header Length
	ntohs(iphdr->ip_len), //IP Data Length
	IsSrcIpInHome, IsDstIpInHome, 
	IsLand, //IP Land
	(ntohs(iphdr->ip_off) & 0xE000 )>>13); //IP Fragmentation Flag

    packetptr += 4*iphdr->ip_hl; //Point to first of IP data, or first of TCP/UDP/ICMP header in packet
    
    switch (iphdr->ip_p) //What's protocol?
    {
        /**********************  Extract TCP header from packet  ***********************/
        case IPPROTO_TCP:  //TCP Packet
            tcphdr = (struct tcphdr*)packetptr;
            sprintf(PacketRecord,"0,%s,%d,%d,%c,%c,%c,%c,%c,%c,%d,%d", 
    		iphdrInfo, //IP Header
    		ntohs(tcphdr->th_sport), //TCP Source Port
		ntohs(tcphdr->th_dport), //TCP Destination Port
    		(tcphdr->th_flags & 1) ? '1' : '0',  // Fin Flag
    		(tcphdr->th_flags & 2) ? '1' : '0',  // Syn Flag
    		(tcphdr->th_flags & 4) ? '1' : '0', // Rst Flag
    		(tcphdr->th_flags & 8) ? '1' : '0', //Push Flag
    		(tcphdr->th_flags & 16) ? '1' : '0', // Ack Flag
    		(tcphdr->th_flags & 32) ? '1' : '0', // Urg Flag
    		4*tcphdr->th_off, //TCP Offset (Header Length)
    		ntohs(tcphdr->th_win)); //TCP Window Size
            
            if (WithSessionType)
            {
                strcat (PacketRecord, ",");
                strcat (PacketRecord, GetPacketSessionType( packethdr->ts.tv_sec, "tcp", SrcIP, DstIP, ntohs(tcphdr->th_sport), ntohs(tcphdr->th_dport) ));
            }
            strcat (PacketRecord, ".");
	    
//            packetptr+=4*tcphdr->th_off;
//            strcat (PacketRecord, packetptr); // If you want send data segment of packet put this line in program
            break;
    
        /**********************  Extract UDP header from packet  ***********************/
        case IPPROTO_UDP: //UDP Packet
            udphdr = (struct udphdr*)packetptr;
            sprintf(PacketRecord,"1,%s,%d,%d", 
    		iphdrInfo, //IP Header 
		ntohs(udphdr->uh_sport), //UDP Source Port
		ntohs(udphdr->uh_dport)); //UDP Destination Port
            if (WithSessionType)
            {
                strcat (PacketRecord, ",");
                strcat (PacketRecord, GetPacketSessionType( packethdr->ts.tv_sec, "udp", SrcIP, DstIP, ntohs(udphdr->uh_sport), ntohs(udphdr->uh_dport) ));
            }
            strcat (PacketRecord, ".");
	    
//            packetptr+=8;
//            strcat (PacketRecord, packetptr);
            break;
    
        /**********************  Extract ICMP header from packet  ***********************/
        case IPPROTO_ICMP: //ICMP Packet
            icmphdr = (struct icmphdr*)packetptr;
            memcpy(&id, (u_char*)icmphdr+4, 2);
            memcpy(&seq, (u_char*)icmphdr+6, 2);
            sprintf(PacketRecord,"2,%s,%d,%d,%d,%d", 
    		iphdrInfo, //IP Header 
		icmphdr->type, //ICMP Type
		icmphdr->code, //ICMP Code
		ntohs(id), //ICMP Id
		ntohs(seq)); //ICMP Sequence 
            if (WithSessionType)
            {
                strcat (PacketRecord, ",");
                strcat (PacketRecord, GetPacketSessionType( packethdr->ts.tv_sec, "icmp", SrcIP, DstIP, 0, 0 ));
            }
            strcat (PacketRecord, ".");
            break;
    }//end of switch

//    printf("%s\n",PacketRecord);
    
}//End of function TcpDumpPacketProvider::ParsePacket


//#################        FUNCTION GET SESSION TYPE OF PACKET       ####################
char * TcpDumpPacketProvider::GetPacketSessionType(time_t ts, char *protocol, char *srcip, char *dstip, short int sport, short int dport)
{
    double Diff;
    if ( difftime(ts, mktime(&CurrentSession.ts)) - CurrentSession.dur > 0 )
	ReadNextSession(SessionListPointer, CurrentSession);
    
    //compare protocols
    if ( !strcasecmp(protocol, CurrentSession.protocol) )  
	
        //compare source ip, if session ip is 0.0.0.0 means "any ip"
        if ( !strcmp(CurrentSession.srcip, "0.0.0.0") ||  !strcmp(srcip, CurrentSession.srcip)) 
	    
            //compare destination ip, if session ip is 0.0.0.0 means "any ip"
            if (!strcmp(CurrentSession.dstip, "0.0.0.0") || !strcmp(dstip, CurrentSession.dstip) ) 
		
                // compare source port, if session port is 0 means "any port"		
                if ( !CurrentSession.sport || sport==CurrentSession.sport) 
		    
                    // compare destination port, if session port is 0 means "any port"
                    if ( !CurrentSession.dport || dport==CurrentSession.dport ) 
                    {
                        Diff=difftime(ts, mktime(&CurrentSession.ts));
                        if ( Diff>=0 && Diff - CurrentSession.dur <=0 )
                            return CurrentSession.type;
                    }
    
    return "normal";
}//End of function TcpDumpPacketProvider::IsAttackPacket


//#################        FUNCTION READ NEXT SESSION       ####################
int TcpDumpPacketProvider::ReadNextSession (FILE * SLPointer, struct Session & CS)
{
    int i, j;
    char SessionStr[512];
    char StrTemp[256];
    
    if (! fgets(SessionStr, 512, SLPointer) ) return 0;
    
    for (i=0; SessionStr[i]!=' ' && SessionStr[i]!='\0'; i++) StrTemp[i]=SessionStr[i];
    StrTemp[i]='\0';
    CS.id=atoi(StrTemp);
	
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]=' ';
    for (i++,j++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';

    StrToTime(StrTemp, CS.ts);
    
    for (j=0, i++; SessionStr[i]!=':' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.dur=3600 * atoi(StrTemp);
    for (j=0, i++; SessionStr[i]!=':' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.dur+=60 * atoi(StrTemp);
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.dur+=atoi(StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    strcpy(CS.protocol, StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.sport=atoi(StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.dport=atoi(StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    strcpy (CS.srcip, StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    strcpy (CS.dstip, StrTemp);
    
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    CS.score=atof(StrTemp);
       
    for (j=0, i++; SessionStr[i]!=' ' && SessionStr[i]!='\0' && SessionStr[i]!='\n'; i++, j++) StrTemp[j]=SessionStr[i];
    StrTemp[j]='\0';
    strcpy (CS.type, StrTemp);

    return 1;
    
}//End of function TcpDumpPacketProvider::ReadNextSession


//#################        FUNCTION STR TO TIME       ####################
void TcpDumpPacketProvider::StrToTime(char * TimeStr, struct tm &TS)
{
    int i, j;
    char StrTemp[20];
    
    for (i=0; TimeStr[i]!='/' && TimeStr[i]!='\0'; i++) StrTemp[i]=TimeStr[i];
    StrTemp[i]='\0';
    TS.tm_mon=atoi(StrTemp)-1;
    for (j=0, i++; TimeStr[i]!='/' && TimeStr[i]!='\0' && TimeStr[i]!='\n'; i++, j++) StrTemp[j]=TimeStr[i];
    StrTemp[j]='\0';
    TS.tm_mday=atoi(StrTemp);    
    for (j=0, i++; TimeStr[i]!=' ' && TimeStr[i]!='\0' && TimeStr[i]!='\n'; i++, j++) StrTemp[j]=TimeStr[i];
    StrTemp[j]='\0';
    TS.tm_year=atoi(StrTemp)-1900;    
    for (j=0, i++; TimeStr[i]!=':' && TimeStr[i]!='\0' && TimeStr[i]!='\n'; i++, j++) StrTemp[j]=TimeStr[i];
    StrTemp[j]='\0';
    TS.tm_hour=atoi(StrTemp);    
    for (j=0, i++; TimeStr[i]!=':' && TimeStr[i]!='\0' && TimeStr[i]!='\n'; i++, j++) StrTemp[j]=TimeStr[i];
    StrTemp[j]='\0';
    TS.tm_min=atoi(StrTemp);    
    for (j=0, i++; TimeStr[i]!=':' && TimeStr[i]!='\0' && TimeStr[i]!='\n'; i++, j++) StrTemp[j]=TimeStr[i];
    StrTemp[j]='\0';
    TS.tm_sec=atoi(StrTemp);
}//End of function TcpDumpPacketProvider::StrToTime


//#################        FUNCTION CLOSE PACKET SOURCE PASSIVE       ####################
void TcpDumpPacketProvider::ClosePacketSourcePassive()
{
    pcap_close(pd);
    if (SessionListPointer) fclose(SessionListPointer);
}//End of function TcpDumpPacketProvider::ClosePacketSourcePassive


//#################        FUNCTION CLOSE PACKET SOURCE ACTIVE       ####################
void ClosePacketSourceActive(int signo)
{
    extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
    
    //Close pcap entry
    pcap_close(TcpDumpPacketProviderObj.pd);
    
    //Load user defined NatMask and NetAddr
    TcpDumpPacketProviderObj.NetMask=preNetMask;
    TcpDumpPacketProviderObj.NetAddr=preNetAddr;
    
    if (signo != -1) exit(0);
}//End of function ClosePacketSourceActive

