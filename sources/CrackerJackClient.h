#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CrackerJackClient.h"

class CrackerJackClient : public QMainWindow
{
    Q_OBJECT

public:
    CrackerJackClient(QWidget *parent = Q_NULLPTR);

private:
    Ui::CrackerJackClientClass ui;
};
