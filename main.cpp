#include "Dab.hpp"

int
main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0.0");
  application.setOrganizationName("Dots and Boxes");

  const dab::Config config = dab::CommandParser().Process(application);
  if (QPointer<QWidget> mainWindow = dab::MainWindowCreator().CreateMainWindow(config)) {
    mainWindow->show();
  }

  return application.exec();
}
