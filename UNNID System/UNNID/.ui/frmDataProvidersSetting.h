/****************************************************************************
** Form interface generated from reading ui file 'frmDataProvidersSetting.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMDATAPROVIDERSSETTING_H
#define FRMDATAPROVIDERSSETTING_H

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
class QTabWidget;
class QToolButton;
class QWidget;

class frmDataProvidersSetting : public QDialog
{
    Q_OBJECT

public:
    frmDataProvidersSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmDataProvidersSetting();

    QLabel* lblFeatureExtraction;
    QComboBox* cmbInputDataFormat;
    QPushButton* btmCancel;
    QPushButton* btmApply;
    QPushButton* btmOk;
    QTabWidget* tabDataProviders;
    QWidget* tab;
    QLabel* lblNetAddress;
    QLabel* textLabel3;
    QLabel* lblNetMask;
    QLabel* textLabel4;
    QLineEdit* txtNetAddr;
    QLineEdit* txtNetMask;
    QWidget* tab_2;
    QLabel* lblTrainFile;
    QLabel* lblTrainSessionFile;
    QToolButton* btmOpenTrainFile;
    QToolButton* btmOpenTrainSessionFile;
    QLineEdit* txtTrainFile;
    QLineEdit* txtTrainSessionFile;
    QWidget* tab_3;
    QLabel* lblTestFile;
    QLabel* lblTestSessionFile;
    QToolButton* btmOpenTestFile;
    QToolButton* btmOpenTestSessionFile;
    QLineEdit* txtTestFile;
    QLineEdit* txtTestSessionFile;
    QWidget* tab_4;
    QLabel* lblApplyFile;
    QToolButton* btmOpenApplyFile;
    QLineEdit* txtApplyFile;
    QWidget* tab_5;
    QGroupBox* grpSnifferSetting;
    QLabel* lblPacketFilter;
    QLabel* lblNetworkDevice;
    QLabel* textLabel4_2;
    QLineEdit* txtNetworkDevice;
    QComboBox* cmbPacketFilter;

public slots:
    virtual void SetDataProvidersParameters();
    virtual void btmOpenTrainFile_clicked();
    virtual void btmOpenTestFile_clicked();
    virtual void btmOpenApplyFile_clicked();
    virtual void btmOpenTrainSessionFile_clicked();
    virtual void btmOpenTestSessionFile_clicked();
    virtual void cmbInputDataFormat_activated( int InputDataFormat );

protected:

protected slots:
    virtual void languageChange();
private:
    void init();

};

#endif // FRMDATAPROVIDERSSETTING_H
