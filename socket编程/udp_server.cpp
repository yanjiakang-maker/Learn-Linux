#include <iostream>
#include <string>
#include "udpsocket.hpp"

using namespace std;

//#define CHECK_RET(q) if((q)==false) {return false;}

int main(int argc, char* argv[]) {
  //argc表示程序运行参数的个数
  if(argc != 3) {
    cout << "Usage: ./udp_server ip prot" << endl;
    return -1;
  }
  uint16_t port = stoi(argv[2]);
  string ip = argv[1];

  UdpSocket srv_sock;
  //创建套接字
  if (srv_sock.Socket() == false) {
    return -1;
  }
  //绑定地址信息
  if (srv_sock.Bind(ip, port) == false) {
    return -1;
  } 
  while(1) {
    //接收数据
    string buf;
    string peer_ip;
    uint16_t peer_port;
    if (srv_sock.Recv(&buf, &peer_ip, &peer_port) == false) {
      break;
    }
    cout << "client[" << peer_ip << ":" << peer_port << "] say: " << buf << endl;

    //回复数据
    string data;
    cout << "server say: ";
    cin >> data;
    if(srv_sock.Send(data, peer_ip, peer_port) == false) {
      break;
    }
  }
  
  //关闭套接字
  srv_sock.Close();

  return 0;
}
