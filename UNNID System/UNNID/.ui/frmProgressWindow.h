/****************************************************************************
** Form interface generated from reading ui file 'frmProgressWindow.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMPROGRESSWINDOW_H
#define FRMPROGRESSWINDOW_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QProgressBar;
class QPushButton;

class frmProgressWindow : public QDialog
{
    Q_OBJECT

public:
    frmProgressWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmProgressWindow();

    QPushButton* btmStop;
    QPushButton* btmCancel;
    QProgressBar* pgbTrainTest;

    virtual int getProgressState();

public slots:
    virtual void btmStop_clicked();
    virtual void btmCancel_clicked();
    virtual void setCaptions( int Action );

protected:

protected slots:
    virtual void languageChange();
private:
    int ProgressState;

    void init();
    void destroy();

};

#endif // FRMPROGRESSWINDOW_H
