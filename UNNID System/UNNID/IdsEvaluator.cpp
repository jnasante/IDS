/***************************************************************************
                          IdsEvaluator.cpp  -  description
                             -------------------
    begin                : Sat Jun 7 2003
    copyright            : (C) 2003 by Morteza Amini	
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include "IdsEvaluator.h"
#include "PreProcessor.h"
#include "NeuralNet.h"
#include "AttackTypes.h"

extern PreProcessor PreProcessorObj;
extern NeuralNet NeuralNetObj;


//#################        FUNCTION IDS EVALUATOR CONSTRUCTOR       ####################
IdsEvaluator::IdsEvaluator()
{
}//End of function IdsEvaluator::Construstor


//#################        FUNCTION IDSEVALUATOR DESTRUCTOR       ####################
IdsEvaluator::~IdsEvaluator()
{
}//End of function IdsEvaluator::~IdsEvaluator


//#################        FUNCTION EVALUATE TEST IDS       ####################
int IdsEvaluator::EvaluateTestIds(char *EvaluationResultsStr)
{
    float DetectionNumber=0,
          TrueTypeNumber=0,
          TrueNumber=0,
          FalsePositiveNumber=0,
          FalseNegativeNumber=0,
          DoSNumber=0,
          TrueDoSNumber=0,
          R2LNumber=0,
          TrueR2LNumber=0,
          U2RNumber=0,
          TrueU2RNumber=0,
          ProbeNumber=0,
          TrueProbeNumber=0;
    char TestResultFileName[512],
         TestResultRecord[512],
         TempStr[20];
    FILE *TRFPointer;
    unsigned int i, j;
    int ExpectedOutput,
        IdsOutput;
    bool *KnownAttack;
    
    NeuralNetObj.GetTestResultFileName(TestResultFileName);
    
    if ( !(TRFPointer=fopen(TestResultFileName, "r") ) )  
    {
        perror ("Error: IdsEvaluator.EvaluateTestIds: Test Result File not found !");
        return 0;
    }
    
    //First Line is consist of known Attacks types
    fgets(TestResultRecord, 512, TRFPointer);
    KnownAttack=new bool[ATTACK_NUMBER];
    for (i=0; i<ATTACK_NUMBER; i++) KnownAttack[i]=false;
    for (i=0; i<strlen(TestResultRecord); i++)
    {
        for (j=0; TestResultRecord[i]!=',' && TestResultRecord[i]!='\n' && TestResultRecord[i]; j++, i++)
        TempStr[j]=TestResultRecord[i];
        TempStr[j]='\0';
        KnownAttack[atoi(TempStr)]=true;
    }
    
    //Begin from first vector of test vectors
    if (!PreProcessorObj.OffLineMode(ON, TEST)) { fclose(TRFPointer);  return 0; }
    PreProcessorObj.GoFirstInputOutputVectors();
 
    while (PreProcessorObj.GetNextOutputVector(ExpectedOutput) && fgets(TestResultRecord, 512, TRFPointer) )    
    {
        DetectionNumber++;
	
        for (i=0; i<strlen(TestResultRecord) && TestResultRecord[i]!='>'; i++);
        i+=2;
        sscanf (TestResultRecord+i, "%d", &IdsOutput);
     
        //Dos Attack True Detection
        if ( AttackDB[ExpectedOutput].Class==DOS )
        {
            DoSNumber++;
            if (IdsOutput!=NORMAL) TrueDoSNumber++;
        }
	
        //R2L Attack True Detection
        else if ( AttackDB[ExpectedOutput].Class==R2L ) 
        {
            R2LNumber++;
            if (IdsOutput!=NORMAL) TrueR2LNumber++;
        }
	
        //U2R Attack True Detection
        else if ( AttackDB[ExpectedOutput].Class==U2R ) 
        {
            U2RNumber++;
            if (IdsOutput!=NORMAL) TrueU2RNumber++;
        }
	
        //Probing True Detection
        else if ( AttackDB[ExpectedOutput].Class==PROBE ) 
        {
            ProbeNumber++;
            if (IdsOutput!=NORMAL) TrueProbeNumber++;
        }
		 
        if (ExpectedOutput==IdsOutput || (!KnownAttack[ExpectedOutput] && IdsOutput==UNKNOWN_ATTACK) ) TrueTypeNumber++;
        if (ExpectedOutput!=NORMAL && IdsOutput==NORMAL) FalseNegativeNumber++;
        else if (ExpectedOutput==NORMAL && IdsOutput!=NORMAL) FalsePositiveNumber++;
        else TrueNumber++;
	    
    }//end of while
    
    fclose(TRFPointer);
    delete[] KnownAttack;
    if (!PreProcessorObj.OffLineMode(OFF)) return 0;
    
    sprintf (EvaluationResultsStr, 
	     "Total Records Number = %2.0f\n\nExact True Type Detection Number = %2.0f\n     Exact True Type Detection Rate = %%%2.2f\n\nTrue Detection Number = %2.0f\n     True Detection Rate = %%%2.2f\n\nFalse Positive Detection Number = %2.0f\n     False Positive Detection Rate = %%%2.2f\n\nFalse Negative Detection Number=%2.0f\n     False Negative Detection Rate = %%%2.2f\n\nDenial of Service (DoS) Attack Detection Number=%2.0f\n     True Denial of Service (DoS) Attack Detection Rate = %%%2.2f\n\nRemote to Local (R2L) Attack Detection Number=%2.0f\n     Remote to Local (R2L) Attack Detection Rate = %%%2.2f\n\nUser to Root (U2R) Attck Detection Number=%2.0f\n     User to Root (U2R) Attck Detection Rate = %%%2.2f\n\nProbing Attack Detection Number=%2.0f\n     Probing Attack Detection Rate = %%%2.2f\n",
	     DetectionNumber,
	     TrueTypeNumber,
	     (TrueTypeNumber*100)/DetectionNumber,
	     TrueNumber,
	     (TrueNumber*100)/DetectionNumber,
	     FalsePositiveNumber,
	     (FalsePositiveNumber*100)/DetectionNumber,
	     FalseNegativeNumber,
	     (FalseNegativeNumber*100)/DetectionNumber,
	     TrueDoSNumber,
	     (DoSNumber==0 ? 0 : (TrueDoSNumber*100)/DoSNumber),
	     TrueR2LNumber,
	     (R2LNumber==0 ? 0 : (TrueR2LNumber*100)/R2LNumber),
	     TrueU2RNumber,
	     (U2RNumber==0 ? 0 : (TrueU2RNumber*100)/U2RNumber),
	     TrueProbeNumber,
	     (ProbeNumber==0 ? 0 : (TrueProbeNumber*100)/ProbeNumber) );

    printf ("=============================================\n");
    printf ("Results of UNNID system evaluation:\n%s\n", EvaluationResultsStr);
    printf ("=============================================\n");
    
    return 1;
}//End of function IdsEvaluator::EvaluateTestIds
