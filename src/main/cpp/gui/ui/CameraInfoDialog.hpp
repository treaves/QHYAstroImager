#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QDialog>

class QHYCamera;

namespace Ui
{
   class CameraInfoDialog;
}

class CameraInfoDialog : public QDialog
{
   Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
   Q_DISABLE_COPY_MOVE(CameraInfoDialog)
#endif

public:
   explicit CameraInfoDialog(QHYCamera * camera, QWidget * parent = nullptr);
   ~CameraInfoDialog();

private:
   Ui::CameraInfoDialog * ui;
};
