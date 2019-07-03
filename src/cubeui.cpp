#include "cubeui.h"
#include <QGridLayout>
#include <QFont>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QStringList>


#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <ctime>
#include <limits>

using std::cout;
using std::endl;
using std::unordered_map;
using namespace QtCharts;

extern "C" {
#include "search.h"
}

void CubeUi::setupFaces(char corners[8][4], char edges[12][3])
{
	f[0] = corners[0][0];
	f[1] = edges[1][1];
	f[2] = corners[1][0];
	f[3] = edges[0][1];
	f[4] = 'F';
	f[5] = edges[2][1];
	f[6] = corners[2][0];
	f[7] = edges[3][1];
	f[8] = corners[3][0];
	r[0] = corners[1][1];
	r[1] = edges[9][0];
	r[2] = corners[6][2];
	r[3] = edges[2][0];
	r[4] = 'R';
	r[5] = edges[6][0];
	r[6] = corners[3][2];
	r[7] = edges[8][1];
	r[8] = corners[4][1];
	u[0] = corners[7][2];
	u[1] = edges[5][0];
	u[2] = corners[6][1];
	u[3] = edges[10][0];
	u[4] = 'U';
	u[5] = edges[9][1];
	u[6] = corners[0][1];
	u[7] = edges[1][0];
	u[8] = corners[1][2];
	d[0] = corners[2][2];
	d[1] = edges[3][0];
	d[2] = corners[3][1];
	d[3] = edges[11][1];
	d[4] = 'D';
	d[5] = edges[8][0];
	d[6] = corners[5][1];
	d[7] = edges[7][0];
	d[8] = corners[4][2];
	l[0] = corners[7][1];
	l[1] = edges[10][1];
	l[2] = corners[0][2];
	l[3] = edges[4][0];
	l[4] = 'L';
	l[5] = edges[0][0];
	l[6] = corners[5][2];
	l[7] = edges[11][0];
	l[8] = corners[2][1];
	b[0] = corners[6][0];
	b[1] = edges[5][1];
	b[2] = corners[7][0];
	b[3] = edges[6][1];
	b[4] = 'B';
	b[5] = edges[4][1];
	b[6] = corners[4][0];
	b[7] = edges[7][1];
	b[8] = corners[5][0];
}

char* CubeUi::scramble()
{
	char rep[64] = "";
	char corners[8][4] = {"FUL", "FRU", "FLD", "FDR", "BRD", "BDL", "BUR", "BLU"};
	char edges[12][3] = {"LF", "UF", "RF", "DF", "LB", "UB", "RB", "DB", "DR", "RU", "UL", "LD"};
	srand(time(NULL));

	char *sol = NULL;
	while (sol == NULL) {
		std::random_shuffle(corners, corners+8);
		std::random_shuffle(edges, edges+12);
		for (int i = 0; i < 8; i++) {
			std::rotate(corners[i], corners[i]+(rand()%3), corners[i]+3);
		}
		for (int i = 0; i < 12; i++) {
			std::rotate(edges[i], edges[i]+(rand()%2), edges[i]+2);
		}
		setupFaces(corners, edges);
		int count = 0;
		for (int i = 0; i < 9; i++) {
			rep[count++] = u[i];
		}
		for (int i = 0; i < 9; i++) {
			rep[count++] = r[i];
		}
		for (int i = 0; i < 9; i++) {
			rep[count++] = f[i];
		}
		for (int i = 0; i < 9; i++) {
			rep[count++] = d[i];
		}
		for (int i = 0; i < 9; i++) {
			rep[count++] = l[i];
		}
		for (int i = 0; i < 9; i++) {
			rep[count++] = b[i];
		}
		rep[count] = '\0';
		char patternized[64];
		patternize("UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB", rep, patternized);
		strcpy(rep, patternized);
		sol = solution(
			rep,
			24,
			1000,
			0,
			"cache"
		      );
	}
	drawImage();
	return sol;
}

void CubeUi::drawImage()
{
	QImage myImage(480, 360, QImage::Format_RGB32);
	unordered_map<char, QColor> colormap;
	colormap['U'] = QColor(255, 255, 255, 127);
	colormap['F'] = QColor(0, 255, 0, 127);
	colormap['L'] = QColor(255, 165, 0, 127);
	colormap['R'] = QColor(255, 0, 0, 127);
	colormap['D'] = QColor(255, 255, 0, 127);
	colormap['B'] = QColor(0, 0, 255, 127);

	// background
	for (unsigned i = 0; i < 480; i++) {
		for (unsigned j = 0; j < 360; j++) {
			myImage.setPixelColor(i, j, QColor(0, 0, 0, 0));
		}
	}

	// white
	for (int i = 120; i < 240; i++) {
		for (int j = 0; j < 120; j++) {
			int x = (i - 120)/40;
			int y = (j)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[u[ind]]);
		}
	}

	// green
	for (int i = 120; i < 240; i++) {
		for (int j = 120; j < 240; j++) {
			int x = (i - 120)/40;
			int y = (j - 120)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[f[ind]]);
		}
	}

	// orange
	for (int i = 0; i < 120; i++) {
		for (int j = 120; j < 240; j++) {
			int x = (i - 0)/40;
			int y = (j - 120)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[l[ind]]);
		}
	}

	// red
	for (int i = 240; i < 360; i++) {
		for (int j = 120; j < 240; j++) {
			int x = (i - 240)/40;
			int y = (j - 120)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[r[ind]]);
		}
	}

	// blue
	for (int i = 360; i < 480; i++) {
		for (int j = 120; j < 240; j++) {
			int x = (i - 360)/40;
			int y = (j - 120)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[b[ind]]);
		}
	}

	// yellow
	for (int i = 120; i < 240; i++) {
		for (int j = 240; j < 360; j++) {
			int x = (i - 120)/40;
			int y = (j - 240)/40;
			int ind = y * 3 + x;
			myImage.setPixelColor(i, j, colormap[d[ind]]);
		}
	}

	for (int i = 0; i < 480; i++) {
		for (int j = 0; j < 360; j++) {
			if (i % 40 == 0 || j % 40 == 0) {
				myImage.setPixelColor(i, j, QColor(0, 0, 0, 0));
			}
			if ((i-1) % 120 == 0 || (j-1) % 120 == 0) {
				myImage.setPixelColor(i, j, QColor(0, 0, 0, 0));
			}
			if ((i+1) % 120 == 0 || (j+1) % 120 == 0) {
				myImage.setPixelColor(i, j, QColor(0, 0, 0, 0));
			}
		}
	}

	scrambleImage->setPixmap(QPixmap::fromImage(myImage));

	scrambleImage->show();
}

void CubeUi::keyPressEvent(QKeyEvent *e)
{
	if (timer_1s->isActive()) {
		float elapsed = timer.elapsed()/1000.0;
		timer_1s->stop();
		std::ofstream outfile;
		if (elapsed > 0.01) {
			outfile.open("times.csv", std::ios_base::app);
			std::string date = QDateTime::currentDateTime().toString().toUtf8().constData();
			outfile << date << "," << elapsed << "," << currentScramble << endl;
			outfile.close();
			times.push_back(elapsed);
			updateAvg();
			QLineSeries* series = new QLineSeries();
			series->setName("Single");
			for (unsigned int i = 0; i < times.size(); i++) {
				if (times[i] == 1.0e+6) {
					continue;
				}
				series->append(i, times[i]);
			}
			QLineSeries* ao5series = new QLineSeries();
			ao5series->setName("ao5");
			for (unsigned int i = 0; i < ao5times.size(); i++) {
				ao5series->append(i, ao5times[i]);
			}
			chart->removeAxis(axisY);
			chart->removeAllSeries();
			chart->addSeries(series);
			chart->addSeries(ao5series);
			axisY = new QValueAxis();
			axisY->setTitleText("Time in Seconds");
			chart->addAxis(axisY, Qt::AlignLeft);
			series->attachAxis(axisY);
			ao5series->attachAxis(axisY);
			QTableWidgetItem *timeW = new QTableWidgetItem(QString::number(elapsed));
			QTableWidgetItem *scrambleW = new QTableWidgetItem(currentScramble.c_str());
			QTableWidgetItem *dateW = new QTableWidgetItem(date.c_str());
			tableWidget->setRowCount(tableWidget->rowCount()+1);
			tableWidget->setItem(tableWidget->rowCount() - 1, 0, dateW);
			tableWidget->setItem(tableWidget->rowCount() - 1, 1, timeW);
			tableWidget->setItem(tableWidget->rowCount() - 1, 2, scrambleW);
			onScramble();
			toStart = false;
		}
	}
}

void CubeUi::keyReleaseEvent(QKeyEvent *e)
{
	if (e->key() != Qt::Key_Space) {
		return;
	}
	if (!timer_1s->isActive() && toStart) {
		timer_1s->start(10);
		timer.start();
	} else if (toStart == false) {
		toStart = true;
	}
}

void CubeUi::setupTable()
{
	tableWidget = new QTableWidget(0, 3, this);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	QStringList headings;
	headings << QString("Date") << QString("Time") << QString("Scramble");
	tableWidget->setHorizontalHeaderLabels(headings);
	std::ifstream ifile("times.csv");
	std::string curline;
	while (std::getline(ifile, curline)) {
		std::stringstream linestream(curline);
		std::string date, time, scramble;
		std::getline(linestream, date, ',');
		std::getline(linestream, time, ',');
		std::getline(linestream, scramble, ',');
		float t = std::stof(time);
		if (t == 1.0e+6) {
			time = "DNF";
		}
		times.push_back(t);
		tableWidget->setRowCount(tableWidget->rowCount()+1);
		QTableWidgetItem *timeW = new QTableWidgetItem(time.c_str());
		QTableWidgetItem *scrambleW = new QTableWidgetItem(scramble.c_str());
		QTableWidgetItem *dateW = new QTableWidgetItem(date.c_str());
		tableWidget->setItem(tableWidget->rowCount() - 1, 0, dateW);
		tableWidget->setItem(tableWidget->rowCount() - 1, 1, timeW);
		tableWidget->setItem(tableWidget->rowCount() - 1, 2, scrambleW);
	}
	ifile.close();
}

void CubeUi::setupUi()
{
	timer_1s = new QTimer(this);
	QObject::connect(timer_1s, SIGNAL(timeout()), this, SLOT(updateTime()));

	toStart = false;

	lbl = new QLabel("0", this);
	timelbl = new QLabel("0", this);
	std::string dummylabel;
	for (int i = 0; i < 250; i++) {
		dummylabel += " ";
	}
	ao = new QLabel("", this);
	scrambleImage = new QLabel("", this);
	dummy = new QLabel(dummylabel.c_str(), this);
	scrambleBtn = new QPushButton("New Scramble", this);
	QObject::connect(scrambleBtn, &QPushButton::clicked, this, &CubeUi::onScramble);
	plus2Btn = new QPushButton("+2", this);
	QObject::connect(plus2Btn, &QPushButton::clicked, this, &CubeUi::onPlus2);
	dnfBtn = new QPushButton("DNF", this);
	QObject::connect(dnfBtn, &QPushButton::clicked, this, &CubeUi::onDnf);
	deleteBtn = new QPushButton("X", this);
	QObject::connect(deleteBtn, &QPushButton::clicked, this, &CubeUi::onDelete);

	QFont f( "Courier", 50, QFont::Bold);
	timelbl->setFont(f);
	timelbl->setAlignment(Qt::AlignCenter);
	QFont f2( "Courier", 15);
	QFont f3( "Courier", 10, QFont::Bold);
	lbl->setFont(f2);
	lbl->setAlignment(Qt::AlignCenter);
	ao->setFont(f3);
	ao->setAlignment(Qt::AlignCenter);
	scrambleImage->setAlignment(Qt::AlignCenter);
}

void CubeUi::setupGraph()
{
	chart = new QChart;
	chart->setTheme(QChart::ChartThemeDark);
	QLineSeries* series = new QLineSeries();
	series->setName("Single");
	for (unsigned int i = 0; i < times.size(); i++) {
		if (times[i] == 1.0e+6) {
			continue;
		}
		series->append(i, times[i]);
	}
	chart->addSeries(series);
	axisY = new QValueAxis();
	axisY->setTitleText("Time in seconds");
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
}

CubeUi::CubeUi(QWidget *parent) : QWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	setupTable();
	setupUi();
	setupGraph();

	QChartView *chartView = new QChartView(chart);

	QGridLayout *grid = new QGridLayout(this);
	grid->addWidget(scrambleBtn, 0, 0, 1, 1);
	grid->addWidget(plus2Btn, 0, 1, 1, 1);
	grid->addWidget(dnfBtn, 0, 2, 1, 1);
	grid->addWidget(deleteBtn, 0, 3, 1, 1);
	grid->addWidget(chartView, 0, 4, 5, 4);
	grid->addWidget(scrambleImage, 5, 4, 5, 4);
	grid->addWidget(ao, 1, 0, 1, 4);
	grid->addWidget(lbl, 2, 0, 1, 4);
	grid->addWidget(timelbl, 3, 0, 1, 4);
	grid->addWidget(dummy, 4, 0, 1, 4);
	grid->addWidget(tableWidget, 5, 0, 5, 4);

	setLayout(grid);
	onScramble();
	updateAvg();
	QLineSeries* ao5series = new QLineSeries();
	ao5series->setName("ao5");
	for (unsigned int i = 0; i < ao5times.size(); i++) {
		ao5series->append(i, ao5times[i]);
	}
	chart->addSeries(ao5series);
	ao5series->attachAxis(axisY);
}

void CubeUi::onScramble()
{
	currentScramble = std::string(scramble());
	lbl->setText(QString(currentScramble.c_str()));
}

vector<std::string> getFile()
{
	std::ifstream ifile("times.csv");
	std::string line;
	vector<std::string> toRet;
	while (getline(ifile, line)) {
		toRet.push_back(line);
	}
	ifile.close();
	return toRet;
}

void CubeUi::onDnf()
{
	vector<std::string> oldfile = getFile();
	std::ofstream ofile("times.csv");
	for (int i = 0; i < oldfile.size() - 1; i++) {
		ofile << oldfile[i] << endl;
	}
	std::stringstream linestream(oldfile.back());
	std::stringstream newline;

	std::string date, time, scramble;
	std::getline(linestream, date, ',');
	std::getline(linestream, time, ',');
	std::getline(linestream, scramble, ',');
	float t = std::stof(time);
	newline << date << "," << 1.0e+6 << "," << scramble;
	ofile << newline.str() << endl;
	ofile.close();
	tableWidget->removeRow(tableWidget->rowCount()-1);
	QTableWidgetItem *timeW = new QTableWidgetItem(QString("DNF"));
	QTableWidgetItem *scrambleW = new QTableWidgetItem(scramble.c_str());
	QTableWidgetItem *dateW = new QTableWidgetItem(date.c_str());
	tableWidget->setRowCount(tableWidget->rowCount()+1);
	tableWidget->setItem(tableWidget->rowCount() - 1, 0, dateW);
	tableWidget->setItem(tableWidget->rowCount() - 1, 1, timeW);
	tableWidget->setItem(tableWidget->rowCount() - 1, 2, scrambleW);
	times[times.size() - 1] = 1.0e+6;
	updateAvg();
}

void CubeUi::onDelete()
{
	vector<std::string> oldfile = getFile();
	std::ofstream ofile("times.csv");
	for (int i = 0; i < oldfile.size() - 1; i++) {
		ofile << oldfile[i] << endl;
	}
	ofile.close();
	tableWidget->removeRow(tableWidget->rowCount()-1);
	times.pop_back();
	updateAvg();
}

void CubeUi::onPlus2()
{
	vector<std::string> oldfile = getFile();
	std::ofstream ofile("times.csv");
	for (int i = 0; i < oldfile.size() - 1; i++) {
		ofile << oldfile[i] << endl;
	}
	std::stringstream linestream(oldfile.back());
	std::stringstream newline;

	std::string date, time, scramble;
	std::getline(linestream, date, ',');
	std::getline(linestream, time, ',');
	std::getline(linestream, scramble, ',');
	float t = std::stof(time);
	newline << date << "," << t + 2.0 << "," << scramble;
	ofile << newline.str() << endl;
	ofile.close();
	tableWidget->removeRow(tableWidget->rowCount()-1);
	QTableWidgetItem *timeW = new QTableWidgetItem(QString::number(t + 2.0));
	QTableWidgetItem *scrambleW = new QTableWidgetItem(scramble.c_str());
	QTableWidgetItem *dateW = new QTableWidgetItem(date.c_str());
	tableWidget->setRowCount(tableWidget->rowCount()+1);
	tableWidget->setItem(tableWidget->rowCount() - 1, 0, dateW);
	tableWidget->setItem(tableWidget->rowCount() - 1, 1, timeW);
	tableWidget->setItem(tableWidget->rowCount() - 1, 2, scrambleW);
	times[times.size() - 1] = t + 2.0;
	updateAvg();
}

void CubeUi::startTimer()
{
	timer_1s = new QTimer(this);
	QObject::connect(timer_1s, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer_1s->start(10);
	timer.start();
}

void CubeUi::updateTime()
{
	timelbl->setText(QString::number(timer.elapsed()/1000.0));
}

void CubeUi::updateAo5()
{
	ao5times.clear();
	for (int i = 0; i < 5; i++) {
		ao5times.push_back(0);
	}
	for (int i = 4; i < times.size(); i++) {
		float min = 1.0e+7, max = -1, sum = 0;
		int numdnf = 0;
		for (int j = i; j > i - 5; j--) {
			if (times[j] == 1.0e+6) {
				numdnf++;
			}
		}
		for (int j = i; j > i - 5; j--) {
			if (times[j] == 1.0e+6) {
				continue;
			}
			if (times[j] < min) {
				min = times[j];
			}
			if (times[j] > max) {
				max = times[j];
			}
			sum += times[j];
		}
		switch(numdnf) {
			case 0:
				sum -= (min + max);
				sum /= 3.0;
				ao5times.push_back(sum);
				break;
			case 1:
				sum -= min;
				sum /= 3.0;
				ao5times.push_back(sum);
				break;
			case 2:
				sum /= 3.0;
				ao5times.push_back(sum);
				break;
			default:
				ao5times.push_back(0);
		}
	}
}

void CubeUi::updateAvg()
{
	if (times.empty()) {
		return;
	}
	std::stringstream toSet;
	float runningAvg = 0;
	float min = times.back();
	float max = times.back();
	for (int i = times.size() - 1; i >= 0; i--) {
		runningAvg += times[i];
		if (times[i] > max) {
			max = times[i];
		}
		if (times[i] < min) {
			min = times[i];
		}
		if (i == (int) times.size() - 3) {
			toSet << "mo3: " << std::fixed << std::setprecision(3) << runningAvg/3.0 << " ";
		}
		if (i == (int) times.size() - 5 || i == (int) times.size() - 12 || i == (int) times.size() - 100) {
			float avg = (runningAvg - min - max) / (float) (times.size() - i - 2);
			toSet << "ao" << times.size() - i << ": " << std::fixed << std::setprecision(3) << avg << " ";
		}
	}
	toSet << "best: " << min;
	ao->setText(QString(toSet.str().c_str()));
	updateAo5();
}
