#ifndef WINTIMETASKAPI_H
#define WINTIMETASKAPI_H

#include <QWidget>
#include <QDebug>
#define _WIN32_DCOM
#include <QCoreApplication>
#include <Windows.h>
#include <taskschd.h>
#include <comdef.h>

// 定义一个结构体来存储单个操作的信息
struct TaskOperation {
    QString executable;            // 提供程序或脚本
    QString parameters;    // 参数列表
    QString startAtDirector;               // 设置起始目录
};



class WinTimeTaskAPI : public QWidget
{
    Q_OBJECT

public:
    WinTimeTaskAPI(QWidget *parent = nullptr);
    ~WinTimeTaskAPI() override;
    //设置公开的API接口
    //1,设置任务计划程序的名称

    //1.1,设置任务计划程序的描述

    //1.2，设置安全选项

    //1.3,设置 是否用户登录时运行,以及是否要存储密码

    //1.4,设置是否使用最高权限运行

    //2,设置触发器

    //3,设置操作

    //4,设置条件

    //5,设置设置(指定影响任务行为的其他设置)




public slots:


public slots:
    // 根据自定义参数来定义
    bool Create_Plan_Task_API();  // 使用默认的 或者用户已经初始化修改过的参数来进行配置
    bool Create_Plan_Task_API(QString TaskName,QString Task_Path,QString Task_Argu);


    // 想全局任务计划中 添加任务
    void AddTaskOperation(const QString& executable, const QString& parameters, const QString& startAt);




protected:
    // 1,任务计划程序 初始化
    bool WinTimeTaskInit();  //初始化连接
    // 2,创建计划任务
    bool Create_Plan_Task(); //创建计划任务
    // 3,创建任务所需要的权限
    bool Create_Plan_Principal();
    // 4,创建任务的settings
    bool Create_Plan_Settings();

    // 5,设置任务 注册信息
    bool Create_Plan_Register();
    // 6,设置任务的动作
    bool Create_Plan_Actions();
    // 7,设置触发器
    bool Create_Plan_Triggers();

    // 8,注册任务 最后一步
    bool Create_Plan_Definition();
    // 9.释放任务计划程序资源
    bool Release_WinTask();


signals:
    void SendErrorDetail(const QVariantMap &data);
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
    ITimeTrigger* m_pTimeTrigger = NULL;
    ITaskSettings* m_pSettings = NULL; //设置任务的设置


    // 根据用户需求变更的自定义参数
private:

    // 所有参数 设置 常规设置
    QString m_Task_Name = "PASS-Name";  //计划任务名称
    QString m_Task_Desc = "PASS-Desc";             // 计划任务的描述信息
    QString m_Task_Creator = "PASS-Creator";          // 计划任务的创建者
    //安全选项
    // 计划任务的用户组
    QString m_Task_Account = "NT AUTHORITY\\SYSTEM";
    // TASK_LOGON_TYPE::TASK_LOGON_SERVICE_ACCOUNT
    TASK_LOGON_TYPE m_LOGON_TYPE = TASK_LOGON_TYPE::TASK_LOGON_SERVICE_ACCOUNT;

    // 任务权限，常规中是否使用最高权限运行 TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_LUA 为普通用户
    TASK_RUNLEVEL_TYPE m_RunLevel_Tyle = TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_HIGHEST;

    // 常规中是否隐藏在 UI中的显示
    VARIANT_BOOL m_Hide_UI_Display = VARIANT_TRUE;

    // TODO:触发器
    TASK_TRIGGER_TYPE2 m_TaskTriggerType;

    // 全局任务计划 已OK
    QList<TaskOperation> m_globalTaskOperations;
    // 条件页面的空闲
    VARIANT_BOOL RunOnlyIfIdle = VARIANT_FALSE;

    // 条件  电源页面功能控制
    VARIANT_BOOL m_Battery_State = VARIANT_FALSE; // 只有在计算机使用交流电源时才启动此任务
    VARIANT_BOOL m_DisallowStartIfBattery = VARIANT_FALSE;// 如果计算机改用电池电源，则停止
    // 唤醒计算机运行此任务
    // 该值指示任务计划程序将在运行任务时唤醒计算机，并保持计算机处于唤醒状态，直到任务完成
    VARIANT_BOOL m_Awaken_Alway_Run = VARIANT_FALSE;
    // TODO:条件 网络 部分
    // 是否只在 网络 可用时 运行任务
    VARIANT_BOOL m_RunOnlyIfNetworkAvailable = VARIANT_FALSE;
    // 1,设置页面 允许按需运行任务
    VARIANT_BOOL m_Run_Tasks_On_Demand = VARIANT_TRUE;
    // 2. 设置页面 如果过了计划开始时间,立即启动任务
    VARIANT_BOOL m_Immediate_Start_After_Scheduled_Time = VARIANT_TRUE;

    // 3. 如果任务失败,按以下频率重新启动，设置尝试重启次数
    QString m_Restart_Frequency = "10"; // 重启频率（单位：分钟）
    int m_Max_Restart_Attempts =9; // 最大重启次数

    // 4.TODO: 仍不完善 如果计划任务事件操作以下时间，停止任务(默认为3天 72小时  PT0S不限制时间)
    QString m_Task_Timeout_Hours = "72"; // 超时时间（小时） 可通过设置 PT0S

    // 5. 如果请求后任务还在运行，强行将其停止
    VARIANT_BOOL m_Force_Stop_On_Request = VARIANT_TRUE;

    // 6. 如果任务没有计划再次运行，则在此之后删除任务
    // 获取或设置
    QString m_Delete_Task_After_No_Schedule = "";

    // 7. 如果此任务已经运行，以下规则使用:
    enum class TaskRunningRule {
        Do_Not_Start_New_Instance, // 请勿启动新实例
        Allow_Concurrent_Instances, // 并行运行新实例
        Queue_New_Instances,        // 对新实例排队
        Stop_Existing_Instance       // 停止现有实例
    };

    TaskRunningRule m_Running_Rule = TaskRunningRule::Do_Not_Start_New_Instance; // 默认
};


#endif // WINTIMETASKAPI_H
