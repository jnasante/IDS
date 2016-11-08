/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qmessagebox.h>
#include "ManagementControl.h"

extern ManagementControl ManagementControlObj;

//#################        FUNCTION INIT       ####################
void frmIdsEvaluator::init()
{
    char EvaluationResultsStr[1024];
    
    txtEvaluationResults->setText("Please Wait...");
    
    if (ManagementControlObj.EvaluateIDS(EvaluationResultsStr))
        txtEvaluationResults->setText(QString(EvaluationResultsStr));
    else 
    {
        QMessageBox::critical (this, "NNIDS", "Can not evaluate results of IDS test.\nAre you sure doing evaluation after test !", QMessageBox::Ok, 0);
        txtEvaluationResults->setText("Error in evaluation...");
    }
}//End of function frmIdsEvaluator::init
