#include "ui/MainWindow.hpp"
#include <QApplication>

#include "Config.h"

int main(int argc, char * argv[])
{
   QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

   QApplication application(argc, argv);
   QCoreApplication::setOrganizationName("Silverfields Technologies Incorporated");
   QCoreApplication::setOrganizationDomain("silverfieldstech.com");
   QCoreApplication::setApplicationName("QHYAstroImager");
   QCoreApplication::setApplicationVersion(VERSION);

   MainWindow window;
   window.show();

   return QApplication::exec();
}
