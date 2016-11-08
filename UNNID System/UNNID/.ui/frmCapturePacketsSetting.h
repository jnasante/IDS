/****************************************************************************
** Form interface generated from reading ui file 'frmCapturePacketsSetting.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMCAPTUREPACKETSSETTING_H
#define FRMCAPTUREPACKETSSETTING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class frmCapturePacketsSetting : public QDialog
{
    Q_OBJECT

public:
    frmCapturePacketsSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmCapturePacketsSetting();

    QLabel* lblPacketFilter;
    QLabel* lblApplyFile;
    QComboBox* cmbPacketFilter;
    QLineEdit* txtDumpFile;
    QToolButton* btmSaveDumpFile;
    QPushButton* btmCapture;
    QPushButton* btmCancel;

public slots:
    virtual void btmSaveDumpFile_clicked();
    virtual void btmCapture_clicked();

protected:

protected slots:
    virtual void languageChange();
};

#endif // FRMCAPTUREPACKETSSETTING_H
