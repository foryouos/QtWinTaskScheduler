# 开源库目标

> 通过Qt调用`WinAPI`来添加语修改`windows`的`计划任务程序`，来方便开发者通过`Qt`来开发程序使`自身`程序加入到`计划任务程序当中`。
>
> 通过`计划任务程序`最大的`优势`是可以通过`系统账户管理员账户运行`，而不像`注册表`和`启动文件夹`将`使用当前登录账户`的`权限`。
>
> * [x] 完成创建任务计划程序的封装
> * [x] 完成 常规设置 接口测试
> * [x] 完成 添加操作 接口测试
> * [x] 完成与触发器一起判断是否应运行该任务的条件
> * [x] 完成影响任务行为的其他设置
> * [ ] 完成 影响任务钟和触发器相关的配置兼容性
> * [ ] 完成触发器配置
> * [ ] 完成API 调用实例



# 调用接口



```cpp
#include "taskscheduler.h"

TaskScheduler taskscheduler;
taskscheduler.setTaskName("Test API");               // 设置账户名称
taskscheduler.setTaskAccount("NT AUTHORITY\\SYSTE"); //设置用户登录的账户
taskscheduler.setTaskCreator("瓶子的跋涉");
taskscheduler.setTaskDesc("瓶子的跋涉API Task 描述");
taskscheduler.setRunLevelType(TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_LUA);// 使用普通用户
// 设置一些执行设置
// 1,控制 空闲条件
taskscheduler.SetIdleCondition(VARIANT_FALSE,"P5D","PT1H",VARIANT_FALSE,VARIANT_FALSE);

// 2,控制 电源配置
taskscheduler.SetBatteryCondition(VARIANT_TRUE,VARIANT_TRUE,VARIANT_TRUE);

// 3,控制网络条件
taskscheduler.SetNetWordCondition(VARIANT_FALSE);

// 4，控制其它设置
taskscheduler.setRunTasksOnDemand(VARIANT_TRUE);
taskscheduler.setImmediateStartAfterScheduledTime(VARIANT_TRUE);
taskscheduler.setRestartFrequency("");
taskscheduler.setMaxRestartAttempts(3);
taskscheduler.setTaskTimeoutHours("");
taskscheduler.setForceStopOnRequest(VARIANT_TRUE);
taskscheduler.setDeleteTaskAfterNoSchedule("");


// 添加执行任务
taskscheduler.AddTaskOperation("C://qt.exe","","");

// 设置触发条件
taskscheduler.setTaskTriggerType(TASK_TRIGGER_TYPE2::TASK_TRIGGER_LOGON);

// 执行创建任务
taskscheduler.Create_Plan_Task();
```



# 时间输入格式字符串

> 如果`计划任务事`件操作以下时间，`停止任务`(默认为`3天`  `72`小时  `PT0S`  `不限制时间`)
> 其格式`有具体的限制`  按照`规范` 设置 否则 `会设置失败` 
>
> 此字符串的格式为 `PnYnMnDTnHnMnS`
>
>  * 其中 `nY` 是`年数`，
>  * `nM` 是`月数`，   例如: `P5M`  ---`五`个月
>  * `nD` 是`天数`，   例如: `P5D` --- `五天`
>  * `T`是`日期/时间`  `分隔符`.
>  * `nH` 是`小时数`，
>  * `nM` 是`分钟数`，
>  * `nS` 是`秒数`
>  *  (例如，`PT5M` 指定 `5 分钟`，`P1M4DT2H5M` 指定 `一个月`，) 4 天 2 小时 5 分钟。
>  * `PT0S` 值将使任务`无限期运行`。



# 默认配置

> `默认配置` 将 采用 使用`系统任务计划程序` `创建任务时` `相同`的 `配置`

![默认常规设置](./assets/image-20241104115819553.png)

![默认条件配置](./assets/image-20241104115833127.png)

![默认设置配置](./assets/image-20241104115840976.png)

# 参考资料

> [`taskschd]`(https://learn.microsoft.com/zh-cn/windows/win32/api/_taskschd/)
>
> * 联系方式:foryouos@qq.com
>
> * 微信公众号: `瓶子的跋涉 `
