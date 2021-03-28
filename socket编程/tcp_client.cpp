#include "tcpsocket.hpp"

using namespace std;

int main() {
  TcpSocket cli_sock;

  //1.创建套接字
  CHECK_RET(cli_sock.Socket());
  //2.绑定地址信息(不推荐)
  //3.向服务端发起连接请求
  CHECK_RET(cli_sock.Connect("172.17.0.8", 9000));

  while(1) {
    //4.发送数据
    string data;
    cout << "client say: ";
    cin >> data;
    CHECK_RET(cli_sock.Send(data));
    //5.接收数据
    string buf;
    CHECK_RET(cli_sock.Recv(&buf));
    cout << "server say: " << buf << endl;
  }

  //6.关闭套接字
  cli_sock.Close();

  return 0;
}
