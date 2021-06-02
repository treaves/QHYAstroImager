/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "CameraWidget.hpp"
#include "ui_CameraWidget.h"

#include "QHYCamera.hpp"
#include <QDebug>

CameraWidget::CameraWidget(QHYCamera * camera, QWidget * parent)
   : QWidget(parent)
   , ui(new Ui::CameraWidget)
   , camera(camera)
{
   ui->setupUi(this);
   connect(ui->comboBoxReadModes, &QComboBox::currentTextChanged, camera, &QHYCamera::setReadMode);
   connect(ui->pushButtonConnection, &QPushButton::toggled, this, &CameraWidget::connectToCamera);
   connect(camera, &QHYCamera::connectedChanged, this, &CameraWidget::cameraConnectionStatusChanged);
   connect(camera, &QHYCamera::readModeChanged, this, &CameraWidget::readModeChanged);
}

CameraWidget::~CameraWidget()
{
   delete ui;
}

/* ***************************************************************************************************************** */
// MARK: - Private methods
/* ***************************************************************************************************************** */

/* ***************************************************************************************************************** */
// MARK: - Private slots
/* ***************************************************************************************************************** */
void CameraWidget::cameraConnectionStatusChanged(bool isConnected) const
{
   if (isConnected) {
      emit newStatusMessage(tr("Connected to %1.").arg(camera->name()));
      ui->pushButtonConnection->setText(tr("Connected"));
      ui->comboBoxReadModes->clear();
      ui->comboBoxReadModes->addItems(camera->readModes());
   } else {
      emit newStatusMessage(tr("Disconnected from %1.").arg(camera->name()));
      ui->comboBoxReadModes->clear();
      ui->pushButtonConnection->setText(tr("Disconnected"));
   }
}

void CameraWidget::connectToCamera(bool connect) const
{
  if (connect && !camera->isConnected()) {
     if(!camera->connect()) {
        emit newStatusMessage(tr("Could not connect to %1.").arg(camera->name()));
        ui->pushButtonConnection->setChecked(false);
     }
  } else if(!connect && camera->isConnected()) {
     if(!camera->disconnect()) {
        emit newStatusMessage(tr("Disconnect from %1 failed.").arg(camera->name()));
        ui->pushButtonConnection->setChecked(true);
     }
  }
}

void CameraWidget::readModeChanged(QString newMode)
{
   if (newMode == ui->comboBoxReadModes->currentText()) {
      emit newStatusMessage(tr("Read mode set to %1.").arg(newMode));
   } else {
      emit newStatusMessage(tr("Setting read mode to %1 failed.").arg(newMode));
      ui->comboBoxReadModes->setCurrentIndex(-1);

   }
}
