#ifndef QING_TIMER_H
#define QING_TIMER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

class QingTimer {
public:
    QingTimer()  { m_start = (double)getTickCount(); }
    ~QingTimer() {}

    double duration() { return ((double)getTickCount() - m_start)/ getTickFrequency() ; }
    void restart() { m_start = (double)getTickCount(); }
private:
    double m_start, m_end;
};

#endif // QING_TIMER_H
