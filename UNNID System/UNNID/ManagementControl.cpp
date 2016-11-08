/***************************************************************************
                          ManagementControl.cpp  -  description
                             -------------------
    begin                : Tue May 6 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include "ManagementControl.h"
#include "TcpDumpPacketProvider.h"
#include "TrainTestDataProvider.h"
#include "PreProcessor.h"
#include "NeuralNet.h"
#include "Responder.h"
#include "IdsEvaluator.h"


extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
extern TrainTestDataProvider TrainTestDataProviderObj;
extern PreProcessor PreProcessorObj;
extern NeuralNet NeuralNetObj;
extern Responder ResponderObj;
extern IdsEvaluator IdsEvaluatorObj;
IntrusionDetectionThread IntrusionDetectionThreadObj;
CapturePacketsThread CapturePacketsThreadObj;


//#############        FUNCTION INTRUSION DETECTION THREAD :: RUN       ################
void IntrusionDetectionThread::run()
{
    TcpDumpPacketProviderObj.StartSniffer(true);
}//End of function IntrusionDetectionThread::run


//#############        FUNCTION CAPTURE PACKETS THREAD :: RUN       ################
void CapturePacketsThread::run()
{
    system(Command);
}//End of function CapturePacketsThread::run


//#############        FUNCTION CAPTURE PACKETS THREAD :: SET COMMAND       ################
void CapturePacketsThread::setCommand(char *Com)
{
    strcpy(Command, Com);
}//End of function CapturePacketsThread::setCommand


//#################        FUNCTION MANAGEMENT CONTROL CONSTRUCTOR       ####################
ManagementControl::ManagementControl()
{
    WorkSpaceModified=false;
}//End of function ManagementControl::ManagementControl


//#################        FUNCTION MANAGEMENT CONTROL DESTRUCTOR       ####################
ManagementControl::~ManagementControl()
{
}//End of function ManagementControl::~ManagementControl


//#################        FUNCTION SET WORK SPACE PARAMETERS       ####################
void ManagementControl::SetWorkSpaceParameters(char *ParamString)
{
    if ( !strcasecmp(ParamString,"null")) return;
}//End of function ManagementControl::SetWorkSpaceParameters


//#################        FUNCTION GET WORK SPACE PARAMETERS       ####################
void ManagementControl::GetWorkSpaceParameters(char *ParamString)
{
    strcpy (ParamString,"null");
}//End of function ManagementControl::GetWorkSpaceParameters


//#################        FUNCTION LOAD WORK SPACE       ####################
int ManagementControl::LoadWorkSpace(const char * WorkSpaceFile)
{
    FILE *WPFile;
    char DupCheck[6],
           ParamString[512],
           ParamUnit[512];
	 
    int i;

    for (i=0; i<6; i++) DupCheck[i]=0;

    if ( !(WPFile=fopen (WorkSpaceFile,"r")) ) 
        return 0;
   
    while (!feof(WPFile))
    {
        fgets (ParamUnit, 512, WPFile);
        if (feof(WPFile)) break;
        else fgets (ParamString, 512, WPFile);

        if (!strncasecmp (ParamUnit,"#ManagementControl Parameters:", 30) && !DupCheck[0])
        {
            SetWorkSpaceParameters(ParamString);
            DupCheck[0]=1;
        }    

        else if (!strncasecmp (ParamUnit,"#TrainTestDataProvider Parameters:", 30) && !DupCheck[1])
        {
            TrainTestDataProviderObj.SetWorkSpaceParameters(ParamString);
            DupCheck[1]=1;
        }    
	
        else if (!strncasecmp (ParamUnit,"#TcpDumpPacketProvider Parameters:", 20) && !DupCheck[2])
        {
            TcpDumpPacketProviderObj.SetWorkSpaceParameters(ParamString);
            DupCheck[2]=1;
        }    

        else if (!strncasecmp (ParamUnit,"#PreProcessor Parameters:", 25) && !DupCheck[3])
        {
            PreProcessorObj.SetWorkSpaceParameters(ParamString);
            DupCheck[3]=1;
        }    

        else if (!strncasecmp (ParamUnit,"#NeuralNet Parameters:", 22) && !DupCheck[4])
        {
            NeuralNetObj.SetWorkSpaceParameters(ParamString);
            DupCheck[4]=1;
        }    

        else if (!strncasecmp (ParamUnit,"#Responder Parameters:", 22) && !DupCheck[5])
        {
            ResponderObj.SetWorkSpaceParameters(ParamString);
            DupCheck[5]=1;
        }    
    }//end of while (!feof(...))
    
    fclose (WPFile);    
    WorkSpaceModified=false;
    return 1;
}//End of function ManagementControl::LoadWorkSpace


//#################        FUNCTION SAVE WORK SPACE       ####################
int ManagementControl::SaveWorkSpace(const char * WorkSpaceFile)
{
    FILE *WPFile;
    char ParamString[512];

    if ( !(WPFile=fopen (WorkSpaceFile,"w")) )
        return 0;

    GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#ManagementControl Parameters:\n");
    fprintf(WPFile, "%s\n", ParamString);
   
   TrainTestDataProviderObj.GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#TrainTestDataProvider Parameters:\n");
    fprintf(WPFile, "%s\n", ParamString);
    
    TcpDumpPacketProviderObj.GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#TcpDumpPacketProvider Parameters:\n");
    fprintf(WPFile, "%s\n", ParamString);

    PreProcessorObj.GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#PreProcessor Parameters:\n");
    fprintf(WPFile, "%s\n", ParamString);

    NeuralNetObj.GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#NeuralNet Parameters:\n");
    fprintf(WPFile, "%s\n", ParamString);

    ResponderObj.GetWorkSpaceParameters(ParamString);
    fprintf (WPFile, "#Responder Parameters:\n");
    fprintf (WPFile, "%s\n", ParamString);

    fclose(WPFile);
    WorkSpaceModified=false;
    return 1;
}//End of function ManagementControl::SaveWorkSpace


//#################        FUNCTION SET WORK SPACE MODIFIED       ####################
void ManagementControl::SetWorkSpaceModified()
{
    WorkSpaceModified=true;
}//End of function ManagementControl::SetWorkSpaceModified


//#################        FUNCTION IS WORK SPACE MODIFIED       ####################
int ManagementControl::IsWorkSpaceModified()
{
    if (WorkSpaceModified) return 1;
    return 0;
}//End of function ManagementControl::IsWorkSpaceModified


//#################        FUNCTION START TRAIN IDS       ####################
int ManagementControl::StartTrainIDS()
{
    if (IntrusionDetectionThreadObj.running()==true) return 0;
    if ( !NeuralNetObj.TrainNet() ) return 0;
    return 1;
}//End of function ManagementControl::StartTrainIDS


//#################        FUNCTION SRART UP IDS       ####################
int ManagementControl::StartRealTimeIDS()
{
    if ( NeuralNetObj.InitializeNet(true) && ResponderObj.Initialize("Real Time"))
    {
        IntrusionDetectionThreadObj.start();
        system ("sleep 1");
        if (IntrusionDetectionThreadObj.running()==true)
        {
            printf("UNNID: Real-Time Unsupervised Neural Net based Intrusion Detector\n Started Successfully...\n");
            return 1;
        }
    }
    
    IntrusionDetectionThreadObj.terminate();
    IntrusionDetectionThreadObj.wait();
    NeuralNetObj.FreeNet();
    ResponderObj.Destroy();
    perror ("UNNID Error: Can not Start Real-Time Unsupervised Neural Net based Intrusion Detector!\n");
    return 0;
}//End of function ManagementControl::StartRealTimeIDS


//#################        FUNCTION SRART UP IDS       ####################
int ManagementControl::StartOffLineIDS()
{
    /* return -1 : for error in intializing NeuralNetObj or ResponderObj
       return 0 : for error in starting off-line sniffer
       return 1 : for successful action
    */
    
    int retval=1;
    
    if ( NeuralNetObj.InitializeNet(true) && ResponderObj.Initialize("Off Line"))
    {
        printf("UNNID: Off-Line Unsupervised Neural Net based Intrusion Detector\n started successfully.\nPlease Wait....\n");
        retval=TcpDumpPacketProviderObj.StartSniffer(false);
        NeuralNetObj.FreeNet();
        ResponderObj.Destroy();
    } else
    {
        perror ("UNNID Error: Can not initialize Neural Net based Analyzer and Responder components !\n");
        return -1;	
    }

    if (!retval)
    {
        perror ("UNNID Error: Can not start Off-line Unsupervised Neural Net based Intrusion Detector !\n");
        return 0;
    }
	
    printf("UNNID: Off-line Unsupervised Neural Net based Intrusion Detection terminated successfully.\n");
    return 1;
}//End of function ManagementControl::StartOffLineIDS


//#################        FUNCTION SRART TEST IDS       ####################
int ManagementControl::StartTestIDS()
{
    if (IntrusionDetectionThreadObj.running()==true) return 0;
    if (!NeuralNetObj.TestNet()) return 0;
    return 1;
}//End of function ManagementControl::TestIDS
			    

//#################        FUNCTION STOP IDS       ####################
int ManagementControl::StopRealTimeIDS()
{
    if (IntrusionDetectionThreadObj.running()==false) 
    {
        perror ("UNNID Error: Can not Correct Stop Real-Time Unsupervised Neural Net based Intrusion Detector!\n");
        return 0;
    }
    IntrusionDetectionThreadObj.terminate();
    IntrusionDetectionThreadObj.wait();
    NeuralNetObj.FreeNet();
    ResponderObj.Destroy();
    
    printf ("UNNID: Real-Time Unsupervised Neural Net based Intrusion Detector\n Stopped Successfully.\n");
    return 1;
}//End of function ManagementControl::StopRealTimeIDS


//#################        FUNCTION EVALUATE IDS       ####################
int ManagementControl::EvaluateIDS(char *EvaluationResultsStr)
{
    if (IntrusionDetectionThreadObj.running()==true) return 0; 
    if (!IdsEvaluatorObj.EvaluateTestIds(EvaluationResultsStr)) return 0;
    return 1;
}//End of function ManagementControl::EvaluateIDS


//#################        FUNCTION NEW WORK SPACE       ####################
int ManagementControl::NewWorkSpace()
{
    //if (IntrusionDetectionThreadObj.running()==true) return 0; 
    SetWorkSpaceParameters("null");
    TrainTestDataProviderObj.SetWorkSpaceParameters("null");
    TcpDumpPacketProviderObj.SetWorkSpaceParameters("null");
    PreProcessorObj.SetWorkSpaceParameters("null");
    NeuralNetObj.SetWorkSpaceParameters("null");
    ResponderObj.SetWorkSpaceParameters("null");
    WorkSpaceModified=true;
    
    return 1;
}//End of function ManagementControl::EvaluateIDS


//#################        FUNCTION START CAPTURE PACKETS       ####################
int ManagementControl::StartCapturePackets(char *ParamString)
{
    //ParamStr : <Filter> , <DumpFileName>
    char Command[512],
         DumpFileName[512],
         Filter[64];
    int i, j;
    
    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) Filter[i]=ParamString[i];
    Filter[i]='\0';
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) DumpFileName[j]=ParamString[i];
    DumpFileName[j]='\0';
    
    strcpy (Command, "tcpdump -w ");
    strcat (Command, DumpFileName);
    strcat (Command, " ");
    strcat (Command, Filter);
    
    CapturePacketsThreadObj.setCommand(Command);    
    CapturePacketsThreadObj.start();
    system ("sleep 1");
    if (CapturePacketsThreadObj.running()==true)
    {
        printf("UNNID: Capturing Network Packets started successfully ...\n");
        return 1;
    }
    
    CapturePacketsThreadObj.terminate();
    CapturePacketsThreadObj.wait();
    perror ("UNNID Error: Can not start Capturing Network Packets !\n");
    return 0;
}//End of function ManagementControl::StartCapturePackets


//#################        FUNCTION START CAPTURE PACKETS       ####################
int ManagementControl::StopCapturePackets()
{
    CapturePacketsThreadObj.terminate();
    CapturePacketsThreadObj.wait();
    printf("UNNID: Capturing Network Packets stoped successfully.\n");
    return 1;    
}//End of function ManagementControl::StopCapturePackets

