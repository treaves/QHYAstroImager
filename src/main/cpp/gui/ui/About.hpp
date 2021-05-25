#pragma once

/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include <QDialog>

namespace Ui
{
   class About;
}

class About : public QDialog
{
Q_OBJECT
   Q_DISABLE_COPY_MOVE(About)

public:
   explicit About(QWidget * parent = nullptr);
   ~About() override;

private:
   Ui::About * ui;
};

