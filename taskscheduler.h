#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <QString>
#include <QList>
#include <windows.h> // For VARIANT_BOOL and TASK_LOGON_TYPE enums.
#include "wintimetaskapi.h"


class TaskScheduler : public QWidget
{
    Q_OBJECT
public:
    // 构造函数
    TaskScheduler();
    ~TaskScheduler() override;

    // 常规设置接口
    QString getTaskName() const;
    void setTaskName(const QString& name);

    QString getTaskDesc() const;
    void setTaskDesc(const QString& desc);

    QString getTaskCreator() const;
    void setTaskCreator(const QString& creator);

    QString getTaskAccount() const;
    void setTaskAccount(const QString& account);

    TASK_LOGON_TYPE getLogonType() const;
    void setLogonType(TASK_LOGON_TYPE type);

    TASK_RUNLEVEL_TYPE getRunLevelType() const;
    void setRunLevelType(TASK_RUNLEVEL_TYPE type);

    VARIANT_BOOL isHideUIDisplay() const;
    void setHideUIDisplay(VARIANT_BOOL hide);

    // 全局任务操作
    QList<TaskOperation> getGlobalTaskOperations() const;
    void setGlobalTaskOperations(const QList<TaskOperation>& operations);

    // 条件页面的空闲
    // VARIANT_BOOL isRunOnlyIfIdle() const;
    // 设置条件属性 空闲选项
    void SetIdleCondition(VARIANT_BOOL Free_Time_Run,QString Free_Time,QString FreeWait_Time,VARIANT_BOOL NoFreeStop,VARIANT_BOOL HavedFreeContinueRun);

    // 电源页面功能控制
    void SetBatteryCondition(VARIANT_BOOL AC_Enable,VARIANT_BOOL BatteryStop,VARIANT_BOOL AwakenAlwaysRun);
    // 网络条件控制
    void SetNetWordCondition(VARIANT_BOOL NetworkEnableRun);



    // 允许按需运行任务
    VARIANT_BOOL isRunTasksOnDemand() const;
    void setRunTasksOnDemand(VARIANT_BOOL value);

    // 如果过了计划开始时间,立即启动任务
    VARIANT_BOOL isImmediateStartAfterScheduledTime() const;
    void setImmediateStartAfterScheduledTime(VARIANT_BOOL value);

    // 重新启动频率和最大重启次数
    QString getRestartFrequency() const;
    void setRestartFrequency(const QString& frequency);

    int getMaxRestartAttempts() const;
    void setMaxRestartAttempts(int attempts);

    // 任务超时时间
    QString getTaskTimeoutHours() const;
    void setTaskTimeoutHours(const QString& timeout);

    // 强行停止任务
    VARIANT_BOOL isForceStopOnRequest() const;
    void setForceStopOnRequest(VARIANT_BOOL value);

    // 删除任务的设置
    QString getDeleteTaskAfterNoSchedule() const;
    void setDeleteTaskAfterNoSchedule(const QString& setting);


    // 设置触发器参数


    // 创建任务计划程序函数
public slots:
    // 简单API 就是 创建名称 脚本 参数 ---简单的计划任务
    bool Create_Plan_Task();
    // 想全局任务计划中 任务名称   添加任务
    void AddTaskOperation(const QString& executable, const QString& parameters, const QString& startAt);
    // 添加触发条件 1,触发器类型，2，此类型的个性化参数，3,触发器全局参数
    void AddTaskTrigger(TASK_TRIGGER_TYPE2 TrigetType,QVariant TypeParam,IDD_ITrigger_Struct Global_Triger);


private:
    WinTimeTaskAPI *wintimetaskapi =  nullptr; // 定义任务计划函数

private:
    Plan_Principal m_planprincipal;
    TaskOperation m_taskoperation;
    PlanSettings m_plansettings;
    PlanRegister m_planregister;
    PlanDefinition m_plandefinition;
    // 添加触发条件，触发条件 可能有多个
    // PlanTriggers m_plantriggers;
    QList<PlanTriggers> m_plantriggers_list;
    // 计划任务列表
    QList<TaskOperation> m_globalTaskOperations;

};

#endif // TASKSCHEDULER_H
