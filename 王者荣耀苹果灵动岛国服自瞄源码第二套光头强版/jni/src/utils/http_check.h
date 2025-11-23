/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

// 发送HTTP请求并获取状态码
int checkHttpStatusCode(const std::string& host) {
    int sockfd;
    struct sockaddr_in serveraddr;
    socklen_t addrlen = sizeof(serveraddr);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        return -1;
    }
    std::memset(&serveraddr, 0, addrlen);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(80);
    struct hostent* host_ent;
    host_ent = gethostbyname(host.c_str());
    if (host_ent == NULL) {
        std::cerr << "gethostbyname: " << strerror(h_errno) << std::endl;
        close(sockfd);
        return -1;
    }
    struct in_addr ip = *((struct in_addr *)host_ent->h_addr);
    serveraddr.sin_addr = ip;
    if (connect(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0) {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }
    std::string request = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request.c_str(), request.length(), 0) < 0) {
        std::cerr << "send: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }
    char buffer[1024];
    int bytes_read = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        std::cerr << "recv: " << strerror(errno) << std::endl;
        close(sockfd);
        return -1;
    }
    buffer[bytes_read] = '\0';
    // 从响应中提取状态码
    const char* status_code_start = std::strstr(buffer, "HTTP/1.1 ");
    if (status_code_start == NULL) {
        std::cerr << "Failed to find status code" << std::endl;
        close(sockfd);
        return -1;
    }
    status_code_start += 9;
    int status_code = std::atoi(status_code_start);
    close(sockfd);
    return status_code;
}