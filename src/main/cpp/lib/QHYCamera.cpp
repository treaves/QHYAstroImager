/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "QHYCamera.hpp"
#include "Config.h"
#include <QDebug>
#include <qhyccd.h>

QHYCamera::QHYCamera(QByteArray name, QObject * parent)
   : QObject(parent)
   , handle(nullptr)
   , id("")
   , model(name.left(name.lastIndexOf('-')))
   , m_name(name)
{
}

QHYCamera::~QHYCamera() noexcept
{
   if (handle != nullptr) {
      disconnect();
   }
}

/* ***************************************************************************************************************** */
// MARK: - Public methods
/* ***************************************************************************************************************** */
auto QHYCamera::connect() -> bool
{
   handle = OpenQHYCCD(m_name.data());
   if (handle != nullptr) {
      initializeReadModes();
   }
   emit connectedChanged(isConnected());
   return isConnected();
}

auto QHYCamera::disconnect() -> bool
{
   if (handle != nullptr) {
      quint32 qhyResult = CloseQHYCCD(handle);
      if (qhyResult == QHYCCD_SUCCESS) {
         handle = nullptr;
      } else {
         qWarning() << tr("There was an error disconnecting from %1.").arg(QLatin1String(m_name));
      }
   }
   emit connectedChanged(isConnected());
   return !isConnected();
}

auto QHYCamera::isConnected() -> bool
{
   return handle != nullptr;
}

auto QHYCamera::name() const -> QString
{
   return QString(m_name);
}

auto QHYCamera::readMode() const -> QString
{
   return m_readMode;
}

auto QHYCamera::readModes() const -> QStringList
{
   return m_readModes;
}

/* ***************************************************************************************************************** */
// MARK: - Public slots
/* ***************************************************************************************************************** */
void QHYCamera::setReadMode(QString readMode)
{
   if (m_readMode != readMode) {
      m_readMode = readMode;
      emit readModeChanged(readMode);
   }
}

/* ***************************************************************************************************************** */
// MARK: - Private methods
/* ***************************************************************************************************************** */
void QHYCamera::initializeReadModes()
{
   if (handle != nullptr) {
      quint32 readModeCount = 0;
      if (GetQHYCCDNumberOfReadModes(handle, &readModeCount) != QHYCCD_SUCCESS) {
         disconnect();
      } else {
         qDebug() << "Found " << readModeCount << " read modes.";
         quint32 readModeIndex = 0;
         while (readModeIndex < readModeCount && handle != nullptr) {
            QByteArray readModeNameBuffer(BufferSizeReadModeName, 0);
            auto status = GetQHYCCDReadModeName(handle, readModeIndex, readModeNameBuffer.data());
            if ( status == QHYCCD_SUCCESS) {
               m_readModes.append(QString(readModeNameBuffer));
               qDebug() << "Found " << QString(readModeNameBuffer) << " read mode.";
            } else {
               qWarning() << tr("GetQHYCCDReadModeName failed with code %1.").arg(status);
               disconnect();
            }
            readModeIndex++;
         }
      }
   }
}

void QHYCamera::readCameraDetails(QByteArray & nameBuffer)
{
   if (handle != nullptr) {

   } else {
      qWarning() << "Could not open camera:" << nameBuffer;
   }
}
