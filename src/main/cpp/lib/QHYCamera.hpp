#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "Config.h"
#include <ostream>
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
   } __attribute__((aligned(Align16Bit)));

   explicit QHYCamera(QByteArray name, QObject * parent = nullptr);
   ~QHYCamera() noexcept override;

   operator QString() const;

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
   void                  connectedChanged(bool connected);
   void                  readModeChanged(QString readMode);
   void                  transferModeChanged(QHYCamera::DataTransferMode mode);
   friend std::ostream & operator<<(std::ostream & os, const QHYCamera & camera);

private:
   void                   initializeReadModes();
   void                   readCameraDetails();
   void                   readChipInfo();
   void                   readControlValues();
   void                   readFirmwareVersion();
   void                   readFPGAVersion();

   qhyccd_handle *        handle;
   QLatin1String          model;
   QByteArray             m_id;
   QString                m_readMode;
   QMap<QString, quint32> m_readModes;
   DataTransferMode       m_transferMode;

   int                    bayerMatrix;
   Binning                binningInfo;
   int                    bitDepth;
   qint32                 bitsPerPixel;
   double                 chipHeight;
   double                 chipWidth;
   int                    filterCount;
   QString                firmwareVersion;
   QString                fpga1Version;
   QString                fpga2Version;
   double                 gain;
   qint32                 imageHeight;
   qint32                 imageWidth;
   int                    maxFrameLength;
   double                 offset;
   double                 pixelHeight;
   double                 pixelWidth;
   Range                  rangeGain;
   Range                  rangeOffset;
   Range                  rangeUSBTraffic;
   bool                   supports16Bit;
   bool                   supportsBinning;
   bool                   supportsChipChamberCyclePump;
   bool                   supportsChipTempSensor;
   bool                   supportsColor;
   bool                   supportsCooler;
   bool                   supportsFPNCalibration;
   bool                   supportsFilterWheel;
   bool                   supportsFineTone;
   bool                   supportsGPS;
   bool                   supportsGain;
   bool                   supportsHighSpeed;
   bool                   supportsHumidity;
   bool                   supportsMechanicalShutter;
   bool                   supportsOffset;
   bool                   supportsPressure;
   bool                   supportsShutterMotorHeating;
   bool                   supportsSignalClamp;
   bool                   supportsTECOverProtection;
   bool                   supportsTrigger;
   bool                   supportsUSBSpeedSetting;
   bool                   supportsUSBTraffic;
};

Q_DECLARE_METATYPE(QHYCamera::DataTransferMode)
