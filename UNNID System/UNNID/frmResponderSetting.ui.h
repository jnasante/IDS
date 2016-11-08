/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qfiledialog.h> 
#include "ManagementControl.h"
#include "Responder.h"

extern ManagementControl ManagementControlObj;
extern Responder ResponderObj;


//#################        FUNCTION INIT       ####################
void frmResponderSetting::init()
{
    //Param String Format: "<Logging> , <LogFileName> , <MessageOnScreenAlarming> , <BeepFlashAlarming>"
    int i,  j, TempVal;
    char StrTemp[512];
    char ParamString[512];
    
    ResponderObj.GetWorkSpaceParameters(ParamString);
    
    for (i=0; ParamString[i]!=',' && ParamString[i]!='\0'; i++) StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    TempVal=atoi(StrTemp);
    if (!TempVal) chkLogging->setChecked(false);
    else chkLogging->setChecked(true);
    chkLogging_stateChanged(TempVal);
	    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtLogFile->setText(QString(StrTemp));
    
    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    if (StrTemp[0]=='0') chkMessageAlarming->setChecked(false);
    else chkMessageAlarming->setChecked(true);

    for (j=0, i++; ParamString[i]!=',' && ParamString[i]!='\0'; i++, j++) StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    if (StrTemp[0]=='0') chkBeepFlashAlarming->setChecked(false);
    else chkBeepFlashAlarming->setChecked(true);


}//End of function  frmResponderSetting::init


//#################        FUNCTION SET RESPONDER PARAMETERS       ####################
void frmResponderSetting::SetResponderParameters()
{
    //Param String Format: "<Logging> , <LogFileName> , <MessageOnScreenAlarming> , <BeepFlashAlarming>"
    char ParamString[512];
    
    sprintf(ParamString, "%c,%s,%c,%c",
	    chkLogging->isChecked() ? '1' : '0',
	    (txtLogFile->text()).latin1(),
	    chkMessageAlarming->isChecked() ? '1' : '0',
	    chkBeepFlashAlarming->isChecked() ? '1' : '0' );
    
/*    if ( chkLogging->isChecked() ) 
        strcpy(ParamString,"1"); else strcpy(ParamString,"0");
    strcat(ParamString, (txtLogFile->text()).latin1() );
    
    if ( chkAlarming->isChecked() ) 
        strcat(ParamString,",1,"); else strcat(ParamString,",0,");
  */  
    ResponderObj.SetWorkSpaceParameters(ParamString);
    ManagementControlObj.SetWorkSpaceModified();

}//End of function frmResponderSetting::SetResponderParameters


//#################        FUNCTION BTM SAVE LOG FILE CLICKED       ####################
void frmResponderSetting::btmSaveLogFile_clicked()
{
    QString fn = QFileDialog::getSaveFileName("", "UNNID Log File (*.unnid)\n""All Files (*.*)", this, "", "Select Weight Map File");
    if (!fn.isEmpty()) txtLogFile->setText(fn);
}//End of function frmResponderSetting::btmSaveLogFile_clicked


//#################        FUNCTION CHK LOGGING STATE CHANGED       ####################
void frmResponderSetting::chkLogging_stateChanged( int State)
{
    if (State) 
    {
        lblLogFile->setEnabled(true);
        txtLogFile->setEnabled(true);    
        btmSaveLogFile->setEnabled(true);
    }
    else 
    {
        lblLogFile->setEnabled(false);
        txtLogFile->setEnabled(false);    
        btmSaveLogFile->setEnabled(false);
    }
}//End of function frmResponderSetting::chkLogging_stateChanged
