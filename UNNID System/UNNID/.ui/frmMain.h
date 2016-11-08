/****************************************************************************
** Form interface generated from reading ui file 'frmMain.ui'
**
** Created: Wed Apr 7 02:07:02 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <qvariant.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    frmMain( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~frmMain();

    QLabel* lblMessage;
    QLabel* lblMsgWarning;
    QMenuBar *menubar;
    QPopupMenu *fileMenu;
    QPopupMenu *PopupMenu;
    QPopupMenu *PopupMenu_2;
    QPopupMenu *PopupMenu_3;
    QPopupMenu *helpMenu;
    QToolBar *toolBar;
    QToolBar *Toolbar_2;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* fileExitAction;
    QAction* helpContentsAction;
    QAction* helpIndexAction;
    QAction* helpAboutAction;
    QAction* runStartTrainAction;
    QAction* runStartRealTimeIdsAction;
    QAction* runStopRealTimeIdsAction;
    QAction* settingsDataProvidersSettingAction;
    QAction* settingsPreProcessorSettingAction;
    QAction* settingsResponderSettingAction;
    QAction* settingsNeuralNetSettingAction;
    QAction* runStartTestAction;
    QAction* runEvaluateIdsAction;
    QAction* runStartOffLineIdsAction;
    QAction* runCapturePacketsAction;

    virtual int saveAndContinue( const QString & action );

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileExit();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void runStartTrain();
    virtual void runStartTest();
    virtual void runStopRealTimeIds();
    virtual void runStartRealTimeIds();
    virtual void settingsResponderSetting();
    virtual void runStartOffLineIds();
    virtual void settingsPreProcessorSetting();
    virtual void settingsNeuralNetSetting();
    virtual void settingsDataProvidersSetting();
    virtual void runEvaluateIds();
    virtual void runCapturePackets();

protected:

protected slots:
    virtual void languageChange();
private:
    char fileName[512];
    bool Exitted;

    void init();
    void destroy();

};

#endif // FRMMAIN_H
