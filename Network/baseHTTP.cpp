#include "baseHTTP.hpp"

#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <sstream>

using std::thread;
using std::mutex;
using std::ostream;
using std::ostringstream;
using std::istream;

using boost::asio::io_service;
using boost::asio::streambuf;
using boost::system::error_code;

HTTPClient::HTTPClient(void (*_onGet)(const string buffer), void (*_onRefuse)(), const string _host, const string _ip, unsigned short _port){
        ip=_ip;
        port=_port;
        host=_host;
        onGet=_onGet;
        onRefuse=_onRefuse;
    }

void startGet(void (*onGet)(const string buffer), void (*onRefuse)(),string page,string host, string ip, unsigned short port){
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::socket socket(io_service);
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
        socket.connect(endpoint);

        streambuf request;
        ostream request_stream(&request);
        request_stream << "GET " << page << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        ostringstream ans;
        try{
            boost::asio::write(socket, request);
            streambuf response;
            boost::asio::read_until(socket, response, "\r\n");
            istream response_stream(&response);

            string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            string status_message;
            if (!response_stream || http_version.substr(0, 5) != "HTTP/" || status_code != 200)
                throw new std::exception();

            boost::asio::read_until(socket, response, "\r\n\r\n");

            error_code error;
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
                ans<<&response;

            if (error != boost::asio::error::eof)
                throw new std::exception();

        } catch(...){
                onRefuse();
                return;
        }

        mutex m;
        m.lock();
        onGet(ans.str());
        m.unlock();
}

void HTTPClient::Get(const string page){
    thread thr(startGet, onGet, onRefuse, page, host, ip, port);
    thr.detach();
}
