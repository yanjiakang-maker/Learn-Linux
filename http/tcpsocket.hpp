#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h> //地址转换接口头文件
#include <netinet/in.h> // 地址结构类型定义头文件
#include <sys/socket.h> //套接字接口头文件

using namespace std;

#define CHECK_RET(q) if((q)==false){return -1;}

class TcpSocket {
  public:
    TcpSocket() :_sockfd(-1) {}

    bool Socket() {
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0) {
        perror("socket error");
        return false;
      }
      return true;
    }
    
    //绑定地址信息
    bool Bind(const string& ip, const int port) {
      //定义地址结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0) {
        perror("bind error");
        return false;
      }
      return true;
    }

    //服务端开始监听
    bool Listen(int backlog = 5) {
     //int listen(int sockfd, backlog)
     int ret = listen(_sockfd, backlog);
     if (ret < 0) {
       perror("listen error");
       return false;
     }
     return true;
    }

    //客户端请求新连接
    bool Connect(const string& ip, int port) {
      //int connect(int sockfd, sockaddr* srvaddr, int addrlen)
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
     int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
     if (ret < 0) {
       perror("connect error");
       return false;
     }
     return true;
    }

    //服务端获取新连接
    bool Accept(TcpSocket* sock, string* ip = nullptr, int* port = nullptr) {
      //int accept(int sockfd, sockaddr* addr, int* len);
      struct sockaddr_in cli_addr;
      socklen_t len = sizeof(struct sockaddr_in);
      int fd = accept(_sockfd, (struct sockaddr*)&cli_addr, &len);
      if(fd < 0) {
        perror("accept error");
        return false;
      }
      //传入的sock对象, 获取新建的连接套接字
      //fd是与cli_addr地址的客户端进行通信的, 外部通过sock与客户端进行通信
      sock->_sockfd = fd;
      if (ip != nullptr) {
        *ip = inet_ntoa(cli_addr.sin_addr);
      }
      if(port != nullptr) {
        *port = ntohs(cli_addr.sin_port);
      }
      return true;
    }

    //接收数据
    bool Recv(string* buf) {
      //ssize_t recv(int sockfd, char* buf, int len, len)
      //返回值: 错误返回-1, 连接断开返回0, 成功返回实际接收的数据长度
      
      char tmp[4096] = {0};
      int ret = recv(_sockfd, tmp, 4096, 0);
      if (ret < 0) {
        perror("recv error");
        return false;
      }
      else if (ret == 0) {
        printf("connect shutdown");
        return false;
      }
      buf->assign(tmp, ret);
      return true;
    }

    //发送数据
    bool Send(const string& data) {
      //ssize_t send(int sockfd, char* data, int len, int flag)
      size_t total_len = 0; //实际发送的数据长度
      while(total_len < data.size()) {
        int ret = send(_sockfd, data.c_str() + total_len, 
            data.size() - total_len, 0);
        if (ret < 0) {
          perror("send error");
          return false;
        }
        total_len += ret;
      }
      return true;
    }

    //关闭套接字
    bool Close () {
      if(_sockfd < 0) {
        close(_sockfd);
        _sockfd = -1;
      } 
      return true;
    }

  private:
    int _sockfd;
};
