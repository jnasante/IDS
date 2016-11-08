#include <qapplication.h>
#include <qmessagebox.h>
#include "frmMain.h"
#include "ManagementControl.h"
#include "TcpDumpPacketProvider.h"
#include "TrainTestDataProvider.h"
#include "PreProcessor.h"
#include "NeuralNet.h"
#include "Responder.h"
#include "IdsEvaluator.h"

ManagementControl ManagementControlObj;
TcpDumpPacketProvider TcpDumpPacketProviderObj;
TrainTestDataProvider TrainTestDataProviderObj;
PreProcessor PreProcessorObj;
NeuralNet NeuralNetObj;
Responder ResponderObj;
IdsEvaluator IdsEvaluatorObj;

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    frmMain w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
