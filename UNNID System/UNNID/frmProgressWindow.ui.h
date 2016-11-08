/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#define TRAIN 0
#define TEST  1

#define ACTIVE 0
#define STOP 1
#define CANCEL 2


//#################        FUNCTION BTM STOP CLICKED       ####################
void frmProgressWindow::btmStop_clicked()
{
    if (ProgressState==ACTIVE)  ProgressState=STOP;
}//End of slot frmProgressWindow::btmStop_clicked


//#################        FUNCTION BTM CANCEL CLICKED       ####################
void frmProgressWindow::btmCancel_clicked()
{
    if (ProgressState==ACTIVE)  ProgressState=CANCEL;
}//End of slot frmProgressWindow::btmCancel_clicked


//#################        FUNCTION GET PROGRESS STATE       ####################
int frmProgressWindow::getProgressState()
{
    return ProgressState;
}//End of function frmProgressWindow::isStopped


//#################        FUNCTION FRM PROGRESS WINDOW       ####################
void frmProgressWindow::init()
{
    ProgressState=ACTIVE;
}//End of constructor frmProgressWindow::init


//#################        FUNCTION SET CAPTIONS       ####################
void frmProgressWindow::setCaptions(int Action)
{
    switch (Action)
    {
        case TRAIN:
            this->setCaption("Training Neural Net based Analyser . . .");
            btmStop->setText("&Stop Train");
            btmCancel->setText("&Cancel Train");
            break;	    
	    
        case TEST:
            this->setCaption("Testing Neural Net based Analyser . . .");
            btmStop->setText("&Stop Test");
            btmCancel->setText("&Cancel Test");
            break;
    }//end of switch
}//End of function frmProgressWindow::setCaptions


//#################        FUNCTION DESTROY       ####################
void frmProgressWindow::destroy()
{
    btmCancel_clicked();
}//End of function frmProgressWindow::destroy
