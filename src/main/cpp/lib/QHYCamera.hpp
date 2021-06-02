#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "Config.h"
#include <QMap>
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
   Q_PROPERTY(DataTransferMode transferMode READ transferMode NOTIFY transferModeChanged)
   Q_PROPERTY(QString name READ name)
   Q_PROPERTY(QString readMode READ readMode NOTIFY readModeChanged)
   Q_PROPERTY(QStringList readModes READ readModes)

public:
   enum DataTransferMode
   {
      SingleImage = 0x00,
      LiveView    = 0x01
   };
   Q_ENUM(DataTransferMode)

   struct Range
   {
      double min{ 0.0 };
      double max{ 0.0 };
      double step{ 0.0 };
   } __attribute__((aligned(Align32Bit)));

   struct Binning
   {
      bool oneByOne{ false };
      bool twoByTwo{ false };
      bool threeByThree{ false };
      bool fourByFour{ false };
      int  binXMaximum{ 1 };
      int  binYMaximum{ 1 };
   } __attribute__((aligned(16)));

   explicit QHYCamera(QByteArray name, QObject * parent = nullptr);
   ~QHYCamera() noexcept override;

   /*!
    * Connects to the QHYCCD camera, and returns success.
    *
    * \return The success of connecting to the QHYCCD camera.
    */
   void               connect();
   void               disconnect();
   [[nodiscard]] auto isConnected() -> bool;
   [[nodiscard]] auto name() const -> QString;
   [[nodiscard]] auto readMode() const -> QString;
   [[nodiscard]] auto readModes() const -> QStringList;
   [[nodiscard]] auto transferMode() const -> DataTransferMode;

public slots:
   void setReadAndTransferModes(QString readMode, QHYCamera::DataTransferMode mode = SingleImage);

signals:
   void connectedChanged(bool connected);
   void readModeChanged(QString readMode);
   void transferModeChanged(QHYCamera::DataTransferMode mode);

private:
   void                   initializeReadModes();
   void                   readCameraDetails();
   void                   readChipInfo();
   void                   readControlValues();
   void                   readFirmwareVersion();
   void                   readFPGAVersion();

   qhyccd_handle *        handle;
   QString                id;
   QLatin1String          model;
   QByteArray             m_name;
   QString                m_readMode;
   QMap<QString, quint32> m_readModes;
   DataTransferMode       m_transferMode;

   QString                firmwareVersion;
   QString                fpga1Version;
   QString                fpga2Version;
   double                 chipWidth;
   double                 chipHeight;
   qint32                 imageWidth;
   qint32                 imageHeight;
   double                 pixelWidth;
   double                 pixelHeight;
   qint32                 bitsPerPixel;
   bool                   supportsColor;
   int                    bayerMatrix;
   bool                   supportsOffset;
   Range                  offsetRange;
   double                 offset;
   bool                   supportsGain;
   Range                  gainRange;
   double                 gain;
   bool                   supportsBinning;
   Binning                binningInfo;
   bool                   supportsHighSpeed;
   bool                   supportsUSBTraffic;
   Range                  usbTraffic;
   bool                   supportsGPS;
};

Q_DECLARE_METATYPE(QHYCamera::DataTransferMode)
