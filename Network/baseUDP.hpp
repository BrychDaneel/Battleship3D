#pragma once
#include <boost/asio.hpp>
#include <string>
#include <mutex>
using std::string;

namespace BattleShipUDP{

extern std::mutex Mutex;

class UDPServer
{

public:
  UDPServer(void (*_onGet)(const char* buffer,const size_t size,const string ip,const unsigned short port),unsigned short port);

    void send(const string ip, const unsigned short port, const char* buf,const size_t length);
    void send(const string ip, const unsigned short port, const char* buf);
    void send(const string ip, const unsigned short port, const string message);

    void send(const unsigned short port, const char* buf,const size_t length);
    void send(const unsigned short port, const char* buf);
    void send(const unsigned short port, const string message);

 unsigned short getPort();


  private:
    void do_receive();
    boost::asio::ip::udp::endpoint sender_endpoint_;
    static const int max_length = 1024;
    char data_[max_length+1];
    void (*onGet)(const char* buffer,const size_t size,const string ip,const unsigned short port);
    boost::asio::ip::udp::socket socket_;
};

}
