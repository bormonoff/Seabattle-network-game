#include <iostream>

#include <boost/asio.hpp>


using namespace std;
namespace net = boost::asio;
using net::ip::tcp;


int main(){
    net::io_context io;

    cout<<"waiting for connetion"<<endl;

    boost::system::error_code ex;
    tcp::socket socket{io};
    socket.connect(tcp::endpoint(net::ip::make_address("51.250.31.12", ex),3333));

    if(ex){
        cout<<"error connect"<<endl;
      
    }

   socket.write_some(net::buffer("Client, aboba"));

   net::streambuf stream_buf;
   net::read_until(socket, stream_buf, '\0', ex);
   std::string client_data{std::istreambuf_iterator<char>(&stream_buf),
                        std::istreambuf_iterator<char>()};

    cout<<client_data<<endl; 

    return 0;
}