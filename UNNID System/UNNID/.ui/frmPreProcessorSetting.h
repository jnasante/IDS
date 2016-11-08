/****************************************************************************
** Form interface generated from reading ui file 'frmPreProcessorSetting.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMPREPROCESSORSETTING_H
#define FRMPREPROCESSORSETTING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QTabWidget;
class QWidget;

class frmPreProcessorSetting : public QDialog
{
    Q_OBJECT

public:
    frmPreProcessorSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmPreProcessorSetting();

    QLabel* lblPreprocessorOption;
    QComboBox* cmbTcpdumpKddcup;
    QTabWidget* tabFeatures;
    QWidget* tabPageBasicTcp;
    QCheckBox* chkDuration;
    QCheckBox* chkDstBytes;
    QCheckBox* chkProtocolType;
    QCheckBox* chkWrongFragment;
    QCheckBox* chkLand;
    QCheckBox* chkService;
    QCheckBox* chkUrgent;
    QCheckBox* chkSrcBytes;
    QCheckBox* chkFlag;
    QWidget* tab;
    QCheckBox* chkNumShells;
    QCheckBox* chkIsHotLogin;
    QCheckBox* chkLoggedIn;
    QCheckBox* chkNumRoot;
    QCheckBox* chkRootShell;
    QCheckBox* chkSuAttempted;
    QCheckBox* chkNumAccessFiles;
    QCheckBox* chkNumCompromised;
    QCheckBox* chkNumFailedLogins;
    QCheckBox* chkNumFileCreations;
    QCheckBox* chkIsGuestLogin;
    QCheckBox* chkHot;
    QCheckBox* chkNumOutboundCmds;
    QWidget* tab_2;
    QCheckBox* chkCount;
    QGroupBox* GroupBox1;
    QCheckBox* chkSynErrorRate;
    QCheckBox* chkRejErrorRate;
    QCheckBox* chkSameServiceRate;
    QCheckBox* chkDiffServiceRate;
    QCheckBox* chkServiceCount;
    QGroupBox* GroupBox2;
    QCheckBox* chkServiceSynErrorRate;
    QCheckBox* chkServiceRejErrorRate;
    QCheckBox* chkServiceDiffHostRate;
    QWidget* tab_3;
    QCheckBox* chkDstHostSynErrorRate;
    QCheckBox* chkDstHostCount;
    QCheckBox* chkDstHostSameSrcPortRate;
    QCheckBox* chkDstHostSrvCount;
    QCheckBox* chkDstHostSrvDiffHostRate;
    QCheckBox* chkDstHostRejErrorRate;
    QCheckBox* chkDstHostSrvSynErrorRate;
    QCheckBox* chkDstHostSrvRejErrorRate;
    QCheckBox* chkDstHostDiffSrvRate;
    QCheckBox* chkDstHostSameSrvRate;
    QTabWidget* tabPacketType;
    QWidget* tab_4;
    QCheckBox* chkIpDestIp;
    QCheckBox* chkIpTimeStamp;
    QCheckBox* chkIpID;
    QCheckBox* chkIpDatagramLen;
    QCheckBox* chkIpSourceIp;
    QCheckBox* chkIpFragFlag;
    QCheckBox* chkIpHeaderLen;
    QCheckBox* chkIpTTL;
    QCheckBox* chkIpTOS;
    QCheckBox* chkPacketLand;
    QWidget* tabTCP;
    QCheckBox* chkTcpSourcePort;
    QCheckBox* chkTcpWindowSize;
    QCheckBox* chkTcpDestPort;
    QCheckBox* chkTcpHeaderLen;
    QGroupBox* grpTcpFlags;
    QCheckBox* chkTcpRstFlag;
    QCheckBox* chkTcpSynFlag;
    QCheckBox* chkTcpAckFlag;
    QCheckBox* chkTcpFinFlag;
    QCheckBox* chkTcpPushFlag;
    QCheckBox* chkTcpUrgFlag;
    QWidget* tabUDP;
    QCheckBox* chkUdpSourcePort;
    QCheckBox* chkUdpDestPort;
    QWidget* tabICMP;
    QCheckBox* chkIcmpType;
    QCheckBox* chkIcmpID;
    QCheckBox* chkIcmpCode;
    QCheckBox* chkIcmpSeq;
    QPushButton* btmOk;
    QPushButton* btmCancel;
    QPushButton* btmApply;

public slots:
    virtual void SetPreProcessorParameters();
    virtual void init();
    virtual void ActivateFeaturesOrPacketTypeTab( int FeaturesOrPacketType );

protected:
    QGridLayout* Layout5;
    QGridLayout* Layout6;
    QVBoxLayout* Layout7;
    QVBoxLayout* Layout8;
    QGridLayout* layout6;
    QGridLayout* layout10;
    QGridLayout* layout10_2;
    QGridLayout* layout12;

protected slots:
    virtual void languageChange();
};

#endif // FRMPREPROCESSORSETTING_H
