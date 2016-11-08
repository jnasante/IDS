/***************************************************************************
                          TrainTestDataProvider.cpp  -  description
                             -------------------
    begin                : Sun Apr 27 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include "TrainTestDataProvider.h"
#include "TcpDumpPacketProvider.h"

extern TcpDumpPacketProvider TcpDumpPacketProviderObj;


//#################        FUNCTION  OFF LINE NET TRAFFIC CONSTRUCTOR     ####################
TrainTestDataProvider::TrainTestDataProvider()
{
}//End of function TrainTestDataProvider::TrainTestDataProvider


//#################        FUNCTION  OFF LINE NET TRAFFIC DESTRUCTOR     ####################
TrainTestDataProvider::~TrainTestDataProvider()
{
}//End of function TrainTestDataProvider::~OffLneNetTraffic


//#################        FUNCTION  SET WORK SPACE PARAMETERS      ####################
void TrainTestDataProvider::SetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format for TrainTestDataProvider: <InputDataFormat> , <TrainFileName> , <TrainSessionFileName> , <TestFileName> , <TestSessionFileName>
    int i, j;
    char StrTemp[512];
    
    if ( !strncasecmp(ParamString, "null", 4)) 
    {
        InputDataFormat=0;
        strcpy(TrainFileName, "");
        strcpy(TrainSessionFileName, "");
        strcpy(TestFileName, "");
        strcpy(TestSessionFileName, "");
        strcpy(ApplyFileName, "");
        return;
    }//end of if
    
    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    InputDataFormat=atoi(StrTemp);
	
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy(TrainFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy(TrainSessionFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy (TestFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy (TestSessionFileName, StrTemp);
    
}//End of function TrainTestDataProvider::SetWorkSpaceParameters


//#################        FUNCTION GET WORK SPACE PARAMETERS       ####################
void TrainTestDataProvider::GetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format for TrainTestDataProvider: <InputDataFormat> , <TrainFileName> , <TrainSessionFileName> , <TestFileName> , <TestSessionFileName> 
    sprintf (ParamString, "%d,%s,%s,%s,%s", InputDataFormat, TrainFileName, TrainSessionFileName, TestFileName, TestSessionFileName);
}//End of function TrainTestDataProvider::GetWorkSpaceParameters


//#################        FUNCTION OPEN OFF LINE DATA BASE      ####################
int TrainTestDataProvider::OpenDataBase(int Action, int &DataFormat)
{
    char DBName[512],
         SLName[512];
    State=Action;
    DataFormat=InputDataFormat;
    switch (Action)
    {
        case TRAIN: 
                strcpy (DBName, TrainFileName);
                strcpy (SLName, TrainSessionFileName);
                break;
        case TEST: 
                strcpy (DBName, TestFileName);
                strcpy (SLName, TestSessionFileName);
                break;
        case APPLY: 
                strcpy (DBName, ApplyFileName);
                strcpy (SLName, "");
    }
    
    switch (InputDataFormat)
    {
        case TCPDUMP:
            if ( !(DBPointer=TcpDumpPacketProviderObj.OpenPacketSourcePassive(DBName, SLName)) )
            {
                perror ("Error: TrainTestDataProvider.OpenDataBase.fopen: File Not Found !");
                return 0;
            }
            break;
	    
        case KDDCUP:
            if ( !(DBPointer=fopen(DBName, "r")) )
            {
                perror ("Error: TrainTestDataProvider.OpenDataBase.fopen: File Not Found !");
                return 0;
            }
    }

    return 1;
}//End of function TrainTestDataProvider::OpenDataBase


//#################        FUNCTION RESET DB POINTER      ####################
int TrainTestDataProvider::ResetDBPointer()
{
    long int Len;
    switch (InputDataFormat)
    {
        case TCPDUMP:
            fseek (DBPointer, 0, SEEK_END);
            Len=ftell(DBPointer);
            CloseDataBase();
            OpenDataBase(State, InputDataFormat);
            return int (float(Len) / 100);
	    
        case KDDCUP:
            fseek (DBPointer, 0, SEEK_END);
            Len=ftell(DBPointer);
            fseek (DBPointer, 0, SEEK_SET);
            return int (float(Len) / 150.45);
    }//end of switch
    
    return 1;
}//End of function TrainTestDataProvider::ResetDBPointer


//#################        FUNCTION GET NEXT RECORD      ####################
int TrainTestDataProvider::GetNextRecord(char *NetConnectionPacketRecord)
{
    switch (InputDataFormat)
    {
        case TCPDUMP:
            if (!TcpDumpPacketProviderObj.ReadOfflinePacket(NetConnectionPacketRecord)) return 0;
            break;
	    
        case KDDCUP:
            if (! fgets(NetConnectionPacketRecord, 512, DBPointer) ) return 0;
    }
    
    return 1;
}//End of function TrainTestDataProvider::GetNextRecord


//#################        FUNCTION CLOSE DATA BASE      ####################
int TrainTestDataProvider::CloseDataBase()
{
    switch (InputDataFormat)
    {
        case TCPDUMP:
            TcpDumpPacketProviderObj.ClosePacketSourcePassive();
            break;
	    
        case KDDCUP:
            //Successfull fclose return 0
            if (fclose (DBPointer))
            {
                perror("Error: TrainTestDataProvider.CloseDataBsePointer.fclose !");
                return 0;
            }
    }//end of switch
    
    return 1;
}//End of function TrainTestDataProvider::CloseDataBase
