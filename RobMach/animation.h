#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QWidget>
#include "solverpos.h"
#include<QTextEdit>
class animation
{
public:
    animation();
    void start();
    bool iscommon();
public:
    SolverPos *pos;
    double speed = 1;           //动画速度
    bool sign_delete = false;   //动画是否强制结束
    bool sign_pause = false;   //动画是否暂停
    QTextEdit *information_text;

};

#endif // ANIMATION_H
