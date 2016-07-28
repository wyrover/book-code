//
//  PlotViewer.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "PlotViewer.h"

#include <QtGui/qapplication.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qpainter.h>

class MainWindow : public QMainWindow {
public:
    MainWindow();
    ~MainWindow();
};