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
   emit connectedChanged(handle == nullptr);
   return handle == nullptr;
}

auto QHYCamera::disconnect() -> void
{
   if (handle != nullptr) {
      quint32 qhyResult = CloseQHYCCD(handle);
      if (qhyResult == QHYCCD_SUCCESS) {
         handle = nullptr;
         emit connectedChanged(false);
      } else {
         qWarning() << tr("There was an error disconnectiong from %1.").arg(QLatin1String(m_name));
      }
   }
}

auto QHYCamera::isConnected() -> bool
{
   return handle == nullptr;
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
      quint32 readModeCount;
      if (GetQHYCCDNumberOfReadModes(handle, &readModeCount) != QHYCCD_SUCCESS) {
         disconnect();
      } else {
         quint32 readModeIndex = 0;
         while (readModeIndex <= readModeCount && handle != nullptr) {
            auto * readModeNameBuffer = // NOLINT(cppcoreguidelines-owning-memory)
              new QByteArray(BufferSizeReadModeName, 0);
            if (GetQHYCCDReadModeName(handle, readModeIndex, readModeNameBuffer->data()) == QHYCCD_SUCCESS) {
               m_readModes.append(QLatin1String(readModeNameBuffer->data()));
               qDebug() << "Found " << QLatin1String(readModeNameBuffer->data()) << " read mode.";
            } else {
               disconnect();
            }
            readModeIndex++;
         }
      }
   }
}