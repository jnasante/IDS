/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <string.h>
#include <qmessagebox.h> 
#include <qfiledialog.h>
#include "ManagementControl.h"
extern ManagementControl ManagementControlObj;

void frmCapturePacketsSetting::btmSaveDumpFile_clicked()
{
    QString fn = QFileDialog::getSaveFileName("", "Network Packets Dump Files (*.tcpdump)\n""All Files (*.*)", this, "", "Save Dump File");
    if (!fn.isEmpty()) txtDumpFile->setText(fn);
}



void frmCapturePacketsSetting::btmCapture_clicked()
{
    char ParamString[512];
    
    if (cmbPacketFilter->currentItem()) 
        strcpy (ParamString, (cmbPacketFilter->text (cmbPacketFilter->currentItem() ).lower()).latin1() );
    else 
        strcpy (ParamString, " ");
    strcat (ParamString, ",");
    strcat (ParamString, txtDumpFile->text().latin1());
    
    if (ManagementControlObj.StartCapturePackets( ParamString) > 0 ) 
    {
        QMessageBox::information (this, "UNNID", "Capturing Network Packets started successfully.\nPlease Wait...\n\nPress Cancel to terminate Capturing.", QMessageBox::Cancel);
        ManagementControlObj.StopCapturePackets();
    }
    else 
        QMessageBox::critical (this, "UNNID", "Can not start Capturing Network Packets !", QMessageBox::Ok, 0);   
}
