/***************************************************************************
                          controlmanagement.h  -  description
                             -------------------
    begin                : Tue May 6 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#ifndef MANAGEMENTCONTROL_H
#define MANAGEMENTCONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qthread.h> 


class ManagementControl
{
    private:
        bool WorkSpaceModified;
        void SetWorkSpaceParameters(char *);
        void GetWorkSpaceParameters(char *);

    public:
        ManagementControl();
        ~ManagementControl();
        int NewWorkSpace();
        int LoadWorkSpace(const char *);
        int SaveWorkSpace(const char *);
        int IsWorkSpaceModified();
        void SetWorkSpaceModified();
        int StartTrainIDS();
        int StartTestIDS();
        int EvaluateIDS(char *);
        int StartRealTimeIDS();
        int StopRealTimeIDS();
        int StartOffLineIDS();
        int StartCapturePackets(char *);
        int StopCapturePackets();
};

class IntrusionDetectionThread : public QThread 
{
    public:
        virtual void run();
};

class CapturePacketsThread : public QThread 
{
    private:
        char Command[512];
    public:
        virtual void run();
        void setCommand(char *);
};



#endif
