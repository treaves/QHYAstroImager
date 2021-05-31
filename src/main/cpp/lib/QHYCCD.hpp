#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "QHYCamera.hpp"
#include <QByteArray>
#include <QObject>
#include <QStringList>

/*! \brief This is the interface to the QHYCCD driver.
 *
 * This class is intended to be the sole interface to the QHYCCD driver.  Users of this class should not need to include
 * any header from the library, nor link to either the QHYCCD library or the USB library.
 *
 * Instantiation of this class does not make it usable; before an instance does anything, initialize() must be called.
 */
class QHYCCD : public QObject
{
   Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
   Q_DISABLE_COPY_MOVE(QHYCCD)
#endif
   Q_PROPERTY(QStringList cameras READ cameras NOTIFY camerasChanged)
   Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)

public:
   explicit QHYCCD(QObject * parent = nullptr);
   ~QHYCCD() override;

   /*!
    * Accessor for the list of connected cameras.
    * @return The list of camera names attached.
    */
   [[nodiscard]] auto cameras() const -> const QStringList;

   /*!
    * Creates a representatioon of a given QHYCCD camera.
    *
    * @param name of the camera of interest.
    * @return The camera of interest, or nullptr if it could not be found.
    */
   [[nodiscard]] auto cameraNamed(QString name) -> QHYCamera *;

   /*!
    * Initializes the QHYCCD driver, and returns success.
    *
    * \return The success of initializing the QHYCCD driver.
    */
   [[nodiscard]] auto initialize() -> bool;

   /*!
    * Flag to track if the QHYCCD driver is initialized.
    * @return If the QHYCCD driver is in a usable state.
    */
   [[nodiscard]] auto isReady() const -> bool;

signals:
   void camerasChanged(const QStringList cameras);
   void readyChanged(bool ready);

private:
   void        populateCameraList();

   QStringList m_cameras;
   bool        m_ready;
};
