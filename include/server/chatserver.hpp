#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <Muduo/TcpServer.h>
#include <Muduo/EventLoop.h>
#include <string>
using namespace std;

// 聊天服务器的主类
class ChatServer{
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    // 启动服务
    void start();

private:
    // 上报链接相关信息的回调函数
    void onConnection(const TcpConnectionPtr &);

    // 上报读写事件相关信息的回调函数
    void onMessage(const TcpConnectionPtr &,
                   Buffer *,
                   Timestamp);

    TcpServer _server; 
    EventLoop *_loop;  
};

#endif