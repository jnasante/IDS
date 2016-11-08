/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <string.h>
#include <qfiledialog.h>
#include "ManagementControl.h"
#include "TcpDumpPacketProvider.h"
#include "TrainTestDataProvider.h"

extern ManagementControl ManagementControlObj;
extern TcpDumpPacketProvider TcpDumpPacketProviderObj;
extern TrainTestDataProvider TrainTestDataProviderObj;


//#################        FUNCTION INIT       ####################
void frmDataProvidersSetting::init()
{
    //ParamString Format for TcpDumpPacketProvider: <NetAddr> , <NetMask> , <Interface> , <Filter> , <OffLineTrafficSourceName>
    //ParamString Format for TrainTestDataProvider: <InputDataFormat> , <TrainFileName> , <TrainSessionFileName> , <TestFileName> , <TestSessionFileName> 
    
    int i, j;
    char ParamString[512], 
           StrTemp[512];
    unsigned int NetAddr, NetMask;
    
    //Read TcpDumpPacketProvider current setting
    TcpDumpPacketProviderObj.GetWorkSpaceParameters(ParamString);
    
    for (i=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n' ; i++, j++)
        StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    NetAddr=atoi(StrTemp);
    sprintf (StrTemp, "%d.%d.%d.%d", 
	     (NetAddr & 0x000000FF),
	     (NetAddr & 0x0000FF00)>>8, 
	     (NetAddr & 0x00FF0000)>>16, 
	     NetAddr>>24);
    txtNetAddr->setText(StrTemp);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n' ; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    NetMask=atoi(StrTemp);
    sprintf (StrTemp, "%d.%d.%d.%d", 
	     (NetMask & 0x000000FF),
	     (NetMask & 0x0000FF00)>>8, 
	     (NetMask & 0x00FF0000)>>16, 
	     NetMask>>24 );
    txtNetMask->setText(StrTemp);

    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n' ; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtNetworkDevice->setText(StrTemp);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n' ; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    if ( !strncasecmp(StrTemp, "IP",2) )
        cmbPacketFilter->setCurrentItem(1);
    else if ( !strncasecmp(StrTemp, "TCP",3) )
        cmbPacketFilter->setCurrentItem(2);
    else if ( !strncasecmp(StrTemp, "UDP",3) )
        cmbPacketFilter->setCurrentItem(3);
    else if ( !strncasecmp(StrTemp, "ICMP",4) )
        cmbPacketFilter->setCurrentItem(4);
    else 
        cmbPacketFilter->setCurrentItem(0);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n'; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtApplyFile->setText(StrTemp);
    
    //Read TrainTestDataProvider current setting
    TrainTestDataProviderObj.GetWorkSpaceParameters(ParamString);
    
    for (i=0; ParamString[i] && ParamString[i]!=','; i++)
        StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    if (StrTemp[0]=='0')
    {
        cmbInputDataFormat->setCurrentItem(0);
        cmbInputDataFormat_activated(0);
    }
    else
    {
        cmbInputDataFormat->setCurrentItem(1);
        cmbInputDataFormat_activated(1);
    }
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n'; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtTrainFile->setText(StrTemp);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n'; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtTrainSessionFile->setText(StrTemp);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n'; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtTestFile->setText(StrTemp);
    
    for (i++, j=0; ParamString[i] && ParamString[i]!=',' && ParamString[i]!='\n'; i++, j++)
        StrTemp[j]=ParamString[i];
    StrTemp[j]='\0';
    txtTestSessionFile->setText(StrTemp);
    
}//End of function frmDataProvidersSetting::init


//#################        FUNCTION SET DATA PROVIDERS PARAMETERS       ####################
void frmDataProvidersSetting::SetDataProvidersParameters()
{
    //ParamString Format for TcpDumpPacketProvider: <Interface> , <Filter> , <OffLineTrafficSourceName>
    //ParamString Format for TrainTestDataProvider: <InputDataFormat> , <TrainFileName> , <TrainSessionFileName> , <TestFileName> , <TestSessionFileName>
    
    int i, j;    
    char ParamString[512], StrTemp[50];
    unsigned int NetAddr, NetMask;
    unsigned char TempByte;
    char NetAddrStr[50], NetMaskStr[50];
    
   
    //Write TcpDumpPacketProvider new setting
    strcpy(NetAddrStr, txtNetAddr->text().latin1());
    for (i=0; NetAddrStr[i]!='.' && NetAddrStr[i]!='\0'; i++) StrTemp[i]=NetAddrStr[i];
    StrTemp[i]='\0';
    TempByte=atoi(StrTemp);
    NetAddr=TempByte;// NetAddr=NetAddr<<8;
    
    for (j=0, i++; NetAddrStr[i]!='.' && NetAddrStr[i]!='\0'; i++, j++) StrTemp[j]=NetAddrStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetAddr=NetAddr | (TempByte<<8); //NetAddr=NetAddr<<8;
    
    for (j=0, i++; NetAddrStr[i]!='.' && NetAddrStr[i]!='\0'; i++, j++) StrTemp[j]=NetAddrStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetAddr=NetAddr | (TempByte<<16); //NetAddr=NetAddr<<8;
    
    for (j=0, i++; NetAddrStr[i]!='.' && NetAddrStr[i]!='\0'; i++, j++) StrTemp[j]=NetAddrStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetAddr=NetAddr | (TempByte<<24);
    
    strcpy(NetMaskStr, txtNetMask->text().latin1());
    for (i=0; NetMaskStr[i]!='.' && NetMaskStr[i]!='\0'; i++, j++) StrTemp[i]=NetMaskStr[i];
    StrTemp[i]='\0';
    TempByte=atoi(StrTemp);
    NetMask=TempByte; //NetMask=NetMask<<8;
    
    for (j=0, i++; NetMaskStr[i]!='.' && NetMaskStr[i]!='\0'; i++, j++) StrTemp[j]=NetMaskStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetMask=NetMask | (TempByte<<8); //NetMask=NetMask<<8;
    
    for (j=0, i++; NetMaskStr[i]!='.' && NetMaskStr[i]!='\0'; i++, j++) StrTemp[j]=NetMaskStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetMask=NetMask | (TempByte<<16); //NetMask=NetMask<<8;
    
    for (j=0, i++; NetMaskStr[i]!='.' && NetMaskStr[i]!='\0'; i++, j++) StrTemp[j]=NetMaskStr[i];
    StrTemp[j]='\0';
    TempByte=atoi(StrTemp);
    NetMask=NetMask | (TempByte<<24);
        
    sprintf(ParamString, "%d,%d,%s,%s,%s",
	    NetAddr, NetMask,
	    txtNetworkDevice->text().latin1(),
	    (cmbPacketFilter->text (cmbPacketFilter->currentItem() ).lower()).latin1(),
	    txtApplyFile->text().latin1() );
    
    TcpDumpPacketProviderObj.SetWorkSpaceParameters(ParamString);
    
    //Write TrainTestDataProvider new setting
    if ( cmbInputDataFormat->currentItem()==0)
        strcpy (ParamString, "0,");
    else 
        strcpy (ParamString, "1,");
    strcat(ParamString, txtTrainFile->text().latin1());
    strcat(ParamString, ",");
    strcat(ParamString, txtTrainSessionFile->text().latin1());
    strcat(ParamString, ",");
    strcat(ParamString, txtTestFile->text().latin1());
    strcat(ParamString, ",");
    strcat(ParamString, txtTestSessionFile->text().latin1());
    
    TrainTestDataProviderObj.SetWorkSpaceParameters(ParamString);
    
    ManagementControlObj.SetWorkSpaceModified();
}//End of slot frmDataProvidersSetting::SetDataProvidersParameters


//#################        FUNCTION BTM OPEN TRAIN FILE CLICKED       ####################
void frmDataProvidersSetting::btmOpenTrainFile_clicked()
{
    QString fn( QFileDialog::getOpenFileName(QString::null,"Network Traffic Audit Files (*.tcpdump *.kddcup)\n""All Files (*)", this, QString::null, "Open Train File" ) );
    if (!fn.isEmpty()) txtTrainFile->setText(fn);

}//End of slot frmDataProviderSetting::btmOpenTrainFile_clicked


//#################        FUNCTION BTM OPEN TEST FILE CLICKED       ####################
void frmDataProvidersSetting::btmOpenTestFile_clicked()
{
    QString fn( QFileDialog::getOpenFileName(QString::null,"Network Traffic Audit Files (*.tcpdump *.kddcup)\n""All Files (*)", this, QString::null, "Open Test File" ) );
    if (!fn.isEmpty()) txtTestFile->setText(fn);
}//End of slot frmDataProvidersSetting::btmOpenTestFile_clicked


//#################        FUNCTION BTM OPEN APPLY FILE CLICKED       ####################
void frmDataProvidersSetting::btmOpenApplyFile_clicked()
{
    QString fn( QFileDialog::getOpenFileName(QString::null,"Network Traffic Audit Files (*.tcpdump)\n""All Files (*)", this, QString::null, "Open IDS Data Source File" ) );
    if (!fn.isEmpty()) txtApplyFile->setText(fn);
}//End of slot frmDataProvidersSetting::btmOpenApplyFile_clicked


//#################        FUNCTION BTM OPEN TRAIN SESSION FILE CLICKED       ####################
void frmDataProvidersSetting::btmOpenTrainSessionFile_clicked()
{
    QString fn( QFileDialog::getOpenFileName(QString::null,"Network Traffic Sessions List Files (*.list)\n""All Files (*)", this, QString::null, "Open Train Sessions List File" ) );
    if (!fn.isEmpty()) txtTrainSessionFile->setText(fn);

}


//#################        FUNCTION BTM OPEN TEST SESSION FILE CLICKED       ####################
void frmDataProvidersSetting::btmOpenTestSessionFile_clicked()
{
    QString fn( QFileDialog::getOpenFileName(QString::null,"Network Traffic Sessions List Files (*.list)\n""All Files (*)", this, QString::null, "Open Test Sessions List File" ) );
    if (!fn.isEmpty()) txtTestSessionFile->setText(fn);

}


//#################        FUNCTION CMB INPUT DATA FORMAT ACTIVATED       ####################
void frmDataProvidersSetting::cmbInputDataFormat_activated( int InputDataFormat )
{
    switch (InputDataFormat)
    {
        case 0:
            lblTrainSessionFile->show();
            txtTrainSessionFile->show();
            btmOpenTrainSessionFile->show();
            lblTestSessionFile->show();
            txtTestSessionFile->show();
            btmOpenTestSessionFile->show();
            break;
	    
        case 1:
            lblTrainSessionFile->hide();
            txtTrainSessionFile->hide();
            btmOpenTrainSessionFile->hide();
            lblTestSessionFile->hide();
            txtTestSessionFile->hide();
            btmOpenTestSessionFile->hide();
           
    }//end of switch

}//End of function frmDataProvidersSetting::cmbInputDataFormat_activated
