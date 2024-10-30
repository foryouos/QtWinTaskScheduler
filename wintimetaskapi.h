#ifndef WINTIMETASKAPI_H
#define WINTIMETASKAPI_H
/* API接口:通过Qt来添加修改windows的任务计划程序，方便在实际的开发过程中进行调用，
 *          涉及到 创建 修改 删除 禁用
 *          目前Github仍然没有找到相关的开源程序，所以就总结一下，以备后面使用
 *
 *    常规:
 *      1,任务名称，
 *      2,任务描述，
 *      3,任务创建者，
 *      4,任务运行用户组，
 *      5,是否使用最高权限，
 *      6,用户登录运行/不管用户是否登录都要运行 ，是否存储密码(不存储密码，该任务将只有访问本地计算机资源的权限)
 *
 *    触发器(即触发条件可多个):
 *      1,开始任务
 *      2,任务设置: 根据不同的开始任务条件 会触发不同的设置
 *      3,高级设置
 *          3.1 : 任务最多延迟事件(随机延迟)
 *          3.2 : 重复任务间隔
 *          3.3 : 任务的运行时间超过此值则停止执行
 *          3.4 : 到期时间
 *          3.5 : 已启用
 *
 *
 *    操作(触发执行的内容,可多个):
 *      1,操作:启动程序
 *      2,程序或脚本:绝对路径  参数(可选) 起始于(可选)
 *
 *
 *    任务计划任务条件: 指定用于与触发器一起判断是否应运行该任务的条件。如果这里指定的条件不为真，该任务不会运行.
 *      1,仅当计算机空闲时间超过下列值时才启动此任务
 *          1,如果计算机不再空闲，则停止
 *          2,如果空闲状态继续，则重新启动
 *      2,电源：
 *          1,只有在计算机使用交流电源时才启动此任务
 *            1,如果计算机改用电池电源，则停止
 *          2,唤醒计算机运行此任务
 *      3,网络:
 *          1,只有在以下网络连接可用时才启动。
 *
 *
 *     设置: 指定影响任务行为的其它设置
 *          1,允许按需运行任务
 *          2,如果过了计划开始时间,立即启动任务
 *          3,如果任务失败,按以下频率重新启动，-设置尝试重启启动最多次数
 *          4,如果计划任务事件操作以下时间，停止任务(默认为3天 72小时)
 *          5,如果请求后任务还在运行，强行将其停止
 *          6,如果任务没有计划再次运行,则在此之后删除任务
 *          7,如果此任务已经运行，以下规则使用:
 *             1,请勿启动新实例
 *             2,并行运行新实例
 *             3,对新实例排队
 *             4,停止现有实例
 *
 * 联系方式:foryouos@qq.com
 * 微信公众号:瓶子的跋涉
 * */

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
    QString m_Task_Name = "TaskAPIName";  //计划任务名称
    QString m_Task_Desc = "";             // 计划任务的描述信息
    QString m_Task_Creator = "";          // 计划任务的创建者
    //安全选项
    QString m_Task_Account = "NT AUTHORITY\\SYSTEM";          // 计划任务的用户组
    TASK_LOGON_TYPE m_LOGON_TYPE = TASK_LOGON_TYPE::TASK_LOGON_SERVICE_ACCOUNT;
    bool    m_Task_Login_Run = true;      // 是否在用户登录时运行 false将不登录就运行
    bool    m_Task_Save_Password = false;  // 是否存储密码,不存储密码 该任务将只有访问本地计算机资源的权限
    TASK_RUNLEVEL_TYPE m_RunLevel_Tyle = TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_LUA;
    bool    m_Hide_Task = false;          // 隐藏任务

    // 触发器
    TASK_TRIGGER_TYPE2 m_TaskTriggerType;

    // 全局任务计划
    QList<TaskOperation> m_globalTaskOperations;
    // 条件
    VARIANT_BOOL m_Battery_State = VARIANT_TRUE;
    VARIANT_BOOL m_DisallowStartIfBattery = VARIANT_FALSE;
    // 设置 允许按需运行任务

    // 1,允许按需运行任务
    VARIANT_BOOL m_Run_Tasks_On_Demand = VARIANT_TRUE;
    // 2. 如果过了计划开始时间,立即启动任务
    VARIANT_BOOL m_Immediate_Start_After_Scheduled_Time = VARIANT_TRUE;

    // 3. 如果任务失败,按以下频率重新启动，设置尝试重启次数
    int m_Restart_Frequency = 10; // 重启频率（单位：分钟）
    int m_Max_Restart_Attempts = 3; // 最大重启次数

    // 4. 如果计划任务事件操作以下时间，停止任务(默认为3天 72小时)
    QString m_Task_Timeout_Hours = "72"; // 超时时间（小时） 可通过设置 PT0S

    // 5. 如果请求后任务还在运行，强行将其停止
    VARIANT_BOOL m_Force_Stop_On_Request = VARIANT_TRUE;

    // 6. 如果任务没有计划再次运行，则在此之后删除任务
    VARIANT_BOOL m_Delete_Task_After_No_Schedule = VARIANT_TRUE;

    // 7. 如果此任务已经运行，以下规则使用:
    enum class TaskRunningRule {
        Do_Not_Start_New_Instance, // 请勿启动新实例
        Allow_Concurrent_Instances, // 并行运行新实例
        Queue_New_Instances,        // 对新实例排队
        Stop_Existing_Instance       // 停止现有实例
    };

    TaskRunningRule m_Running_Rule = TaskRunningRule::Do_Not_Start_New_Instance; // 默认







};

inline void WinTimeTaskAPI::AddTaskOperation(const QString &executable, const QString &parameters, const QString &startAt)
{
    TaskOperation operation; // 创建一个 TaskOperation 实例
    operation.executable = executable; // 设置可执行文件
    operation.parameters = parameters;   // 设置参数
    operation.startAtDirector = startAt;         // 设置起始时间

    // 将操作添加到全局任务计划
    m_globalTaskOperations.append(operation);
}
#endif // WINTIMETASKAPI_H
