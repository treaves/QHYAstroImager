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
   ui->comboBoxReadModes->addItems(camera->readModes());
   connect(ui->comboBoxReadModes, &QComboBox::currentTextChanged, this, &CameraWidget::selectReadMode);
   connect(ui->comboBoxReadModes, &QComboBox::setEnabled, camera, &QHYCamera::isConnected);
   if(camera->connect()) {
      emit newStatusMessage(tr("Connected to %a").arg(camera->name()));
   } else {
      emit newStatusMessage(tr("Could not connect to %a").arg(camera->name()));
   }
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
void CameraWidget::selectReadMode(QString readMode)
{
   qDebug() << "Selected read mode:" << readMode;
}