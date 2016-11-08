/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
****************************************************************************/
#include <string.h>
#include "ManagementControl.h"
#include "PreProcessor.h"

extern ManagementControl ManagementControlObj;
extern PreProcessor PreProcessorObj;


//#################        FUNCTION SET PRE PROCESSOR PARAMETERS       ####################
void frmPreProcessorSetting::SetPreProcessorParameters()
{
    char ParamString[512];
    
    /************************************Connection Features***************************************/
    if ( chkDuration->isChecked() ) 
        strcpy(ParamString,"1"); else strcpy(ParamString,"0");
    if ( chkProtocolType->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkService->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkFlag->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkSrcBytes->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstBytes->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkLand->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkWrongFragment->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkUrgent->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkHot->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumFailedLogins->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkLoggedIn->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumCompromised->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkRootShell->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkSuAttempted->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumRoot->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumFileCreations->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumShells->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumAccessFiles->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkNumOutboundCmds->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIsHotLogin->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIsGuestLogin->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkCount->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkServiceCount->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkSynErrorRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkServiceSynErrorRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkRejErrorRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkServiceRejErrorRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkSameServiceRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDiffServiceRate->isChecked() ) 
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkServiceDiffHostRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostCount->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSrvCount->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSameSrvRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostDiffSrvRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSameSrcPortRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSrvDiffHostRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSynErrorRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSrvSynErrorRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostRejErrorRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkDstHostSrvRejErrorRate->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    
    /**********************************Packet Header Fields**************************************/
    strcat(ParamString,",1"); //Protocol Type (TCP, UD, ICMP)
    if ( chkIpTimeStamp->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpID->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpTOS->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpTTL->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpHeaderLen->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpDatagramLen->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpSourceIp->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpDestIp->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkPacketLand->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIpFragFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpSourcePort->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpDestPort->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpSynFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpFinFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpAckFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpPushFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpRstFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpUrgFlag->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpHeaderLen->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkTcpWindowSize->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkUdpSourcePort->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkUdpDestPort->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIcmpType->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIcmpCode->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIcmpID->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    if ( chkIcmpSeq->isChecked() )
        strcat(ParamString,",1"); else strcat(ParamString,",0");
    
    PreProcessorObj.SetWorkSpaceParameters(ParamString);
    ManagementControlObj.SetWorkSpaceModified();
}//End of function frmPreProcessorSetting::SetPreProcessorParameters


//#################        FUNCTION INIT       ####################
void frmPreProcessorSetting::init()
{
    int i;
    char ParamString [512];
    
    PreProcessorObj.GetWorkSpaceParameters(ParamString);
    
    ActivateFeaturesOrPacketTypeTab(0);
    
    /************************************Connection Features***************************************/
    i=0;
    if (ParamString[i]=='1')
        chkDuration->setChecked(true);
    else
        chkDuration->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkProtocolType->setChecked(true);
    else
        chkProtocolType->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkService->setChecked(true);
    else
        chkService->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkFlag->setChecked(true);
    else
        chkFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkSrcBytes->setChecked(true);
    else
        chkSrcBytes->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstBytes->setChecked(true);
    else
        chkDstBytes->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkLand->setChecked(true);
    else
        chkLand->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkWrongFragment->setChecked(true);
    else
        chkWrongFragment->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkUrgent->setChecked(true);
    else
        chkUrgent->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkHot->setChecked(true);
    else
        chkHot->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumFailedLogins->setChecked(true);
    else
        chkNumFailedLogins->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkLoggedIn->setChecked(true);
    else
        chkLoggedIn->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumCompromised->setChecked(true);
    else
        chkNumCompromised->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkRootShell->setChecked(true);
    else
        chkRootShell->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkSuAttempted->setChecked(true);
    else
        chkSuAttempted->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumRoot->setChecked(true);
    else
        chkNumRoot->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumFileCreations->setChecked(true);
    else
        chkNumFileCreations->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumShells->setChecked(true);
    else
        chkNumShells->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumAccessFiles->setChecked(true);
    else
        chkNumAccessFiles->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkNumOutboundCmds->setChecked(true);
    else
        chkNumOutboundCmds->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIsHotLogin->setChecked(true);
    else
        chkIsHotLogin->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIsGuestLogin->setChecked(true);
    else
        chkIsGuestLogin->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkCount->setChecked(true);
    else
        chkCount->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkServiceCount->setChecked(true);
    else
        chkServiceCount->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkSynErrorRate->setChecked(true);
    else
        chkSynErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkServiceSynErrorRate->setChecked(true);
    else
        chkServiceSynErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkRejErrorRate->setChecked(true);
    else
        chkRejErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkServiceRejErrorRate->setChecked(true);
    else
        chkServiceRejErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkSameServiceRate->setChecked(true);
    else
        chkSameServiceRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDiffServiceRate->setChecked(true);
    else
        chkDiffServiceRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkServiceDiffHostRate->setChecked(true);
    else
        chkServiceDiffHostRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostCount->setChecked(true);
    else
        chkDstHostCount->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSrvCount->setChecked(true);
    else
        chkDstHostSrvCount->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSameSrvRate->setChecked(true);
    else
        chkDstHostSameSrvRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostDiffSrvRate->setChecked(true);
    else
        chkDstHostDiffSrvRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSameSrcPortRate->setChecked(true);
    else
        chkDstHostSameSrcPortRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSrvDiffHostRate->setChecked(true);
    else
        chkDstHostSrvDiffHostRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSynErrorRate->setChecked(true);
    else
        chkDstHostSynErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSrvSynErrorRate->setChecked(true);
    else
        chkDstHostSrvSynErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostRejErrorRate->setChecked(true);
    else
        chkDstHostRejErrorRate->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkDstHostSrvRejErrorRate->setChecked(true);
    else
        chkDstHostSrvRejErrorRate->setChecked(false);
   
    /**********************************Packet Header Fields**************************************/
    i+=4;
    if (ParamString[i]=='1')
        chkIpTimeStamp->setChecked(true);
    else
        chkIpTimeStamp->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpID->setChecked(true);
    else
        chkIpID->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpTOS->setChecked(true);
    else
        chkIpTOS->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpTTL->setChecked(true);
    else
        chkIpTTL->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpHeaderLen->setChecked(true);
    else
        chkIpHeaderLen->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpDatagramLen->setChecked(true);
    else
        chkIpDatagramLen->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpSourceIp->setChecked(true);
    else
        chkIpSourceIp->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpDestIp->setChecked(true);
    else
        chkIpDestIp->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkPacketLand->setChecked(true);
    else
        chkPacketLand->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIpFragFlag->setChecked(true);
    else
        chkIpFragFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpSourcePort->setChecked(true);
    else
        chkTcpSourcePort->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpDestPort->setChecked(true);
    else
        chkTcpDestPort->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpSynFlag->setChecked(true);
    else
        chkTcpSynFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpFinFlag->setChecked(true);
    else
        chkTcpFinFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpAckFlag->setChecked(true);
    else
        chkTcpAckFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpPushFlag->setChecked(true);
    else
        chkTcpPushFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpRstFlag->setChecked(true);
    else
        chkTcpRstFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpUrgFlag->setChecked(true);
    else
        chkTcpUrgFlag->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpHeaderLen->setChecked(true);
    else
        chkTcpHeaderLen->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkTcpWindowSize->setChecked(true);
    else
        chkTcpWindowSize->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkUdpSourcePort->setChecked(true);
    else
        chkUdpSourcePort->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkUdpDestPort->setChecked(true);
    else
        chkUdpDestPort->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIcmpType->setChecked(true);
    else
        chkIcmpType->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIcmpCode->setChecked(true);
    else
        chkIcmpCode->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIcmpID->setChecked(true);
    else
        chkIcmpID->setChecked(false);
    i+=2;
    if (ParamString[i]=='1')
        chkIcmpSeq->setChecked(true);
    else
        chkIcmpSeq->setChecked(false);
 
}//End of slot frmPreProcessingSetting::init


//#################        FUNCTION ACTIVATE FEATURES OR PACKET TYPE TAB       ####################
void frmPreProcessorSetting::ActivateFeaturesOrPacketTypeTab( int FeaturesOrPacketType )
{
    if ( FeaturesOrPacketType==0 )
    {
        tabPacketType->show();
        tabFeatures->hide();
    }
    else 
    {
        tabPacketType->hide();
        tabFeatures->show();
    }
}//End of slot frmPreProcessorSetting::cmbFeatureExtraction_activated
