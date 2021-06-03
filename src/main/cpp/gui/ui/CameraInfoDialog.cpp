#include "CameraInfoDialog.hpp"
#include "ui_CameraInfoDialog.h"

#include "QHYCamera.hpp"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>

CameraInfoDialog::CameraInfoDialog(QHYCamera * camera, QWidget * parent)
   : QDialog(parent)
   , ui(new Ui::CameraInfoDialog)
{
   ui->setupUi(this);
   ui->treeWidget->setColumnCount(2);

   ui->treeWidget->addTopLevelItem(
     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList{ tr("Model"), camera->model() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Bayers Matrix"), QString::number(camera->capabilities().bayerMatrix) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Binning support"), QVariant(camera->capabilities().supportsBinning).toString() }));

   auto * binningInfo = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList{ tr("Binning") });
   new QTreeWidgetItem(
     binningInfo, QStringList{ tr("Support 1x1"), QVariant(camera->capabilities().binningInfo.oneByOne).toString() });
   new QTreeWidgetItem(
     binningInfo, QStringList{ tr("Support 2x2"), QVariant(camera->capabilities().binningInfo.twoByTwo).toString() });
   new QTreeWidgetItem(
     binningInfo,
     QStringList{ tr("Support 3x3"), QVariant(camera->capabilities().binningInfo.threeByThree).toString() });
   new QTreeWidgetItem(
     binningInfo, QStringList{ tr("Support 4x4"), QVariant(camera->capabilities().binningInfo.fourByFour).toString() });
   ui->treeWidget->addTopLevelItem(binningInfo);

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("16 bit support"), QVariant(camera->capabilities().supports16Bit).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Bits per pixel"), QString::number(camera->capabilities().bitsPerPixel) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{
         tr("Chip Size"),
         QString("%1w x %2h").arg(camera->capabilities().chipWidth).arg(camera->capabilities().chipHeight) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Chip Chamber Cycle Pump support"),
                                      QVariant(camera->capabilities().supportsChipChamberCyclePump).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Chip cooler support"), QVariant(camera->capabilities().supportsCooler).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Chip temperature sensor support"),
                                      QVariant(camera->capabilities().supportsChipTempSensor).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Color support"), QVariant(camera->capabilities().supportsColor).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Filter wheel support"), QVariant(camera->capabilities().supportsFilterWheel).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Filter wheel capacity"), QString::number(camera->capabilities().filterWheelCapacity) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Fine tone support"), QVariant(camera->capabilities().supportsFineTone).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Firmware version"), camera->capabilities().firmwareVersion }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("FPGA #1 firmware version"), camera->capabilities().fpga1Version }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("FPGA #2 firmware version"), camera->capabilities().fpga2Version }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("FPN calibration support"),
                                      QVariant(camera->capabilities().supportsFPNCalibration).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("GPS support"), QVariant(camera->capabilities().supportsGPS).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Gain support"), QVariant(camera->capabilities().supportsGain).toString() }));

   auto * gainInfo = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList{ tr("Gain Range") });
   new QTreeWidgetItem(gainInfo, QStringList{ tr("Minimum"), QString::number(camera->capabilities().rangeGain.min) });
   new QTreeWidgetItem(gainInfo, QStringList{ tr("Maximum"), QString::number(camera->capabilities().rangeGain.max) });
   new QTreeWidgetItem(gainInfo,
                       QStringList{ tr("Step size"), QString::number(camera->capabilities().rangeGain.step) });
   ui->treeWidget->addTopLevelItem(gainInfo);

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("High speed support"), QVariant(camera->capabilities().supportsHighSpeed).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Humidity sensor support"), QVariant(camera->capabilities().supportsHumidity).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{
         tr("Image"),
         QString("%1w x %2h").arg(camera->capabilities().imageWidth).arg(camera->capabilities().imageHeight) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Maximum frame length"), QString::number(camera->capabilities().maxFrameLength) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Mechanical shutter support"),
                                      QVariant(camera->capabilities().supportsMechanicalShutter).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Offset support"), QVariant(camera->capabilities().supportsOffset).toString() }));

   auto * offsetInfo = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList{ tr("Offset Range") });
   new QTreeWidgetItem(offsetInfo,
                       QStringList{ tr("Minimum"), QString::number(camera->capabilities().rangeOffset.min) });
   new QTreeWidgetItem(offsetInfo,
                       QStringList{ tr("Maximum"), QString::number(camera->capabilities().rangeOffset.max) });
   new QTreeWidgetItem(offsetInfo,
                       QStringList{ tr("Step size"), QString::number(camera->capabilities().rangeOffset.step) });
   ui->treeWidget->addTopLevelItem(offsetInfo);

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{
         tr("Pixel size"),
         QString("%1w x %2h").arg(camera->capabilities().pixelWidth).arg(camera->capabilities().pixelHeight) }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Pressure sensor support"), QVariant(camera->capabilities().supportsPressure).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("Shutter motor heater support"),
                                      QVariant(camera->capabilities().supportsShutterMotorHeating).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Signal clamping support"), QVariant(camera->capabilities().supportsSignalClamp).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("TEC overload protection support"),
                                      QVariant(camera->capabilities().supportsTECOverProtection).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("Trigger signal support"), QVariant(camera->capabilities().supportsTrigger).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                         QStringList{ tr("USB speed setting support"),
                                      QVariant(camera->capabilities().supportsUSBSpeedSetting).toString() }));

   ui->treeWidget->addTopLevelItem(

     new QTreeWidgetItem(
       static_cast<QTreeWidget *>(nullptr),
       QStringList{ tr("USB traffic support"), QVariant(camera->capabilities().supportsUSBTraffic).toString() }));

   auto * usbTrafficInfo =
     new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList{ tr("USB Traffic Range") });
   new QTreeWidgetItem(usbTrafficInfo,
                       QStringList{ tr("Minimum"), QString::number(camera->capabilities().rangeUSBTraffic.min) });
   new QTreeWidgetItem(usbTrafficInfo,
                       QStringList{ tr("Maximum"), QString::number(camera->capabilities().rangeUSBTraffic.max) });
   new QTreeWidgetItem(usbTrafficInfo,
                       QStringList{ tr("Step size"), QString::number(camera->capabilities().rangeUSBTraffic.step) });
   ui->treeWidget->addTopLevelItem(usbTrafficInfo);
}

CameraInfoDialog::~CameraInfoDialog()
{
   delete ui;
}
