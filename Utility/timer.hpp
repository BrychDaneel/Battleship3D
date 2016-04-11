#pragma once
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


class timer{
    public:
    timer(int millisecond, void(*onTime)());
    private:
        boost::asio::deadline_timer t;

};
