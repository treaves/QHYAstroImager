/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "QHYCamera.hpp"

#include <QDebug>
#include <QStringBuilder>
#include <QTimer>

#include <qhyccd.h>

QHYCamera::QHYCamera(QByteArray name, QObject * parent)
   : QObject(parent)
   , handle(nullptr)
   , model(name.left(name.lastIndexOf('-')))
   , m_id(name)
   , m_transferMode(SingleImage)
   , bayerMatrix(0)
   , bitDepth(0)
   , bitsPerPixel(0)
   , chipHeight(0.0)
   , chipWidth(0.0)
   , filterCount(0)
   , gain(0.0)
   , imageHeight(0.0)
   , imageWidth(0.0)
   , maxFrameLength(0)
   , offset(0.0)
   , pixelHeight(0.0)
   , pixelWidth(0.0)
   , supports16Bit(false)
   , supportsBinning(false)
   , supportsChipChamberCyclePump(false)
   , supportsChipTempSensor(false)
   , supportsColor(false)
   , supportsCooler(false)
   , supportsFPNCalibration(false)
   , supportsFilterWheel(false)
   , supportsFineTone(false)
   , supportsGPS(false)
   , supportsGain(false)
   , supportsHighSpeed(false)
   , supportsHumidity(false)
   , supportsMechanicalShutter(false)
   , supportsOffset(false)
   , supportsPressure(false)
   , supportsShutterMotorHeating(false)
   , supportsSignalClamp(false)
   , supportsTECOverProtection(false)
   , supportsTrigger(false)
   , supportsUSBSpeedSetting(false)
   , supportsUSBTraffic(false)
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
void QHYCamera::connect()
{
   handle = OpenQHYCCD(m_id.data());
   if (handle != nullptr) {
      initializeReadModes();
   }
   emit connectedChanged(isConnected());
}

void QHYCamera::disconnect()
{
   if (handle != nullptr) {
      quint32 qhyResult = CloseQHYCCD(handle);
      if (qhyResult == QHYCCD_SUCCESS) {
         handle = nullptr;
      } else {
         qWarning() << tr("There was an error disconnecting from %1.").arg(QLatin1String(m_id));
      }
   }
   emit connectedChanged(isConnected());
}

auto QHYCamera::isConnected() -> bool
{
   return handle != nullptr;
}

auto QHYCamera::name() const -> QString
{
   return QString(m_id);
}

auto QHYCamera::readMode() const -> QString
{
   return m_readMode;
}

auto QHYCamera::readModes() const -> QStringList
{
   return m_readModes.keys();
}

auto QHYCamera::transferMode() const -> DataTransferMode
{
   return m_transferMode;
}

/* ***************************************************************************************************************** */
// MARK: - Public slots
/* ***************************************************************************************************************** */
void QHYCamera::setReadAndTransferModes(QString readMode, QHYCamera::DataTransferMode mode)
{
   QTimer::singleShot(0, this, [this, readMode, mode]() {
      if (isConnected() && !readMode.isEmpty() && m_readMode != readMode) {
         // if m_readMode has been set, we must disconnect & reconnect the camera.
         if (!m_readMode.isEmpty()) {
            disconnect();
            connect();
         }
         if (isConnected()) {
            if (SetQHYCCDReadMode(handle, m_readModes.value(readMode)) == QHYCCD_SUCCESS) {
               m_readMode = readMode;
               emit readModeChanged(readMode);
               if (SetQHYCCDStreamMode(handle, mode) == QHYCCD_SUCCESS) {
                  if (InitQHYCCD(handle) == QHYCCD_SUCCESS) {
                     m_transferMode = mode;
                     emit transferModeChanged(mode);
                     readCameraDetails();
                  } else {
                     qWarning() << tr("Could not initialize camera %1").arg(QLatin1String(m_id));
                     disconnect();
                  }
               } else {
                  qWarning() << tr("Could not set stream mode of camera %1 to %2.").arg(QLatin1String(m_id)).arg(mode);
                  disconnect();
               }
            } else {
               qWarning() << tr("Could not set camera %1 read mode to %2 with index %3")
                              .arg(QLatin1String(m_id))
                              .arg(readMode)
                              .arg(m_readModes.value(readMode));
               disconnect();
            }
         }
      }
   });
}

/* ***************************************************************************************************************** */
// MARK: - Private methods
/* ***************************************************************************************************************** */
void QHYCamera::initializeReadModes()
{
   // read modes shouldn't change so once read, do not re-read.
   if (isConnected() && m_readModes.isEmpty()) {
      quint32 readModeCount = 0;
      if (GetQHYCCDNumberOfReadModes(handle, &readModeCount) != QHYCCD_SUCCESS) {
         disconnect();
      } else {
         qDebug() << "Found " << readModeCount << " read modes.";
         quint32 readModeIndex = 0;
         while (readModeIndex < readModeCount && handle != nullptr) {
            QByteArray readModeNameBuffer(BufferSizeReadModeName, 0);
            auto       status = GetQHYCCDReadModeName(handle, readModeIndex, readModeNameBuffer.data());
            if (status == QHYCCD_SUCCESS) {
               m_readModes[QString(readModeNameBuffer)] = readModeIndex;
               qDebug() << "Found " << QString(readModeNameBuffer) << "read mode.";
            } else {
               qWarning() << tr("GetQHYCCDReadModeName failed with code %1.").arg(status);
               disconnect();
            }
            readModeIndex++;
         }
      }
   }
}

void QHYCamera::readCameraDetails()
{
   readFirmwareVersion();
   readFPGAVersion();
   readChipInfo();
   readControlValues();
}

void QHYCamera::readChipInfo()
{
   if (isConnected()) {
      auto qhyResult = GetQHYCCDChipInfo(handle,
                                         &chipWidth,
                                         &chipHeight,
                                         reinterpret_cast<uint32_t *>(&imageWidth),  // NOLINT
                                         reinterpret_cast<uint32_t *>(&imageHeight), // NOLINT
                                         &pixelWidth,
                                         &pixelHeight,
                                         reinterpret_cast<uint32_t *>(&bitsPerPixel)); // NOLINT
      if (qhyResult != QHYCCD_SUCCESS) {
         qWarning() << tr("Error reading chip information for camera %1").arg(QLatin1String(m_id));
      }
   }
}

void QHYCamera::readControlValues()
{
   auto qhyResult = IsQHYCCDControlAvailable(handle, CAM_COLOR);
   if (qhyResult == QHYCCD_ERROR) {
      supportsColor = false;
   } else {
      supportsColor = true;
      bayerMatrix   = static_cast<int>(qhyResult);
   }

   supportsOffset = IsQHYCCDControlAvailable(handle, CONTROL_OFFSET) == QHYCCD_SUCCESS;
   if (supportsOffset) {
      qhyResult =
        GetQHYCCDParamMinMaxStep(handle, CONTROL_OFFSET, &rangeOffset.min, &rangeOffset.max, &rangeOffset.step);
      if (qhyResult == QHYCCD_ERROR) {
         rangeOffset.max  = 0.0;
         rangeOffset.min  = 0.0;
         rangeOffset.step = 0.0;
      }
   }
   offset       = GetQHYCCDParam(handle, CONTROL_OFFSET);

   supportsGain = IsQHYCCDControlAvailable(handle, CONTROL_GAIN) == QHYCCD_SUCCESS;
   if (supportsGain) {
      qhyResult = GetQHYCCDParamMinMaxStep(handle, CONTROL_GAIN, &rangeGain.min, &rangeGain.max, &rangeGain.step);
      if (qhyResult == QHYCCD_ERROR) {
         rangeGain.max  = 0.0;
         rangeGain.min  = 0.0;
         rangeGain.step = 0.0;
      }
   }
   gain = GetQHYCCDParam(handle, CONTROL_GAIN);

   if (IsQHYCCDControlAvailable(handle, CAM_BIN1X1MODE) == QHYCCD_SUCCESS) {
      binningInfo.binXMaximum = 1;
      binningInfo.binYMaximum = 1;
      binningInfo.oneByOne    = true;
      supportsBinning         = true;
   }
   if (IsQHYCCDControlAvailable(handle, CAM_BIN2X2MODE) == QHYCCD_SUCCESS) {
      binningInfo.binXMaximum = 2;
      binningInfo.binYMaximum = 2;
      binningInfo.twoByTwo    = true;
      supportsBinning         = true;
   }
   if (IsQHYCCDControlAvailable(handle, CAM_BIN3X3MODE) == QHYCCD_SUCCESS) {
      binningInfo.binXMaximum  = 3;
      binningInfo.binYMaximum  = 3;
      binningInfo.threeByThree = true;
      supportsBinning          = true;
   }
   if (IsQHYCCDControlAvailable(handle, CAM_BIN4X4MODE) == QHYCCD_SUCCESS) {
      binningInfo.binXMaximum = 4;
      binningInfo.binYMaximum = 4;
      binningInfo.fourByFour  = true;
      supportsBinning         = true;
   }

   supportsHighSpeed  = IsQHYCCDControlAvailable(handle, CONTROL_SPEED) == QHYCCD_SUCCESS;
   supportsUSBTraffic = IsQHYCCDControlAvailable(handle, CONTROL_USBTRAFFIC) == QHYCCD_SUCCESS;
   if (supportsUSBTraffic) {
      qhyResult = GetQHYCCDParamMinMaxStep(
        handle, CONTROL_USBTRAFFIC, &rangeUSBTraffic.min, &rangeUSBTraffic.max, &rangeUSBTraffic.step);
      if (qhyResult == QHYCCD_ERROR) {
         rangeUSBTraffic.max  = 0.0;
         rangeUSBTraffic.min  = 0.0;
         rangeUSBTraffic.step = 0.0;
      }
   }

   supportsGPS = IsQHYCCDControlAvailable(handle, CAM_GPS) == QHYCCD_SUCCESS;

   if (IsQHYCCDControlAvailable(handle, CONTROL_TRANSFERBIT) == QHYCCD_SUCCESS) {
      if (IsQHYCCDControlAvailable(handle, CAM_16BITS) == QHYCCD_SUCCESS) {
         supports16Bit = true;
         bitDepth      = BitDepth16;
      } else {
         supports16Bit = false;
         bitDepth      = BitDepth8;
      }
      qhyResult = SetQHYCCDParam(handle, CONTROL_TRANSFERBIT, bitDepth);
   }

   if (IsQHYCCDControlAvailable(handle, CONTROL_CFWPORT) == QHYCCD_SUCCESS) {
      auto filtersSupported = GetQHYCCDParam(handle, CONTROL_CFWSLOTSNUM);
      if (filtersSupported > 9) {
         filterCount = 9;
      } else {
         filterCount = static_cast<int>(filtersSupported);
      }
   }

   supportsCooler              = IsQHYCCDControlAvailable(handle, CONTROL_COOLER) == QHYCCD_SUCCESS;
   supportsHumidity            = IsQHYCCDControlAvailable(handle, CAM_HUMIDITY) == QHYCCD_SUCCESS;
   supportsPressure            = IsQHYCCDControlAvailable(handle, CAM_PRESSURE) == QHYCCD_SUCCESS;
   supportsMechanicalShutter   = IsQHYCCDControlAvailable(handle, CAM_MECHANICALSHUTTER) == QHYCCD_SUCCESS;
   supportsTrigger             = IsQHYCCDControlAvailable(handle, CAM_TRIGER_INTERFACE) == QHYCCD_SUCCESS;
   supportsShutterMotorHeating = IsQHYCCDControlAvailable(handle, CAM_SHUTTERMOTORHEATING_INTERFACE) == QHYCCD_SUCCESS;
   supportsTECOverProtection   = IsQHYCCDControlAvailable(handle, CAM_TECOVERPROTECT_INTERFACE) == QHYCCD_SUCCESS;
   supportsSignalClamp         = IsQHYCCDControlAvailable(handle, CAM_SINGNALCLAMP_INTERFACE) == QHYCCD_SUCCESS;
   supportsFPNCalibration      = IsQHYCCDControlAvailable(handle, CAM_CALIBRATEFPN_INTERFACE) == QHYCCD_SUCCESS;
   supportsChipTempSensor  = IsQHYCCDControlAvailable(handle, CAM_CHIPTEMPERATURESENSOR_INTERFACE) == QHYCCD_SUCCESS;
   supportsUSBSpeedSetting = IsQHYCCDControlAvailable(handle, CAM_USBREADOUTSLOWEST_INTERFACE) == QHYCCD_SUCCESS;
   supportsChipChamberCyclePump = IsQHYCCDControlAvailable(handle, CONTROL_SensorChamberCycle_PUMP) == QHYCCD_SUCCESS;

   maxFrameLength               = static_cast<int>(GetQHYCCDMemLength(handle));
}

void QHYCamera::readFirmwareVersion()
{
   if (isConnected()) {
      std::array<quint8, BufferSizeFirmwareVersion> firmwareVersionBuffer{ 0 };
      auto qhyResult = GetQHYCCDFWVersion(handle, firmwareVersionBuffer.data());
      if (qhyResult == QHYCCD_SUCCESS) {
         auto year = firmwareVersionBuffer[0] >> 4U;
         if (year < 10) { // NOLINT
            year += 0x10; // NOLINT
         }
         firmwareVersion = QString("20%1-%2-%3")
                             .arg(year)
                             .arg(firmwareVersionBuffer[0] & ~0xf0U)
                             .arg(firmwareVersionBuffer[1]); // NOLINT
         qDebug() << "Firmware version:" % firmwareVersion;
      } else {
         qWarning() << "Error reading GetQHYCCDFWVersion";
      }
   }
}

void QHYCamera::readFPGAVersion()
{
   if (isConnected()) {
      std::array<quint8, BufferSizeFirmwareVersion> fpgaVersionBuffer{ 0 };
      auto qhyResult = GetQHYCCDFPGAVersion(handle, 0, fpgaVersionBuffer.data());
      if (qhyResult == QHYCCD_SUCCESS) {
         fpga1Version = QString("%1-%2-%3-%4")
                          .arg(fpgaVersionBuffer[0])
                          .arg(fpgaVersionBuffer[1])
                          .arg(fpgaVersionBuffer[2])
                          .arg(fpgaVersionBuffer[3]);
         qDebug() << "FPGA1 version:" % fpga1Version;

         qhyResult = GetQHYCCDFPGAVersion(handle, 1, fpgaVersionBuffer.data());
         if (qhyResult == QHYCCD_SUCCESS) {
            fpga2Version = QString("%1-%2-%3-%4")
                             .arg(fpgaVersionBuffer[0])
                             .arg(fpgaVersionBuffer[1])
                             .arg(fpgaVersionBuffer[2])
                             .arg(fpgaVersionBuffer[3]);
            qDebug() << "FPGA2 version:" % fpga2Version;
         } else {
            qWarning() << "Error reading second GetQHYCCDFPGAVersion";
         }
      } else {
         qWarning() << "Error reading first GetQHYCCDFPGAVersion";
      }
   }
}
/* ****************************************************************************************************************** */
// MARK: - Operators
/* ****************************************************************************************************************** */
QHYCamera::operator QString() const
{
   return QString("QHYCamera[") % "]";
}
