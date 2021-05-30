/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "About.hpp"
#include "CameraWidget.hpp"
#include "Config.h"
#include "QHYCamera.hpp"
#include "QHYCCD.hpp"
#include <QAction>
#include <QSettings>

MainWindow::MainWindow(QWidget * parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
   , qhyccd(new QHYCCD(this))
{
   ui->setupUi(this);
   readSettings();
   createMenus();
   ui->statusbar->showMessage(tr("No cameras found."));

   connect(qhyccd, &QHYCCD::camerasChanged, this, &MainWindow::updateCameraList);
   if (!qhyccd->initialize()) {
      ui->statusbar->showMessage(tr("Initialization of the QHYCCD driver failed."));
   }
}

MainWindow::~MainWindow()
{
   delete ui;
}

/* ***************************************************************************************************************** */
// MARK: - QMaiaWindow over-rides
/* ***************************************************************************************************************** */
void MainWindow::closeEvent(QCloseEvent * event)
{
   QWidget::closeEvent(event);
   writeSettings();
   //   event->accept();
}

/* ***************************************************************************************************************** */
// MARK: - Private methods
/* ***************************************************************************************************************** */
auto MainWindow::cameraTabExists(const QString & cameraName) const -> bool
{
   bool found = false;
   for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
      if (ui->tabWidget->tabText(tabIndex) == cameraName) {
         found = true;
         break;
      }
   }
   return found;
}

void MainWindow::createMenus()
{
   auto * menu   = menuBar()->addMenu(tr("&File"));
   menu          = menuBar()->addMenu(tr("&Help"));
   auto * action = new QAction(tr("&About")); // NOLINT(cppcoreguidelines-owning-memory)
   connect(action, &QAction::triggered, this, &MainWindow::displayAboutDialog);
   action->setStatusTip(tr("About."));
   menu->addAction(action);
}

void MainWindow::writeSettings()
{
   QSettings settings;

   settings.setValue(MAIN_WINDOW_SIZE, size());
   settings.setValue(MAIN_WINDOW_POSITION, pos());
}

void MainWindow::readSettings()
{
   QSettings settings;
   resize(settings.value(MAIN_WINDOW_SIZE, QSize(DefaultWindowWidth, DefaultWindowHeight)).toSize());
   move(settings.value(MAIN_WINDOW_POSITION, QPoint(DefaultWindowPositionX, DefaultWindowPositionY)).toPoint());
}
/* ***************************************************************************************************************** */
// MARK: - Private Slots
/* ***************************************************************************************************************** */
void MainWindow::displayAboutDialog() const
{
   About aboutDialog(this->topLevelWidget());
   aboutDialog.exec();
}

void MainWindow::displayStatusMessage(QString message) const
{
   ui->statusbar->showMessage(message);
}

void MainWindow::updateCameraList(const QStringList & cameraNames)
{
   for (const auto & cameraName : cameraNames) {
      if (!cameraTabExists(cameraName)) {
         QHYCamera * camera = qhyccd->cameraNamed(cameraName);
         if (camera != nullptr) {
            auto * cameraTab = new CameraWidget(camera, this);
            ui->tabWidget->addTab(cameraTab, cameraName);
         } else {
            qWarning() << tr("The camera named %1 could not be found.").arg(cameraName);
            ui->statusbar->showMessage(tr("The camera named %1 could not be found.").arg(cameraName));
         }
      }
   }

   // Check for any camera that has been removed
   for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
      if (!cameraNames.contains(ui->tabWidget->tabText(tabIndex))) {
         auto * cameraWidget = ui->tabWidget->widget(tabIndex);
         ui->tabWidget->removeTab(tabIndex);
         if (cameraWidget != nullptr) {
            delete cameraWidget;
         }
      }
   }
}
