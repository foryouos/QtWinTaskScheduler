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
    // 测试WinTime Task API 接口
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

    // **************  设置 触发条件 全局条件

    IDD_ITrigger_Struct itrigger;
    itrigger.ITriggerEnabled = VARIANT_TRUE; //启用状态
    itrigger.TimeLimit = "P2DT5S"; //任务运行时间超过此值停止运行
    // 时间格式 YYYY-MM-DDTHH：MM：SS (+-) HH
    itrigger.StartBoundary = "2025-01-01T00:01:00Z";  // 开始时间，Z 表示 UTC 跨时区同步
    itrigger.EndBoundary = "2025-01-01T00:01:00Z";    // 结束时间，Z 表示 UTC
    itrigger.ID = "GlobalTrigger"; //设置触发器的标识符。
    //itrigger.TimeLimit("");

    // *******        设置 系统启动TASK_TRIGGER_BOOT  触发器的
    Task_Boot_Params    bootparams;
    bootparams.Dalay = "PT5M";  // 延迟任务时间


    //设置  TASK_TRIGGER_DAILY 的c触发条件
    DailyTriggerParams DailyParams;
    DailyParams.DayInterval = 1;
    DailyParams.RandomDelay ="P2DT5S";

    // 设置用户登录的触发条件
    Task_Logon_Params Logon_params;
    Logon_params.Delay="PT5M"; // ，该值指示用户登录和任务启动之间的时间。
    Logon_params.UserId=""; // 设置用户的标识符。

    // 每月计划
    MonthlyTriggerParams month_params;
    month_params.MonthsOfYear = MonthsOfYear::September;
    month_params.DaysOfMonth = DaysOfMonth::Day18;

    // 想触发器中添加触发数据条件
    // 1,触发器类型,2,触发器个性化参数，3,触发器全局参数
    taskscheduler.AddTaskTrigger(TASK_TRIGGER_TYPE2::TASK_TRIGGER_BOOT,QVariant::fromValue(bootparams),itrigger);
    taskscheduler.AddTaskTrigger(TASK_TRIGGER_TYPE2::TASK_TRIGGER_DAILY,QVariant::fromValue(DailyParams),itrigger);
    taskscheduler.AddTaskTrigger(TASK_TRIGGER_TYPE2::TASK_TRIGGER_LOGON,QVariant::fromValue(Logon_params),itrigger);
    taskscheduler.AddTaskTrigger(TASK_TRIGGER_TYPE2::TASK_TRIGGER_MONTHLY,QVariant::fromValue(month_params),itrigger);
    // 执行创建任务
    taskscheduler.Create_Plan_Task();
    return 0;
#endif

}
