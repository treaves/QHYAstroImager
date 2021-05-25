/**
 * Copyright © 2021 Timothy Reaves
 *
 * For the license, see the root LICENSE file.
 */

#include "About.hpp"
#include "ui_About.h"

#include "Config.h"


About::About(QWidget * parent)
  : QDialog(parent)
  , ui(new Ui::About)
{
   ui->setupUi(this);
}

About::~About()
{
   delete ui;
}

