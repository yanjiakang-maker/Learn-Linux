#include "tcpsocket.hpp"
#include <pthread.h>

void* thr_start(void* arg) {
  TcpSocket* con_sock = (TcpSocket*)arg;
  while(1) {
    string buf;
    int ret = con_sock->Recv(&buf);
    if (ret == false) {
      con_sock->Close();
      delete con_sock;
      return NULL;
    }
    printf("client say:%s\n", buf.c_str());

    string data;
    cout << "server say: ";
    cin >> data;
    ret = con_sock->Send(data);
    if (ret == false) {
      con_sock->Close();
      delete con_sock;
      return NULL;
    }
  }
  delete con_sock;
  return NULL;
}

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
    TcpSocket* con_sock = new TcpSocket();
    int ret = lst_sock.Accept(con_sock);
    if (ret == false) {
      //获取失败就重新去获取
      continue;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, thr_start, (void*)con_sock);
    pthread_detach(tid);
  }

  //6.关闭套接字
  lst_sock.Close();
  return 0;
}
