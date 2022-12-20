#include <iostream>

#include <boost/asio.hpp>


using namespace std;
namespace net = boost::asio;
using net::ip::tcp;


int main(){
    net::io_context io;

    tcp::acceptor acceptor(io, tcp::endpoint(net::ip::address::from_string("127.0.0.1"), 3333));
    cout<<"waiting for connetion"<<endl;

    boost::system::error_code ex;
    tcp::socket socket{io};
    acceptor.accept(socket, ex);

    if(ex){
        cout<<"error"<<endl;
        return 1;
    }

    // char buff[512];
    net::streambuf stream_buf;
    net::read_until(socket, stream_buf, '\0', ex);
    std::string client_data{std::istreambuf_iterator<char>(&stream_buf),
                        std::istreambuf_iterator<char>()};

    cout<<client_data<<endl;

    socket.write_some(net::buffer("Server response")); 
    

    return 0;
}