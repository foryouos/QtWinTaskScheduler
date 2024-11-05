#ifndef WINTIMETASKAPI_H
#define WINTIMETASKAPI_H

#include <QWidget>
#include <QDebug>
#define _WIN32_DCOM
#include <QCoreApplication>
#include <Windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <wincred.h>

enum class TaskRunningRule {
    Do_Not_Start_New_Instance, // 请勿启动新实例
    Allow_Concurrent_Instances, // 并行运行新实例
    Queue_New_Instances,        // 对新实例排队
    Stop_Existing_Instance       // 停止现有实例
};
// 定义结构体
struct Plan_Principal
{
    TASK_LOGON_TYPE m_LOGON_TYPE;       //
    QString m_Task_Account;             // 计划任务的用户组
    TASK_RUNLEVEL_TYPE m_RunLevel_Tyle; // 任务权限，常规中是否使用最高权限运行
};

// 定义一个结构体来存储单个操作的信息
struct TaskOperation {
    QString executable;             // 提供程序或脚本
    QString parameters;             // 参数列表
    QString startAtDirector;        // 设置起始目录
};

// 计划设置参数
struct PlanSettings {
    VARIANT_BOOL RunOnlyIfIdle;  // 条件页面的空闲 仅当计算机空闲时间超过如下值才启动此任务
    VARIANT_BOOL StopOnIdleEnd; // 该值表示如果空闲条件在任务完成之前结束，任务计划将终止任务，空闲条件在计算机不再空闲时结束
    QString IdleDuration;       // 指示运行任务之前计算机必须处于空闲状态的时间
    VARIANT_BOOL RestartOnIdle;  // 如果空闲状态继续则重新启动
    QString WaitTimeout;         // 计划任务程序空闲条件出现的时间量
    VARIANT_BOOL Run_Tasks_On_Demand;// 1,设置计划任务 允许按需运行任务
    VARIANT_BOOL Immediate_Start_After_Scheduled_Time;
    VARIANT_BOOL Battery_State; // 只有在计算机使用交流电源时才启动此任务
    VARIANT_BOOL DisallowStartIfBattery; // 如果计算机改用电池电源，则停止
    VARIANT_BOOL Hide_UI_Display;  // 常规中是否隐藏在 UI中的显示
    VARIANT_BOOL Awaken_Alway_Run;     // 唤醒计算机运行此任务 该值指示任务计划程序将在运行任务时唤醒计算机，并保持计算机处于唤醒状态，直到任务完成
    VARIANT_BOOL RunOnlyIfNetworkAvailable; // 是否只在 网络 可用时 运行任务
    VARIANT_BOOL m_Force_Stop_On_Request;  // 如果请求后任务还在运行，强行将其停止
    int Max_Restart_Attempts; // 使用int来存储最大重启次数
    QString m_Task_Timeout_Hours; // 此字符串的格式为 PnYnMnDTnHnMnS， 如果计划任务事件超过以下时间，停止任务(默认为3天 72小时  PT0S不限制时间)
    QString m_Delete_Task_After_No_Schedule; // 6. 如果任务没有计划再次运行，则在此之后删除任务
    QString m_Restart_Frequency; // 重启程序频率（单位：分钟）
};

// 定义IRepetitionPattern 接口任务运行的频率
struct IRepetitionPattern_Struct
{
    QString Duration;  //设置重复模式的时长
    QString Interval;  // 设置每次重启任务之间的时间量
    VARIANT_BOOL StopAtDurationEnd; //任务运行实例是否在重复模式持续时间结束时停止
};
// 定义编辑触发器的全局参数
struct IDD_ITrigger_Struct {
    VARIANT_BOOL ITriggerEnabled; //是否启用触发器
    QString EndBoundary;          // 停用触发器的日期和时间(在停用任务后无法启动任务)
    QString TimeLimit;           // 设置允许触发器启动的任务运行的最长时间
    QString ID;                   // 设置触发器的标识符
    QString StartBoundary;        // 设置激活触发器的日期和时间.
    struct IRepetitionPattern_Struct *m_IRepetitionpatter;//

};
// 创建计划触发器
// TODO:完善触发器的其它参数
// 定义不同触发器的参数结构体
struct DailyTriggerParams {
    int DayInterval;
    QString RandomDelay;
};

struct WeeklyTriggerParams {
    int dayOfWeek;
    int WeeksInterval;
    QString RandomDelay;
};
struct MonthlyTriggerParams {
    int MonthsOfYear;
    int DaysOfMonth;
    QString RandomDelay;
    VARIANT_BOOL RunOnLastDayOfMonth;
};
struct MONTHLYDOWTriggerParams {
    int DaysOfWeek;
    int WeeksOfMonth;
    int MonthsOfYear;
    QString RandomDelay;
    VARIANT_BOOL RunOnLastWeekOfMonth;
};
struct TASK_TRIGGER_SESSION_Params{
    QString UserId;
    QString Delay;
    TASK_SESSION_STATE_CHANGE_TYPE StateChange;

};
struct Task_Logon_Params{
    QString UserId;
    QString Delay;
};
struct Task_Boot_Params{
    QString Dalay;
};
struct Task_Registration_Param{
    QString Dalay;
};

struct TASK_TRIGGER_IDLE_param{

};
struct TASK_TRIGGER_TIME_param{
    QString RandomDelay;
};
struct TASK_TRIGGER_EVENT_Param{
    QString Delay;
    QString Subscription;
};


// 注册自定义类型到 QVariant
// Q_DECLARE_METATYPE 宏会将这些自定义类型注册到Qt的元对象系统中
Q_DECLARE_METATYPE(DailyTriggerParams)
Q_DECLARE_METATYPE(WeeklyTriggerParams)

Q_DECLARE_METATYPE(MonthlyTriggerParams)
Q_DECLARE_METATYPE(MONTHLYDOWTriggerParams)
Q_DECLARE_METATYPE(TASK_TRIGGER_SESSION_Params)
Q_DECLARE_METATYPE(Task_Logon_Params)
Q_DECLARE_METATYPE(Task_Boot_Params)
Q_DECLARE_METATYPE(Task_Registration_Param)
Q_DECLARE_METATYPE(TASK_TRIGGER_IDLE_param)
Q_DECLARE_METATYPE(TASK_TRIGGER_TIME_param)
Q_DECLARE_METATYPE(TASK_TRIGGER_EVENT_Param)

struct PlanTriggers
{
    TASK_TRIGGER_TYPE2 m_TaskTriggerType; //什么时候触发，触发类型
    struct IDD_ITrigger_Struct *m_ITrigger;  //定义编辑触发器的全局参数
    QVariant  m_TriggerParams;  // 使用variant存储不同类型的触发器参数

};




// 创建 计划 寄存器参数
struct PlanRegister
{
    QString m_Task_Creator; // 计划任务的创建者
    QString m_Task_Desc;  // 计划任务的描述信息
};
// 创建计划 Definition
struct PlanDefinition
{
    QString m_Task_Name;  //计划任务名称
};


class WinTimeTaskAPI : public QWidget
{
    Q_OBJECT

public:
    WinTimeTaskAPI(QWidget *parent = nullptr);
    ~WinTimeTaskAPI() override;

public slots:

    // 1,任务计划程序 初始化
    bool WinTimeTaskInit();  //初始化连接
    // 2,创建计划任务
    bool Create_Plan_Task(); //创建计划任务
    // 3,创建任务所需要的权限
    bool Create_Plan_Principal(const Plan_Principal& planprincipal);
    // 4,创建任务的settings
    bool Create_Plan_Settings(const PlanSettings& settings);

    // 5,设置任务 注册信息
    bool Create_Plan_Register(const PlanRegister& planregister);
    // 6,设置任务的动作
    bool Create_Plan_Actions(QList<TaskOperation> m_globalTaskOperations);
    // 7,设置触发器
    bool Create_Plan_Triggers(const PlanTriggers& plantrigers);

    // 8,注册任务 最后一步
    bool Create_Plan_Definition(const PlanDefinition& plandefine);
    // 9.释放任务计划程序资源
    bool Release_WinTask();
private:
    void Deal_Fail_Hr(HRESULT m_hr);

signals:
    void SendErrorDetail(const QString &data);
    // 计划任务创建与修改过程中的 全局参数
private:
    HRESULT m_hr;
    ITaskService* m_pService = NULL;
    ITaskDefinition* m_pTask = NULL;
    IRegistrationInfo* m_pRegInfo = NULL;
    IPrincipal* m_pPrincipal = NULL;
    IActionCollection* m_pActionCollection = NULL;
    IAction* m_pAction = NULL;
    ITriggerCollection* m_pTriggerCollection = NULL;
    ITrigger* m_pTrigger = NULL;

    ITaskSettings* m_pSettings = NULL; //设置任务的设置
    IIdleSettings *m_IIdlesettings = nullptr;  //指定任务计划程序在

    // 设置一些触发器配置
    ITimeTrigger* m_pTimeTrigger = NULL; // 在一天中的特定时间触发任务
    IEventTrigger *m_pEventTrigger = NULL;
    IDailyTrigger *m_IDailyTrigger = nullptr; // 按每日计划触发任务。
    IWeeklyTrigger *m_pWeeklyTrigger = nullptr;
    IMonthlyTrigger *m_IMonthlyTrigger = nullptr;
    IMonthlyDOWTrigger *m_IMonthlyDOWTrigger = nullptr;
    IIdleTrigger *m_IIdleTrigger = nullptr;   //计算机进入空闲状态时启动任务的触发器
    IRegistrationTrigger *m_IRegistrationTrigger = nullptr;
    IBootTrigger *m_IBootTrigger = nullptr;
    ILogonTrigger *m_ILogonTrigger = nullptr;
    ISessionStateChangeTrigger *m_ISessionStateChangeTrigger = nullptr;
    // 根据用户需求变更的自定义参数
private:

};


#endif // WINTIMETASKAPI_H
