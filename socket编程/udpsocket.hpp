#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h> //地址转换接口头文件
#include <netinet/in.h> // 地址结构类型定义头文件
#include <sys/socket.h> //套接字接口头文件

using namespace std;


class UdpSocket {
  public:
    UdpSocket():_socket(-1) {}
    
    //创建套接字
    bool Socket() {
      //int socket(int domain, int type, int protocol);
      _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_socket < 0) {
        perror("socket error");
        return false;
      }
      return true;
    }

    //为套接字绑定地址信息
    bool Bind(const string& ip, int port) {
      //定义ipv4地址结构 struct socketaddr_in
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      //htons:将主机字节序短整型数据转换为网络字节序数据
      addr.sin_port = htons(port);
      //将字符串IP地址转换为网络字节序
      addr.sin_addr.s_addr = inet_addr(ip.c_str());

      //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_socket, (struct sockaddr*)&addr, len);
      if(ret < 0) {
        perror("bind error");
        return false;
      }
      return true;
    }

    bool Send(string& data, string& ip, int port) {
      //sento(套接字句柄,数据首地址,数据长度,标志位,对端地址信息,地址信息长度)
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htos(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_socket, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
      if(ret < 0) {
        perror("sendto error");
        return false;
      }
      return true;
    }

    //接收数据,获取发送端的地址信息
    bool Recv(string* buf, string* ip = nullptr, uint16_t* port = nullptr) {
      //recvfrom(套接字句柄,接收缓冲区,数据长度,标志,源端地址,地址长度)
      struct sockaddr_in peer_addr;
      socklen_t len = sizeof(struct sockaddr_in);
      char tmp[4096] = {0};
      int ret = recvfrom(_socket, tmp, 4096, 0, (struct sockaddr*)&peer_addr, &len);
      if(ret < 0) {
        perror("recvfrom error");
        return false;
      }
      //从指定字符串中截取指定长度的数据到buf中
      buf->assign(tmp, ret);
      if(port != nullptr) {
        //网络字节序到主机字节序的转换
        *port = ntohs(peer_addr.sin_port);
      }
      if(ip != nullptr) {
        //网络字节序到字符串IP地址的转换
        *ip = inet_ntoa(peer_addr.sin_addr);
      }
      return true;
    }

    //关闭套接字
    bool Close() {
      if(_socket > 0) {
        close(_socket);
        _socket = -1;
      }
      return true;
    }

  private:
    int _socket;
};
