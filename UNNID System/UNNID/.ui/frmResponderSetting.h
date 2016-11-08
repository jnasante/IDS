/****************************************************************************
** Form interface generated from reading ui file 'frmResponderSetting.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMRESPONDERSETTING_H
#define FRMRESPONDERSETTING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class frmResponderSetting : public QDialog
{
    Q_OBJECT

public:
    frmResponderSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmResponderSetting();

    QLabel* lblLogFile;
    QLineEdit* txtLogFile;
    QToolButton* btmSaveLogFile;
    QCheckBox* chkLogging;
    QPushButton* btmCancel;
    QPushButton* btmApply;
    QPushButton* btmOk;
    QCheckBox* chkMessageAlarming;
    QCheckBox* chkBeepFlashAlarming;

public slots:
    virtual void SetResponderParameters();
    virtual void btmSaveLogFile_clicked();
    virtual void chkLogging_stateChanged( int State );

protected:

protected slots:
    virtual void languageChange();
private:
    void init();

};

#endif // FRMRESPONDERSETTING_H
