/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include <qfiledialog.h> 
#include "ManagementControl.h"
#include "NeuralNet.h"

extern ManagementControl ManagementControlObj;
extern NeuralNet NeuralNetObj;


//#################        FUNCTION SET NEURAL NET PARAMETERS       ####################
void frmNeuralNetSetting::SetNeuralNetParameters()
{
    //ParamString Format: "<NetModel> , <InputDataType> , <WeightMapFileName> , <TestResultFileName> , <MaxEpochs> , <Layer[1]> , <LearningRate> , <LRDec> , <Radius> , <NeighborhoodType> , <Vigilance> , <L> , <LearningIteration> , <Noise> , <A> , <B> , <C> , <D> , <E>"
    
    char ParamString[512];
   
    sprintf (ParamString, "%d,%d,%s,%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
	     cmbNeuralNetModel->currentItem(), //NetModel
	     cmbInputDataType->currentItem(), //InputDataType
	     (txtWeightMapFile->text()).latin1(), //WeightMapFileName
	     (txtTestResultFile->text()).latin1(), //TestResultFileName
	     (spnMaxEpochs->text()).latin1(), //MaxEpochs
	     (spnClusterUnit->text()).latin1(), //Layer[1]
	     ((cmbNeuralNetModel->currentItem()==0) ? ((txtLearningRateSom->text()).latin1()) : ((txtLearningRateArt2->text()).latin1())), //LearningRate
	     (txtLrDec->text()).latin1(), //LRDec
	     (spnRadius->text()).latin1(), //Radius
	     cmbNeighborhoodType->currentItem(), //NeighborhoodType
	     ((cmbNeuralNetModel->currentItem()==1) ? ((txtVigilance1->text()).latin1()) : ((txtVigilance2->text()).latin1())), //Vigilance
	     (txtL->text()).latin1(), //L
	     (spnLearningIteration->text()).latin1(), //LearningIteration
	     (txtNoise->text()).latin1(), //Noise
	     (txtA->text()).latin1(), //A
	     (txtB->text()).latin1(), //B
	     (txtC->text()).latin1(), //C
	     (txtD->text()).latin1(), //D
	     (txtE->text()).latin1()); //E
    
    NeuralNetObj.SetWorkSpaceParameters(ParamString);
    ManagementControlObj.SetWorkSpaceModified();
}//End of function frmNeuralNetSetting::SetNeuralNetParameters


//#################        FUNCTION BTM OPEN WEIGHT MAP FILE CLICKED       ####################
void frmNeuralNetSetting::btmOpenWeightMapFile_clicked()
{
    QString fn = QFileDialog::getSaveFileName("", "Neural Network Weight Map Files (*.nwm)\n""All Files (*.*)", this, "", "Select Weight Map File");
    if (!fn.isEmpty()) txtWeightMapFile->setText(fn);
}//End of function frmNeuralNetSetting::btmOpenWeightMapFile_clicked


//#################        FUNCTION BTM OPEN TEST RESULT FILE CLICKED       ####################
void frmNeuralNetSetting::btmOpenTestResultFile_clicked()
{
    QString fn = QFileDialog::getSaveFileName("", "Neural Network Test Results Files (*.ntr)\n""All Files (*.*)", this, "", "Select Test Results File");
    if (!fn.isEmpty()) txtTestResultFile->setText(fn);
}//End of slot frmNeuralNetSetting::btmOpenTestResultFile_clicked


//#################        FUNCTION INIT       ####################
void frmNeuralNetSetting::init()
{
    //ParamString Format: "<NetModel> , <InputDataType> , <WeightMapFileName> , <TestResultFileName> , <MaxEpochs> , <Layer[1]> , <LearningRate> , <LRDec> , <Radius> , <NeighborhoodType> , <Vigilance> , <L> , <LearningIteration> , <Noise> , <A> , <B> , <C> , <D> , <E>"
    
    int i, j;
    char ParamString[512];
    char StrTemp[512];

    NeuralNetObj.GetWorkSpaceParameters(ParamString);
    
    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    cmbNeuralNetModel->setCurrentItem(atoi(StrTemp));
    cmbNeuralNetModel_activated(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    cmbInputDataType->setCurrentItem(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtWeightMapFile->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtTestResultFile->setText(QString(StrTemp));

    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    spnMaxEpochs->setValue(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    spnClusterUnit->setValue(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtLearningRateSom->setText(QString(StrTemp));
    txtLearningRateArt2->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtLrDec->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    spnRadius->setValue(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    cmbNeighborhoodType->setCurrentItem(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtVigilance1->setText(QString(StrTemp));
    txtVigilance2->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtL->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    spnLearningIteration->setValue(atoi(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtNoise->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtA->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtB->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtC->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtD->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0' && ParamString[i]!='\n' && ParamString[i]!='\r'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtE->setText(QString(StrTemp));
}//End of function frmNeuralNetSetting::init


//#################        FUNCTION CMB NEURAL NET MODEL ACTIVATED       ####################
void frmNeuralNetSetting::cmbNeuralNetModel_activated(int NeuralNetModel)
{
    switch (NeuralNetModel)
    {
        case 0: //SOM
            grpSomParam->show();
            grpArt1Param->hide();
            grpArt2Param->hide();
            cmbInputDataType->setEnabled(true);
            break;
        case 1: //ART1
            grpSomParam->hide();
            grpArt1Param->show();
            grpArt2Param->hide();
            cmbInputDataType->setCurrentItem(0);
            cmbInputDataType->setEnabled(false);
            break;
        case 2: //ART2
            grpSomParam->hide();
            grpArt1Param->hide();
            grpArt2Param->show();
            cmbInputDataType->setEnabled(true);
    }//end of switch

}//End of function frmNeuralNetSetting::cmbNeuralNetModel_activated
