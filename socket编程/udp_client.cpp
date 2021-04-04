#include <iostream>
#include <string>
#include "udpsocket.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  //客户端参数获取的ip地址是服务端绑定的地址, 也就是客户端发送的目标地址
  //不是为了自己绑定的
  if (argc != 3) {
    cout << "Usage: ./udp_cli ip port" << endl;
    return -1;
  }
  string srv_ip = argv[1];
  uint16_t srv_port = stoi(argv[2]);

  UdpSocket cli_sock;
  //创建套接字
  if(cli_sock.Socket() == false) {
    return -1;
  }
  //绑定地址(不推荐)
  while(1) {
    //发送数据
    cout << "client say:";
    string data;
    cin >> data;
    if (cli_sock.Send(data, srv_ip, srv_port) == false) {
      return -1;
    }
    //接收数据
    string buf;
    //客户端不需要关心服务器的ip和port信息, 只需要接收数据就行了
    if (cli_sock.Recv(&buf) == false) {
      return -1;
    }
    cout << "server say: " << buf << endl;
  }

  //关闭套接字
  cli_sock.Close();
  return 0;
}
