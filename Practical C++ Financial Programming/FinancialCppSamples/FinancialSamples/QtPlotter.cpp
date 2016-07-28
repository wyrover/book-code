//
//  QtPlotter.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "QtPlotter.h"

#include <QtGui/qapplication.h>
#include <QtGui/qmainwindow.h>
#include <QtGui/qpainter.h>

#include <algorithm>
#include <cmath>

#include <iostream>


using std::vector;

class PlotWindow : public QMainWindow {
public:
    PlotWindow();
    ~PlotWindow();
    void paintEvent(QPaintEvent *event);
    void setData(const vector<double> &xdata, const vector<double> &ydata);
private:
    vector<double> m_xdata;
    vector<double> m_ydata;
};

PlotWindow::PlotWindow()
{
}

PlotWindow::~PlotWindow()
{
}

void PlotWindow::setData(const vector<double> &xdata, const vector<double> &ydata)
{
    m_xdata = xdata;
    m_ydata = ydata;
}

void PlotWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);

    // define margins
    double marginX = 10;
    double marginY = 10;
    double lengthX = 500;
    double lengthY = 400;


    // define axis
    int maxX = lengthX, maxY = lengthY;
    painter.drawLine(marginX,marginY, marginX, lengthY+marginY);
    painter.drawLine(marginX,lengthY + marginY, lengthX, lengthY + marginY);

    // find units
    int largeX = 0, largeY = 0;
    double largeXd = 0, largeYd = 0;
    for (unsigned i=1; i<m_xdata.size(); ++i)
    {
        if (largeXd < m_xdata[i]) largeXd = m_xdata[i];
        if (largeYd < m_ydata[i]) largeYd = m_ydata[i];
    }
    largeX = (int)largeXd + 1;
    largeY = (int)largeYd + 1;

    int unitX = maxX / largeX;
    int unitY = maxY / largeY;

    // paint ticks
    for (int i=0; i<largeY; ++i)
    {
        painter.drawLine(marginX-5, i*unitY+marginY, marginX, i*unitY+marginY);
    }
    for (int i=0; i<largeX; ++i)
    {
        painter.drawLine(marginX+i*unitX, lengthY+marginY, marginX+i*unitX, lengthY+5+marginY);
    }

    // draw plot
    for (unsigned i=1; i<m_xdata.size(); ++i)
    {
        painter.drawLine(marginX+unitX*m_xdata[i-1], unitY*m_ydata[i-1]+marginY,
                         marginX+unitX*m_xdata[i], unitY*m_ydata[i]+marginY);
    }
}


QtPlotter::QtPlotter()
{
}

QtPlotter::~QtPlotter()
{
}

QtPlotter::QtPlotter(const QtPlotter&p)
: m_xdata(p.m_xdata),
  m_ydata(p.m_ydata)
{

}

QtPlotter &QtPlotter::operator=(const QtPlotter &p)
{
    if (&p != this)
    {
        m_xdata = p.m_xdata;
        m_ydata = p.m_ydata;
    }
    return *this;
}

void QtPlotter::setData(const std::vector<double> &xdata, const std::vector<double> &ydata)
{
    m_xdata = xdata;
    m_ydata = ydata;
}

int QtPlotter::plotWindowRun()
{
    char *arg = (char *)"plotter";
    int argc = 1;
    return plotWindowRun(argc, &arg);
}

int QtPlotter::plotWindowRun(int argc, char **argv)
{
    QApplication app(argc, argv);

    app.setApplicationName("Qt Plotter");
    PlotWindow window;

    window.resize(600, 600);
    window.show();
    return app.exec();
}

int main_qt()
{
    QtPlotter plotter;

    vector<double> xdata;
    vector<double> ydata;
    for (int i=0; i<100; ++i)
    {
        double x = i*10/100.0;
        xdata.push_back(x);
        ydata.push_back(sin(x)+1);
    }


    plotter.setData(xdata, ydata);

    return plotter.plotWindowRun();
}
