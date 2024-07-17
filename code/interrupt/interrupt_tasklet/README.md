在Linux kernel中，tasklet 是一种基于软中断（softirq）机制的低延迟任务执行方式。

1. tasklet_init
tasklet_init 函数用于初始化一个 tasklet 结构体，使其准备好被调度执行。其原型如下所示:

void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data);

    > struct tasklet_struct *t：指向要初始化的 tasklet 结构体的指针。
    > void (*func)(unsigned long)：当 tasklet 被执行时调用的函数指针。
    > unsigned long data：传递给 func 函数的参数。

    这个函数设置了 tasklet 的回调函数和数据，但没有立即执行它。


2. tasklet_schedule
tasklet_schedule 函数用于调度一个已经初始化的 tasklet 以便在将来某个时间点执行。原型如下所示：

void tasklet_schedule(struct tasklet_struct *t);

    > struct tasklet_struct *t：指向要调度的 tasklet 结构体的指针。
这个函数将 tasklet 添加到软中断的待处理列表中，以便在下一个软中断执行周期中处理。

3. tasklet_kill
tasklet_kill 函数用于停止并等待一个 tasklet 完成其执行（如果它当前正在执行）。原型如下所示：

void tasklet_kill(struct tasklet_struct *t);

    > struct tasklet_struct *t：指向要停止的 tasklet 结构体的指针。
这个函数会清除 tasklet 的待处理状态，并一直等待直到 tasklet 的回调函数不再执行。

