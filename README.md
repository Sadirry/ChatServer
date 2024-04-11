# ChatServer
- 基于重构Muduo网络库
- 集群聊天服务器和客户端
- 利用在nginx实现tcp负载均衡，redis发布-订阅模式

## 部署：
### Muduo网络库：
**安装：**
切换到Muduo文件夹下，执行`depoly_muduo.sh`脚本一键安装
```bash
cd Muduo/
sudo depoly_muduo.sh
```
**测试：**
切换到Muduo/example/文件夹下，执行`make`生成`testserver`可执行文件
```bash
cd Muduo/example/
make
./testserver
```
**测试结果：**
![image.png](https://github.com/Sadirry/ChatServer/blob/main/figure/muduo.png)

### MySQL数据库：
**安装：**
```bash
sudo apt-get install mysql-server
sudo apt-get install libmysqlclient-dev
```
数据表初始文件：`./mysql_init/chat.sql`
```bash
cd ./mysql_init
sudo mysql -u root -p123456 < chat.sql
```
新增MySQL用户和密码：
```plsql
CREATE USER 'violet'@'%' IDENTIFIED BY '123456';
GRANT ALL PRIVILEGES ON chat.* TO 'violet'@'%';
FLUSH PRIVILEGES;
```
验证MySQL数据表：

<img src="https://github.com/Sadirry/ChatServer/blob/main/figure/mysql.png" width="50%">


### Nginx负载均衡：
利用压缩包进行安装配置：
（压缩包下载：[https://nginx.org/](https://nginx.org/)）
```bash
tar -axvf nginx-1.22.1.tar.gz
cd nginx-1.22.1
./configure --with-stream  # 激活TCP负载均衡模块(注意：按照报错添加相应的库文件)
sudo make && sudo make install # 需要root权限
```
默认安装在`/usr/local/nginx`目录
```cpp
cd /usr/local/nginx/conf
sudo vim nginx.conf
```
添加配置TCP负载均衡的信息：

- `127.0.0.1:6000` 和 `127.0.0.1:6002` 两个服务器程序
- 对外监听端口为 `8000`
```bash
stream {
    upstream Myserver {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
    }
    server {
        proxy_connect_timeout 1s;
        listen 8000;
        proxy_pass MyServer;
        tcp_nodelay on;
    }
}
```
配置完成后启动Nginx：
```bash
cd /usr/local/nginx/sbin
sudo ./nginx
# sudo ./nginx -s reload 重启Nginx
```

### Redis中间件:
```bash
sudo apt-get install redis-server  # 安装Redis
git clone https://github.com/redis/hiredis # 安装hiredis
cd hiredis
make
sudo make install
sudo ldconfig /usr/local/lib
```
 
### ChatServer服务启动：
在`/bin`目录下生成服务器和客户端的可执行文件：
```cpp
bash autobulid.sh
```
执行服务器程序：
`./ChatClient 127.0.0.1 6000`  `./ChatClient 127.0.0.1 6002`
执行客户端程序：
`./ChatClient 127.0.0.1 8000`

## 效果展示：
**TCP负载均衡：**
启动两个客户端，分别连接在不同的服务器程序上。
![image.png](https://github.com/Sadirry/ChatServer/blob/main/figure/test1.png)
![image.png](https://github.com/Sadirry/ChatServer/blob/main/figure/test2.png)

**Redis跨服务器通信：**
![image.png](https://github.com/Sadirry/ChatServer/blob/main/figure/test3.png)
