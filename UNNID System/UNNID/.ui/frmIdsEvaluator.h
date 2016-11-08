/****************************************************************************
** Form interface generated from reading ui file 'frmIdsEvaluator.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMIDSEVALUATOR_H
#define FRMIDSEVALUATOR_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;
class QTextEdit;

class frmIdsEvaluator : public QDialog
{
    Q_OBJECT

public:
    frmIdsEvaluator( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmIdsEvaluator();

    QTextEdit* txtEvaluationResults;
    QLabel* lblEvaluationResults;
    QPushButton* btmOk;

protected:

protected slots:
    virtual void languageChange();
private:
    void init();

};

#endif // FRMIDSEVALUATOR_H
