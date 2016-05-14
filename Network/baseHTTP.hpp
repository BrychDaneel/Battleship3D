#include <iostream>
using std::string;

class HTTPClient{
    public:
        HTTPClient(void (*_onGet)(const string buffer),void (*_onRefuse)(),const string _host, const string _ip, unsigned short _port);
        void Get(const string page);

    private:
        void (*onRefuse)();
        void (*onGet)(const string buffer);
        string ip, host;
        unsigned short port;
};

