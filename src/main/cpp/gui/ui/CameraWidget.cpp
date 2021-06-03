/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "CameraWidget.hpp"
#include "ui_CameraWidget.h"

#include <QAction>
#include <QDebug>
#include <QMenu>

#include "CameraInfoDialog.hpp"

CameraWidget::CameraWidget(QHYCamera * camera, QWidget * parent)
   : QWidget(parent)
   , ui(new Ui::CameraWidget)
   , camera(camera)
   , cameraMenu(new QMenu())
{
   ui->setupUi(this);
   connect(ui->comboBoxReadMode, &QComboBox::currentTextChanged, camera, [=]() {
      camera->setReadAndTransferModes(this->ui->comboBoxReadMode->currentText());
   });
   connect(ui->comboBoxTransferMode, &QComboBox::currentTextChanged, this, &CameraWidget::transferModeSelected);
   connect(ui->pushButtonConnection, &QPushButton::toggled, this, &CameraWidget::connectToCamera);
   connect(camera, &QHYCamera::connectedChanged, this, &CameraWidget::cameraConnectionStatusChanged);
   connect(camera, &QHYCamera::readModeChanged, this, &CameraWidget::readModeChanged);
   connect(camera, &QHYCamera::transferModeChanged, this, &CameraWidget::transferModeChanged);

   this->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(this, &CameraWidget::customContextMenuRequested, this, &CameraWidget::showContextMenu);
   auto * action = new QAction(tr("&Capabilities")); // NOLINT(cppcoreguidelines-owning-memory)
   connect(action, &QAction::triggered, this, &CameraWidget::showCameraInfoDialog);
   action->setStatusTip(tr("This cameras capabilities."));
   cameraMenu->addAction(action);
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
      emit newStatusMessage(tr("Connected to %1.").arg(camera->id()));
      ui->pushButtonConnection->setText(tr("Connected"));
      ui->comboBoxReadMode->clear();
      ui->comboBoxReadMode->addItems(camera->readModes());
   } else {
      emit newStatusMessage(tr("Disconnected from %1.").arg(camera->id()));
      ui->comboBoxReadMode->clear();
      ui->pushButtonConnection->setText(tr("Disconnected"));
   }
}

void CameraWidget::connectToCamera(bool connect) const
{
   if (connect && !camera->isConnected()) {
      camera->connect();
   } else if (!connect && camera->isConnected()) {
      camera->disconnect();
   }
}

void CameraWidget::readModeChanged(QString newMode) const
{
   if (newMode == ui->comboBoxReadMode->currentText()) {
      emit newStatusMessage(tr("Read mode set to %1.").arg(newMode));
   } else {
      emit newStatusMessage(tr("Setting read mode to %1 failed.").arg(newMode));
      ui->comboBoxReadMode->setCurrentIndex(-1);
   }
}

void CameraWidget::showCameraInfoDialog() const
{
   CameraInfoDialog dialog(camera);
   dialog.exec();
}

void CameraWidget::showContextMenu(const QPoint & point) const
{
   cameraMenu->exec(mapToGlobal(point));
}

void CameraWidget::transferModeChanged(QHYCamera::DataTransferMode newMode) const
{
   QString newModeName = newMode == QHYCamera::SingleImage ? tr("Single Image") : tr("Live View");
   if (newMode == ui->comboBoxTransferMode->currentData()) {
      emit newStatusMessage(tr("Transfer mode set to %1.").arg(newModeName));
   } else {
      emit newStatusMessage(tr("Setting transfer mode to %1 failed.").arg(newModeName));
      ui->comboBoxTransferMode->setCurrentIndex(-1);
   }
}

void CameraWidget::transferModeSelected(QString modeName) const
{
   Q_UNUSED(modeName)
   camera->setReadAndTransferModes(ui->comboBoxReadMode->currentText(),
                                   ui->comboBoxTransferMode->currentData().value<QHYCamera::DataTransferMode>());
}
