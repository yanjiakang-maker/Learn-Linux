#include "tcpsocket.hpp"

int main() {
  //监听套接字
  TcpSocket lst_sock;
  
  //1.创建套接字
  CHECK_RET(lst_sock.Socket());
  
  //2.绑定地址信息
  CHECK_RET(lst_sock.Bind("0.0.0.0", 9000)); 
  
  //3.开始监听
  CHECK_RET(lst_sock.Listen());
  while(1) {
    //4.获取新连接
    TcpSocket con_sock;
    string cli_ip;
    int cli_port;
    int ret = lst_sock.Accept(&con_sock, &cli_ip, &cli_port);
    if (ret == false) {
      //获取失败就重新去获取
      continue;
    }
  
    //5.收发数据
    string buf;
    ret = con_sock.Recv(&buf);
    if (ret == false) {
      con_sock.Close();
      continue;
    }
    printf("[%s:%d] say:%s\n", cli_ip.c_str(), cli_port, buf.c_str());
    
    string data;
    cout << "server say: ";
    cin >> data;
    ret = con_sock.Send(data);
    if (ret == false) {
      con_sock.Close();
      continue;
    }
  }

  //6.关闭套接字
  lst_sock.Close();
  return 0;
}
