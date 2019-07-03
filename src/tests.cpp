#include <QtTest/QtTest>
#include <QMutex>
#include <QKeyEvent>
#include <string>

#define private public
#define protected public
#include "cubeui.h"
#undef private
#undef protected

using std::string;

class CubeUiTest: public QObject {
    Q_OBJECT

private:
    CubeUi cb;

private slots:
    void initTestCase() {
      qDebug("Initialize tests");
    }

    void saneScramble() {
      string ans = string(cb.scramble());
      QVERIFY(ans.size() >= 20); // Decently sized scramble
      QCOMPARE(1, 1); // compare two values
    }

    void scrambleChanges() {
      string curScramble = cb.currentScramble;
      QMutex mut;
      mut.lock();
      mut.tryLock(1000);
      mut.unlock();
      cb.onScramble();
      QVERIFY(curScramble != cb.currentScramble);
    }

    void testTimer() {
      size_t initSize = cb.times.size();
      QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      QKeyEvent *event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      QMutex mut;
      mut.lock();
      mut.tryLock(1000);
      mut.unlock();
      event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      QCOMPARE(initSize + 1, cb.times.size()); // adds exactly one time
    }

    void testDelete() {
      size_t initSize = cb.times.size();
      cb.onDelete();
      QCOMPARE(initSize - 1, cb.times.size());
    }

    void testTimerAccuracy() {
      QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      QKeyEvent *event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      QMutex mut;
      mut.lock();
      mut.tryLock(1023);
      mut.unlock();
      event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
      event2 = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
      cb.keyPressEvent(event);
      cb.keyReleaseEvent(event2);
      // makes sure it's at a reasonable accuracy for timing rubik's cube solves
      QVERIFY(abs(1.023 - cb.times.back()) <= 0.00001);
      cb.onDelete();
    }

    void cleanupTestCase() {
      qDebug("Tests completed");
    }
};

QTEST_MAIN(CubeUiTest)
#include "tests.moc"
