#include "TaskScheduler.h"

// 构造函数
TaskScheduler::TaskScheduler()
{
    // 初始化其他成员变量
    // 初始化 TaskOperation
    m_taskoperation.executable = "";
    m_taskoperation.parameters = "";
    m_taskoperation.startAtDirector = "";
    // 初始化 PlanSettings
    m_plansettings.RunOnlyIfIdle = VARIANT_TRUE;
    m_plansettings.Run_Tasks_On_Demand = VARIANT_FALSE;
    m_plansettings.Immediate_Start_After_Scheduled_Time = VARIANT_TRUE;
    m_plansettings.Battery_State = VARIANT_FALSE;
    m_plansettings.DisallowStartIfBattery = VARIANT_TRUE;
    m_plansettings.Hide_UI_Display = VARIANT_FALSE;
    m_plansettings.Awaken_Alway_Run = VARIANT_TRUE;
    m_plansettings.RunOnlyIfNetworkAvailable = VARIANT_FALSE;
    m_plansettings.Max_Restart_Attempts = 3;
    m_plansettings.m_Restart_Frequency = "10";
    m_plansettings.m_Force_Stop_On_Request = VARIANT_TRUE;
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
    m_plansettings.m_Task_Timeout_Hours = "P5D";                                //如果程序运行超过设置的此时间则停止运行
    m_plansettings.m_Delete_Task_After_No_Schedule="";

    // 初始化 PlanRegister
    m_planregister.m_Task_Creator = "TaskScheduler Qt";                        //计划任务的创建者
    m_planregister.m_Task_Desc = "This is a sample plan definition.";          //计划任务的描述信息

    // 初始化 PlanTriggers
    m_plantriggers.m_TaskTriggerType = TASK_TRIGGER_TYPE2::TASK_TRIGGER_BOOT;  // 触发器 当系统启动时

    // 初始化 PlanDefinition
    m_plandefinition.m_Task_Name = "TaskScheduler API";                         // 测试计划任务程序 API
    // 初始化 m_planprincipal
    m_planprincipal.m_LOGON_TYPE = TASK_LOGON_TYPE::TASK_LOGON_NONE;            // 不管用户是否登录都要运行
    m_planprincipal.m_Task_Account = "NT AUTHORITY\\SYSTEM";                    // 申请 系统权限
    m_planprincipal.m_RunLevel_Tyle = TASK_RUNLEVEL_TYPE::TASK_RUNLEVEL_HIGHEST; // 默认使用最高权限

    wintimetaskapi = new WinTimeTaskAPI;


    this->AddTaskOperation("C://qt.exe","","");


    // TODO:创建计划任务测试
    this->Create_Plan_Task_API();


}
bool TaskScheduler::Create_Plan_Task_API()
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

// void TaskScheduler::setRunOnlyIfIdle(VARIANT_BOOL value) {
//     m_RunOnlyIfIdle = value;
// }

VARIANT_BOOL TaskScheduler::isBatteryState() const {
    return m_plansettings.Battery_State;
}

void TaskScheduler::setBatteryState(VARIANT_BOOL value) {
    m_plansettings.Battery_State = value;
}

VARIANT_BOOL TaskScheduler::isDisallowStartIfBattery() const {
    return m_plansettings.DisallowStartIfBattery;
}

void TaskScheduler::setDisallowStartIfBattery(VARIANT_BOOL value) {
    m_plansettings.DisallowStartIfBattery = value;
}

VARIANT_BOOL TaskScheduler::isAwakenAlwaysRun() const {
    return m_plansettings.Awaken_Alway_Run;
}

void TaskScheduler::setAwakenAlwaysRun(VARIANT_BOOL value) {
    m_plansettings.Awaken_Alway_Run = value;
}

VARIANT_BOOL TaskScheduler::isRunOnlyIfNetworkAvailable() const {
    return m_plansettings.RunOnlyIfNetworkAvailable;
}

void TaskScheduler::setRunOnlyIfNetworkAvailable(VARIANT_BOOL value) {
    m_plansettings.RunOnlyIfNetworkAvailable = value;
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

