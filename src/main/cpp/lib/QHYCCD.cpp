/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "QHYCCD.hpp"

#include "Config.h"
#include <QByteArray>
#include <QDebug>
#include <qhyccd.h>
#include <QString>

/* ***************************************************************************************************************** */
// MARK: - ctors & dtors
/* ***************************************************************************************************************** */
QHYCCD::QHYCCD(QObject * parent)
   : QObject(parent)
{
}

QHYCCD::~QHYCCD() {}

/* ***************************************************************************************************************** */
// MARK: - Public methods
/* ***************************************************************************************************************** */
auto QHYCCD::cameras() const -> const QStringList
{
   return m_cameras;
}

auto QHYCCD::cameraNamed(QString name) -> QHYCamera *
{
   auto * camera = new QHYCamera(QByteArray(name.toStdString().c_str()));
   return camera;
}

auto QHYCCD::initialize() -> bool
{
   quint32 qhyResult = InitQHYCCDResource();
   if (qhyResult == QHYCCD_SUCCESS) {
      populateCameraList();
      m_ready = true;
   } else {
      qWarning() << "InitQHYCCDResource: failed";
      m_ready = false;
   }
   emit readyChanged(m_ready);
   return m_ready;
}

auto QHYCCD::isReady() const -> bool
{
   return m_ready;
}

/* ***************************************************************************************************************** */
// MARK: - Private methods
/* ***************************************************************************************************************** */
void QHYCCD::populateCameraList()
{
   m_cameras.clear();
   quint32 connectedCameraCount = ScanQHYCCD();
   qDebug() << "Cameras found:" << connectedCameraCount;

   QByteArray nameBuffer(BufferSizeCameraName, 0);
   quint32    qhyResult{ QHYCCD_ERROR };
   for (quint32 cameraIndex = 0; cameraIndex < connectedCameraCount; cameraIndex++) {
      qhyResult = GetQHYCCDId(cameraIndex, nameBuffer.data());
      if (qhyResult == QHYCCD_SUCCESS) {
         QLatin1String cameraName(nameBuffer);
         qDebug() << cameraName;
         m_cameras.append(cameraName);
      } else {
         qWarning() << QString(tr("GetQHYCCDId index[%1] failure")).arg(qhyResult);
      }
   }
   emit camerasChanged(m_cameras);
}
