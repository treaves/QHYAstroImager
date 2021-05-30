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
class QHYCamera;
class CameraWidget;

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
   void displayAboutDialog() const;
   void displayStatusMessage(QString message) const;
   void updateCameraList(const QStringList & cameraNames);

private:
   [[nodiscard]] auto    cameraTabExists(const QString & cameraName) const -> bool;
   void                  createMenus();
   void                  readSettings();
   void                  writeSettings();

   Ui::MainWindow *      ui;
   QHYCCD *              qhyccd;
};
