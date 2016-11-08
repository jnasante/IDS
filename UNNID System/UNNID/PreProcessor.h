/***************************************************************************
                          PreProcessor.h  -  description
                             -------------------
    begin                : Mon Apr 28 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#define TCPDUMP 0
#define KDDCUP  1

#define TCP  0
#define UDP  1
#define ICMP 2
#define UNP  3 //Unknown Protocol

#define IP_LEN 11
#define TCP_LEN  10
#define UDP_LEN  2
#define ICMP_LEN 4

#define KDD_FEATURES_NUMBER 41
#define TCPDUMP_FIELDS_NUMBER 27

#include <string.h>
#include <stdio.h>

class PreProcessor
{
    private:
        int    ExtractFeatureFromPacket();
        bool SelectedFeatures[50],
             SelectedHeaderFields[50];
        int  FeatureBinaryLen[50],
             HeaderFieldBinaryLen[50],
             IpSelectedFieldsNumber,
             TcpSelectedFieldsNumber,
             UdpSelectedFieldsNumber,
             IcmpSelectedFieldsNumber;
        int ExtractFeaturesInputVector(char *, float *);
        int ExtractHeaderFieldsInputVector(char *, float *);
        int ExtractIpHeadersInputVector(char *, int, float *, int &);
        int ExtractTcpHeadersInputVector(char *, float *);
        int ExtractUdpHeadersInputVector(char *, float *);
        int ExtractIcmpHeadersInputVector(char *, float *);
        int ExtractPacketContentFeaturesInputVector(char *, int, float *, int &);
        int ExtractExpectedOutput(char *);
        int ConvertToBinary(float *, int );
        int ConvertFeaturesToBinary(float *, int );
        int ConvertHeaderFieldsToBinary(float *, int );
        int ConvertToNormalizedForm(float *);
        int ConvertFeaturesToNormalizedForm(float *);
        int ConvertHeaderFieldsToNormalizedForm(float *);
        float GetServiceCode(char *);
        float GetFlagCode(char *);
        void DecimalToBinary(float, float *, int);

    public:
        PreProcessor();
        ~PreProcessor();
        void GetWorkSpaceParameters(char *);
        void SetWorkSpaceParameters(char *);
        int  OffLineMode(int , int =0);
        int  GoFirstInputOutputVectors();
        int  GetNextInputOutputVectors(float *, int &, int);
        int  GetNextOutputVector(int &);
        int  GivePacket(char *);
};//End of class definition PreProcessor

#endif
