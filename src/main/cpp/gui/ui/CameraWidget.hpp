#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QWidget>

#include "QHYCamera.hpp"

namespace Ui
{
   class CameraWidget;
}

class CameraWidget : public QWidget
{
   Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
   Q_DISABLE_COPY_MOVE(CameraWidget)
#endif

public:
   explicit CameraWidget(QHYCamera * camera, QWidget * parent = nullptr);
   ~CameraWidget() override;

signals:
   void newStatusMessage(QString message) const;

private slots:
   void cameraConnectionStatusChanged(bool isConnected) const;
   void connectToCamera(bool connect) const;
   void readModeChanged(QString newMode) const;
   void transferModeChanged(QHYCamera::DataTransferMode newMode) const;
   void transferModeSelected(QString modeName) const;

private:

   Ui::CameraWidget * ui;
   QHYCamera *        camera;
};
