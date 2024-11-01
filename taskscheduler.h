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

    TASK_TRIGGER_TYPE2 getTaskTriggerType() const;
    void setTaskTriggerType(TASK_TRIGGER_TYPE2 triggerType);

    // 全局任务操作
    QList<TaskOperation> getGlobalTaskOperations() const;
    void setGlobalTaskOperations(const QList<TaskOperation>& operations);

    // 条件页面的空闲
    VARIANT_BOOL isRunOnlyIfIdle() const;
    void setRunOnlyIfIdle(VARIANT_BOOL value);

    // 电源页面功能控制
    VARIANT_BOOL isBatteryState() const;
    void setBatteryState(VARIANT_BOOL value);

    VARIANT_BOOL isDisallowStartIfBattery() const;
    void setDisallowStartIfBattery(VARIANT_BOOL value);

    VARIANT_BOOL isAwakenAlwaysRun() const;
    void setAwakenAlwaysRun(VARIANT_BOOL value);

    VARIANT_BOOL isRunOnlyIfNetworkAvailable() const;
    void setRunOnlyIfNetworkAvailable(VARIANT_BOOL value);

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

    // 创建任务计划程序函数
public slots:
    // 简单API 就是 创建名称 脚本 参数 ---简单的计划任务
    bool Create_Plan_Task_API();
    // 想全局任务计划中 添加任务
    void AddTaskOperation(const QString& executable, const QString& parameters, const QString& startAt);



private:
    WinTimeTaskAPI *wintimetaskapi =  nullptr; // 定义任务计划函数

private:
    Plan_Principal m_planprincipal;
    TaskOperation m_taskoperation;
    PlanSettings m_plansettings;
    PlanRegister m_planregister;
    PlanTriggers m_plantriggers;
    PlanDefinition m_plandefinition;
    // 计划任务列表
    QList<TaskOperation> m_globalTaskOperations;

};

#endif // TASKSCHEDULER_H
