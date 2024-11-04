#include "wintimetaskapi.h"


using namespace std;

WinTimeTaskAPI::WinTimeTaskAPI(QWidget *parent)
    : QWidget(parent)
{
}

WinTimeTaskAPI::~WinTimeTaskAPI()
{

}

bool WinTimeTaskAPI::WinTimeTaskInit()
{
    // 初始化COM
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // 创建任务服务对象
    m_hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&m_pService);
    if (FAILED(m_hr))
    {
        emit SendErrorDetail(QString("Failed to create TaskService instance:%1").arg(_com_error(m_hr).ErrorMessage()));
        qDebug()<<"Failed to create TaskService instance: " << _com_error(m_hr).ErrorMessage();
        return false;
    }

    // 连接到任务服务
    m_hr = m_pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(m_hr))
    {
        emit  SendErrorDetail(QString("Failed to connect to TaskService:%1").arg(_com_error(m_hr).ErrorMessage()));
        qDebug()<<"Failed to connect to TaskService: " << _com_error(m_hr).ErrorMessage();
        m_pService->Release();
        return false;
    }
    return true;
}

bool WinTimeTaskAPI::Create_Plan_Task()
{
    // 创建任务定义
    m_hr = m_pService->NewTask(0, &m_pTask);
    if (FAILED(m_hr))
    {
        emit SendErrorDetail(QString("Failed to create new task::%1").arg(_com_error(m_hr).ErrorMessage()));
        qDebug()<<"Failed to create new task: " << _com_error(m_hr).ErrorMessage();
        m_pService->Release();
        return false;
    }
    // m_
    return true;
}

bool WinTimeTaskAPI::Create_Plan_Principal(const Plan_Principal& planprincipal)
{
    // 定义任务所需要的权限      获取任务的 Principal
    m_hr = m_pTask->get_Principal(&m_pPrincipal);
    if (SUCCEEDED(m_hr)) {
        // 设置 Principal 为系统账户并要求最高权限 m_Task_Account.toStdWString().c_str()
        // m_pPrincipal->put_UserId(_bstr_t(L"NT AUTHORITY\\SYSTEM")); //系统用户
        m_pPrincipal->put_UserId(SysAllocString(reinterpret_cast<const wchar_t*>(planprincipal.m_Task_Account.utf16()))); //系统用户
        m_pPrincipal->put_LogonType(planprincipal.m_LOGON_TYPE);    //
        m_pPrincipal->put_RunLevel(planprincipal.m_RunLevel_Tyle); // 设置为最高权限

        // 释放 Principal
        m_pPrincipal->Release();
        return true;
    }
    else
    {
        this->Deal_Fail_Hr(m_hr);
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Settings(const PlanSettings& settings)
{
    // 访问结构体成员
    VARIANT_BOOL RunOnlyIfIdle = settings.RunOnlyIfIdle;

    VARIANT_BOOL StopOnIdleEnd = settings.StopOnIdleEnd;
    QString IdleDuration = settings.IdleDuration;
    VARIANT_BOOL RestartOnIdle = settings.RestartOnIdle;
    QString WaitTimeout = settings.WaitTimeout;
    VARIANT_BOOL m_Run_Tasks_On_Demand = settings.Run_Tasks_On_Demand;
    VARIANT_BOOL m_Immediate_Start_After_Scheduled_Time = settings.Immediate_Start_After_Scheduled_Time;
    VARIANT_BOOL m_Battery_State = settings.Battery_State;
    VARIANT_BOOL m_DisallowStartIfBattery = settings.DisallowStartIfBattery;
    VARIANT_BOOL m_Hide_UI_Display = settings.Hide_UI_Display;
    VARIANT_BOOL m_Awaken_Alway_Run = settings.Awaken_Alway_Run;
    VARIANT_BOOL m_RunOnlyIfNetworkAvailable = settings.RunOnlyIfNetworkAvailable;
    VARIANT_BOOL m_Force_Stop_On_Request = settings.m_Force_Stop_On_Request;
    int m_Max_Restart_Attempts = settings.Max_Restart_Attempts;
    QString m_Task_Timeout_Hours = settings.m_Task_Timeout_Hours;
    QString m_Delete_Task_After_No_Schedule = settings.m_Delete_Task_After_No_Schedule;
    QString m_Restart_Frequency = settings.m_Restart_Frequency;

    //设置 任务的 Settings
    m_hr = m_pTask->get_Settings(&m_pSettings);
    if (SUCCEEDED(m_hr))
    {
        // 创建并配置空闲设置
        // IIdleSettings idlesetting;
        // // 应用空闲设置
        m_hr = m_pSettings->put_RunOnlyIfIdle(RunOnlyIfIdle);
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // IIdleSettings 接口表示指定任务计划程序在计算机处于空闲状态时如何执行任务
        m_pSettings->get_IdleSettings(&m_IIdlesettings);
        if(SUCCEEDED(m_hr))
        {
            // 指示在运行任务之前计算机必须处于空闲状态的时间
            m_hr = m_IIdlesettings->put_IdleDuration(SysAllocString(IdleDuration.toStdWString().c_str()));
            if(FAILED(m_hr))
            {
                this->Deal_Fail_Hr(m_hr);
            }
            // 如果空闲状态继续，则重新启动
            m_hr = m_IIdlesettings->put_RestartOnIdle(RestartOnIdle);
            if(FAILED(m_hr))
            {
                this->Deal_Fail_Hr(m_hr);
            }
            // 如果计算机不再空闲，则停止
            m_hr = m_IIdlesettings->put_StopOnIdleEnd(StopOnIdleEnd);
            if(FAILED(m_hr))
            {
                this->Deal_Fail_Hr(m_hr);
            }

            // 设置等待空闲条件出现的时间量
            m_hr = m_IIdlesettings->put_WaitTimeout(SysAllocString(reinterpret_cast<const wchar_t*>(WaitTimeout.utf16())));
            if(FAILED(m_hr))
            {
                this->Deal_Fail_Hr(m_hr);
            }
        }
        else
        {
            this->Deal_Fail_Hr(m_hr);
        }



        // m_pSettings->put_IdleSettings(idleSettings);
        m_hr = m_pSettings->put_StartWhenAvailable(m_Run_Tasks_On_Demand); // 可用时启动
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        m_hr = m_pSettings->put_AllowDemandStart(m_Immediate_Start_After_Scheduled_Time);   // 允许手动启动
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        m_hr = m_pSettings->put_StopIfGoingOnBatteries(m_Battery_State); // 如果在电池上停止
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        m_hr = m_pSettings->put_DisallowStartIfOnBatteries(m_DisallowStartIfBattery); // 不限制电池下启动
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // TODO:设置最大运行时间，例如 1 小时
        // SysAllocString(reinterpret_cast<const wchar_t*>(m_Task_Timeout_Hours.utf16()))
        //HRESULT result =  m_pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT5M")); // 允许手动启动 。 值为 PT0S 将使任务可以无限期运行。
        HRESULT result =  m_pSettings->put_ExecutionTimeLimit(SysAllocString(reinterpret_cast<const wchar_t*>(m_Task_Timeout_Hours.utf16())));

        if (SUCCEEDED(result)) {
            qDebug()<<"put_ExecutionTimeLimit:"<<"设置成功";
        } else {
            // 错误处理.
            this->Deal_Fail_Hr(result);
            qDebug()<<"put_ExecutionTimeLimit:"<<"设置 失败 Fail";
        }
        // 设置过期后删除任务的时间（例如 30 天后）
        m_hr = m_pSettings->put_Hidden(m_Hide_UI_Display);

        // m_pSettings->put_NetworkSettings(INetworkSettings);
        BSTR delay;
        m_hr = m_pSettings->get_DeleteExpiredTaskAfter(&delay);
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        qDebug()<<"get_DeleteExpiredTaskAfter:"<<QString::fromWCharArray(delay);

        // 如果任务没有计划再次运行，则在此之后删除任务
        // 如果未为此属性指定任何只，则任务计划程序服务不会删除该任务
        m_hr = m_pSettings->put_DeleteExpiredTaskAfter(SysAllocString(reinterpret_cast<const wchar_t*>(m_Delete_Task_After_No_Schedule.utf16())));
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // 该值指示任务计划程序将在运行任务时唤醒计算机，并保持计算机处于唤醒状态，直到任务完成
        m_hr = m_pSettings->put_WakeToRun(m_Awaken_Alway_Run);
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // 是否 只在网络 可用时 运行任务
        m_hr = m_pSettings->put_RunOnlyIfNetworkAvailable(m_RunOnlyIfNetworkAvailable);
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }

        // 如果任务失败，按如下频率重新启动
        m_hr = m_pSettings->put_RestartInterval(SysAllocString(reinterpret_cast<const wchar_t*>(m_Restart_Frequency.utf16())));
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // 尝试重新启动最多次数
        m_hr = m_pSettings->put_RestartCount(m_Max_Restart_Attempts);
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }

        m_pSettings->Release();
        return true;
    }
    else
    {
        this->Deal_Fail_Hr(m_hr);
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Register(const PlanRegister& planregister)
{
    // 设置注册信息
    m_hr = m_pTask->get_RegistrationInfo(&m_pRegInfo);
    // 设置创建者
    if (SUCCEEDED(m_hr))
    {
        // 创造计划的用户 和 用户描述
        m_hr = m_pRegInfo->put_Author(SysAllocString(reinterpret_cast<const wchar_t*>(planregister.m_Task_Creator.utf16())));
        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        m_hr = m_pRegInfo->put_Description(SysAllocString(reinterpret_cast<const wchar_t*>(planregister.m_Task_Desc.utf16())));

        if(FAILED(m_hr))
        {
            this->Deal_Fail_Hr(m_hr);
        }
        // pRegInfo->put_Date();


        m_pRegInfo->Release();
        return true;
    }
    else
    {
        this->Deal_Fail_Hr(m_hr);
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Actions(QList<TaskOperation> m_globalTaskOperations)
{
    // 设置任务的动作
    m_hr = m_pTask->get_Actions(&m_pActionCollection);
    if (SUCCEEDED(m_hr) && m_globalTaskOperations.size() != 0 && !m_globalTaskOperations.first().executable.isEmpty())
    {
        for(int i =0;i<m_globalTaskOperations.size();i++)
        {
            // 通过循环的形式创建多个执行任务
            m_hr = m_pActionCollection->Create(TASK_ACTION_EXEC, &m_pAction);
            if (SUCCEEDED(m_hr))
            {
                    // 设置要执行的程序
                    IExecAction* pExecAction = NULL;
                    m_hr = m_pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
                    if(FAILED(m_hr))
                    {
                        this->Deal_Fail_Hr(m_hr);
                    }
                    //pExecAction->put_Path(SysAllocString(L"C:\\Path\\To\\YourExecutable.exe")); //设置程序执行目录
                    m_hr = pExecAction->put_Path(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).executable.utf16())));
                    if(FAILED(m_hr))
                    {
                        this->Deal_Fail_Hr(m_hr);
                    }
                    m_hr = pExecAction->put_Arguments(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).parameters.utf16()))); //设置参数
                    if(FAILED(m_hr))
                    {
                        this->Deal_Fail_Hr(m_hr);
                    }
                    m_hr = pExecAction->put_WorkingDirectory(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).startAtDirector.utf16())));  //设置起始于
                    if(FAILED(m_hr))
                    {
                        this->Deal_Fail_Hr(m_hr);
                    }
                    pExecAction->Release();

            }
            else
            {
                this->Deal_Fail_Hr(m_hr);

            }
        }

        m_pActionCollection->Release();
        return true;
    }
    else
    {
        this->Deal_Fail_Hr(m_hr);
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Triggers(const PlanTriggers& plantrigers)
{
    // 设置触发器
    m_hr = m_pTask->get_Triggers(&m_pTriggerCollection);
    if (FAILED(m_hr)) return false;

    // 创建触发器
    m_hr = m_pTriggerCollection->Create(plantrigers.m_TaskTriggerType, &m_pTrigger);
    if (FAILED(m_hr)) return false;

    // 根据触发器类型设置参数
    switch (plantrigers.m_TaskTriggerType) {
    case TASK_TRIGGER_TIME:
    case TASK_TRIGGER_DAILY:
    case TASK_TRIGGER_WEEKLY:
        // 设置时间触发器的开始时间
        if (SUCCEEDED(m_pTrigger->QueryInterface(IID_ITimeTrigger, (void**)&m_pTimeTrigger))) {
            m_pTimeTrigger->put_StartBoundary(SysAllocString(L"2023-10-29T10:00:00"));
            m_pTimeTrigger->Release();
        } else {
            return false;
        }
        break;

    case TASK_TRIGGER_IDLE:
        // 设置空闲触发器的参数
        // (根据需要设置空闲参数)
        break;

    case TASK_TRIGGER_BOOT:
        // 设置启动触发器的参数
        // (根据需要设置启动参数)
        break;

    case TASK_TRIGGER_LOGON:
        // 设置登录触发器的参数
        // (根据需要设置登录参数)
        break;

        // 添加其他触发器类型的处理
    default:
        return false; // 不支持的触发器类型
    }

    // 释放触发器集合
    m_pTrigger->Release();
    m_pTriggerCollection->Release();

    return true;
}

bool WinTimeTaskAPI::Create_Plan_Definition(const PlanDefinition& plandefine)
{
    // 注册任务
    ITaskFolder* pRootFolder = NULL;
    m_hr = m_pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (SUCCEEDED(m_hr))
    {
        // 获取任务的定义
        qDebug()<<"创建任务";
        IRegisteredTask* pRegisteredTask = NULL;
        m_hr = pRootFolder->RegisterTaskDefinition(
            _bstr_t(plandefine.m_Task_Name.toStdWString().c_str()),// _bstr_t(L"API Test"),  // 任务名称
            m_pTask,                 // 已注册的任务的定义
            TASK_CREATE_OR_UPDATE, // 定义任务的类型 创建或者更新
            _variant_t(L"NT AUTHORITY\\SYSTEM"), // 使用系统账户
            _variant_t(),         // 不需要密码
            TASK_LOGON_SERVICE_ACCOUNT, // 定义用于运行已注册任务的登录技术
            _variant_t(),         // 与已注册的任务关联的安全描述符
            &pRegisteredTask                   // 表示新任务的RegisteredTask对象
            );
        pRootFolder->Release();

    }
    else
    {
        this->Deal_Fail_Hr(m_hr);
    }


    return 1;
}

bool WinTimeTaskAPI::Release_WinTask()
{
    // 释放资源
    m_pTask->Release();
    m_pService->Release();
    CoUninitialize();
    return true;
}

void WinTimeTaskAPI::Deal_Fail_Hr(HRESULT m_hr)
{
    // 处理错误，发送错误信息
    _com_error err(m_hr);
    QString errorMessage = QString::fromWCharArray(err.ErrorMessage());
    emit SendErrorDetail(errorMessage); // 发出信号
}



