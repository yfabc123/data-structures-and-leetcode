#ifndef _MYTHREADPOOL_HEAD_H
#define _MYTHREADPOOL_HEAD_H
// 构成任务队列包括执行函数和参数
int global = 0;
typedef struct job
{
    void *(*function)(void *);
    void *arg;
} QJob;
typedef struct pool
{
    QJob *qJob;               // 任务队列
    int maxJob;               // 最大任务数量
    int curJob;               // 当前任务数量
    int qhead;                // 任务队列头
    int qtail;                // 任务队列尾
    pthread_t *threadPool;    // 线程id集合
    int maxThread;            // 最大线程数
    pthread_cond_t condFull;  // 任务满信号  为了阻塞生产者 通知消费者
    pthread_cond_t condEmpty; // 任务空信号 为了阻塞消费者 通知生产者
    pthread_mutex_t mutex;    // 队列锁
} TPool;
static TPool *createPool(int pmaxThread, int pmaxJob);
static void *threadFun(void *parg); // 消费者具体任务
static void addJob(TPool *tPool);   // 生产者把具体执行函数还有参数封装到结构体中 然后存入任务队列
static void *job(void *arg);
#endif