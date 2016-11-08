/***************************************************************************
                          NeuralNet.h  -  description
                             -------------------
    begin                : Sun Apr 27 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#ifndef NEURALNET_H
#define NEURALNET_H

#define LINEAR      0
#define RECTANGULAR 1
#define HEXAGONAL   2

#define SOM  0
#define ART1 1
#define ART2 2

#define NOTRAINED 0
#define TRAINED    1

#define TRAIN 0
#define TEST  1
#define APPLY 2

#define CNC -1
#define NOT_CLUSTERED 1

#define OFF 0
#define ON  1

#define BU 0 //Bottom-Up
#define TD 1 //Top-Down

#define BINARY     0
#define NORMALIZED 1
#define REAL       2

#define STOP 1
#define CANCEL 2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <qapplication.h>
#include <qprogressbar.h>
#include "frmProgressWindow.h"


class NeuralNet
{
    public:
        NeuralNet();
        ~NeuralNet();
        void  GetWorkSpaceParameters(char *);
        void  SetWorkSpaceParameters(char *);
        void  SetInputVectorLen(int ,int ,int ,int );
        void  GetTestResultFileName(char *);

        int   InitializeNet(bool );
        int   TrainNet();
        int   TestNet();
        void NeuralNetAttackDetection(char *,float *);

        int   FreeNet();
        int   GetInputDataType();
	
    private:
        frmProgressWindow *frmProgressWindowObj;
        char WeightMapFileName[512];
        char TestResultFileName[512];
        bool NetMemoryAllocated,
               *KnownAttack; //Known attacks is determined based on attacks exist in WeightMap file and is saved in TestResult file
        int   NetModel,          //Net Model can be SOM, ART1 and ART2
              InputDataType,  //Input Data Type can be Binary or Continuous
              NetState,         //Net State can be TRAINED and NOTRAINED
              Layer[2],         //Number of neurons in each layer in SOM, ART1 and ART2
              NeighborhoodType, //Used in SOM that can be LINEAR, RECTANGULAR and HEXAGONAL
              MaxEpochs,        //Maximum Epochs in training 
              LearningIteration,    //Learning Iteration in updating weights in ART2
              **BMUFreq,          //Best Match Unit Frequency for SOM, ART1 and ART2	
              *ClusteringMap,  //Clustering Map for SOM, ART1 and ART2
              InputVectorBinaryLen[2], //For determining Layer[0] sensors number in SOM, ART1 and ART2
              InputVectorNonBinaryLen[2], //For determining Layer[0] sensors number in SOM, ART1 and ART2
              ExpectedOutput; //Expected neural net output in SOM, ART1 and ART2

        float LearningRate,     //Learning Rate in SOM
              LRDec;            //Learning Rate Decrement factor in SOM
        int   Radius;           //Radius of neighboring in SOM
        float *Dist,
	*ClusterDistance;            //Distance vector

        float ** Weight[2],   //Weights of last layer connections in SOM, ART1 and ART2
              *InputVector,     //Input Vector in SOM, ART1 and ART2
              *ClusteringMapAD; //Clustering Map Assurance Degree for SOM, ART1, ART2

        float Vigilance,        //Vigilance parameter in ART1 and ART2
              *X,               //Neurons of F1 layer in ART1 and ART2
              *Y,               //Neurons of F2 layer in ART1 and ART2
              NormX;

        float L,      //ART1 Parameter, range: L>1, default: L=2
              Noise,  //Noise suppression parameter in ART2, default: Noise=1/sqrt(Layer[1])
              A,      //ART2 parameter (U[i] to W[i] fix weights), default: A=10
              B,      //ART2 parameter (Q[i] to V[i] fix weights), default: B=10
              C,      //ART2 parameter (P[i] to Reset node fix weights), default: C=0.1
              D,      //ART2 parameter (activation of winning F2 unit), default: D=0.9
              E,      //ART2 parameter (prevent devision by zero), default: E=0.01
                      //(C*D/(1-D)) must be less than or equal to 1
              *W,     //Neurons of F1 layer in ART2
              *P,     //Neurons of F1 layer in ART2
              *Q,     //Neurons of F1 layer in ART2
              *U,     //Neurons of F1 layer in ART2
              *V,     //Neurons of F1 layer in ART2
              *R;    //Reset Neurons in ART2
	
        int   InitializeSOM(bool );
        int   InitializeART1(bool );
        int   InitializeART2(bool );
	
        int   TrainSOM();
        int   TrainART1();
        int   TrainART2();
        
        int   FindClusterSOM(float *);
        int   FindClusterART1(float *);
        int   FindClusterART2(float *);
	
        int   TestSOM();
        int   TestART1();
        int   TestART2();
	
        int   ApplyNet(float *, float&);
        int   ApplySOM(float *, float&);
        int   ApplyART1(float *, float&);
        int   ApplyART2(float *, float&);
	
        int   DetermineNeighbors(int, int *, int);
        int   LoadWeightsAndMap();
        int   LoadWeightsAndMapSOM(FILE *);
        int   LoadWeightsAndMapART1(FILE *);
        int   LoadWeightsAndMapART2(FILE *);
	
        int   SaveWeightsAndMap();
        int   SaveWeightsAndMapSOM(FILE *);
        int   SaveWeightsAndMapART1(FILE *);
        int   SaveWeightsAndMapART2(FILE *);

        float Norm(float *, int);
        float ActFunc(float);

        int ShowProgressWindow(int, int =0, int =TRAIN);
};

#endif
