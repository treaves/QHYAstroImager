#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QAction>
#include <QJsonArray>
#include <QList>
#include <QMainWindow>


namespace Ui
{
   class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT
   Q_DISABLE_COPY_MOVE(MainWindow)

public:
   explicit MainWindow(QWidget * parent = nullptr);
   ~MainWindow() override;

protected:
   void closeEvent(QCloseEvent * event) override;

private slots:
   void connectToCamera();
   void disconnectFromCamera();
   void displayAboutDialog() const;
   void populateCameraList();

private:
   void             createMenus();
   void             readSettings();
   void             writeSettings();

   Ui::MainWindow * ui;
};
