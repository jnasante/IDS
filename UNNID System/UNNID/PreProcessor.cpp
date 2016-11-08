/***************************************************************************
                          PreProcessor.cpp  -  description
                             -------------------
    begin                : Mon Apr 28 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#include "PreProcessor.h"
#include "TcpDumpPacketProvider.h"
#include "TrainTestDataProvider.h"
#include "NeuralNet.h"
#include "AttackTypes.h"

extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
extern TrainTestDataProvider TrainTestDataProviderObj;
extern NeuralNet NeuralNetObj;

//#################        FUNCTION PRE PROCESSOR CONSTRUCTOR      ####################
PreProcessor::PreProcessor()
{
    //Assign FeatureBinaryLen
    FeatureBinaryLen[0]=4; //duration = [0 , 58329]  divided to unequal intervals
    FeatureBinaryLen[1]=2; //protocol_type = 3 symbols
    FeatureBinaryLen[2]=7; //service = 70 symbols
    FeatureBinaryLen[3]=4; //flag = 11 symbols
    FeatureBinaryLen[4]=4; //src_bytes = [0 , 1.3 billion]
    FeatureBinaryLen[5]=4; //dst_bytes = [0 , 1.3 billion]
    FeatureBinaryLen[6]=1; //land = binary
    FeatureBinaryLen[7]=2; //wrong_fragment = [0 , 3]
    FeatureBinaryLen[8]=5; //urgent = [0 , 30]
    FeatureBinaryLen[9]=5; //hot = [0 , 101] > 31
    FeatureBinaryLen[10]=3; //num_failed_logins = [0 , 5]
    FeatureBinaryLen[11]=1; //logged_in = binary
    FeatureBinaryLen[12]=3; //num_compromised = [0 , 9] > 7
    FeatureBinaryLen[13]=1; //root_shell = binary
    FeatureBinaryLen[14]=1; //su_attempted = binary
    FeatureBinaryLen[15]=7; //num_root = [0 , 7468] > 255
    FeatureBinaryLen[16]=5; //num_file-creations = [0 , 100] > 31
    FeatureBinaryLen[17]=3; //num_shells = [0 , 6]
    FeatureBinaryLen[18]=2; //num_access_files = [0 , 9]
    FeatureBinaryLen[19]=2; //num_outbound_cmds = [0 , 3]
    FeatureBinaryLen[20]=1; //is_hot_login = binary
    FeatureBinaryLen[21]=1; //is_guest_login = binary
    FeatureBinaryLen[22]=8; //count = [0 , 511] > 255
    FeatureBinaryLen[23]=7; //srv_counts = [0 , 511] > 127
    FeatureBinaryLen[24]=5; //serror_rate = [0 , 1.0]
    FeatureBinaryLen[25]=5; //srv_serror_rate = [0 , 1.0]
    FeatureBinaryLen[26]=5; //rerror_rate = [0 , 1.0]
    FeatureBinaryLen[27]=5; //srv_rerror_rate = [0 , 1.0]
    FeatureBinaryLen[28]=5; //same_srv_rate = [0 , 1.0]
    FeatureBinaryLen[29]=5; //diff_srv_rate = [0 , 1.0]
    FeatureBinaryLen[30]=5; //srv_diff_host_rate = [0 , 1.0]
    FeatureBinaryLen[31]=6; //dst_host_count = [0 , 255]
    FeatureBinaryLen[32]=6; //dst_host_srv_count = [0 , 255]
    FeatureBinaryLen[33]=5; //dst_host_same_srv_rate = [0 , 1.0]
    FeatureBinaryLen[34]=5; //dst_host_diff_srv_rate = [0 , 1.0]
    FeatureBinaryLen[35]=5; //dst_host_same_src_port_rate = [0 , 1.0]
    FeatureBinaryLen[36]=5; //dst_host_srv_diff_host_rate = [0 , 1.0]
    FeatureBinaryLen[37]=5; //dst_host_serror_rate = [0 , 1.0]
    FeatureBinaryLen[38]=5; //dst_host_srv_serror_rate = [0 , 1.0]
    FeatureBinaryLen[39]=5; //dst_host_rerror_rate = [0 , 1.0]
    FeatureBinaryLen[40]=5; //dst_host_srv_rerror_rate = [0 , 1.0]
    
    //Assign HeaderFieldBinaryLen
    HeaderFieldBinaryLen[0]=2; //Protocol Type
    HeaderFieldBinaryLen[1]=16; //Time Stamp
    HeaderFieldBinaryLen[2]=16; //Identification
    HeaderFieldBinaryLen[3]=8; //Type Of Service (TOS)
    HeaderFieldBinaryLen[4]=8; //Time To Live (TTL)
    HeaderFieldBinaryLen[5]=4; //IP Header Length
    HeaderFieldBinaryLen[6]=16; //Datagram Length
    HeaderFieldBinaryLen[7]=1; //Source IP 
    HeaderFieldBinaryLen[8]=1; //Destination IP
    HeaderFieldBinaryLen[9]=1; //Land : Same Source,Destination IP
    HeaderFieldBinaryLen[10]=3; //Fragmentation
    HeaderFieldBinaryLen[11]=16; //TCP Source Port
    HeaderFieldBinaryLen[12]=16; //TCP Destination Port
    HeaderFieldBinaryLen[13]=1; //TCP SYN Flag
    HeaderFieldBinaryLen[14]=1; //TCP FIN Flag
    HeaderFieldBinaryLen[15]=1; //TCP ACK Flag
    HeaderFieldBinaryLen[16]=1; //TCP PUSH Flag
    HeaderFieldBinaryLen[17]=1; //TCP RST Flag
    HeaderFieldBinaryLen[18]=1; //TCP URG Flag
    HeaderFieldBinaryLen[19]=4; //TCP Header Length
    HeaderFieldBinaryLen[20]=16; //TCP Window Size
    HeaderFieldBinaryLen[21]=16; //UDP Source Port
    HeaderFieldBinaryLen[22]=16; //UDP Destination Port
    HeaderFieldBinaryLen[23]=8; //ICMP Type
    HeaderFieldBinaryLen[24]=8; //ICMP Code
    HeaderFieldBinaryLen[25]=16; //ICMP ID
    HeaderFieldBinaryLen[26]=16; //ICMP Sequence
}//End of function PreProcessor::PreProcessor


//#################        FUNCTION PRE PROCESSOR DESTRUCTOR      ####################
PreProcessor::~PreProcessor()
{
  
}//End of function PreProcessor::~PreProcessor


//#################        FUNCTION GET WORK SPACE PARAMETERS      ####################
void PreProcessor::GetWorkSpaceParameters(char *ParamString)
{
    int i;
    
    if (SelectedFeatures[0])
        strcpy(ParamString,"1");
    else
        strcpy(ParamString,"0");
    
    for (i=1; i<KDD_FEATURES_NUMBER; i++)
        if (SelectedFeatures[i])
            strcat(ParamString,",1");
        else
            strcat(ParamString,",0");
	
    for (i=0; i<TCPDUMP_FIELDS_NUMBER; i++)
    {
        if (SelectedHeaderFields[i])
            strcat(ParamString,",1");
        else
            strcat(ParamString,",0");
    }
		   
}//End of function PreProcessor::GetWorkSpaceParameters


//#################        FUNCTION SET WORK SPACE PARAMETERS      ####################
void PreProcessor::SetWorkSpaceParameters(char *ParamString)
{
    unsigned int i, j;
    int InputVectorBinaryLen[2],
        InputVectorNonBinaryLen[2];
    IpSelectedFieldsNumber=0;
    TcpSelectedFieldsNumber=0;
    UdpSelectedFieldsNumber=0;
    IcmpSelectedFieldsNumber=0;
    
    InputVectorBinaryLen[0]=InputVectorBinaryLen[1]=InputVectorNonBinaryLen[0]=InputVectorNonBinaryLen[1]=0;
    
    for (i=0; i<KDD_FEATURES_NUMBER; i++) SelectedFeatures[i]=false;
    for (i=0; i<TCPDUMP_FIELDS_NUMBER; i++) SelectedHeaderFields[i]=false;
    
    if (!strncasecmp(ParamString, "null", 4)) 
    {
        NeuralNetObj.SetInputVectorLen(0,0,0,0);
        return;
    }//end of if
    
    for (i=0; i<KDD_FEATURES_NUMBER; i++)
        if ( (2*i)<strlen(ParamString) && ParamString[2*i]=='1')
        {
            SelectedFeatures[i]=true;
            InputVectorNonBinaryLen[KDDCUP]++;
            InputVectorBinaryLen[KDDCUP]+=FeatureBinaryLen[i];
        }
    
    for (i=0, j=0; i<TCPDUMP_FIELDS_NUMBER; i++, j++)
    {
        if ( (2*i+2*KDD_FEATURES_NUMBER)<strlen(ParamString) && ParamString[2*i+2*KDD_FEATURES_NUMBER]=='1')
        {
            if (j<IP_LEN) IpSelectedFieldsNumber++;
            else if (j>=IP_LEN && j<IP_LEN+TCP_LEN) TcpSelectedFieldsNumber++;
            else if (j>=IP_LEN+TCP_LEN && j<IP_LEN+TCP_LEN+UDP_LEN) UdpSelectedFieldsNumber++;
            else IcmpSelectedFieldsNumber++;
	       
            SelectedHeaderFields[j]=true;
            InputVectorNonBinaryLen[TCPDUMP]++;
            InputVectorBinaryLen[TCPDUMP]+=HeaderFieldBinaryLen[j];
        }//end of if (strlen...)
    }//end of for (i, j)

    NeuralNetObj.SetInputVectorLen(InputVectorBinaryLen[TCPDUMP], InputVectorNonBinaryLen[TCPDUMP], InputVectorBinaryLen[KDDCUP], InputVectorNonBinaryLen[KDDCUP]);
}//End of function PreProcessor::SetWorkSpaceParameters


//#################        FUNCTION OFF LINE MODE      ####################
int PreProcessor::OffLineMode(int OffOn, int Action)
{
    switch (OffOn)
    {
        case ON  : if (!TrainTestDataProviderObj.OpenDataBase(Action, TrainTestDataProviderObj.InputDataFormat)) return 0;
              break;
        case OFF : if (!TrainTestDataProviderObj.CloseDataBase()) return 0;
              break;
    }
    return 1;
}//End of function PreProcessor::OffLineTrainMode


//#################        FUNCTION GO FIRST INPUT OUTPUT VECTORS      ####################
int PreProcessor::GoFirstInputOutputVectors()
{
    return TrainTestDataProviderObj.ResetDBPointer();
}//End of function PreProcessor::GoFirstInputOutputVectors


//#################        FUNCTION GET NEXT INPUT OUTPUT VECTORS      ####################
int PreProcessor::GetNextInputOutputVectors(float *InputVector, int &ExpectedOutput, int ConversionType)
{
    char NetConnectionPacketRecord[1024];
    int VectorLen=0, Len=0;
    
    if (!TrainTestDataProviderObj.GetNextRecord(NetConnectionPacketRecord)) return 0;
    
    switch (TrainTestDataProviderObj.InputDataFormat)
    {
        case KDDCUP : Len=ExtractFeaturesInputVector(NetConnectionPacketRecord, InputVector);
            break;
        case TCPDUMP: Len=ExtractHeaderFieldsInputVector(NetConnectionPacketRecord, InputVector);
    }//end of switch (TrainTestDataProviderObj.InputDataFormat)
    
    
    ExpectedOutput=ExtractExpectedOutput(NetConnectionPacketRecord);
    
    switch (ConversionType)
    {
        case BINARY:
            VectorLen=ConvertToBinary(InputVector, Len);
            break;  
        case NORMALIZED:
            VectorLen=ConvertToNormalizedForm(InputVector);
             break;
        case REAL:
            VectorLen=Len;
             break;
    }//end of switch (ConversionType)

    return 1;
}//End of function PreProcessor::GetInputOutputVectors


//#################        FUNCTION GET NEXT OUTPUT VECTOR      ####################
int PreProcessor::GetNextOutputVector(int &ExpectedOutput)
{
    char NetConnectionPacketRecord[1024];
    
    if (!TrainTestDataProviderObj.GetNextRecord(NetConnectionPacketRecord)) return 0;
    ExpectedOutput=ExtractExpectedOutput(NetConnectionPacketRecord);
    return 1;
}//End of function PreProcessor::GetNextOutputVector

	
//#################        FUNCTION EXTRACT FEATURES INPUT VECTOR      ####################
int PreProcessor::ExtractFeaturesInputVector(char *NetConnectionRecord, float *InputVector)
{
    char Feature[50];
    int j, k, FeatureCounter;
    unsigned int i;
    
    for (FeatureCounter=0, k=0, i=0, j=0; FeatureCounter<42 && i<strlen(NetConnectionRecord); i+=(j+1), FeatureCounter++)
    {
        for (j=0; NetConnectionRecord[i+j]!=',' && NetConnectionRecord[i+j]!='\n' && NetConnectionRecord[i+j]; j++)
            Feature[j]=NetConnectionRecord[i+j];
        Feature[j]='\0';
        if (SelectedFeatures[FeatureCounter])
            switch (FeatureCounter)
            {
                case  0:  //duration
                    InputVector[k++]=atof(Feature);
                    break;
		
                case  1: //protocol_type
                    if ( !strncasecmp(Feature,"tcp", 3))
                        InputVector[k++]=TCP;
                    else if ( !strncasecmp(Feature,"udp", 3))
                        InputVector[k++]=UDP;
                    else if ( !strncasecmp(Feature,"icmp", 4))
                        InputVector[k++]=ICMP;
                    else //unknown protocol
                        InputVector[k++]=UNP;
                    break;
		
                case  2: //service
                    InputVector[k++]=GetServiceCode(Feature);
                    break;
		
                case  3: //flag
                    InputVector[k++]=GetFlagCode(Feature);
                    break;
		    
                case  4: //src_bytes
                case  5: //dst_bytes
                case  6: //land
                case  7: //wrong_fragment
                case  8: //urgent
                case  9: //hot
                case 10: //num_failed_logins
                case 11: //logged_in
                case 12: //num_compromised
                case 13: //root_shell
                case 14: //su_attempted
                case 15: //num_root
                case 16: //num_file-creations
                case 17: //num_shells
                case 18: //num_access_files
                case 19: //num_outbound_cmds
                case 20: //is_hot_login
                case 21: //is_guest_login
                case 22: //count
                case 23: //srv_counts
                case 24: //serror_rate
                case 25: //srv_serror_rate
                case 26: //rerror_rate
                case 27: //srv_rerror_rate
                case 28: //same_srv_rate
                case 29: //diff_srv_rate
                case 30: //srv_diff_host_rate
                case 31: //dst_host_count
                case 32: //dst_host_srv_count
                case 33: //dst_host_same_srv_rate
                case 34: //dst_host_diff_srv_rate
                case 35: //dst_host_same_src_port_rate
                case 36: //dst_host_srv_diff_host_rate
                case 37: //dst_host_serror_rate
                case 38: //dst_host_srv_serror_rate
                case 39: //dst_host_rerror_rate
                case 40: //dst_host_srv_rerror_rate
                    InputVector[k++]=atof(Feature);
                    break;
            }//end of switch (FeatureCounter)
    }//end of for

    return k;
}//End of function PreProcessor::ExtractFeaturesInputVector


//#################        FUNCTION GET NEXT INPUT VECTOR      ####################
int PreProcessor::ExtractHeaderFieldsInputVector(char *NetPacketRecord, float *InputVector)
{
    for (int i=0; i<IpSelectedFieldsNumber+TcpSelectedFieldsNumber+UdpSelectedFieldsNumber+IcmpSelectedFieldsNumber; i++) InputVector[i]=0;
    switch (NetPacketRecord[0])
    {
        case '0' : //TCP
            return ExtractTcpHeadersInputVector(NetPacketRecord, InputVector);
            break;
        case '1' : //UDP
            return ExtractUdpHeadersInputVector(NetPacketRecord, InputVector);
            break;
        case '2' : //ICMP
            return ExtractIcmpHeadersInputVector(NetPacketRecord, InputVector);
    }//end of switch (NetPacketRecord[0])
   
    return 0;
}//End of function PreProcessor::ExtractHeaderFieldsIputVector


//#################        FUNCTION EXTRACT IP HEADERS INPUT VECTOR      ####################
int PreProcessor::ExtractIpHeadersInputVector(char *NetPacketRecord, int Start, float *InputVector, int &k)
{
    char HeaderField[50];
    int j, HFCounter;
    unsigned int i;
    
    for (HFCounter=1, i=Start, j=0; i<strlen(NetPacketRecord) && HFCounter<IP_LEN; i+=(j+1), HFCounter++)
    {
        for (j=0; NetPacketRecord[i+j]!=',' && NetPacketRecord[i+j]; j++)
            HeaderField[j]=NetPacketRecord[i+j];
        HeaderField[j]='\0';
	
        // 1 : Time Stamp
        // 2 : Identification
        // 3 : Type Of Service (TOS)
        // 4 : Time To Live (TTL)
        // 5 : IP Header Length
        // 6 : Datagram Length
        // 7 : Source IP Address
        // 8 : Destination IP Address
        // 9 : Land : Same Source & Destination IP Address
        //10 : Fragmentation
        if (SelectedHeaderFields[HFCounter]) InputVector[k++]=atof(HeaderField);

    }//end of for
    
    return i;
}//End of function PreProcessor::ExtractIpHeadersInputVector


//#################        FUNCTION EXTRACT TCP HEADERS INPUT VECTOR      ####################
int PreProcessor::ExtractTcpHeadersInputVector(char *NetPacketRecord, float *InputVector)
{
    char HeaderField[50];
    int j, k, HFCounter, Pos;
    unsigned int i;
    
    k=0; //Counter of InputVector (determine input vector length finally)
    Pos=IP_LEN; //Position of TCP header fields in Input Vector of Analyzer
    InputVector[k++]=TCP;
    i=ExtractIpHeadersInputVector(NetPacketRecord, 2, InputVector, k);
    
    for (HFCounter=Pos,  j=0; NetPacketRecord[i-1]!='.'; i+=(j+1), HFCounter++)
    {
        for (j=0; NetPacketRecord[i+j]!=',' && NetPacketRecord[i+j]!='.'; j++)
            HeaderField[j]=NetPacketRecord[i+j];
        HeaderField[j]='\0';
        // 0 : TCP Source Port
        // 1 : TCP Destination Port
        // 2 : TCP SYN Flag
        // 3 : TCP FIN Flag
        // 4 : TCP ACK Flag
        // 5 : TCP PUSH Flag
        // 6 : TCP RST Flag
        // 7 : TCP URG Flag
        // 8 : TCP Header Length
        // 9 : TCP Window Size
        if (SelectedHeaderFields[HFCounter])  InputVector[k++]=atof(HeaderField);
    }//end of for
    
    k+=UdpSelectedFieldsNumber+IcmpSelectedFieldsNumber; //UDP & ICMP fields are remained 0
    ExtractPacketContentFeaturesInputVector(NetPacketRecord, i, InputVector, k);
    return k;
}//End of function PreProcessor::ExtractTcpHeadersInputVector


//#################        FUNCTION EXTRACT UDP HEADERS INPUT VECTOR      ####################
int PreProcessor::ExtractUdpHeadersInputVector(char *NetPacketRecord, float *InputVector)
{
    char HeaderField[50];
    int j, k, HFCounter, Pos;
    unsigned int i;
    
    k=0; //Counter of InputVector (determine input vector length finally)
    Pos=IP_LEN+TCP_LEN; //Position of UDP header fields in Input Vector of Analyzer
    InputVector[k++]=UDP;
    i=ExtractIpHeadersInputVector(NetPacketRecord, 2, InputVector, k);
    k+=TcpSelectedFieldsNumber; //TCP fields are remained 0
    
    for (HFCounter=Pos,  j=0; NetPacketRecord[i-1]!='.'; i+=(j+1), HFCounter++)
    {
        for (j=0; NetPacketRecord[i+j]!=',' && NetPacketRecord[i+j]!='.'; j++)
            HeaderField[j]=NetPacketRecord[i+j];
        HeaderField[j]='\0';
        // 0 : UDP Source Port
        // 1 : UDP Destination Port
        if (SelectedHeaderFields[HFCounter])  InputVector[k++]=atof(HeaderField);
    }//end of for
    
    k+=IcmpSelectedFieldsNumber; //ICMP fields are remained 0
    ExtractPacketContentFeaturesInputVector(NetPacketRecord, i, InputVector, k);
    return k;   
}//End of function PreProcessor::ExtractTcpHeadersInputVector


//#################        FUNCTION EXTRACT ICMP HEADERS INPUT VECTOR      ####################
int PreProcessor::ExtractIcmpHeadersInputVector(char *NetPacketRecord, float *InputVector)
{
    char HeaderField[50];
    int j, k, HFCounter, Pos; 
    unsigned int i;
    
    
    k=0; //Counter of InputVector (determine input vector length finally)
    Pos=TCP_LEN + UDP_LEN; //Position of ICMP header fields in Input Vector of Analyzer
    InputVector[k++]=ICMP;
    i=ExtractIpHeadersInputVector(NetPacketRecord, 2, InputVector, k);
    k+=TcpSelectedFieldsNumber+UdpSelectedFieldsNumber; //TCP & UDP fields are remained 0
    
    for (HFCounter=Pos,  j=0; NetPacketRecord[i-1]!='.'; i+=(j+1), HFCounter++)
    {
        for (j=0; NetPacketRecord[i+j]!=',' && NetPacketRecord[i+j]!='.'; j++)
            HeaderField[j]=NetPacketRecord[i+j];
        HeaderField[j]='\0';
        // 0 : ICMP Type
        // 1 : ICMP Code
        // 2 : ICMP ID
        // 3 : ICMP Sequence
        if (SelectedHeaderFields[HFCounter])  InputVector[k++]=atof(HeaderField);
    }//end of for
    
    ExtractPacketContentFeaturesInputVector(NetPacketRecord, i, InputVector, k);
    return k;
}//End of function PreProcessor::ExtractTcpHeadersInputVector


//#########        FUNCTION EXTRACT PACKET CONTENT FEATURES INPUT VECTOR      ##########
int PreProcessor::ExtractPacketContentFeaturesInputVector(char *NetPacketRecord, int Start, float *InputVector, int &k)
{
//    InputVector[k++]=ContentFeatures
}//End of function PreProcessor::ExtractPacketContentFeaturesInputVector


//#################        FUNCTION CONVERT TO BINARY      ####################
int PreProcessor::ConvertToBinary(float *InputVector, int InputVectorLen)
{
    switch (TrainTestDataProviderObj.InputDataFormat)
    {
        case KDDCUP : return ConvertFeaturesToBinary(InputVector, InputVectorLen);
        case TCPDUMP: return ConvertHeaderFieldsToBinary(InputVector, InputVectorLen);
    }
    
    return 0;
}//End of function PreProcessor::ConvertToBinary


//#################        FUNCTION CONVERT TO NORMALIZED FORM       ####################
int PreProcessor::ConvertToNormalizedForm(float *InputVector)
{
    switch (TrainTestDataProviderObj.InputDataFormat)
    {
        case KDDCUP : return ConvertFeaturesToNormalizedForm(InputVector);
        case TCPDUMP: return ConvertHeaderFieldsToNormalizedForm(InputVector);
    }
    
    return 0;
}//End of function PreProcessor::ConvertToNormalizedForm


//#################        FUNCTION CONVERT FEATURES TO BINARY      ####################
int PreProcessor::ConvertFeaturesToBinary(float *IV, int VectorLen)
{
    int FeatureCounter, i, k;
    float TempVector[500];
    
    for (i=0; i<VectorLen; i++) TempVector[i]=IV[i];
    
    for (FeatureCounter=0, i=0, k=0; FeatureCounter<KDD_FEATURES_NUMBER; FeatureCounter++)
    {
        if (SelectedFeatures[FeatureCounter])
       {
            switch (FeatureCounter)
            {
                case  0:  //duration
                    if (TempVector[k]>=1 && TempVector[k]<=10) TempVector[k]=1;
                    else if (TempVector[k]>=11 && TempVector[k]<=100) TempVector[k]=2;
                    else if (TempVector[k]>=101 && TempVector[k]<=300) TempVector[k]=3;
                    else if (TempVector[k]>=301 && TempVector[k]<=600) TempVector[k]=4;
                    else if (TempVector[k]>=601 && TempVector[k]<=1000) TempVector[k]=5;
                    else if (TempVector[k]>=1001 && TempVector[k]<=1500) TempVector[k]=6;
                    else if (TempVector[k]>=1501 && TempVector[k]<=2500) TempVector[k]=7;
                    else if (TempVector[k]>=2501 && TempVector[k]<=4000) TempVector[k]=8;
                    else if (TempVector[k]>=4001 && TempVector[k]<=7000) TempVector[k]=9;
                    else if (TempVector[k]>=7001 && TempVector[k]<=10000) TempVector[k]=10;
                    else if (TempVector[k]>=10001 && TempVector[k]<=15000) TempVector[k]=11;
                    else if (TempVector[k]>=15001 && TempVector[k]<=20000) TempVector[k]=12;
                    else if (TempVector[k]>=20001 && TempVector[k]<=30000) TempVector[k]=13;
                    else if (TempVector[k]>=30001 && TempVector[k]<=45000) TempVector[k]=14;
                    else if (TempVector[k]>=45001) TempVector[k]=15;
                    else TempVector[k]=0;
		    
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[0]);
                    i+=FeatureBinaryLen[0];
                    break;
		
                case  1: //protocol_type
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[1]);
                    i+=FeatureBinaryLen[1];
                    break;
		
                case  2: //service
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[2]);
                    i+=FeatureBinaryLen[2];
                    break;
		
                case  3: //flag
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[3]);
                    i+=FeatureBinaryLen[3];
                    break;
		    
                case  4: //src_bytes
                case  5: //dst_bytes
                    if (TempVector[k]>=1 && TempVector[k]<=10) TempVector[k]=1;
                    else if (TempVector[k]>=11 && TempVector[k]<=100) TempVector[k]=2;
                    else if (TempVector[k]>=101 && TempVector[k]<=300) TempVector[k]=3;
                    else if (TempVector[k]>=301 && TempVector[k]<=600) TempVector[k]=4;
                    else if (TempVector[k]>=601 && TempVector[k]<=1000) TempVector[k]=5;
                    else if (TempVector[k]>=1001 && TempVector[k]<=1500) TempVector[k]=6;
                    else if (TempVector[k]>=1501 && TempVector[k]<=2500) TempVector[k]=7;
                    else if (TempVector[k]>=2501 && TempVector[k]<=4000) TempVector[k]=8;
                    else if (TempVector[k]>=4001 && TempVector[k]<=7000) TempVector[k]=9;
                    else if (TempVector[k]>=7001 && TempVector[k]<=10000) TempVector[k]=10;
                    else if (TempVector[k]>=10001 && TempVector[k]<=15000) TempVector[k]=11;
                    else if (TempVector[k]>=15001 && TempVector[k]<=20000) TempVector[k]=12;
                    else if (TempVector[k]>=20001 && TempVector[k]<=40000) TempVector[k]=13;
                    else if (TempVector[k]>=40001 && TempVector[k]<=100000) TempVector[k]=14;
                    else if (TempVector[k]>=100000) TempVector[k]=15;
                    else TempVector[k]=0;
		    
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[4]);
                    i+=FeatureBinaryLen[4];
                    break;
		    
                case  6: //land
                    IV[i++]=TempVector[k];
                    break;
		    
                case  7: //wrong_fragment
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[7]);
                    i+=FeatureBinaryLen[7];
                    break;
		    
                case  8: //urgent
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[8]);
                    i+=FeatureBinaryLen[8];
                    break;
		    
                case  9: //hot
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[9]);
                    i+=FeatureBinaryLen[9];
                    break;
		    
                case 10: //num_failed_logins
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[10]);
                    i+=FeatureBinaryLen[10];
                    break;
		    
                case 11: //logged_in
                    IV[i++]=TempVector[k];
                    break;
		    
                case 12: //num_compromised
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[12]);
                    i+=FeatureBinaryLen[12];
                    break;
		    
                case 13: //root_shell
                case 14: //su_attempted
                    IV[i++]=TempVector[k];
                    break;
		    
                case 15: //num_root
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[15]);
                    i+=FeatureBinaryLen[15];
                    break;
		    
                case 16: //num_file_creations
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[16]);
                    i+=FeatureBinaryLen[16];
                    break;
		    
                case 17: //num_shells
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[17]);
                    i+=FeatureBinaryLen[17];
                    break;
		    
                case 18: //num_access_files
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[18]);
                    i+=FeatureBinaryLen[18];
                    break;
		    
                case 19: //num_outbound_cmds
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[19]);
                    i+=FeatureBinaryLen[19];
                    break;

                case 20: //is_hot_login
                case 21: //is_guest_login
                    IV[i++]=TempVector[k];
                    break;
		    
                case 22: //count
                    DecimalToBinary(TempVector[k]/2, IV+i, FeatureBinaryLen[22]);
                    i+=FeatureBinaryLen[22];
                    break;
		    
                case 23: //srv_counts
                    DecimalToBinary(TempVector[k], IV+i, FeatureBinaryLen[23]);
                    i+=FeatureBinaryLen[23];
                    break;
		    
                case 31: //dst_host_count
                    DecimalToBinary(TempVector[k]/4, IV+i, FeatureBinaryLen[31]);
                    i+=FeatureBinaryLen[31];
                    break;
		    
                case 32: //dst_host_srv_count
                    DecimalToBinary(TempVector[k]/4, IV+i, FeatureBinaryLen[32]);
                    i+=FeatureBinaryLen[32];
                    break;
		    
                case 24: //serror_rate
                case 25: //srv_serror_rate
                case 26: //rerror_rate
                case 27: //srv_rerror_rate
                case 28: //same_srv_rate
                case 29: //diff_srv_rate
                case 30: //srv_diff_host_rate
                case 33: //dst_host_same_srv_rate
                case 34: //dst_host_diff_srv_rate
                case 35: //dst_host_same_src_port_rate
                case 36: //dst_host_srv_diff_host_rate
                case 37: //dst_host_serror_rate
                case 38: //dst_host_srv_serror_rate
                case 39: //dst_host_rerror_rate
                case 40: //dst_host_srv_rerror_rate
                    for (int l=0; l<5; l++) IV[i+l]=0;
                    if (TempVector[k]>0.50)
                    {
                        IV[i]=1;
                        TempVector[k]-=0.50;
                    }
                    if (TempVector[k]>0.25)
                    {
                        IV[i+1]=1;
                        TempVector[k]-=0.25;
                    }
                    if (TempVector[k]>0.125)
                    {
                        IV[i+2]=1;
                        TempVector[k]-=0.125;
                    }
                    if (TempVector[k]>0.0625)
                    {
                        IV[i+3]=1;
                        TempVector[k]-=0.0625;
                    }
                    if (TempVector[k]>0.03125)
                        IV[i+4]=1;
                    i+=FeatureBinaryLen[40];
		    
            }//end of switch (FeatureCounter)
            k++;
	    
        }//end of if
    }//end of for
    
    return i;
}//End of function PreProcessor::ConvertFeaturesToBinary


//#################        FUNCTION CONVERT HEADER FIELDS TO BINARY      ####################
int PreProcessor::ConvertHeaderFieldsToBinary(float *IV, int VectorLen)
{
    int HFCounter, i, k;
    float TempVector[200];
    
    for (i=0; i<VectorLen; i++) TempVector[i]=IV[i];
    
    
    for (HFCounter=0, i=0, k=0; HFCounter<IP_LEN+TCP_LEN+UDP_LEN+ICMP_LEN; HFCounter++)
    {
        if (SelectedHeaderFields[HFCounter])
       {
            switch (HFCounter)
            {
                case  0:  //Protocol Type
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[0]);
                    i+=HeaderFieldBinaryLen[0];
                    break;
		    
                case  1:  //Time Stamp
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[1]);
                    i+=HeaderFieldBinaryLen[1];
                    break;
		    
                case  2:  //Identification
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[2]);
                    i+=HeaderFieldBinaryLen[2];
                    break;
		
                case  3:  //Type Of Service (TOS)
                case  4:  //Time To Live (TTL)
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[3]);
                    i+=HeaderFieldBinaryLen[3];
                    break;
		    
                case  5:  //IP Header Length
                    DecimalToBinary(TempVector[k]/4, IV+i, HeaderFieldBinaryLen[5]);
                    i+=HeaderFieldBinaryLen[5];
                    break;
		    
                case  6:  //Datagram Length
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[6]);
                    i+=HeaderFieldBinaryLen[6];
                    break;
		
                case  7:  //Source IP 
                case 8:  //Destination IP
                case 9 : //Land : Same Source & Destination IP Address
                    IV[i++]=TempVector[k];
                    break;
		    
                case 10: //Fragmentation		    
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[10]);
                    i+=HeaderFieldBinaryLen[10];
                    break;
		    
                case 11:  //TCP Source Port
                case 12:  //TCP Destination Port
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[16]);
                    i+=HeaderFieldBinaryLen[16];
                    break;
		
                case 13:  //TCP SYN Flag
                case 14:  //TCP FIN Flag
                case 15:  //TCP ACK Flag
                case 16:  //TCP PUSH Flag
                case 17:  //TCP RST Flag
                case 18:  //TCP URG Flag
                    IV[i++]=TempVector[k];
                    break;
		    
                case 19:  //TCP Header Length
                    DecimalToBinary(TempVector[k]/4, IV+i, HeaderFieldBinaryLen[19]);
                    i+=HeaderFieldBinaryLen[19];
                    break;
		
                case 20:  //TCP Window Size
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[20]);
                    i+=HeaderFieldBinaryLen[20];
                    break;
		    
                case 21:  //UDP Source Port
                case 22:  //UDP Destination Port
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[21]);
                    i+=HeaderFieldBinaryLen[21];
                    break;
		    
                case 23:  //ICMP Type
                case 24:  //ICMP Code
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[23]);
                    i+=HeaderFieldBinaryLen[23];
                    break;
		    
                case 25:  //ICMP ID
                case 26:  //ICMP Sequence
                    DecimalToBinary(TempVector[k], IV+i, HeaderFieldBinaryLen[25]);
                    i+=HeaderFieldBinaryLen[25];
                    break;
		    
            }//end of switch
            k++;
	    
        }//end of if
    }//end of for
    
    return i;
}//End of function PreProcessor::ConvertHeaderFieldsToBinary


//#################        FUNCTION CONVERT FEATURES TO NORMALIZED FORM      ####################
int PreProcessor::ConvertFeaturesToNormalizedForm(float *IV)
{
    int FeatureCounter, i;
    
    for (FeatureCounter=0, i=0; FeatureCounter<KDD_FEATURES_NUMBER; FeatureCounter++)
    {
        if (SelectedFeatures[FeatureCounter])
       {
            switch (FeatureCounter)
            {
                case  0:  //duration
                    IV[i]=sqrt(IV[i]) / 200;
                    if (IV[i]>1) IV[i]=1;
                    break;
		
                case  1: //protocol_type
                    IV[i]=IV[i] / 4;
                    break;
		
                case  2: //service
                    IV[i]=IV[i] / 70;
                    break;
		
                case  3: //flag
                    break;
		    
                case  4: //src_bytes
                case  5: //dst_bytes
                    IV[i]=sqrt(IV[i]) / 300;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case  6: //land
                    break;
		    
                case  7: //wrong_fragment
                    IV[i]=IV[i] / 3;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case  8: //urgent
                    IV[i]=IV[i] / 30;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case  9: //hot
                    IV[i]=IV[i] / 31;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 10: //num_failed_logins
                    IV[i]=IV[i] / 5;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 11: //logged_in
                    break;
		    
                case 12: //num_compromised
                    IV[i]=IV[i] / 9;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 13: //root_shell
                case 14: //su_attempted
                    break;
		    
                case 15: //num_root
                    IV[i]=IV[i] / 255;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 16: //num_file-creations
                    IV[i]=IV[i] / 31;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 17: //num_shells
                    IV[i]=IV[i] / 5;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 18: //num_access_files
                    IV[i]=IV[i] / 9;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 19: //num_outbound_cmds
                    IV[i]=IV[i] / 3;
                    if (IV[i]>1) IV[i]=1;
                    break;

                case 20: //is_hot_login
                case 21: //is_guest_login
                    break;
		    
                case 22: //count
                    IV[i]=IV[i] / 511;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 23: //srv_counts
                    IV[i]=IV[i] / 127;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 31: //dst_host_count
                case 32: //dst_host_srv_count
                    IV[i]=IV[i] / 255;
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
                case 24: //serror_rate
                case 25: //srv_serror_rate
                case 26: //rerror_rate
                case 27: //srv_rerror_rate
                case 28: //same_srv_rate
                case 29: //diff_srv_rate
                case 30: //srv_diff_host_rate
                case 33: //dst_host_same_srv_rate
                case 34: //dst_host_diff_srv_rate
                case 35: //dst_host_same_src_port_rate
                case 36: //dst_host_srv_diff_host_rate
                case 37: //dst_host_serror_rate
                case 38: //dst_host_srv_serror_rate
                case 39: //dst_host_rerror_rate
                case 40: //dst_host_srv_rerror_rate
                    if (IV[i]>1) IV[i]=1;
                    break;
		    
            }//end of switch (FeatureCounter)
            i++;
        }//end of if
    }//end of for
    
    return i;
}//End of function PreProcessor::ConvertFeaturesToNormalizedForm


//#################        FUNCTION CONVERT HEADER FIELDS TO NORMALIZED FORM      ####################
int PreProcessor::ConvertHeaderFieldsToNormalizedForm(float *IV)
{
    int HFCounter, i;
    
    for (HFCounter=0, i=0; HFCounter<IP_LEN+TCP_LEN+UDP_LEN+ICMP_LEN; HFCounter++)
    {
        if (SelectedHeaderFields[HFCounter])
       {
            switch (HFCounter)
            {
                case  0:  //Protocol Type
                    IV[i]=IV[i] / 3;
                    break;
		    
                case  1:  //Time Stamp
                    if (IV[i]>65535) IV[i]=1;
                    else IV[i]=IV[i] / 65535;
                    break;
		    
                case  2:  //Identification
                    IV[i]=IV[i]/65535;
                    break;
		
                case  3:  //Type Of Service (TOS)
                case  4:  //Time To Live (TTL)
                    IV[i]=IV[i] / 255;
                    break;
		    
                case  5:  //IP Header Length
                    IV[i]=IV[i] / 60;
                    break;
		    
                case  6:  //Datagram Length
                    IV[i]=IV[i] / 65535;
                    break;
		
                case  7:  //Source IP
                case  8:  //Destination IP
                case  9: //Land :Same Source & Destination IP Address
                    break;
		    
                case 10: //Fragmentation
                    IV[i]=IV[i] / 7;
                    break;
		    
                case 11:  //TCP Source Port
                    IV[i]=IV[i] / 65535;
                    break;
		    
                case 12:  //TCP Destination Port
                    IV[i]=IV[i] / 65535;
                    break;
		
                case 13:  //TCP SYN Flag
                case 14:  //TCP FIN Flag
                case 15:  //TCP ACK Flag
                case 16:  //TCP PUSH Flag
                case 17:  //TCP RST Flag
                case 18:  //TCP URG Flag
                    break;
		    
                case 19:  //TCP Header Length
                    IV[i]=IV[i] / 60;
                    break;
		
                case 20:  //TCP Window Size
                    IV[i]=IV[i] / 65535;
                    break;
		    
                case 21:  //UDP Source Port
                    IV[i]=IV[i] / 65535;
                    break;
		
                case 22:  //UDP Destination Port
                    IV[i]=IV[i] / 65535;
                    break;
		    
                case 23:  //ICMP Type
                case 24:  //ICMP Code
                    IV[i]=IV[i] / 255;
                    break;
		    
                case 25:  //ICMP ID
                case 26:  //ICMP Sequence
                    IV[i]=IV[i] / 65535;
		    
            }//end of switch
            i++;
        }//end of if
    }//end of for
    
    return i;
}//End of function PreProcessor::ConvertHeaderFieldsToNormalizedForm


//#################        FUNCTION GET SERVICE CODE      ####################
float PreProcessor::GetServiceCode(char *ServiceName)
{
    if ( !strncasecmp(ServiceName,"http", 4))
        return 0;
    else if ( !strncasecmp(ServiceName,"ftp", 3))
        return 1;
    else if ( !strncasecmp(ServiceName,"ftp_data", 8))
        return 2;
    else if ( !strncasecmp(ServiceName,"smtp", 4))
        return 3;
    else if ( !strncasecmp(ServiceName,"telnet", 6))
        return 4;
    else if ( !strncasecmp(ServiceName,"private", 7))
        return 5;
    else if ( !strncasecmp(ServiceName,"domain_u", 8))
        return 6;
    else if ( !strncasecmp(ServiceName,"auth", 4))
        return 7;
    else if ( !strncasecmp(ServiceName,"finger", 6))
        return 8;
    else if ( !strncasecmp(ServiceName,"eco_i", 5))
        return 9;
    else if ( !strncasecmp(ServiceName,"ntp_u", 5))
        return 10;
    else if ( !strncasecmp(ServiceName,"ecr_i", 5))
        return 11;
    else if ( !strncasecmp(ServiceName,"IRC", 3))
        return 12;
    else if ( !strncasecmp(ServiceName,"X11", 3))
        return 13;
    else if ( !strncasecmp(ServiceName,"time", 4))
        return 14;
    else if ( !strncasecmp(ServiceName,"netbios_dgm", 11))
        return 15;
    else if ( !strncasecmp(ServiceName,"netbios_ssn", 11))
        return 16;
    else if ( !strncasecmp(ServiceName,"netbios_ns", 10))
        return 17;
    else if ( !strncasecmp(ServiceName,"imap4", 5))
        return 18;
    else if ( !strncasecmp(ServiceName,"sql_net", 7))
        return 19;
    else if ( !strncasecmp(ServiceName,"vmnet", 5))
        return 20;
    else if ( !strncasecmp(ServiceName,"bgp", 3))
        return 21;
    else if ( !strncasecmp(ServiceName,"z39_50", 6))
        return 22;
    else if ( !strncasecmp(ServiceName,"ldap", 4))
        return 23;
    else if ( !strncasecmp(ServiceName,"nnsp", 4))
        return 24;
    else if ( !strncasecmp(ServiceName,"http_443", 8))
        return 25;
    else if ( !strncasecmp(ServiceName,"shell", 5))
        return 26;
    else if ( !strncasecmp(ServiceName,"printer", 7))
        return 27;
    else if ( !strncasecmp(ServiceName,"login", 5))
        return 28;
    else if ( !strncasecmp(ServiceName,"exec", 4))
        return 29;
    else if ( !strncasecmp(ServiceName,"efs", 3))
        return 30;
    else if ( !strncasecmp(ServiceName,"klogin", 6))
        return 31;
    else if ( !strncasecmp(ServiceName,"kshell", 6))
        return 32;
    else if ( !strncasecmp(ServiceName,"uucp", 4))
        return 33;
    else if ( !strncasecmp(ServiceName,"systat", 6))
        return 34;
    else if ( !strncasecmp(ServiceName,"mtp", 3))
        return 35;
    else if ( !strncasecmp(ServiceName,"remote_job", 10))
        return 36;
    else if ( !strncasecmp(ServiceName,"rje", 3))
        return 37;
    else if ( !strncasecmp(ServiceName,"link", 4))
        return 38;
    else if ( !strncasecmp(ServiceName,"pop_2", 5))
        return 39;
    else if ( !strncasecmp(ServiceName,"pop_3", 5))
        return 40;
    else if ( !strncasecmp(ServiceName,"nntp", 4))
        return 41;
    else if ( !strncasecmp(ServiceName,"echo", 4))
        return 42;
    else if ( !strncasecmp(ServiceName,"discard", 7))
        return 43;
    else if ( !strncasecmp(ServiceName,"daytime", 7))
        return 44;
    else if ( !strncasecmp(ServiceName,"ssh", 3))
        return 45;
    else if ( !strncasecmp(ServiceName,"name", 4))
        return 46;
    else if ( !strncasecmp(ServiceName,"whois", 5))
        return 47;
    else if ( !strncasecmp(ServiceName,"gopher", 6))
        return 48;
    else if ( !strncasecmp(ServiceName,"ctf", 3))
        return 49;
    else if ( !strncasecmp(ServiceName,"supdup", 6))
        return 50;
    else if ( !strncasecmp(ServiceName,"iso_tsap", 8))
        return 51;
    else if ( !strncasecmp(ServiceName,"host_names", 10))
        return 52;
    else if ( !strncasecmp(ServiceName,"sunrpc", 6))
        return 53;
    else if ( !strncasecmp(ServiceName,"uucp_path", 9))
        return 54;
    else if ( !strncasecmp(ServiceName,"courier", 7))
        return 55;
    else if ( !strncasecmp(ServiceName,"csnet_ns", 8))
        return 56;
    else if ( !strncasecmp(ServiceName,"domain", 6))
        return 57;
    else if ( !strncasecmp(ServiceName,"tim_i", 5))
        return 58;
    else if ( !strncasecmp(ServiceName,"netstat", 7))
        return 59;
    else if ( !strncasecmp(ServiceName,"pm_dump", 7))
        return 60;
    else if ( !strncasecmp(ServiceName,"urp_i", 5))
        return 61;
    else if ( !strncasecmp(ServiceName,"urh_i", 5))
        return 62;
    else if ( !strncasecmp(ServiceName,"hostnames", 9))
        return 63;
    else if ( !strncasecmp(ServiceName,"icmp", 4))
        return 64;
    else //unknown or other services
        return 70;
}//End of function PreProceesor::GetServiceCode


//#################        FUNCTION GET FLAG CODE      ####################
float PreProcessor::GetFlagCode(char *FlagName)
{
    if ( !strncasecmp(FlagName,"SF", 2))
        return 0;
    else if ( !strncasecmp(FlagName,"REJ", 3))
        return 1;
    else if ( !strncasecmp(FlagName,"S0", 2))
        return 2;
    else if ( !strncasecmp(FlagName,"S1", 2))
        return 3;
    else if ( !strncasecmp(FlagName,"S2", 2))
        return 4;
    else if ( !strncasecmp(FlagName,"S3", 2))
        return 5;
    else if ( !strncasecmp(FlagName,"RSTO", 4))
        return 6;
    else if ( !strncasecmp(FlagName,"RSTR", 4))
        return 7;
    else if ( !strncasecmp(FlagName,"RSTOSO", 6))
        return 8;
    else if ( !strncasecmp(FlagName,"SH", 2))
        return 9;
    else if ( !strncasecmp(FlagName,"OTH", 3))
        return 10;
    else //unknown or other flags
        return 11;
}//End of PreProcessor::GetFlagCode


//#################        FUNCTION DECIMAL TO BINARY      ####################
void PreProcessor::DecimalToBinary(float Dec, float *Bin, int Len)
{
    int i, X;
    
    if (Dec>=pow(2, Len)) 
    {
        for (i=0; i<Len; i++)  Bin[i]=1;	    
        return;
    }
    else
    {
        X=int(floor(Dec));
        for (i=0; i<Len; i++)
        {
            Bin[Len-i-1]=float(X%2);
            X=X/2;
        }
    }//end of else    
}//End of function PreProcessor::DecimalToBinary


//#################        FUNCTION EXTRACT EXPECTED OUTPUT      ####################
int PreProcessor::ExtractExpectedOutput(char *NetConnectionPacketRecord)
{
    unsigned int i, j;
    for (i=0; i<strlen(NetConnectionPacketRecord) && NetConnectionPacketRecord[i]!='.'; i++);
    for (i--; i>0 && NetConnectionPacketRecord[i]!=','; i--);
    i++;
    
    if (!strncasecmp(NetConnectionPacketRecord+i, "normal", 6))  return NORMAL;
    else 
        for (j=0; j<ATTACK_NUMBER; j++)
            //Attack Code is equal to its index in the AttackDB array
            if ( !strncasecmp(NetConnectionPacketRecord+i, AttackDB[j].Name, strlen(AttackDB[j].Name) ) )  return j; 
    
    return UNKNOWN_ATTACK;	    
}//End of function PreProcessor::ExtractExpectedOutput


//#################        FUNCTION GIVE PACKET      ####################
int PreProcessor::GivePacket(char *NetPacketRecord)
{
    int VectorLen=0, Len=0;
    int ConversionType;
    float TempInputVector[500];

    switch (TrainTestDataProviderObj.InputDataFormat)
    {
        case KDDCUP : Len=ExtractFeaturesInputVector(NetPacketRecord, TempInputVector);
            break;
        case TCPDUMP: Len=ExtractHeaderFieldsInputVector(NetPacketRecord, TempInputVector);
    }//end of switch (TrainTestDataProviderObj.InputDataFormat)
   
    ConversionType=NeuralNetObj.GetInputDataType();
    
    switch (ConversionType)
    {
        case BINARY:
            VectorLen=ConvertToBinary(TempInputVector, Len);
            break;  
        case NORMALIZED:
            VectorLen=ConvertToNormalizedForm(TempInputVector);
             break;
        case REAL:
            VectorLen=Len;
             break;
    }//end of switch (ConversionType)
    
    NeuralNetObj.NeuralNetAttackDetection(NetPacketRecord, TempInputVector);
   
    return 1;
}//End of function PreProcessor::GivePacket    
