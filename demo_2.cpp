#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "ThreadPool.h"

std::mutex coutMutex;  // 全局互斥锁用于同步输出，避免出现输出文本内容因并发执行而相互交织

void longComputationTask(int taskId) {
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Task " << taskId << "   started on thread" <<std::this_thread::get_id()<< std::endl;
    }

    //生成3~6之间的随机整数作为任务的时延delay
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
    //启动一个含有四个线程的线程池
    ThreadPool pool(4);
    std::vector<std::future<void>> results;
    //向线程池的任务队列提交任务
    for (int i = 0; i < 10; ++i) {
        results.emplace_back(pool.enqueue(longComputationTask, i));
    }
    //确保所有提交的任务都已经完成
    for (auto&& result : results) {
        result.wait();
    }

    std::cout << "All tasks completed." << std::endl;

    return 0;
}