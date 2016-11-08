SOURCES	+= ManagementControl.cpp \
	main.cpp \
	NeuralNet.cpp \
	PreProcessor.cpp \
	IdsEvaluator.cpp \
	Responder.cpp \
	TcpDumpPacketProvider.cpp \
	TrainTestDataProvider.cpp
HEADERS	+= ManagementControl.h \
	NeuralNet.h \
	PreProcessor.h \
	IdsEvaluator.h \
	AttackTypes.h \
	Responder.h \
	TcpDumpPacketProvider.h \
	TrainTestDataProvider.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= frmPreProcessorSetting.ui \
	frmNeuralNetSetting.ui \
	frmMain.ui \
	frmDataProvidersSetting.ui \
	frmProgressWindow.ui \
	frmIdsEvaluator.ui \
	frmResponderSetting.ui \
	frmCapturePacketsSetting.ui
IMAGES	= images/Contents.png \
	images/DataProviderSetting.png \
	images/EvaluateNet.png \
	images/Exit.png \
	images/FileNew.png \
	images/FileOpen.png \
	images/FileSaveAs.png \
	images/Help.png \
	images/NeuralNetSetting.png \
	images/PreProcessorSetting.png \
	images/Print.png \
	images/ResponderSetting.png \
	images/StartOffLine.png \
	images/StartOnLine.png \
	images/StopOnLine.png \
	images/TestNet.png \
	images/TrainNet.png \
	images/UNNID.png \
	images/FileSave.png \
	images/FileOpen_Small.png
TEMPLATE	=app
CONFIG	+= qt warn_on release
LANGUAGE	= C++
