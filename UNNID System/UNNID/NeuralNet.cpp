/***************************************************************************
                          NeuralNet.cpp  -  description
                             -------------------
    begin                : Sun Apr 27 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/

#include "NeuralNet.h"
#include "TrainTestDataProvider.h"
#include "PreProcessor.h"
#include "Responder.h"
#include "AttackTypes.h"
#include <time.h>

extern TrainTestDataProvider TrainTestDataProviderObj;
extern PreProcessor PreProcessorObj;
extern Responder ResponderObj;


//#################        FUNCTION NEURAL NET CONSTRUCTOR       ####################
NeuralNet::NeuralNet()
{
    NetState=TRAINED;
    NetMemoryAllocated=false;
}//End of function NeuralNet::NeuralNet


//#################        FUNCTION NEURAL NET DESTRUCTOR       ####################
NeuralNet::~NeuralNet()
{
}//End of function NeuralNet::~NeuralNet


//#################        FUNCTION TRAIN NET       ####################
int NeuralNet::TrainNet()
{
    int retval=1;
    time_t now;			

    time(&now);
    printf("************************************************************\n");
    printf("UNNID: Start Training UNNID system at: %s", asctime(localtime( &now  )));
    
    //Initializing Neural Net based on neural net model
    if (!InitializeNet(false))
        { FreeNet(); return 0; }
    
    //Prepare PreProcessor for acting in off-line mode for training
    if (!PreProcessorObj.OffLineMode(ON, TRAIN))
        { FreeNet(); return 0; }

    ShowProgressWindow(0, 0, TRAIN);    
    
    //Training Net based on neural net type
    switch (NetModel)
    {
       case SOM : if (!TrainSOM()) retval=0;
                  break;

       case ART1: if (!TrainART1()) retval=0;
                  break;

       case ART2: if (!TrainART2()) retval=0;
    }
    ShowProgressWindow(3);    
    
    PreProcessorObj.OffLineMode(OFF);
    
    //Saving trained weights to its file
    if (retval)
        if (!SaveWeightsAndMap()) retval=0;
 
    //Free allocated memories after success training
    if (!FreeNet()) return 0;
    
    time(&now);
    printf("UNNID: Finish Training UNNID system at: %s", asctime(localtime( &now  )));
    printf("************************************************************\n");
    
    //Return 1, if Initilizing, Training, Saving Weighs,Freeing Memories are done successfully
    return retval;
}//End of function NeuralNet::TrainNet


//#################        FUNCTION TEST NET       ####################
int NeuralNet::TestNet()
{
    int retval=1;
    
    //Neural Net must be trained before testing
    if (NetState==NOTRAINED) return 0;
    
    //Initializing Neural Net based on neural net model
    if (!InitializeNet(true))
        { FreeNet(); return 0; }
    
    //Prepare PreProcessor for acting in off-line mode for training
    if (!PreProcessorObj.OffLineMode(ON, TEST))
        { FreeNet(); return 0; }

    ShowProgressWindow(0, 0,TEST);    
    
    //Training Net based on neural net type
    switch (NetModel)
    {
       case SOM : if (!TestSOM()) retval=0;
                  break;

       case ART1: if (!TestART1()) retval=0;
                  break;

       case ART2: if (!TestART2()) retval=0;
    }

    ShowProgressWindow(3);    
    PreProcessorObj.OffLineMode(OFF);
    
    //Free allocated memories after success testing
    if (!FreeNet()) return 0;

    //Return 1, if Initilizing, Testing, Saving Results and Freeing Memories are done successfully
    return retval;
    
}//End of function NeuralNet::TestNet


//#################        FUNCTION APPLY NET       ####################
int NeuralNet::ApplyNet(float *InputVector, float &AssuranceDegree)
{
    switch (NetModel)
    {
        case SOM : return ApplySOM(InputVector, AssuranceDegree);
        case ART1: return ApplyART1(InputVector, AssuranceDegree);
        case ART2: return ApplyART2(InputVector, AssuranceDegree);
    }//end of switch
    return 0;
}//End of function NeuralNet::ApplyNet


//#################        FUNCTION INITIALIZE NET       ####################
int NeuralNet::InitializeNet(bool LoadTrainedWeighsMap)
{
    switch (NetModel)
    {
        case SOM  : return InitializeSOM(LoadTrainedWeighsMap);
        case ART1 : return InitializeART1(LoadTrainedWeighsMap);
        case ART2 : return InitializeART2(LoadTrainedWeighsMap);
    }
    return 0;
}//Endof  function NeuralNet::InitializeNet


//#################        FUNCTION INITIALIZE SOM       ####################
int NeuralNet::InitializeSOM(bool LoadTrainedWeighsMap)
{
    int i, j;
    
    //Set Layer[0] sensors number
    if (InputDataType==BINARY) Layer[0]=InputVectorBinaryLen[TrainTestDataProviderObj.InputDataFormat];
    else Layer[0]=InputVectorNonBinaryLen[TrainTestDataProviderObj.InputDataFormat];
    
    //Memory Allocation for Weights Vector
    Weight[BU]=new float *[Layer[0]];
    for (i=0; i<Layer[0]; i++)
        Weight[BU][i]=new float[Layer[1]];

    //Memory Allocation for Input Vector
    InputVector = new float[Layer[0]];
    
    //Memory Allocation for Best MatchUnit Frequency
    BMUFreq= new int *[Layer[1]];
    for (i=0; i< Layer[1]; i++)
    {
        BMUFreq[i]=new int [ATTACK_NUMBER];
        for (j=0; j<ATTACK_NUMBER; j++) BMUFreq[i][j]=0;
    }
    
    //Memory Allocation for Clustering Map & Cluster Distance
    ClusteringMap=new int [Layer[1]];
    ClusteringMapAD=new float [Layer[1]];
    ClusterDistance=new float [Layer[1]];
    KnownAttack=new bool [ATTACK_NUMBER];
    
    //Memory Allocation for Distance Vector
    Dist = new float [Layer[1]];

    switch (LoadTrainedWeighsMap)
    {
        case false :
            //Initializing Weights with random numbers and Clustering Map with not-clustered and Cluster Distance with under its minimum value (-1)
            for (j=0; j<Layer[1]; j++)
            {
                for (i=0; i<Layer[0]; i++)
                    Weight[BU][i][j] = (float(rand())/RAND_MAX); //if normalizing in interval [-1,+1]
                ClusteringMap[j]=NOT_CLUSTERED;
                ClusteringMapAD[j]=100;
                ClusterDistance[j]=-1;
            }
            break;

        case true   :	    
            //Initializing Known Attacks array, known attack will determined based on attacks exist in WeightMap file
            for (i=0; i<ATTACK_NUMBER; i++) KnownAttack[i]=false;
	    
            //Load Trained Weights and Clustering Map from their file
           
            if (!LoadWeightsAndMap()) return 0;

            //Test & Set Net Sensivity

    }//end of switch

    NetMemoryAllocated=true;
    return 1;
}//End of function NeuralNet::InitializeSOM


//#################        FUNCTION INITALIZE ART1       ####################
int NeuralNet::InitializeART1(bool LoadTrainedWeighsMap)
{
    int i, j;
    
    //Set Layer[0] sensors number
    if (InputDataType==BINARY) Layer[0]=InputVectorBinaryLen[TrainTestDataProviderObj.InputDataFormat];
    else Layer[0]=InputVectorNonBinaryLen[TrainTestDataProviderObj.InputDataFormat];

    //Memory Allocation for Bottom-Up and Top-Down Weights Vector
    Weight[BU]=new float *[Layer[0]];
    Weight[TD]=new float *[Layer[1]];
    for (i=0; i<Layer[0]; i++)
        Weight[BU][i]=new float[Layer[1]];
    for (j=0; j<Layer[1]; j++)
        Weight[TD][j]=new float[Layer[0]];

    //Memory Allocation for Input Vector
    InputVector=new float[Layer[0]];

    //Memory Allocation for Best MatchUnit Frequency
    BMUFreq= new int *[Layer[1]];
    for (i=0; i< Layer[1]; i++)
    {
        BMUFreq[i]=new int [ATTACK_NUMBER];
        for (j=0; j<ATTACK_NUMBER; j++) BMUFreq[i][j]=0;
    }
    
    //Memory Allocation for Clustering Map
    ClusteringMap=new int [Layer[1]];
    ClusteringMapAD=new float [Layer[1]];
    KnownAttack= new bool [ATTACK_NUMBER];

    
    //Memory Allocation for layers neurons
    X=new float[Layer[0]];
    Y=new float[Layer[1]];

    switch (LoadTrainedWeighsMap)
    {
        case false :
            //Initializing Bottom-Up and Top-Down Weights with random numbers and Clustring Map with not-clustered
            for (j=0; j<Layer[1]; j++)
            {
                for (i=0; i<Layer[0]; i++)
                {
                    Weight[BU][i][j] = (float(rand()) / RAND_MAX) * (L / (L-1+Layer[0]) ); 
                    Weight[TD][j][i] = 1;
                }
                ClusteringMap[j]=NOT_CLUSTERED;
                ClusteringMapAD[j]=100;
            }
            break;

        case true   :
            //Initializing Known Attacks array, known attack will determined based on attacks exist in WeightMap file
            for (i=0; i<ATTACK_NUMBER; i++) KnownAttack[i]=false;
	    
            //Load Trained Weights and Clustering Map from their file
            if (!LoadWeightsAndMap()) return 0;
            //Test & Set Net Sensivity

    }//end of switch
     
    NetMemoryAllocated=true;
    return 1;
}//End of function NeuralNet::InitializeART1


//#################        FUNCTION INITALIZE ART2       ####################
int NeuralNet::InitializeART2(bool LoadTrainedWeighsMap)
{
    int i, j;
    
    //Set Layer[0] sensors number
    if (InputDataType==BINARY) Layer[0]=InputVectorBinaryLen[TrainTestDataProviderObj.InputDataFormat];
    else Layer[0]=InputVectorNonBinaryLen[TrainTestDataProviderObj.InputDataFormat];
    
    //Memory Allocation for Bottom-Up and Top-Down Weights Vector
    Weight[BU]=new float *[Layer[0]];
    Weight[TD]=new float *[Layer[1]];
    for (i=0; i<Layer[0]; i++)
        Weight[BU][i]=new float[Layer[1]];
    for (j=0; j<Layer[1]; j++)
        Weight[TD][j]=new float[Layer[0]];

    //Memory Allocation for Input Vector
    InputVector = new float[Layer[0]];

    //Memory Allocation for Best MatchUnit Frequency
    BMUFreq= new int *[Layer[1]];
    for (i=0; i< Layer[1]; i++)
    {
        BMUFreq[i]=new int [ATTACK_NUMBER];
        for (j=0; j<ATTACK_NUMBER; j++) BMUFreq[i][j]=0;
    }
    
    //Memory Allocation for Clustering Map
    ClusteringMap=new int [Layer[1]];
    ClusteringMapAD=new float [Layer[1]];
    KnownAttack= new bool [ATTACK_NUMBER];
    
    //Memory Allocation for layers neurons
    X=new float[Layer[0]];
    P=new float[Layer[0]];
    Q=new float[Layer[0]];
    U=new float[Layer[0]];
    V=new float[Layer[0]];
    W=new float[Layer[0]];
    R=new float[Layer[0]];
    Y=new float[Layer[1]];

    switch (LoadTrainedWeighsMap)
    {
        case false :
            //Initializing Bottom-Up and Top-Down Weights with random numbers and Clustering Map with not-clustered
            for (j=0; j<Layer[1]; j++)
            {
                for (i=0; i<Layer[0]; i++)
                {
                    Weight[BU][i][j] = (float (rand()) / RAND_MAX)  / ((1-D)*sqrt(Layer[0])); 
                    Weight[TD][j][i] = 0;
                }
                ClusteringMap[j]=NOT_CLUSTERED;
                ClusteringMapAD[j]=100;
            }
            break;

        case true   :
            //Initializing Known Attacks array, known attack will determined based on attacks exist in WeightMap file
            for (i=0; i<ATTACK_NUMBER; i++) KnownAttack[i]=false;
	    
            //Load Trained Weights and Clustering Map from their file
            if (!LoadWeightsAndMap()) return 0;

            //Test & Set Net Sensivity

    }//end of switch
     
    NetMemoryAllocated=true;
   return 1;
}//End of function NeuralNet::InitializeART2


//#################        FUNCTION FREE NET       ####################
int NeuralNet::FreeNet()
{
    int i, j;
    
    if (!NetMemoryAllocated) return 0;
    
    //Freeing all allocated memories in InitializeNet function for training, testing or applying neural network
    delete[] InputVector;
    
    for (i=0; i<Layer[0]; i++)
        delete[] Weight[BU][i];
    delete[] Weight[BU];
    
    for (j=0; j<Layer[1]; j++)
        delete[] BMUFreq[j];
    delete[] BMUFreq;
    delete[] ClusteringMap;
    delete[] ClusteringMapAD;
    delete[] KnownAttack;
 
    switch (NetModel)
    {
        case SOM:
            delete[] Dist;
            delete[] ClusterDistance;
            break;

        case ART1:
            for (j=0; j<Layer[1]; j++)
                delete[] Weight[TD][j];
            delete[] Weight[TD];
            delete[] X;
            delete[] Y;
            break;

        case ART2:
            for (j=0; j<Layer[1]; j++)
                delete[] Weight[TD][j];
            delete[] Weight[TD];
            delete[] X;
            delete[] P;
            delete[] Q;
            delete[] U;
            delete[] V;
            delete[] W;
            delete[] Y;
            delete[] R;

    }//end of switch (NetModel)

    NetMemoryAllocated=false;
    return 1;
}//End of function NeuralNet::FreeNet


//#################        FUNCTION TRAIN SOM       ####################
int NeuralNet::TrainSOM()
{
    int i, j, R,
        MinJ,
        MaxJ,
        NeighborNumber,
        *Neighbor,
        Epochs,
        RecordsNumber,
        ProgressState,
        SumBMU;
    float LR;
    
    //Memory allocation for Neighbor (local variable)
    Neighbor=new int [Layer[1]];

    //Step 0: Initialize Parameters
    Epochs=0;
    R=Radius;
    LR=LearningRate;
    
    //Step 1: While stopping condition is false, do steps 2-8 
    while (Epochs<MaxEpochs) //Stop Condition
    {
        //Step 2: for each input vector, do steps 3-5
        PreProcessorObj.GoFirstInputOutputVectors();
        RecordsNumber=0;
        while (PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
        {
            MinJ=FindClusterSOM(InputVector);
	    
            //Determine neighbors of unit MinJ based on NeighborhoodType and selected radius
            NeighborNumber=DetermineNeighbors(MinJ, Neighbor, R);
            if (NeighborNumber==-1) return 0;

            //Step 5: For all units j within a specified neighborhood of MinJ, and all i update their weights
            for (j=0; j<NeighborNumber; j++)
                for (i=0; i<Layer[0]; i++)
                    Weight[BU][i][Neighbor[j]]=Weight[BU][i][Neighbor[j]] + LR*(InputVector[i]-Weight[BU][i][Neighbor[j]]);
	    
            RecordsNumber++;
            if (RecordsNumber>=200)
            {
                if (ShowProgressWindow(1)==CANCEL) break;
                RecordsNumber=0;
             }
        }//end of while (PreProcessorObj.Get...)

        //Step 6: Update learning rate
        LR = LR * LRDec;
	
        //Step 7: Reduce radius of topological neighborhood at specified times
        if ( ((Epochs+1) % int(ceil(float(MaxEpochs) / float(Radius+1))) ) ==0 && R>0) R--;
	
        ProgressState=ShowProgressWindow(2, ((Epochs+1)*100) / MaxEpochs );
        if (ProgressState)
            if (ProgressState==STOP) break;
	else if (ProgressState==CANCEL) {delete[] Neighbor;  return 0;}
	
        Epochs++;
    }//end of while. Step 8: Test stop condition
    
    PreProcessorObj.GoFirstInputOutputVectors();
    while (PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
    {
        MinJ=FindClusterSOM(InputVector);
        BMUFreq[MinJ][ExpectedOutput]++;
        if (ClusterDistance[MinJ] < Dist[MinJ]) ClusterDistance[MinJ]=Dist[MinJ];
     }
    
    //Cluster label of ech unit is that, the frequency of that type are more than other
    for (i=0; i<Layer[1]; i++)
    {
        SumBMU=0;
        MaxJ=0;
        for (j=0; j<ATTACK_NUMBER; j++)
        {
            if ( BMUFreq[i][j] > BMUFreq[i][MaxJ] ) MaxJ=j;
            SumBMU+=BMUFreq[i][j];
        }
        if (MaxJ) 
        {
            ClusteringMap[i]=MaxJ;
            ClusteringMapAD[i]= (100*BMUFreq[i][MaxJ]) / SumBMU;
        }
    }//end of for (i=0,...)
    
    delete[] Neighbor;
    return 1;
}//End of function NeuralNet::TrainSOM


//#################        FUNCTION TRAIN ART1       ####################
int NeuralNet::TrainART1()
{
    int i, j, MaxJ, Epochs, RecordsNumber, ProgressState, SumBMU;
    
    //Step 0: Initialize parameters
    Epochs=0;
    
    //Step 1: While stopping condition is false
    while (Epochs<MaxEpochs) //Sop Condition
    {
        //Step 2: For each training input, do steps 3-12
        PreProcessorObj.GoFirstInputOutputVectors();
        RecordsNumber=0;
        while ( PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
        {
            MaxJ=FindClusterART1(InputVector);
            if (MaxJ!=CNC)
            {
                //Step 12: Update the weights for note MaxJ (fast learning)
                for (i=0; i<Layer[0]; i++)
                {
                    Weight[BU][i][MaxJ]=(L*X[i]) / (L-1+NormX);
                    Weight[TD][MaxJ][i]=X[i];
                }
            }//end of if (MaxJ!=-1)
	    
            RecordsNumber++;
            if (RecordsNumber>=400)
            {
                if (ShowProgressWindow(1)) break;
                RecordsNumber=0;
             }
        }//end of while (PreProcessorObj.Get...)
      
        ProgressState=ShowProgressWindow(2, ((Epochs+1)*100) / MaxEpochs );
        if (ProgressState)
            if (ProgressState==STOP) break;
                else if (ProgressState==CANCEL) return 0;
		
        Epochs++;
    //Step 13: Test for stopping condition
    }//end of while (!StopCondition)

    //Generating Clustering Map after end of training
    PreProcessorObj.GoFirstInputOutputVectors();
    RecordsNumber=0;
    while ( PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
    {
        MaxJ=FindClusterART1(InputVector);
        if (MaxJ!=CNC) //if Input Vector clustered
            BMUFreq[MaxJ][ExpectedOutput]++;
	    
    }//end of while (PreProcessorObj.Get...)
    
    //Cluster label of ech unit is that, the frequency of that type are more than other
    for (i=0; i<Layer[1]; i++)
    {
        SumBMU=0;
        MaxJ=0;
        for (j=0; j<ATTACK_NUMBER; j++)
        {
            if ( BMUFreq[i][j] > BMUFreq[i][MaxJ] ) MaxJ=j;
            SumBMU+=BMUFreq[i][j];
        }
        if (MaxJ) 
        {
            ClusteringMap[i]=MaxJ;
            ClusteringMapAD[i]= (100*BMUFreq[i][MaxJ]) / SumBMU;
        }
    }//end of for (i=0,...)
    
    return 1;
}//End of function NeuralNet::TrainART1


//#################        FUNCTION TRAIN ART2       ####################
int NeuralNet::TrainART2()
{
    int i, j, Epochs, Iteration, MaxJ, RecordsNumber, ProgressState, SumBMU;
    float NormW, NormP, NormV;
    
    //Step 0: Initialize parameters
    Epochs=0;
    
    //Step 1: Do steps 2-12 MaxEpochs epochs times
    while(Epochs<MaxEpochs) //Stop Condition
    {
	
        //Step 2: For each input vector, do steps 3-11
        PreProcessorObj.GoFirstInputOutputVectors();
        RecordsNumber=0;
        while ( PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType) )
        {
            MaxJ=FindClusterART2(InputVector);
            if (MaxJ!=CNC)
            {
	    
                //Do Steps 9-11 LearningIteration times
                for (Iteration=0; Iteration<LearningIteration; Iteration++)
                {
                    //Step 9: Update weights for wining unit MaxJ
                    for (i=0; i<Layer[0]; i++)
                    {
                        Weight[TD][MaxJ][i]=LearningRate*D*U[i] + (1+LearningRate*D*(D-1))*Weight[TD][MaxJ][i];
                        Weight[BU][i][MaxJ]=LearningRate*D*U[i] + (1+LearningRate*D*(D-1))*Weight[BU][i][MaxJ];
                    }//end of for Layer[0]
		
                    //Step 10: Update F1 activations
                    NormV=Norm(V,Layer[0]);
                    for (i=0; i<Layer[0]; i++)
                    {
                        U[i]=V[i] / (E+NormV);
                        W[i]=InputVector[i] + A*U[i];
                        P[i]=U[i] + D*Weight[TD][MaxJ][i];
                    }//end of for Layer[0]
                    NormW=Norm(W,Layer[0]);
                    NormP=Norm(P,Layer[0]);
                    for (i=0; i<Layer[0]; i++)
                    {
                        X[i]=W[i] / (E+NormW);
                        Q[i]=P[i] / (E+NormP);
                        V[i]=ActFunc(X[i]) + B*ActFunc(Q[i]);
                    }//end of for Layer[0]
                }//end of for (Iteration...)
            }//end of if (IsClustered)
	    
            RecordsNumber++;
            if (RecordsNumber>=500)
            {
                if (ShowProgressWindow(1)) break;
                RecordsNumber=0;
             }
        }//end of while (PreProcesorObj.Get...)
	
        ProgressState=ShowProgressWindow(2, ((Epochs+1)*100) / MaxEpochs );
        if (ProgressState)
            if (ProgressState==STOP) break;
	else if (ProgressState==CANCEL) return 0;
		
        Epochs++;
    }//end of while (Stop condition)
    
    //Generating Clustering Map after end of training    
    PreProcessorObj.GoFirstInputOutputVectors();
    RecordsNumber=0;
    while ( PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType) )
    {
        MaxJ=FindClusterART2(InputVector);
        if (MaxJ!=CNC) //if Input Vector clustered
            BMUFreq[MaxJ][ExpectedOutput]++;
    }//end of while (PreProcessorObj.Get...)
    
    //Cluster label of ech unit is that, the frequency of that type are more than other
    for (i=0; i<Layer[1]; i++)
    {
        SumBMU=0;
        MaxJ=0;
        for (j=0; j<ATTACK_NUMBER; j++)
        {
            if ( BMUFreq[i][j] > BMUFreq[i][MaxJ] ) MaxJ=j;
            SumBMU+=BMUFreq[i][j];
        }
        if (MaxJ) 
        {
            ClusteringMap[i]=MaxJ;
            ClusteringMapAD[i]= (100*BMUFreq[i][MaxJ]) / SumBMU;
        }
    }//end of for (i=0,...)
    
    return 1;
}//End of function NeuralNet::TrainART2


//#################        FUNCTION FIND CLUSTER SOM       ####################
int NeuralNet::FindClusterSOM(float *Vector)
{
    int i, j, MinJ;
    
    MinJ=0;
    for (j=0; j<Layer[1]; j++)
    {
        Dist[j]=0;
        for (i=0; i<Layer[0]; i++)
            Dist[j]=Dist[j]+pow((Weight[BU][i][j]-Vector[i]),2);
        if (Dist[j]<Dist[MinJ]) MinJ=j;
    }//end of for j
    
    return MinJ;
}//End of function NeuralNey::FindClusterSOM


//#################        FUNCTION FIND CLUSTER ART1       ####################
int NeuralNet::FindClusterART1(float *Vector)
{
    int i, j, MaxJ;
    float NormS=0;
    
    //Step 3: Set activations of all F2 units to zero
    for (j=0; j<Layer[1]; j++)  Y[j]=0;

    //Step 4: Compute the norm of input vector S

    //Step 5: Send input signal from F1(a) to the F1(b) layer
    NormS=0;
    for (i=0; i<Layer[0]; i++)
    {
        NormS+=Vector[i];
        X[i]=Vector[i];
    }//end of for Layer[0];

    //Step 6: For each F2 node that is not inhibited
    for (j=0; j<Layer[1]; j++)
        if (Y[j]!=-1)
            for (i=0; i<Layer[0]; i++)
                Y[j]+=Weight[BU][i][j]*X[i];

    //Step 7: while reset is true, do teps 8-11
    do{
        //Step 8: Find MaxJ such that Y[MaxJ]>=Y[j]
        MaxJ=0;
        for (j=0; j<Layer[1]; j++)
            if (Y[MaxJ]<Y[j])  MaxJ=j;
		
        if (Y[MaxJ]==-1)  //The pattern can not be clustered
            return -1;

        //Step 9: Recompute activation X of F1(b)
        for (i=0; i<Layer[0]; i++)
            X[i]=Vector[i]*Weight[TD][MaxJ][i];

        //Step 10: Compute the norm of vector X
        for (NormX=0, i=0; i<Layer[0]; i++) NormX+=X[i];

        //Step 11: Test for reset
        if (NormX/NormS < Vigilance)  Y[MaxJ]=-1;
    }while(NormX/NormS < Vigilance);
    
    return MaxJ;
}//End of function NeuralNet::FindClusterART1


//#################        FUNCTION FIND CLUSTER ART2       ####################
int NeuralNet::FindClusterART2(float *Vector)
{
    int i, j, MaxJ;
    float NormS, NormW, NormP, NormV, NormU, NormR;
    
    NormS=Norm (Vector,Layer[0]);

    //Step 3: Update F1 unit activations
    for (i=0; i<Layer[0]; i++)
    {
        U[i]=0;
        W[i]=Vector[i];
        P[i]=0;
        X[i]=(Vector[i] / (E+NormS));
        Q[i]=0;
        V[i]=ActFunc(X[i]);
    }//end of for Layer[0]

    //Update F1 unit activations again
    NormV=Norm(V,Layer[0]);
    for (i=0; i<Layer[0]; i++)
    {
        U[i]=V[i] / (E+NormV);
        W[i]=Vector[i] + A*U[i];
        P[i]=U[i];
    }//end of for Layer[0]
    NormW=Norm(W,Layer[0]);
    NormP=Norm(P,Layer[0]);
    for (i=0; i<Layer[0]; i++)
    {
        X[i]=W[i] / (E+NormW);
        Q[i]=P[i] / (E+NormP);
        V[i]=ActFunc(X[i]) + B*ActFunc(Q[i]);
    }//end of for Layer[0]

    //Step 4: Compute signals to F2 units
    for (j=0; j<Layer[1]; j++)
    {
        Y[j]=0;
        for (i=0; i<Layer[0]; i++)
            Y[j]+= Weight[BU][i][j]*P[i];
    }//end of for Layer[1]

    //Step 5: While reset is true, do steps 6-7
    do{
        //Step 6: Find F2 unit Y[j] with largest signal
        MaxJ=0;
        for (j=0; j<Layer[1]; j++)
            if (Y[MaxJ]<Y[j])  MaxJ=j;
        if (Y[MaxJ]==-1)
            return CNC; //The pattern can not be clustered

        //Step 7: Check for reset
        NormV=Norm(V,Layer[0]);
        for (i=0; i<Layer[0]; i++)
        {
            U[i]=V[i] / (E+NormV);
            P[i]=U[i] + D*Weight[TD][MaxJ][i];
        }//end of for Layer[0]
        NormU=Norm(U,Layer[0]);
        NormP=Norm(P,Layer[0]);
        for (i=0; i<Layer[0]; i++)
            R[i]=(U[i]+C*P[i]) / (E+NormU+C*NormP);

        NormR=Norm(R,Layer[0]);
        if (NormR < (Vigilance-E))
            Y[MaxJ]=-1;
        else
        {
            for (i=0; i<Layer[0]; i++)
                W[i]=Vector[i] + A*U[i];
                        
            NormW=Norm(W,Layer[0]);
            for (i=0; i<Layer[0]; i++)
            {
                X[i]=W[i] / (E+NormW);
                Q[i]=P[i] / (E+NormP);
                V[i]=ActFunc(X[i]) + B*ActFunc(Q[i]);
            }//end of for Layer[0]
        }//end of else
    }while (NormR<(Vigilance-E)); //end of do

 //   printf ("Connection Status -> Node=(%d) : IDS Output=(%d)\n", MaxJ, ClusteringMap[MaxJ]);
    return MaxJ;
}//End of function NeuralNey::FindClusterART2


//#################        FUNCTION NORM       ####################
float NeuralNet::Norm(float *NormVector, int NormVectorLen)
{
    int i;
    float Sum=0;
    
    for (i=0; i< NormVectorLen; i++)
        Sum = Sum + NormVector[i] * NormVector[i];
   
    return sqrt(Sum);
}//End of function NeuralNet::Norm


//#################        FUNCTION ACT FUNC       ####################
float NeuralNet::ActFunc(float X)
{
    //Calculate Activation function of neurons in ART2
    if (X<Noise) return 0;
    return X;
}//End of function NeuralNet::ActFunc


//#################        FUNCTION TEST SOM       ####################
int NeuralNet::TestSOM()
{
    float AssuranceDegree;
    int i, RecordCounter, TotalRecords, ProgressState, NeuralNetAnalyzerOutput;
    FILE *TRFPointer;
    bool FirstItem;
    
    if ( !(TRFPointer=fopen(TestResultFileName, "w") ) )  
    {
        perror ("Error: NeuralNet.TestSOM : Can not creat Test Result File !");
        return 0;
    }

    //Saving Known Attacks (the attacks that cooperate in training phase) in TestResult file
    for (i=0, FirstItem=true; i<ATTACK_NUMBER; i++) 
        if (KnownAttack[i]) 
        {
            if (!FirstItem) fprintf (TRFPointer, ",");
            else FirstItem=false;
            fprintf (TRFPointer, "%d", i);
        }
    fprintf (TRFPointer, "\n");
    
    //Begin from first vector of training vectors
    RecordCounter=0;
    TotalRecords=PreProcessorObj.GoFirstInputOutputVectors();
    while (PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
    {
        RecordCounter++;
        NeuralNetAnalyzerOutput=ApplySOM(InputVector, AssuranceDegree);
        switch ( NeuralNetAnalyzerOutput )
        {
            case NOT_CLUSTERED: 
                fprintf (TRFPointer, "%d \t-> %d \tNew Unknown Attack with %3.2f%% Assurance Degree\n", RecordCounter, UNKNOWN_ATTACK, AssuranceDegree);
                break;
		
            case NORMAL:
                fprintf (TRFPointer, "%d \t-> %d \tNormal with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AssuranceDegree);
                break;
		
            default:  //Attack
                fprintf (TRFPointer, "%d \t-> %d \t%s with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AttackDB[NeuralNetAnalyzerOutput].Desc, AssuranceDegree);
        }//end of switch
	
        ProgressState=ShowProgressWindow(2, (RecordCounter*100)/TotalRecords);
        if (ProgressState)
            if (ProgressState==STOP) break;
                else if (ProgressState==CANCEL) return 0;

    }//end of while (PreProcessorObj.Get...)
    
    fclose(TRFPointer);
    return 1;
}//End of function NeuralNet::TestSOM


//#################        FUNCTION TEST ART1       ####################
int NeuralNet::TestART1()
{
    float AssuranceDegree;
    int i, RecordCounter, TotalRecords, ProgressState, NeuralNetAnalyzerOutput;
    FILE *TRFPointer;
    bool FirstItem;
    
    if ( !(TRFPointer=fopen(TestResultFileName, "w") ) )  
    {
        perror ("Error: NeuralNet.TestART1: Can not creat Test Result File !");
        return 0;
    }

    //Saving Known Attacks (the attacks that cooperate in training phase) in TestResult file
    for (i=0, FirstItem=true; i<ATTACK_NUMBER; i++) 
        if (KnownAttack[i]) 
        {
            if (!FirstItem) fprintf (TRFPointer, ",");
            else FirstItem=false;
            fprintf (TRFPointer, "%d", i);
        }
    fprintf (TRFPointer, "\n");
    
    //Begin from first vector of training vectors
    RecordCounter=0;
    TotalRecords=PreProcessorObj.GoFirstInputOutputVectors();
    while (PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
    {
        RecordCounter++;
        NeuralNetAnalyzerOutput=ApplyART1(InputVector, AssuranceDegree);
        switch ( NeuralNetAnalyzerOutput )
        {
            case CNC: 
                fprintf (TRFPointer, "%d \t-> \tCan Not Cluster\n", RecordCounter);
                break;
		
            case NOT_CLUSTERED: 
                fprintf (TRFPointer, "%d \t-> %d \tNew Unknown Attack with %3.2f%% Assurance Degree\n", RecordCounter, UNKNOWN_ATTACK, AssuranceDegree);
                break;
		
            case NORMAL:
                fprintf (TRFPointer, "%d \t-> %d \tNormal with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AssuranceDegree);
                break;
		
            default:  //Attack
                fprintf (TRFPointer, "%d \t-> %d \t%s with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AttackDB[NeuralNetAnalyzerOutput].Desc, AssuranceDegree);
        }//end of switch
	
        ProgressState=ShowProgressWindow(2, (RecordCounter*100)/TotalRecords);
        if (ProgressState)
            if (ProgressState==STOP) break;
                else if (ProgressState==CANCEL) return 0;

    }//end of while (PreProcessorObj.Get...)
    
    fclose(TRFPointer);
    return 1;
}//End of function NeuralNet::TestART1


//#################        FUNCTION TEST ART2       ####################
int NeuralNet::TestART2()
{
    float AssuranceDegree;
    int i, RecordCounter, TotalRecords, ProgressState, NeuralNetAnalyzerOutput;
    FILE *TRFPointer;
    bool FirstItem;
    
    if ( !(TRFPointer=fopen(TestResultFileName, "w") ) )  
    {
        perror ("Error: NeuralNet.TestART1: Can not creat Test Result File !");
        return 0;
    }
    
    //Saving Known Attacks (the attacks that cooperate in training phase) in TestResult file
    for (i=0, FirstItem=true; i<ATTACK_NUMBER; i++) 
        if (KnownAttack[i]) 
        {
            if (!FirstItem) fprintf (TRFPointer, ",");
            else FirstItem=false;
            fprintf (TRFPointer, "%d", i);
        }
    fprintf (TRFPointer, "\n");

    //Begin from first vector of training vectors
    RecordCounter=0;
    TotalRecords=PreProcessorObj.GoFirstInputOutputVectors();
    while (PreProcessorObj.GetNextInputOutputVectors(InputVector, ExpectedOutput, InputDataType))
    {
        RecordCounter++;
        NeuralNetAnalyzerOutput=ApplyART2(InputVector, AssuranceDegree);
        switch ( NeuralNetAnalyzerOutput )
        {
            case CNC: 
                fprintf (TRFPointer, "%d \t-> \tCan Not Cluster\n", RecordCounter);
                break;
		
            case NOT_CLUSTERED: 
                fprintf (TRFPointer, "%d \t-> %d \tNew Unknown Attack with %3.2f%% Assurance Degree\n", RecordCounter, UNKNOWN_ATTACK, AssuranceDegree);
                break;
		
            case NORMAL:
                fprintf (TRFPointer, "%d \t-> %d \tNormal with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AssuranceDegree);
                break;
		
            default:  //Attack
                fprintf (TRFPointer, "%d \t-> %d \t%s with %3.2f%% Assurance Degree\n", RecordCounter, NeuralNetAnalyzerOutput, AttackDB[NeuralNetAnalyzerOutput].Desc, AssuranceDegree);
        }//end of switch
	
        ProgressState=ShowProgressWindow(2, (RecordCounter*100)/TotalRecords);
        if (ProgressState)
            if (ProgressState==STOP) break;
                else if (ProgressState==CANCEL) return 0;
    }//end of while (PreProcessorObj.Get...)
    
    fclose(TRFPointer);
    return 1;
}//End of function NeuralNet::TestART2


//#################        FUNCTION APPLY SOM       ####################
int NeuralNet::ApplySOM(float *Vector, float &AssuranceDegree)
{
    int ClusterUnit;
    
    ClusterUnit=FindClusterSOM(Vector);
    if (ClusteringMap[ClusterUnit]==NORMAL && Dist[ClusterUnit] <= (ClusterDistance[ClusterUnit]+0.001)) return NORMAL;
    AssuranceDegree=ClusteringMapAD[ClusterUnit];
    return ClusteringMap[ClusterUnit];

}//End of function NeuralNet::ApplySOM


//#################        FUNCTION APPLY ART1       ####################
int NeuralNet::ApplyART1(float *Vector, float &AssuranceDegree)
{
    int ClusterUnit;
    
    ClusterUnit=FindClusterART1(Vector);
    if (ClusterUnit==CNC) return CNC;
    
    AssuranceDegree=ClusteringMapAD[ClusterUnit];
    return ClusteringMap[ClusterUnit];
}//End of function NeuralNet::ApplyART1


//#################        FUNCTION APPLY ART2       ####################
int NeuralNet::ApplyART2(float *Vector, float &AssuranceDegree)
{
    int ClusterUnit;
    
    ClusterUnit=FindClusterART2(Vector);
    if (ClusterUnit==CNC) return CNC;
    
    AssuranceDegree=ClusteringMapAD[ClusterUnit];
    return ClusteringMap[ClusterUnit];
}//End of function NeuralNet::ApplyART2


//#################        FUNCTION DETERMINE NEIGHBORS       ####################
int NeuralNet::DetermineNeighbors(int k, int *Neighbor, int r)
{
    int i, j, a, b, nn, dim;
    nn=0;
    dim=int (sqrt(Layer[1]) );
    switch (NeighborhoodType)
    {
        case LINEAR:
            for (i=k-r; i<=k+r; i++)
                if (i>=0 && i<Layer[1])
                    Neighbor[nn++]=i;
	return nn;
	
        case RECTANGULAR:
            for (i=(k % dim)-r; i<=(k % dim)+r; i++)
                for (j=(k / dim)-r; j<=(k / dim)+r; j++)
                    if ( i>=0 && i<dim && j>=0 && j<dim )
                        Neighbor[nn++]=j*dim+i;
            return nn;
        
        case HEXAGONAL:
            a=k % dim;
            b=k / dim;
            for (j=b-r; j<=b+r; j++)
                if ( (b % 2)==0 )
                {
                    for (i=a-r+(abs(j-b))/2; i<=a-r+(abs(j-b))/2+2*r-abs(j-b); i++)
                        if (i>=0 && i<dim && j>=0 && j<dim)
                            Neighbor[nn++]=j*dim+i;
                }
	    else 
                {
                    for (i=a-r+(abs(j-b)+1)/2; i<=a-r+(abs(j-b)+1)/2+2*r-abs(j-b); i++)
                        if (i>=0 && i<dim && j>=0 && j<dim)
                            Neighbor[nn++]=j*dim+i;
                }
                return nn;    
    }//end of switch
    return -1;
}//End of function NeuralNet::DetermineNeighbors


//#################        FUNCTION LOAD WEIGHTS AND MAP       ####################
int NeuralNet::LoadWeightsAndMap()
{
    int retval=1;
    char WeightsNetModel[512];
    FILE *WMFPointer;
    
    if ( !(WMFPointer=fopen(WeightMapFileName, "r")) ) return 0;
    fgets(WeightsNetModel, 80, WMFPointer);
    
    switch (NetModel)
    {
        case SOM:
	if ( strncasecmp (WeightsNetModel, "SOM Weights:", 12) ) retval= 0;
            else if ( !LoadWeightsAndMapSOM(WMFPointer) ) retval=0;
            break;    
        case ART1:
	if ( strncasecmp (WeightsNetModel, "ART1 Weights:", 13) ) retval= 0;
            else if ( !LoadWeightsAndMapART1(WMFPointer) ) retval=0;
            break;    
        case ART2:
	if ( strncasecmp (WeightsNetModel, "ART2 Weights:", 13) ) retval= 0;
            else if ( !LoadWeightsAndMapART2(WMFPointer) ) retval=0;
            break;    
    }//end of switch
    
    fclose (WMFPointer);
    return retval;
}//End of function NeuralNet::LoadWeightsAndMap


//#################        FUNCTION LOAD WEIGHTS AND MAP  SOM      ####################
int NeuralNet::LoadWeightsAndMapSOM(FILE *WMFPointer)
{
    int i, j, k;
    float w, d, ad;
    char MapNetModel[512];       
    
    for (k=0; k<Layer[0]*Layer[1] && !feof(WMFPointer); k++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f\n", &i, &j, &w) ) return 0;
        if (i>=Layer[0] || j>=Layer[1]) return 0;
        else Weight[BU][i][j]=w;
    }// enf of while
    
    fgets(MapNetModel, 80, WMFPointer);
    if ( strncasecmp (MapNetModel, "SOM Clustering Map:", 19) ) return 0;
    
    for (k=0; k<Layer[1] && !feof(WMFPointer); k++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f %f\n", &i, &j, &ad, &d) ) return 0;
	if (i>=Layer[1]) return 0;
        ClusteringMap[i]=j;
        ClusteringMapAD[i]=ad;
        ClusterDistance[i]=d;
        KnownAttack[j]=true;
    }
        
    return 1;
}//End of function NeuralNet::LoadWeightsAndMapSOM


//#################        FUNCTION LOAD WEIGHTS AND MAP ART1      ####################
int NeuralNet::LoadWeightsAndMapART1(FILE *WMFPointer)
{
    int i, j, k;
    float wbu, wtd, ad;
    char MapNetModel[512];   
    
    for (k=0; k<Layer[0]*Layer[1] && !feof(WMFPointer); k++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f %f\n", &i, &j, &wbu, &wtd) ) return 0;
        if (i>=Layer[0] || j>=Layer[1]) return 0;
        else 
        {
            Weight[BU][i][j]=wbu;
            Weight[TD][j][i]=wtd;
        }
    }// enf of while
    
    fgets(MapNetModel, 80, WMFPointer);
    if ( strncasecmp (MapNetModel, "ART1 Clustering Map:", 20) ) return 0;
    
    for (k=0; k<Layer[1] && !feof(WMFPointer); i++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f\n", &i, &j, &ad) ) return 0;
        if (i>=Layer[1]) return 0;
        else 
        {
            ClusteringMap[i]=j;
            ClusteringMapAD[i]=ad;
            KnownAttack[j]=true;
        }
    }    
    
    return 1;
}//End of function NeuralNet::LoadWeightsAndMapART1


//#################        FUNCTION LOAD WEIGHTS AND MAP ART2      ####################
int NeuralNet::LoadWeightsAndMapART2(FILE *WMFPointer)
{
    int i, j, k;
    float wbu, wtd, ad;
    char MapNetModel[512];   
    
    for (k=0; k<Layer[0]*Layer[1] && !feof(WMFPointer); k++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f %f\n", &i, &j, &wbu, &wtd) ) return 0;
        if (i>=Layer[0] || j>=Layer[1]) return 0;
        else 
        {
            Weight[BU][i][j]=wbu;
            Weight[TD][j][i]=wtd;
        }
    }// enf of while
    
    fgets(MapNetModel, 80, WMFPointer);
    if ( strncasecmp (MapNetModel, "ART2 Clustering Map:", 20) ) return 0;
    
    for (k=0; k<Layer[1] && !feof(WMFPointer); i++)
    {
        if ( !fscanf (WMFPointer, "%d %d %f\n", &i, &j, &ad) ) return 0;
        if (i>=Layer[1]) return 0;
        else 
        {
            ClusteringMap[i]=j;
            ClusteringMapAD[i]=ad;
            KnownAttack[j]=true;
        }
    }   
    
    return 1;
}//End of function NeuralNet::LoadWeightsAndMapART2


//#################        FUNCTION SAVE WEIGHTS AND MAP      ####################
int NeuralNet::SaveWeightsAndMap()
{
    int retval=1;
    FILE *WMFPointer;
    
    if ( !(WMFPointer=fopen(WeightMapFileName, "w") ) )  return 0;

    switch (NetModel)
    {
        case SOM:
	if ( !SaveWeightsAndMapSOM(WMFPointer) ) retval=0;
              break;
        case ART1:
	if ( !SaveWeightsAndMapART1(WMFPointer) ) retval=0;
	break;
        case ART2:
	if ( !SaveWeightsAndMapART2(WMFPointer) ) retval=0;
            break;
    }//end of switch
    
    fclose (WMFPointer);
    return retval;
}//End of function NeuralNet::SaveWeightsAndMap


//#################        FUNCTION SAVE WEIGHTS AND MAP SOM       ####################
int NeuralNet::SaveWeightsAndMapSOM(FILE *WMFPointer)
{
    int i, j;
    
    fprintf (WMFPointer, "SOM Weights:\n");
    for (i=0; i<Layer[0]; i++)
        for (j=0; j<Layer[1]; j++)
            if (fprintf (WMFPointer, "%d %d %2.30f\n", i, j, Weight[BU][i][j])==-1 ) return 0;
    
    fprintf (WMFPointer, "SOM Clustering Map:\n");
    for (i=0; i<Layer[1]; i++)
        if (fprintf (WMFPointer, "%d %d %3.2f %2.30f\n", i, ClusteringMap[i], ClusteringMapAD[i], ClusterDistance[i])==-1 ) return 0;
    
    return 1;
}//End of unction NeuralNet::SaveWeightsAndMapSOM


//################        FUNCTION SAVE WEIGHTS AND MAP ART1       ####################
int NeuralNet::SaveWeightsAndMapART1(FILE *WMFPointer)
{
    int i, j;
    
    fprintf (WMFPointer, "ART1 Weights:\n");
    for (i=0; i<Layer[0]; i++)
        for (j=0; j<Layer[1]; j++)
            if ( fprintf (WMFPointer, "%d %d %2.30f %2.30f\n", i, j, Weight[BU][i][j], Weight[TD][j][i])==-1 ) return 0;
    
    fprintf (WMFPointer, "ART1 Clustering Map:\n");
    for (i=0; i<Layer[1]; i++)
        if (fprintf (WMFPointer, "%d %d %3.2f\n", i, ClusteringMap[i], ClusteringMapAD[i])==-1 ) return 0;
    
    return 1;
}//End of unction NeuralNet::SaveWeightsAndMapART1


//#################        FUNCTION SAVE WEIGHTS AND MAP ART2       ####################
int NeuralNet::SaveWeightsAndMapART2(FILE *WMFPointer)
{
    int i, j;
    
    fprintf (WMFPointer, "ART2 Weights:\n");
    for (i=0; i<Layer[0]; i++)
        for (j=0; j<Layer[1]; j++)
            if ( fprintf (WMFPointer, "%d %d %2.30f %2.30f\n", i, j, Weight[BU][i][j], Weight[TD][j][i])==-1 ) return 0;
    
    fprintf (WMFPointer, "ART2 Clustering Map:\n");
    for (i=0; i<Layer[1]; i++)
        if (fprintf (WMFPointer, "%d %d %3.2f\n", i, ClusteringMap[i], ClusteringMapAD[i])==-1 ) return 0;
    
    return 1;
}//End of unction NeuralNet::SaveWeightsAndMapART2


//#################        FUNCTION GET WORK SPACE PARAMETERS       ####################
void NeuralNet::GetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format: "<NetModel> , <InputDataType> , <WeightMapFileName> , <TestResultFileName> , <MaxEpochs> , <Layer[1]> , <LearningRate> , <LRDec> , <Radius>  , <NeighborhoodType> , <Vigilance> , <L> , <LearningIteration> , <Noise> , <A> , <B> , <C> , <E>"
    
    sprintf (ParamString, "%d,%d,%s,%s,%d,%d,%2.5f,%2.5f,%d,%d,%2.5f,%2.5f,%d,%2.5f,%2.5f,%2.5f,%2.5f,%2.5f,%2.5f", 
	     NetModel, 
	     InputDataType,
	     WeightMapFileName, 
	     TestResultFileName, 
	     MaxEpochs,
	     Layer[1], 
	     LearningRate, 
	     LRDec, 
	     Radius, 
	     NeighborhoodType,
	     Vigilance,
	     L,
	     LearningIteration,
	     Noise,
	     A, B, C, D, E);
}//End of function NeuralNet::GetWorkSpaceParameters


//#################        FUNCTION SET WORK SPACE PARAMETERS       ####################
void NeuralNet::SetWorkSpaceParameters(char *ParamString)
{
    //ParamString Format: "<NetModel> , <InputDataType> , <WeightMapFileName> , <TestResultFileName> , <MaxEpochs> , <Layer[1]> , <LearningRate> , <LRDec> , <Radius> , <NeighborhoodType> , <Vigilance> , <L> , <LearningIteration> , <Noise> , <A> , <B> , <C> , <E>"
    
    int i, j;
    char StrTemp[512];
    
    if ( !strncasecmp(ParamString,"null",4)) 
    {
        NetModel=0;
        InputDataType=0;
        strcpy(WeightMapFileName,"");
        strcpy(TestResultFileName,"");
        MaxEpochs=0;
        Layer[1]=0;
        LearningRate=0;
        LRDec=0;
        Radius=0;
        NeighborhoodType=0;
        Vigilance=0;
        L=0;
        LearningIteration=0;
        Noise=0;
        A=B=C=D=E=0;
        return;
    }//end of if
    
    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    NetModel=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    InputDataType=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy (WeightMapFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    strcpy (TestResultFileName, StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    MaxEpochs=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    Layer[1]=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    LearningRate=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    LRDec=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    Radius=atoi(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    NeighborhoodType=atoi(StrTemp);

    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    Vigilance=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    L=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    LearningIteration=atoi(StrTemp);

    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    Noise=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    A=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    B=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    C=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    D=atof(StrTemp);
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' && ParamString[i]!='\r'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    E=atof(StrTemp);
}//End of function NeuralNet::SetWorkSpaceParameters


//#################        FUNCTION SET INPUT VECTOR LEN      ####################
void NeuralNet::SetInputVectorLen(int TCPDUMP_BinaryLen, int TCPDUMP_NonBinaryLen, int KDDCUP_BinaryLen, int KDDCUP_NonBinaryLen)
{
    InputVectorBinaryLen[TCPDUMP]=TCPDUMP_BinaryLen;
    InputVectorNonBinaryLen[TCPDUMP]=TCPDUMP_NonBinaryLen;
    
    InputVectorBinaryLen[KDDCUP]=KDDCUP_BinaryLen;
    InputVectorNonBinaryLen[KDDCUP]=KDDCUP_NonBinaryLen;
}//End of function NeuralNet::SetInputVectorLen


//#################        FUNCTION GET TEST RESULT FILE NAME      ####################
void NeuralNet::GetTestResultFileName(char *ReturnName)
{
    strcpy (ReturnName, TestResultFileName);
}//End of function NeurlaNet::GetTestResultFileName


//#################        FUNCTION SHOW TRAIN PROGRESS      ####################
int NeuralNet::ShowProgressWindow(int State, int Progress, int Action)
{
    switch (State)
    {
        case 0:
            frmProgressWindowObj=new frmProgressWindow;
            if (Action==TRAIN) 
                frmProgressWindowObj->setCaptions(TRAIN);
            else 
                frmProgressWindowObj->setCaptions(TEST);
		
            frmProgressWindowObj->show();
            frmProgressWindowObj->pgbTrainTest->setProgress(0);
            qApp->processEvents();
            break;
     
        case 1:
            qApp->processEvents();
            return frmProgressWindowObj->getProgressState();
            break;
	    
        case 2:
            frmProgressWindowObj->pgbTrainTest->setProgress( Progress );
            qApp->processEvents();
            return frmProgressWindowObj->getProgressState();
            break;
	    
        case 3:
            delete frmProgressWindowObj;
    }//end of switch
    
    return 0;
}//End of function NeruralNet::ShowProgressWindow


//#################        FUNCTION NEURAL NET ATTACK DETECTION      ####################
void NeuralNet::NeuralNetAttackDetection(char *NetPacketRecord,float *InputVector)
{
    int AnalyzerOutput;
    float AnalyzerAssuranceDegree;
    
    AnalyzerOutput=ApplyNet(InputVector, AnalyzerAssuranceDegree);
    if (AnalyzerOutput != NORMAL)
        ResponderObj.GiveNeuralNetAnalyzerOutput(AnalyzerOutput, AnalyzerAssuranceDegree, NetPacketRecord);
}//End of funcion NeuralNet::NeuralNetAttackDetection


//#################        FUNCTION GET INPUT DATA      ####################
int NeuralNet::GetInputDataType()
{
    return InputDataType;
}//End of function NeuralNet::GetInputDataType
