#include <QApplication>
#include "taskscheduler.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    TaskScheduler w;
    w.show();
    return a.exec();
#else
    //TODO: 测试WinTime Task API 接口
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
    taskscheduler.setRestartFrequency("PT5M");
    taskscheduler.setMaxRestartAttempts(3);
    taskscheduler.setTaskTimeoutHours("P5D");
    taskscheduler.setForceStopOnRequest(VARIANT_TRUE);
    taskscheduler.setDeleteTaskAfterNoSchedule("");


    // 添加执行任务
    taskscheduler.AddTaskOperation("C://qt.exe","","");

    // 设置触发条件
    taskscheduler.setTaskTriggerType(TASK_TRIGGER_TYPE2::TASK_TRIGGER_LOGON);

    // 执行创建任务
    taskscheduler.Create_Plan_Task();
    return 0;
#endif

}
