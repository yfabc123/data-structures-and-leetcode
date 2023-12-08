// 用线程池实现生产消费者模型 生产者往任务队列添加数字 消费打印数字
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "myThreadPool.h"
TPool *createPool(int pmaxThread, int pmaxJob)
{
    TPool *tPool = (TPool *)malloc(sizeof(TPool));
    if (!tPool)
    {
        printf("申请tPool报错\n");
        goto err;
    }
    tPool->qJob = (QJob *)malloc(sizeof(QJob) * pmaxJob);
    if (!tPool->qJob)
    {
        printf("申请qJob报错\n");
        goto err;
    }
    tPool->threadPool = (pthread_t *)malloc(sizeof(pthread_t) * pmaxThread);
    if (!tPool->threadPool)
    {
        printf("申请threadPool报错\n");
        goto err;
    }
    tPool->curJob = 0;
    tPool->maxJob = pmaxJob;
    tPool->maxThread = pmaxThread;
    tPool->qhead = 0;
    tPool->qtail = 0;
    pthread_mutex_init(&tPool->mutex, NULL);
    pthread_cond_init(&tPool->condEmpty, NULL);
    pthread_cond_init(&tPool->condFull, NULL);
    // 创建线程
    for (int i = 0; i < pmaxThread; i++)
    {
        int ret = pthread_create(&tPool->threadPool[i], NULL, threadFun, tPool);
        if (ret < 0)
        {
            printf("创建线程异常\n");
            goto err;
        }
    }
    return tPool;
err:
    printf("进入err\n");
    if (!tPool)
    {
        printf("freetPool\n");
        free(tPool);
        exit(EXIT_FAILURE);
    }
    else if (!tPool->qJob)
    {
        printf("freetPool\n");
        free(tPool->qJob);
        free(tPool);
        exit(EXIT_FAILURE);
    }
    else if (!tPool->threadPool)
    {
        free(tPool->threadPool);
        free(tPool->qJob);
        free(tPool);
        exit(EXIT_FAILURE);
    }
}
void *threadFun(void *parg) // 消费者具体任务
{
    TPool *tPool = (TPool *)parg;
    while (1)
    {
         sleep(1);
        pthread_mutex_lock(&tPool->mutex); // 抢锁
        while (tPool->curJob == 0)
        {
            pthread_cond_wait(&tPool->condEmpty, &tPool->mutex); // 等待队列不为空的信号
        }
        // 解析任务具体执行内容
        void *(*fun)(void *) = (tPool->qJob)[tPool->qhead].function; // 搞个变量存函数名 否则名字太长..
        void *para = (tPool->qJob)[tPool->qhead].arg;
        tPool->qhead = (tPool->qhead + 1) % tPool->maxJob; // 偏移队列位置
        tPool->curJob--;
        pthread_cond_signal(&tPool->condFull);
        pthread_mutex_unlock(&tPool->mutex); // 释放锁
        fun(para);                           // 执行函数
    }
}
void addJob(TPool *tPool) // 生产者把具体执行函数还有参数封装到结构体中 然后存入任务队列
{
    while (1)
    {
         sleep(1);
        pthread_mutex_lock(&tPool->mutex);
        while (tPool->curJob == tPool->maxJob)
        {
            pthread_cond_wait(&tPool->condFull, &tPool->mutex);
        }
        printf("tPool->qtail:%d\n", tPool->qtail);
        (tPool->qJob)[tPool->qtail].function = job;
        (tPool->qJob)[tPool->qtail].arg = (void *)(global++); // 全局变量每使用一个加1
        tPool->qtail = (tPool->qtail + 1) % tPool->maxJob;
        tPool->curJob++;
        printf("生产了%d\n", global - 1);
        pthread_cond_signal(&tPool->condEmpty);
        pthread_mutex_unlock(&tPool->mutex);
    }
}
void *job(void *arg)
{
    int result = (int *)arg;
    printf("线程%ld执行了%d\n", pthread_self(), result);
}
int main()
{
    TPool *tPool = createPool(5, 5);
    addJob(tPool);
    return 0;
}