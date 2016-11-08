/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include <string.h>
#include "ManagementControl.h"
#include "Sniffer.h"

extern ManagementControl ManagementControlObj;
extern Sniffer SnifferObj;

void frmSnifferSetting::SetSnifferParameters()
{
    char ParamString[100];
    
    strcpy(ParamString, txtNetworkDevice->text().latin1());
    strcat(ParamString, ",");
    strcat(ParamString, (cmbFilter->text (cmbFilter->currentItem() ).lower()).latin1() );
    
    SnifferObj.SetWorkSpaceParameters(ParamString);
    ManagementControlObj.SetWorkSpaceModified();
}

void frmSnifferSetting::init()
{
    int i, j;
    char ParamString[100], StrTemp[100];
    
    SnifferObj.GetWorkSpaceParameters(ParamString);
    
    for (i=0; ParamString[i] && ParamString[i]!=','; i++)
        StrTemp[i]=ParamString[i];
    StrTemp[i]='\0';
    txtNetworkDevice->setText(StrTemp);
    
    if (ParamString[i])
    {
        for (i++, j=0; ParamString[i]; i++, j++)
            StrTemp[j]=ParamString[i];
        StrTemp[j]='\0';
        if ( !strncasecmp(StrTemp, "IP",2) )
            cmbFilter->setCurrentItem(1);
        else if ( !strncasecmp(StrTemp, "TCP",3) )
            cmbFilter->setCurrentItem(2);
        else if ( !strncasecmp(StrTemp, "UDP",3) )
            cmbFilter->setCurrentItem(3);
        else if ( !strncasecmp(StrTemp, "ICMP",4) )
            cmbFilter->setCurrentItem(4);
        else 
            cmbFilter->setCurrentItem(0);
    }//end of if (ParamString...)   
}
