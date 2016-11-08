/***************************************************************************
                          Responser.h  -  description
                             -------------------
    begin                : Thu May 15 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#ifndef RESPONDER_H
#define RESPONDER_H

#define CLOSE 0
#define OPEN 1

#include <string.h>


class Responder
{
    private:
        int   Logging,
	MessageOnScreenAlarming,
	BeepFlashAlarming,
	LogState;
        char DetectionMode[20];
        char LogFileName[512];
        FILE *LogFilePointer;
	
        void Log(char *, char *, float, char*);
        void MessageOnScreenAlarm(char *, char *, float, char*);
        void BeepFlashAlarm(void);
	
    public:
        Responder();
        ~Responder();
        void SetWorkSpaceParameters(char *);
        void GetWorkSpaceParameters(char *);
        int  Initialize(char *);
        void Destroy();
        void GiveNeuralNetAnalyzerOutput(int, float, char *);

};

#endif
