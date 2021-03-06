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
   Q_PROPERTY(QString id READ id)
   Q_PROPERTY(QString model READ model)
   Q_PROPERTY(QString readMode READ readMode NOTIFY readModeChanged)
   Q_PROPERTY(QStringList readModes READ readModes)

public:
   enum DataTransferMode
   {
      SingleImage = 0x00,
      LiveView    = 0x01
   };
   Q_ENUM(DataTransferMode)

   struct Binning
   {
      bool oneByOne{ false };
      bool twoByTwo{ false };
      bool threeByThree{ false };
      bool fourByFour{ false };
      int  binXMaximum{ 1 };
      int  binYMaximum{ 1 };
   } __attribute__((aligned(Align16Bit)));

   struct Range
   {
      double min{ 0.0 };
      double max{ 0.0 };
      double step{ 0.0 };
   } __attribute__((aligned(Align32Bit)));

   struct Capabilities
   {
      Range   rangeGain;//d
      Range   rangeOffset;//d
      Range   rangeUSBTraffic;//d
      Binning binningInfo; //d
      double  chipHeight; //d
      double  chipWidth;//d
      QString firmwareVersion;//d
      QString fpga1Version;//d
      QString fpga2Version;//d
      double  pixelHeight;//d
      double  pixelWidth;//d
      int     bayerMatrix;//d
      qint32  bitsPerPixel;//d
      int     filterWheelCapacity;//d
      qint32  imageHeight;//d
      qint32  imageWidth;//d
      int     maxFrameLength;//d
      bool    supports16Bit;//d
      bool    supportsBinning;//d
      bool    supportsChipChamberCyclePump; //d
      bool    supportsChipTempSensor; //d
      bool    supportsColor; //d
      bool    supportsCooler; //d
      bool    supportsFPNCalibration; //d
      bool    supportsFilterWheel; //d
      bool    supportsFineTone; //d
      bool    supportsGPS; //d
      bool    supportsGain; //d
      bool    supportsHighSpeed; //d
      bool    supportsHumidity; //d
      bool    supportsMechanicalShutter; //d
      bool    supportsOffset; //d
      bool    supportsPressure; //d
      bool    supportsShutterMotorHeating; //d
      bool    supportsSignalClamp; //d
      bool    supportsTECOverProtection; //d
      bool    supportsTrigger; //d
      bool    supportsUSBSpeedSetting; //d
      bool    supportsUSBTraffic;//d
   };

   explicit QHYCamera(QByteArray name, QObject * parent = nullptr);
   ~QHYCamera() noexcept override;

                      operator QString() const;

   [[nodiscard]] auto capabilities() const -> const Capabilities;

   /*!
    * Connects to the QHYCCD camera, and returns success.
    *
    * \return The success of connecting to the QHYCCD camera.
    */
   void               connect();
   void               disconnect();
   [[nodiscard]] auto isConnected() -> bool;
   [[nodiscard]] auto id() const -> QString;
   [[nodiscard]] auto model() const -> QString;
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
   QByteArray             m_id;
   QLatin1String          m_model;
   QString                m_readMode;
   QMap<QString, quint32> m_readModes;
   DataTransferMode       m_transferMode;
   Capabilities           m_capabilities;

   int                    bitDepth;
   double                 gain;
   double                 offset;
   bool                   tecProtectEnabled;
   bool                   clampSignalEnabled;
   bool                   slowestDownloadEnabled;
};

Q_DECLARE_METATYPE(QHYCamera::DataTransferMode)
