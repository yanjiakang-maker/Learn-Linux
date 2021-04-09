#include "tcpsocket.hpp"
#include <sstream>

using namespace std;


int main() {
  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind("0.0.0.0", 9000));
  CHECK_RET(lst_sock.Listen());
  while(1) {
    //获取新连接
    TcpSocket con_sock;
    lst_sock.Accept(&con_sock);

    //接收信息
    string buf;
    con_sock.Recv(&buf);
    cout << "请求信息: [" << buf << "]" << endl;
    
    string body; //正文
    body = "<html><body><h1>Zjb好帅！~</h1></body></html>";

    string empty_line = "\r\n"; //空行

    stringstream header; //头部
    header << "Content-Type: text/html\r\n";
    header << "Content-Length: " << body.size() << "\r\n";
    header << "Connection: close\r\n";
    header << "Location: http://www.baidu.com\r\n";

    string first_line;//首行
    first_line = "HTTP/1.1 302 OK\r\n";

    //发送数据
    con_sock.Send(first_line);
    con_sock.Send(header.str());
    con_sock.Send(empty_line);
    con_sock.Send(body);

    con_sock.Close();
  }
  lst_sock.Close();

  return 0;
}
