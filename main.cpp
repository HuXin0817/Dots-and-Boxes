#include "frontend/CommandParser.hpp"

using namespace dab::frontend;

int
main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0.0");
  application.setOrganizationName("Dots and Boxes");

  Config config = CommandParser().Process(application);
  if (QPointer<QWidget> mainWindow = MainWindowCreator().CreateMainWindow(config)) {
    mainWindow->show();
  }

  return application.exec();
}
