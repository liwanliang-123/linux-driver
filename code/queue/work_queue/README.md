工作队列:

在Linux内核中，工作队列（workqueue）是一种异步处理机制，用于延迟执行一些需要在进程上下文中执行的任务。这种机制通过work_struct结构体来实现，使得内核模块或驱动程序能够避免在中断上下文中执行长时间运行的操作，从而提高系统的响应性和稳定性。

一、work_struct结构体
        work_struct是Linux内核中用于描述工作项（work item）的结构体，它是工作队列中的基本元素。该结构体通常包含回调函数（即需要延迟执行的任务）和与该任务相关的数据。结构体定义如下：

struct work_struct {  
    atomic_long_t data;  // 用于内部状态管理  
    struct list_head entry;  // 链接到工作队列的链表项  
    work_func_t func;  // 指向工作处理函数的指针
#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;  // 用于锁调试的lockdep_map等  
#endif
};


其中，work_func_t是一个函数指针类型，定义如下：

typedef void (*work_func_t)(struct work_struct *work);


二、work queue 工作流程如下：

    1、定义一个或多个work_struct结构体变量，并将它们与相应的回调函数关联起来。

    2、使用INIT_WORK宏或INIT_DELAYED_WORK宏来初始化工作项。
    
    3、通过调用schedule_work或schedule_delayed_work函数，将工作项提交给工作队列。这会将工作项添加到内核维护的工作队列中，以便稍后由工作线程（worker thread）执行。
    
    4、工作线程会不断从工作队列中取出工作项并执行其关联的回调函数。
    
    5、回调函数执行完毕后，工作项会被自动从工作队列中删除。如果需要再次执行该工作项，可以重新调用schedule_work或schedule_delayed_work函数。
