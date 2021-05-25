/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QAction>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include "About.hpp"
#include "Config.h"

MainWindow::MainWindow(QWidget * parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   readSettings();
   createMenus();
}

MainWindow::~MainWindow()
{
   delete ui;
}

/* *****************************************************************************************************************
 */
// MARK: - QMaiaWindow over-rides
/* *****************************************************************************************************************
 */
void MainWindow::closeEvent(QCloseEvent * event)
{
   QWidget::closeEvent(event);
   writeSettings();
   //   event->accept();
}

/* *****************************************************************************************************************
 */
// MARK: - Private methods
/* *****************************************************************************************************************
 */
void MainWindow::createMenus()
{
   auto * menu   = menuBar()->addMenu(tr("&File"));
   auto * action = new QAction(tr("&Connect camera")); // NOLINT(cppcoreguidelines-owning-memory)
   connect(action, &QAction::triggered, this, &MainWindow::connectToCamera);
   action->setStatusTip(tr("Connect to camera."));
   menu->addAction(action);

   menu   = menuBar()->addMenu(tr("&Help"));
   action = new QAction(tr("&About")); // NOLINT(cppcoreguidelines-owning-memory)
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
/* *****************************************************************************************************************
 */
// MARK: - Private Slots
/* *****************************************************************************************************************
 */
void MainWindow::connectToCamera()
{
   //
}

void MainWindow::disconnectFromCamera()
{
   //
}

void MainWindow::displayAboutDialog() const
{
   About aboutDialog;
   aboutDialog.exec();
}