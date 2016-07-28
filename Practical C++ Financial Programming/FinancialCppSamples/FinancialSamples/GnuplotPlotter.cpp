//
//  GnuplotPlotter.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "GnuplotPlotter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

using std::ofstream;
using std::vector;
using std::cout;
using std::endl;

GnuplotPlotter::GnuplotPlotter(const std::string &fileName)
: m_fileName(fileName),
  m_isDate(false)
{
}

GnuplotPlotter::GnuplotPlotter(const GnuplotPlotter &p)
: m_fileName(p.m_fileName),
  m_xheader(p.m_xheader),
  m_yheader(p.m_yheader),
  m_xdata(p.m_xdata),
  m_ydata(p.m_ydata),
  m_isDate(p.m_isDate)
{
}

GnuplotPlotter::~GnuplotPlotter()
{
}

GnuplotPlotter &GnuplotPlotter::operator=(const GnuplotPlotter &p)
{
	if (&p != this)
	{
		m_fileName = p.m_fileName;
		m_xheader = p.m_xheader;
		m_yheader = p.m_yheader;
		m_xdata = p.m_xdata;
		m_ydata = p.m_ydata;
		m_isDate = p.m_isDate;
	}
    return *this;
}

void GnuplotPlotter::setData(const std::vector<std::string> &xdata, const std::vector<double> &ydata)
{
	m_xdata = xdata;
	m_ydata = ydata;
	m_isDate = true; // assume that x-axis is a date
}

void GnuplotPlotter::setData(const std::vector<double> &xdata, const std::vector<double> &ydata)
{
	for (unsigned i = 0; i < xdata.size(); ++i)
	{
		std::stringstream ss;
		ss << xdata[i];
		m_xdata.push_back(ss.str());
	}
	m_ydata = ydata;
	m_isDate = false; // x-axis cannot be a date.
}

void GnuplotPlotter::setHeaders(const std::string &xheader, const std::string &yheader)
{
	m_xheader = xheader;
	m_yheader = yheader;
}

void GnuplotPlotter::generateCmds(const std::string &cmdFileName)
{
	ofstream file;

	file.open(cmdFileName.c_str());
	if (file.fail())
	{
		cout << "failed to open file " << m_fileName << endl;
		return;
	}

	if (m_isDate)
	{
		file << "set xdata time"  << endl;
		file << "set timefmt \"%Y-%m-%d\" "  << endl;
	}
	file << "set datafile separator \",\" "  << endl;
	file << "plot '" << m_fileName <<  "'  u 1:2  title columnhead w lines " << endl;
}

void GnuplotPlotter::csvWrite()
{
	ofstream file;

	file.open(m_fileName.c_str());
	if (file.fail())
	{
		cout << "failed to open file " << m_fileName << endl;
		return;
	}

	if (m_xdata.size() != m_ydata.size())
	{
		cout << "data has incorrect size " << endl;
		return;
	}

	file << m_xheader << "," << m_yheader << endl;

	for (unsigned i = 0; i < m_xdata.size(); ++i)
	{
		file << m_xdata[i] << "," << m_ydata[i] << endl;
	}
}

int main_gp()
{
    GnuplotPlotter plotter("test.csv");
    plotter.setHeaders("x", "sin(x)");

    vector<double> xdata;
    vector<double> ydata;
    for (int i=0; i<100; ++i)
    {
        double x = i*10/100.0;
        xdata.push_back(x);
        ydata.push_back(sin(x));
    }

    plotter.setData(xdata, ydata);
    plotter.csvWrite();
    plotter.generateCmds("testcmds.gp");
    return 0;
}
