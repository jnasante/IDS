/****************************************************************************
** Form interface generated from reading ui file 'frmNeuralNetSetting.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMNEURALNETSETTING_H
#define FRMNEURALNETSETTING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QToolButton;

class frmNeuralNetSetting : public QDialog
{
    Q_OBJECT

public:
    frmNeuralNetSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmNeuralNetSetting();

    QPushButton* btmOk;
    QPushButton* btmApply;
    QPushButton* btmCancel;
    QLabel* lblNeuralNetModel;
    QLabel* lblInputDataType;
    QLabel* lblWeightMapFile;
    QLabel* lblTestResultFile;
    QLabel* lblMaxEpochs;
    QLabel* lblClusterNumber;
    QComboBox* cmbNeuralNetModel;
    QSpinBox* spnMaxEpochs;
    QSpinBox* spnClusterUnit;
    QComboBox* cmbInputDataType;
    QToolButton* btmOpenTestResultFile;
    QToolButton* btmOpenWeightMapFile;
    QLineEdit* txtWeightMapFile;
    QLineEdit* txtTestResultFile;
    QGroupBox* grpArt2Param;
    QLabel* lblLearningRateArt2;
    QLabel* lblLearningIteration;
    QLabel* lblVigilance2;
    QLabel* lblNoise;
    QLabel* lblA;
    QLabel* lblB;
    QLabel* lblC;
    QLabel* lblD;
    QLabel* lblE;
    QLineEdit* txtLearningRateArt2;
    QSpinBox* spnLearningIteration;
    QLineEdit* txtVigilance2;
    QLineEdit* txtNoise;
    QLineEdit* txtA;
    QLineEdit* txtB;
    QLineEdit* txtC;
    QLineEdit* txtD;
    QLineEdit* txtE;
    QGroupBox* grpArt1Param;
    QLabel* lblVigilance1;
    QLineEdit* txtVigilance1;
    QLabel* lblL;
    QLineEdit* txtL;
    QGroupBox* grpSomParam;
    QComboBox* cmbNeighborhoodType;
    QLabel* lblNeighborType;
    QLabel* lblLrDec;
    QLabel* lblLearningRateSom;
    QLabel* lblRadius;
    QLineEdit* txtLearningRateSom;
    QSpinBox* spnRadius;
    QLineEdit* txtLrDec;

public slots:
    virtual void SetNeuralNetParameters();
    virtual void btmOpenWeightMapFile_clicked();
    virtual void btmOpenTestResultFile_clicked();
    virtual void init();
    virtual void cmbNeuralNetModel_activated( int NeuralNetModel );

protected:
    QVBoxLayout* layout19;
    QVBoxLayout* layout8;
    QVBoxLayout* layout18;
    QVBoxLayout* layout9;
    QVBoxLayout* layout19_2;
    QGridLayout* layout17;
    QVBoxLayout* layout11;
    QVBoxLayout* layout11_2;
    QVBoxLayout* layout10;

protected slots:
    virtual void languageChange();
};

#endif // FRMNEURALNETSETTING_H
