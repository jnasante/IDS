/***************************************************************************
                          Responder.cpp  -  description
                             -------------------
    begin                : Thu May 15 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <qapplication.h>
//#include <curses.h>

#include "Responder.h"
#include "AttackTypes.h"

//#################        FUNCTION RESPONDER CONSTRUCTOR       ####################
Responder::Responder()
{
    LogState=CLOSE;
}


//#################        FUNCTION RESPONDER DESTRUCTOR       ####################
Responder::~Responder()
{
}


//#################        FUNCTION INITIALIZE       ####################
int Responder::Initialize(char *_DetectionMode)
{
    strcpy(DetectionMode, _DetectionMode);
    if (LogState==OPEN) fclose(LogFilePointer);
    if ( Logging && !(LogFilePointer=fopen(LogFileName, "a") ) )
    {
        perror("Error:Responder.fopen: Can not Open or Create Log File!");
        return 0;
    }
    LogState=OPEN;
    return 1;
}


//#################        FUNCTION DESTROY       ####################
void Responder::Destroy()
{
    if (LogState==OPEN) 
    {
        fclose(LogFilePointer);
        LogState=CLOSE;
    }
}


//#################        FUNCTION SET WORK SPACE PARAMETERS       ####################
void Responder::SetWorkSpaceParameters(char *ParamString)
{    
    //Param String Format: "<Logging> , <LogFileName> , <MessageOnScreenAlarming> , <BeepFlashAlarming>"
    int i, j;
    char StrTemp[512];
    
    
    if ( !strncasecmp(ParamString, "null", 4)) 
    {
        Logging=MessageOnScreenAlarming=BeepFlashAlarming=0;
        strcpy(LogFileName, "");
        return;
    }

    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    Logging=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy(LogFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    MessageOnScreenAlarming=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' && ParamString[i]!='\r'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    BeepFlashAlarming=atoi(StrTemp);
    
}//End of function Responser::SetWorkSpaceParameters


//#################        FUNCTION GET WORK SPACE PARAMETERS       ####################
void Responder::GetWorkSpaceParameters(char *ParamString)
{
    //Param String Format: "<Logging> , <LogFileName> , <MessageOnScreenAlarming> , <BeepFlashAlarming>"
    sprintf (ParamString, "%d,%s,%d,%d", Logging, LogFileName ,MessageOnScreenAlarming, BeepFlashAlarming);
}//End of function Responser::GetWorkSpaceParameters


//#################        FUNCTION LOG       ####################
void Responder::Log(char *Attack, char *ProtocolType, float AssuranceDegree, char *PacketInfo)
{
    time_t CurrentDate;			

    time(&CurrentDate);

    fprintf (LogFilePointer, "## Detected Attack in %s Mode :: %s   with %3.2f%% Assurance Degree ##\nDate: %sProtocol Type: %s\nPacket Header: %s\n\n", DetectionMode, Attack, AssuranceDegree, asctime(localtime(&CurrentDate )), ProtocolType, PacketInfo);
}//End of function Responder::Log


//#################        FUNCTION ALARM       ####################
void Responder::MessageOnScreenAlarm(char *Attack, char *ProtocolType, float AssuranceDegree, char *PacketInfo)
{
    time_t CurrentDate;			

    time(&CurrentDate);
    printf ("UNNID: ## Detected Attack in %s Mode :: %s   with %3.2f%% Assurance Degree ##\nDate: %sProtocol Type: %s\nPacket Header: %s\n\n", DetectionMode, Attack, AssuranceDegree, asctime(localtime(&CurrentDate )), ProtocolType, PacketInfo);
}//End of function Responder::Alarm


//#################        FUNCTION BEEP FLASH ALARM       ####################
void Responder::BeepFlashAlarm()
{
    QApplication::beep();
    //    beep();
}//End of function Responder::BeepFlashAlarm


//#################        FUNCTION GIVE NEURAL NET ANALZER OUTPUT       ####################
void Responder::GiveNeuralNetAnalyzerOutput(int NeuralNetAnalyzerOutput, float NeuralNetAnalyzerAssuranceDegree, char *NetPacketRecord)
{
    char ProtocolType[10];
    
    switch (NetPacketRecord[0])
    {
        case '0':
            strcpy(ProtocolType, "TCP");
            break;
	    
        case '1':
            strcpy(ProtocolType, "UDP");
            break;
	    
        case '2':
            strcpy(ProtocolType, "ICMP");
    }//end of switch
    
    if (Logging)
        Log(AttackDB[NeuralNetAnalyzerOutput].Desc, ProtocolType, NeuralNetAnalyzerAssuranceDegree, NetPacketRecord+2);
    if (MessageOnScreenAlarming)
        MessageOnScreenAlarm(AttackDB[NeuralNetAnalyzerOutput].Desc, ProtocolType, NeuralNetAnalyzerAssuranceDegree, NetPacketRecord+2);
    
    if (BeepFlashAlarming) BeepFlashAlarm();
}//End of function Responder::GiveNeuralNetAnalyzerOutput
