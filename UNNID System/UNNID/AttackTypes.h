#ifndef ATTACKTYPES_H
#define ATTACKTYPES_H

/***********************************  Attack Classes  **************************************/
#define OTHER	0 // Other classes of attacks
#define DOS	1 // dos: denial of service attack
#define U2R	2 // r2l: remote to local attack
#define R2L	3 // u2r: user to root attack
#define PROBE	4 // probe: probing attack

/***********************************  Attack Types  **************************************/
#define UNKNOWN_ATTACK	1 //New Unknown Attack
#define NORMAL			2 //Normal Traffic

struct Attack
{
    char Name[25];
    char Desc[100];
    int Class;
};

#define ATTACK_NUMBER 	63

static Attack AttackDB[ATTACK_NUMBER]=
{ 
    {"####################", "Not Used", -1}, //0
    {"####################", "New Unknown Attack", -1}, //1
    {"normal", "Normal", -1}, //2
    {"back", "Back (DoS) Attack", DOS}, //3
    {"buffer_overflow", "Buffer Overflow (U2L) Attack", U2R}, //4
    {"ftp_write", "FTP Write (R2L) Attack", R2L}, //5
    {"guess_passwd", "Guess Password (R2L) Attack", R2L}, //6
    {"imap", "IMAP (R2L) Attack", R2L}, //7
    {"ipsweep", "IP Sweep (Probe) Attack", PROBE}, //8
    {"land", "Land (DoS) Attack", DOS}, //9
    {"loadmodule", "Load Module (U2R) Attack", U2R}, //10
    {"multihop", "Multi Hop (R2L) Attack", R2L}, //11
    {"neptune", "Neptune (DoS) Attack", DOS}, //12
    {"nmap", "Nmap (Probe) Attack", PROBE}, //13
    {"perl", "Perl (U2R) Attack", U2R}, //14
    {"phf", "Phf (R2L) Attack", R2L}, //15
    {"pod", "Pod (DoS) Attack", DOS}, //16
    {"portsweep", "Port Sweep (Probe) Attack", PROBE}, //17
    {"rootkit", "Root Kit (U2R) Attack", U2R}, //18
    {"satan", "Satan (Probe) Attack", PROBE}, //19
    {"smurf", "Smurf (DoS) Attack", DOS}, //20
    {"spy", "Spy (R2L) Attack", R2L}, //21
    {"teardrop", "Tear Drop (DoS) Attack", DOS}, //22
    {"warezclient", "Warez Client (R2L) Attack", R2L}, //23
    {"warezmaster", "Warez Master (R2L) Attack", R2L}, //24
    {"syndrop", "Syn Drop (DoS) Attack", DOS}, //25
    {"jolt", "Jolt (DoS) Attack", DOS}, //26
    {"winnuke", "Winnuke (DoS) Attack", DOS}, //27
    {"nestea", "Nestea (DoS) Attack", DOS}, //28
    {"processtable", "Process Table (DoS) Attack", DOS}, //29
    {"mailbomb", "Mail Bomb (DoS) Attack", DOS}, //30
    {"apache2", "Apache2 (DoS) Attack", DOS}, //31
    {"udpstorm", "UDP Storm (DoS) Attack", DOS}, //32
    {"httptunnel", "HTTP Tunnel (R2L) Attack", R2L}, //33
    {"named", "named Daemon (R2L) Attack", R2L}, //34
    {"xlock", "Xlock (R2L) Attack", R2L}, //35
    {"xsnoop", "Xsnoop (R2L) Attack", R2L}, //36
    {"sendmail", "Send Mail (R2L) Attack", R2L}, //37
    {"xterm", "Xterm (U2R) Attack", U2R}, //38
    {"snmpgetattack", "SNMP Get (U2R) Attack", U2R}, //39
    {"snmpguess", "SNMP Guess (U2R) Attack", U2R}, //40
    {"mscan", "Mscan (Probe) Attack", PROBE}, //41
    {"saint", "Saint (Probe) Attack", PROBE}, //42
    {"worm", "Worm Attack", OTHER}, //43
    {"sqlattack","SQL Attack", OTHER}, //44
    {"ps", "PS (U2R) Attack", U2R}, //45
    {"dosnuke", "NetBIOS Dosnuke (DoS) Attack", DOS}, //46
    {"synscan", "SYN Stealth Port Scan (Probe) Attack", PROBE}, //47
    {"bonk", "Bonk (DoS) Attack" , DOS}, //48
    {"newtear", "New Tear (Dos) Attack", DOS}, //49
    {"1234", "1234 (DoS) Attack", DOS}, //50
    {"saihyousen", "Saihyousen (DoS) Attack", DOS}, //51
    {"oshare", "Oshare (DoS) Attack", DOS}, //52
    {"smbdie", "SMBdie Windows (U2R) Attack",U2R}, //53
    {"killwin", "Kill Windows98 (DoS) Attack", DOS}, //54
    {"octopus", "Octopus (DoS) Attack", DOS}, //55
    {"twinge", "TWinge Windows98 (DoS) Attack", DOS}, //56
    {"tcpwindowscan", "TCP Window Scan (Proble) Attack", PROBE}, //57
    {"echochargen", "Echo <-> Chargen (DoS) Attack", DOS}, //58
    {"xmastree_scan", "Xmas Tree Stealth Port Scan (Probe) Attack", PROBE}, //59
    {"linuxicmp", "Linux Special ICMP Flood (DoS) Attack", DOS}, //60
    {"moyari13", "Windows98 Moyari13 ICMP Flood (DoS) Attack", DOS}, //61
    {"sesquipedalian", "Linux 2.1.89-2.1.3 SesQuiPeDalian Fragmented Packet (DoS) Attack", DOS} //62
};

#endif
