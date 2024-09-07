/*
 * @Author: zhu
 * @Date: 2024-08-16 15:47:19
 * @LastEditors: zhu 3180104456@zju.edu.cn
 * @LastEditTime: 2024-09-07 18:24:17
 * @FilePath: \tiny threadpool\demo_2.cpp
 * @Description: 
 * 
 */
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

#include "ThreadPool.h"

std::mutex coutMutex;  // 全局互斥锁用于同步输出，避免输出文本内容因并发执行而相互交织

/**
 * @description: 
 * 模拟运算任务，生成随机数模拟不同运算任务的运行时间，输出任务开始与完成的信息
 * @param {int} taskId 任务标志
 * @return {*}
 */
void longComputationTask(int taskId) {
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Task " << taskId << "   started on thread" <<std::this_thread::get_id()<< std::endl;
    }
    std::uniform_int_distribution<unsigned> u(3,6);
    std::default_random_engine e;
    int delay=u(e);
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Task " << taskId << " completed on thread" <<std:: this_thread::get_id()<< std::endl;
    }
}

int main() {
    ThreadPool pool(4);
    std::vector<std::future<void>> results;
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.enqueue(longComputationTask, i));
    }
    for (auto&& result : results) {
        result.wait();
    }
    std::cout << "All tasks completed." << std::endl;
    return 0;
}