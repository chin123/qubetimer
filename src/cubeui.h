#pragma once

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QTableWidget>

#include <vector>

using std::vector;
using namespace QtCharts;

class CubeUi: public QWidget {
    
  Q_OBJECT

  public:
    CubeUi(QWidget *parent = 0);

  private slots:
    void onScramble();
    void onDnf();
    void onPlus2();
    void onDelete();
    void updateTime();
    void startTimer();

  private:
    /* The label which displays the scramble. */
    QLabel *lbl;

    /* The label which displays the time. */
    QLabel *timelbl;

    /* Displays the average statistics. */
    QLabel *ao;

    /* Used for alignment purposes. */
    QLabel *dummy;

    /* Holds the image of the scramble. */
    QLabel *scrambleImage;

    /* Ticks the timer to update the clock. */
    QTimer *timer_1s;

    /* The statistics graph. */
    QChart *chart;

    /* The Y axis of the statistics graph. */
    QValueAxis *axisY;

    /* Stores the elapsed time. */
    QElapsedTimer timer;

    /* Stores the past times. */
    QTableWidget *tableWidget;

    /* The button which generates a new scramble. */
    QPushButton *scrambleBtn;

    /* Adds +2 to the previous time. */
    QPushButton *plus2Btn;

    /* Makes the previous time a DNF. */
    QPushButton *dnfBtn;

    /* Deletes the previous time. */
    QPushButton *deleteBtn;

    /* Stores whether the current keypress should be used to start the timer. */
    int toStart;

    /* Stores all the times recorded so far. */
    vector<float> times;

    /* Stores the ao5 of all times recorded so far. */
    vector<float> ao5times;

    /* Updates the average statistics. */
    void updateAvg();

    /* Updates the Ao5 statistics. */
    void updateAo5();

    /* Draws the image of the scramble. */
    void drawImage();

    /* Does the initial setup of the table. */
    void setupTable();

    /* Does the initial setup of the Grid UI. */
    void setupUi();

    /* Does the initial setup of the statistics graph. */
    void setupGraph();

    /* Converts the cube representation from corners and edges to each individual face. */
    void setupFaces(char corners[8][4], char edges[12][3]);

    /* Returns a scramble representing the current setup. */
    char* scramble();

    /* Stores the current scramble. */
    std::string currentScramble;

    /* The colors on each face of the cube. */
    char u[9], r[9], f[9], d[9], l[9], b[9];

  protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};
