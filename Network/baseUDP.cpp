#include "baseUDP.hpp"

#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <string>

using std::string;
using std::thread;
using std::mutex;


static boost::asio::io_service io_service;

namespace BattleShipUDP{
static void startServ(){
    io_service.run();
}


  UDPServer::UDPServer(void (*_onGet)(const char* buffer,const size_t size,const string ip,const unsigned short port), short port)
    : socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
  {
    onGet=_onGet;
    do_receive();
    thread thr(startServ);
    thr.detach();
  }

  void UDPServer::send(const string ip, const unsigned short port, const char* buf,const size_t length)
  {
    socket_.set_option(boost::asio::socket_base::broadcast(false));
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);

    socket_.async_send_to(
        boost::asio::buffer(buf, length), endpoint,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
        //  do_receive();
        });
  }


  void UDPServer::send(const string ip, const unsigned short port, const char* buf){
        send(ip, port, buf, strlen(buf));
  }

  void UDPServer::send(const string ip, const unsigned short port, const string message){
        send(ip, port, message.c_str());
  }

void UDPServer::send(const unsigned short port, const char* buf,const size_t length)
  {
    socket_.set_option(boost::asio::socket_base::broadcast(true));
    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address_v4::broadcast(), port);

    socket_.async_send_to(
        boost::asio::buffer(buf, length), endpoint,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
        //  do_receive();
        });
  }

void UDPServer::send(const unsigned short port, const char* buf){
        send(port, buf, strlen(buf));
}

void UDPServer::send(const unsigned short port, const string message){
        send(port,message.c_str());
}

 unsigned short UDPServer::getPort(){
    boost::asio::ip::udp::endpoint e=socket_.local_endpoint();
   return e.port();
  }


   void UDPServer::do_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd)
        {
          if (!ec && bytes_recvd > 0)
          {
            //do_send(bytes_recvd);
            data_[bytes_recvd]=0;
            if (onGet){
                mutex m;
                m.lock();
                (*onGet)(data_,bytes_recvd,sender_endpoint_.address().to_string(),sender_endpoint_.port());
                m.unlock();

            }
          }
            do_receive();
        });
  }

  }

