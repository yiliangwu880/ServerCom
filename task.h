
配置读取

进程：
{
     进程控制消息：参考Xfire,启动程序本身，加参数来给进程发控制消息
     网络消息处理：需要连接 connect进程 + 共享内存
     读取配置
     日志
     timer
}

服务器集群：
{
    配置中心化：redis
    
}

常用应用库
{
    mysql 客户端
    mongodb 客户端
    redis客户端
    http 通讯 (curl)
    加密解密
    protobuf
    excel 配置转换工具
    udp
    oss log (object storage service)
    etcd 客户端z`
}

工具：
{
    单个文件依赖构建，（比如，批量实现proto to .cpp文件依赖）
    
}