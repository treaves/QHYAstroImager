#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QObject>
#include <QStringList>

using qhyccd_handle = void;

/*! \brief A QHYCCD camera.
 *
 * This class is intended to be the interface to a QHYCCD camera.
 *
 * Instantiation of this class does not make it usable; before an instance does anything, connect() must be called.
 */
class QHYCamera : public QObject
{
   Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
   Q_DISABLE_COPY_MOVE(QHYCamera)
#endif
   Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
   Q_PROPERTY(QString name READ name)
   Q_PROPERTY(QString readMode READ readMode WRITE setReadMode NOTIFY readModeChanged)
   Q_PROPERTY(QStringList readModes READ readModes)

public:
   explicit QHYCamera(QByteArray name, QObject * parent = nullptr);
   ~QHYCamera() noexcept;

   /*!
    * Connects to the QHYCCD camera, and returns success.
    *
    * \return The success of connecting to the QHYCCD camera.
    */
   [[nodiscard]] auto connect() -> bool;
   auto               disconnect() -> bool;
   [[nodiscard]] auto isConnected() -> bool;
   [[nodiscard]] auto name() const -> QString;
   [[nodiscard]] auto readMode() const -> QString;
   [[nodiscard]] auto readModes() const -> QStringList;

public slots:
   void setReadMode(QString readMode);

signals:
   void connectedChanged(bool connected);
   void readModeChanged(QString readMode);

private:
   void            initializeReadModes();
   void        readCameraDetails(QByteArray & nameBuffer);

   qhyccd_handle * handle;
   QString         id;
   QLatin1String   model;
   QByteArray      m_name;
   QString         m_readMode;
   QStringList     m_readModes;
};