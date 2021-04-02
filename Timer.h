
#ifndef SOFTRENDERER_TIMER_H
#define SOFTRENDERER_TIMER_H
#include <chrono>

class Timer{
public:
    Timer(){ current = clock::now();}
    void start(){current = clock::now();}
    double elapsed() const{
        return std::chrono::duration_cast<sec>(clock::now() - current).count();
    }
private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::duration<double> sec;
    std::chrono::time_point<clock> current;
};

#endif //SOFTRENDERER_TIMER_H
