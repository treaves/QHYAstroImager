/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "About.hpp"
#include "Config.h"
#include "QHYCamera.hpp"
#include "QHYCCD.hpp"
#include <QAction>
#include <QSettings>

MainWindow::MainWindow(QWidget * parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
   , cameras(new QActionGroup(this))
   , menuCameras(nullptr)
   , qhyccd(new QHYCCD(this))
{
   ui->setupUi(this);
   readSettings();
   createMenus();
   ui->statusbar->showMessage(tr("No cameras found."));

   connect(qhyccd, &QHYCCD::camerasChanged, this, &MainWindow::updateCameraList);
   connect(cameras, &QActionGroup::triggered, this, &MainWindow::cameraSelected);
   if (!qhyccd->initialize()) {
      ui->statusbar->showMessage(tr("Initialization of the QHYCCD driver failed."));
   }

   cameras->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
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
auto MainWindow::cameraExists(const QString & cameraName) const -> bool
{
   return std::any_of(cameras->actions().constBegin(), cameras->actions().constEnd(), [cameraName](QAction * action) {
      return action->text() == cameraName;
   });
}

void MainWindow::connectToCamera(QString cameraName)
{
   connectedCamera = qhyccd->cameraNamed(cameraName);
   if (!connectedCamera->connect()) {
      connectedCamera = nullptr;
   }
}

void MainWindow::createMenus()
{
   auto * menu   = menuBar()->addMenu(tr("&File"));

   menuCameras   = menuBar()->addMenu(tr("&Cameras"));

   menu          = menuBar()->addMenu(tr("&Help"));
   auto * action = new QAction(tr("&About")); // NOLINT(cppcoreguidelines-owning-memory)
   connect(action, &QAction::triggered, this, &MainWindow::displayAboutDialog);
   action->setStatusTip(tr("About."));
   menu->addAction(action);
}

void MainWindow::disconnectFromCamera(QString cameraName)
{
   Q_UNUSED(cameraName)
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
void MainWindow::cameraSelected(QAction * camera)
{
   // This block is to disconnect a camera when a different camera is selected, as there is only one event.
   if (!selectedCameraName.isEmpty() && selectedCameraName != camera->text()) {
      disconnectFromCamera(selectedCameraName);
      selectedCameraName = "";
   }
   if (camera->isChecked()) {
      ui->statusbar->showMessage(tr("Camera %1 selected.").arg(camera->text()), FiveSeconds);
      connectToCamera(camera->text());
      selectedCameraName = camera->text();
   } else {
      ui->statusbar->showMessage(tr("Camera %1 deselected.").arg(camera->text()), FiveSeconds);
      disconnectFromCamera(camera->text());
      selectedCameraName = "";
   }
}

void MainWindow::displayAboutDialog() const
{
   About aboutDialog(this->topLevelWidget());
   aboutDialog.exec();
}

void MainWindow::updateCameraList(const QStringList & cameraNames)
{
   qDebug() << "====> updateCameraList() called.";
   for (const auto & cameraName : cameraNames) {
      if (!cameraExists(cameraName)) {
         auto * action = new QAction(cameraName); // NOLINT(cppcoreguidelines-owning-memory)
         action->setCheckable(true);
         cameras->addAction(action);
         menuCameras->addAction(action);
      }
   }

   // Check for any camera that has been removed
   for (auto * camera : cameras->actions()) {
      if (!cameraNames.contains(camera->text())) {
         cameras->removeAction(camera);
         menuCameras->removeAction(camera);
      }
   }
}
