/* 
 * 由光头强内核（原XF内核）开源
 因频道账号被盗用后决定开源，正版为光头强内核，
开源文件仅供学习交流，任何个人或组织可在遵循协议条款的前提下，自由下载、使用、修改、分发本项目代码及衍生作品，无需额外授权。使用过程中请保留原项目版权声明及开源协议文本，不得用于违法违规场景。本项目以“现状”提供，开发者不承担任何直接或间接的使用风险及法律责任，亦不提供商业技术支持。
删除此内容死全家，没几把
* 注入时间: 2025-11-16 20:14:49
 * 联系方式: TG:@GTQNHNB666
QQ群：784316342
 */
// 在公共头文件 common.h 中添加声明
//#pragma once
#ifndef PROXY_H
#define PROXY_H

#include <string>
void stop_box();        // 只保留声明
void stop_iptables();   // 不包含实现
int init_proxy_system(); // 代理系统初始化函数
void* proxy_thread_main(void* arg); // 代理线程主函数
void start_proxy_thread(); // 启动代理线程
void stop_proxy_thread(); // 停止代理线程
int proxy_main(int argc, char* argv[]); // proxy/main.cpp中的主函数

// 注意：所有函数现在都在proxy/main.cpp中实现

#endif
