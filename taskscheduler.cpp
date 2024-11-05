#include "TaskScheduler.h"

// 构造函数
TaskScheduler::TaskScheduler()
{
    // 计划任务的名称
    m_plandefinition.m_Task_Name = "TaskScheduler API";
    // 计划任务的登录用户组 系统用户组
    m_planprincipal.m_Task_Account = "NT AUTHORITY\\SYSTEM";
    //计划任务的创建者
    m_planregister.m_Task_Creator = "TaskScheduler Qt";
    //计划任务的描述信息
    m_planregister.m_Task_Desc = "This is a sample plan definition.";
    // 不管用户是否登录都要运行
    m_planprincipal.m_LOGON_TYPE = TASK_LOGON_TYPE::TASK_LOGON_NONE;            // 不管用户是否登录都要运行
    // 是否使用管理员权限
    m_planprincipal.m_RunLevel_Tyle = TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_HIGHEST; // 默认使用最高权限
    // 隐藏
    m_plansettings.Hide_UI_Display = VARIANT_FALSE;

    // 初始化 PlanSettings
    //  是否启用仅当计算机空闲时间超过下列值时才启动此任务
    m_plansettings.RunOnlyIfIdle = VARIANT_TRUE;
    // 指示运行任务之前计算机必须处于空闲状态的时间
    m_plansettings.IdleDuration ="P5D";
    // 空闲时间 等待的时间量
    m_plansettings.WaitTimeout = "PT1H";

    // 如果计算机不再空闲 则停止
    m_plansettings.StopOnIdleEnd = VARIANT_TRUE;

    // 如果空闲状态继续，则重新启动
    m_plansettings.RestartOnIdle = VARIANT_FALSE;
    // 是否只有在计算机使用交流电源时才启动任务
    m_plansettings.DisallowStartIfBattery = VARIANT_TRUE;
    // 如果计算机改用电池电源，则停止
    m_plansettings.Battery_State = VARIANT_TRUE;

    // 唤醒计算机 运行此任务
    m_plansettings.Awaken_Alway_Run = VARIANT_FALSE;
    // 只有在以下网络连接可用时才启动
    m_plansettings.RunOnlyIfNetworkAvailable = VARIANT_FALSE;

    // 设置: 允许按需运行任务
    m_plansettings.Run_Tasks_On_Demand = VARIANT_TRUE;
    // 设置:如果过了计划开始时间，立即启动任务
    m_plansettings.Immediate_Start_After_Scheduled_Time = VARIANT_FALSE;
    // 如果任务失败，按一下频率重新启动
    m_plansettings.m_Restart_Frequency = "PT1M";
    // 尝试重新启动最多次数
    m_plansettings.Max_Restart_Attempts = 3;
    // 4: 如果计划任务事件操作以下时间，停止任务(默认为3天 72小时  PT0S不限制时间)
    // 其格式有具体的限制  按照规范 设置 否则 会设置失败
    /* 此字符串的格式为 PnYnMnDTnHnMnS，
     * 其中 nY 是年数，
     * nM 是月数，   // P5M  ---五个月
     * nD 是天数，   //P5D --- 五天
     * “T”是日期/时间分隔符，
     * nH 是小时数，
     * nM 是分钟数，
     * nS 是秒数
     *  (例如，PT5M 指定 5 分钟，P1M4DT2H5M指定一个月，) 4 天 2 小时 5 分钟。
     *  PT0S 值将使任务无限期运行。
     *
     * */
    m_plansettings.m_Task_Timeout_Hours = "P3D";//如果程序运行超过设置的此时间则停止运行
    // TODO:如果请求后任务还在运行，强行将其停止  未生效
    m_plansettings.m_Force_Stop_On_Request = VARIANT_TRUE;
    // TODO: 暂未生效 需要触发器配合 如果任务在此之后没有运行 则删除任务  如果未为此属性指定任何只，则任务计划程序服务不会删除该任务
    m_plansettings.m_Delete_Task_After_No_Schedule="";


    // 初始化 任务执行清单
    m_taskoperation.executable = "";
    m_taskoperation.parameters = "";
    m_taskoperation.startAtDirector = "";
    // 初始化 触发器 PlanTriggers
    m_plantriggers.m_TaskTriggerType = TASK_TRIGGER_TYPE2::TASK_TRIGGER_BOOT;  // 触发器 当系统启动时

    // 测试计划任务程序 API


    wintimetaskapi = new WinTimeTaskAPI;


    // this->AddTaskOperation("C://qt.exe","","");
    // // TODO:创建计划任务测试
    // this->Create_Plan_Task_API();


}
// 任务计划的析构函数
TaskScheduler::~TaskScheduler()
{

}
bool TaskScheduler::Create_Plan_Task()
{
    if(wintimetaskapi == nullptr)
    {
        wintimetaskapi = new WinTimeTaskAPI;
    }
    qDebug()<<"创建计划任务";
    // 由于全局参数在 taskscheduler函数中，所以此处调用集体参数，传给接口函数
    // 1. 任务计划程序初始化
    if (!wintimetaskapi->WinTimeTaskInit()) {
        qDebug() << "Failed to initialize task scheduler.";
        return false;
    }

    // 2. 创建计划任务
    if (!wintimetaskapi->Create_Plan_Task()) {
        qDebug() << "Failed to create plan task.";
        return false;
    }

    // 3. 创建任务所需要的权限
    if (!wintimetaskapi->Create_Plan_Principal(m_planprincipal)) {
        qDebug() << "Failed to create plan principal.";
        return false;
    }

    // 4. 创建任务的 settings
    if (!wintimetaskapi->Create_Plan_Settings(m_plansettings)) {
        qDebug() << "Failed to create plan settings.";
        return false;
    }

    // 5. 设置任务注册信息
    if (!wintimetaskapi->Create_Plan_Register(m_planregister)) {
        qDebug() << "Failed to create plan register.";
        return false;
    }

    // 6. 设置任务的动作
    if (!wintimetaskapi->Create_Plan_Actions(m_globalTaskOperations)) {
        qDebug() << "Failed to create plan actions.";
        return false;
    }

    // 7. 设置触发器
    if (!wintimetaskapi->Create_Plan_Triggers(m_plantriggers)) {
        qDebug() << "Failed to create plan triggers.";
        return false;
    }

    // 8. 注册任务最后一步
    if (!wintimetaskapi->Create_Plan_Definition(m_plandefinition)) {
        qDebug() << "Failed to create plan definition.";
        return false;
    }

    // 9. 释放任务计划程序资源
    wintimetaskapi->Release_WinTask();

    qDebug() << "Task created successfully.";
    return true;

}

void TaskScheduler::AddTaskOperation(const QString &executable, const QString &parameters, const QString &startAt)
{
    TaskOperation operation; // 创建一个 TaskOperation 实例
    operation.executable = executable; // 设置可执行文件
    operation.parameters = parameters;   // 设置参数
    operation.startAtDirector = startAt;         // 设置起始时间
    // 将操作添加到全局任务计划
    m_globalTaskOperations.append(operation);
}

// Getter 和 Setter 实现
QString TaskScheduler::getTaskName() const {
    return m_plandefinition.m_Task_Name;
}

void TaskScheduler::setTaskName(const QString& name) {
    m_plandefinition.m_Task_Name = name;
}

QString TaskScheduler::getTaskDesc() const {
    return m_planregister.m_Task_Desc;
}

void TaskScheduler::setTaskDesc(const QString& desc) {
    m_planregister.m_Task_Desc = desc;
}

QString TaskScheduler::getTaskCreator() const {
    return m_planregister.m_Task_Creator;
}

void TaskScheduler::setTaskCreator(const QString& creator) {
    m_planregister.m_Task_Creator = creator;
}

QString TaskScheduler::getTaskAccount() const {
    return m_planprincipal.m_Task_Account;
}

void TaskScheduler::setTaskAccount(const QString& account) {
    m_planprincipal.m_Task_Account = account;
}

TASK_LOGON_TYPE TaskScheduler::getLogonType() const {
    return m_planprincipal.m_LOGON_TYPE;
}

void TaskScheduler::setLogonType(TASK_LOGON_TYPE type) {
    m_planprincipal.m_LOGON_TYPE = type;
}

TASK_RUNLEVEL_TYPE TaskScheduler::getRunLevelType() const {
    return m_planprincipal.m_RunLevel_Tyle;
}

void TaskScheduler::setRunLevelType(TASK_RUNLEVEL_TYPE type) {
    m_planprincipal.m_RunLevel_Tyle = type;
}

VARIANT_BOOL TaskScheduler::isHideUIDisplay() const {
    return m_plansettings.Hide_UI_Display;
}

void TaskScheduler::setHideUIDisplay(VARIANT_BOOL hide) {
    m_plansettings.Hide_UI_Display = hide;
}

TASK_TRIGGER_TYPE2 TaskScheduler::getTaskTriggerType() const {
    return m_plantriggers.m_TaskTriggerType;
}

void TaskScheduler::setTaskTriggerType(TASK_TRIGGER_TYPE2 triggerType) {
    m_plantriggers.m_TaskTriggerType = triggerType;
}

// Getter 和 Setter 实现
QList<TaskOperation> TaskScheduler::getGlobalTaskOperations() const {
    return m_globalTaskOperations;
}

void TaskScheduler::setGlobalTaskOperations(const QList<TaskOperation>& operations) {
    m_globalTaskOperations = operations;
}

// VARIANT_BOOL TaskScheduler::isRunOnlyIfIdle() const {
//     return m_RunOnlyIfIdle;
// }

void TaskScheduler::SetIdleCondition(VARIANT_BOOL Free_Time_Run,QString Free_Time,QString FreeWait_Time,VARIANT_BOOL NoFreeStop,VARIANT_BOOL HavedFreeContinueRun)
{
    m_plansettings.RunOnlyIfIdle = Free_Time_Run;
    // 指示运行任务之前计算机必须处于空闲状态的时间
    m_plansettings.IdleDuration =Free_Time;
    // 空闲时间 等待的时间量
    m_plansettings.WaitTimeout = FreeWait_Time;

    // 如果计算机不再空闲 则停止
    m_plansettings.StopOnIdleEnd = NoFreeStop;

    // 如果空闲状态继续，则重新启动
    m_plansettings.RestartOnIdle = HavedFreeContinueRun;
}
void TaskScheduler::SetBatteryCondition(VARIANT_BOOL AC_Enable, VARIANT_BOOL BatteryStop, VARIANT_BOOL AwakenAlwaysRun)
{
    // 是否只有在计算机使用交流电源时才启动任务
    m_plansettings.DisallowStartIfBattery = AC_Enable;
    // 如果计算机改用电池电源，则停止
    m_plansettings.Battery_State = BatteryStop;
    // 唤醒计算机 运行此任务
    m_plansettings.Awaken_Alway_Run = AwakenAlwaysRun;
}

void TaskScheduler::SetNetWordCondition(VARIANT_BOOL NetworkEnableRun)
{
    m_plansettings.RunOnlyIfNetworkAvailable = NetworkEnableRun;
}

VARIANT_BOOL TaskScheduler::isRunTasksOnDemand() const {
    return m_plansettings.Run_Tasks_On_Demand;
}

void TaskScheduler::setRunTasksOnDemand(VARIANT_BOOL value) {
    m_plansettings.Run_Tasks_On_Demand = value;
}

VARIANT_BOOL TaskScheduler::isImmediateStartAfterScheduledTime() const {
    return m_plansettings.Immediate_Start_After_Scheduled_Time;
}

void TaskScheduler::setImmediateStartAfterScheduledTime(VARIANT_BOOL value) {
    m_plansettings.Immediate_Start_After_Scheduled_Time = value;
}

QString TaskScheduler::getRestartFrequency() const {
    return m_plansettings.m_Restart_Frequency;
}

void TaskScheduler::setRestartFrequency(const QString& frequency) {
    m_plansettings.m_Restart_Frequency = frequency;
}

int TaskScheduler::getMaxRestartAttempts() const {
    return m_plansettings.Max_Restart_Attempts;
}

void TaskScheduler::setMaxRestartAttempts(int attempts) {
    m_plansettings.Max_Restart_Attempts = attempts;
}

QString TaskScheduler::getTaskTimeoutHours() const {
    return m_plansettings.m_Task_Timeout_Hours;
}

void TaskScheduler::setTaskTimeoutHours(const QString& timeout) {
    m_plansettings.m_Task_Timeout_Hours = timeout;
}

VARIANT_BOOL TaskScheduler::isForceStopOnRequest() const {
    return m_plansettings.m_Force_Stop_On_Request;
}

void TaskScheduler::setForceStopOnRequest(VARIANT_BOOL value) {
    m_plansettings.m_Force_Stop_On_Request = value;
}

QString TaskScheduler::getDeleteTaskAfterNoSchedule() const {
    return m_plansettings.m_Delete_Task_After_No_Schedule;
}

void TaskScheduler::setDeleteTaskAfterNoSchedule(const QString& setting) {
    m_plansettings.m_Delete_Task_After_No_Schedule = setting;
}

