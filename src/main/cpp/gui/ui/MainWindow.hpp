#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QAction>
#include <QActionGroup>
#include <QJsonArray>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QStringList>

class QHYCCD;

namespace Ui
{
   class MainWindow;
}

class MainWindow : public QMainWindow
{
   Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
   Q_DISABLE_COPY_MOVE(MainWindow)
#endif

public:
   explicit MainWindow(QWidget * parent = nullptr);
   ~MainWindow() override;

protected:
   void closeEvent(QCloseEvent * event) override;

private slots:
   void cameraSelected(QAction * camera);
   void displayAboutDialog() const;
   void updateCameraList(QStringList cameraNames);

private:
   [[nodiscard]] auto cameraExists(const QString & cameraName) const -> bool;
   void               connectToCamera(QString cameraName);
   void               createMenus();
   void               disconnectFromCamera(QString cameraName);
   void               readSettings();
   void               writeSettings();

   Ui::MainWindow *   ui;
   QActionGroup *     cameras;
   QMenu *            menuCameras;
   QHYCCD *           qhyccd;
   QString            selectedCameraName;
};
