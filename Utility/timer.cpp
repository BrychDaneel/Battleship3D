#include "timer.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <thread>
#include <mutex>

using namespace std;
static boost::asio::io_service io;

static void startio(){
    io.run();
}

void static onTick(const boost::system::error_code& /*e*/,  boost::asio::deadline_timer* t, void(*onTime)(),int millisecond){
        mutex m;
        m.lock();
        (*onTime)();
        m.unlock();
        t->expires_at(t->expires_at() + boost::posix_time::millisec(millisecond));
        t->async_wait(boost::bind(onTick, boost::asio::placeholders::error, t, onTime, millisecond));
}

timer::timer(int millisecond, void(*onTime)()):t(io, boost::posix_time::millisec(millisecond)){
        t.async_wait(boost::bind(onTick, boost::asio::placeholders::error, &t, onTime,millisecond));
        thread thr(startio);
        thr.detach();
    }

