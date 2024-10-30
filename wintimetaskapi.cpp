#include "wintimetaskapi.h"


using namespace std;

WinTimeTaskAPI::WinTimeTaskAPI(QWidget *parent)
    : QWidget(parent)
{
    //添加参数
    AddTaskOperation("C:\\Path\\To\\YourExecutable.exe", "--option1 --option2", "2024-10-30T10:00:00");
    AddTaskOperation("C:\\Path\\To\\AnotherExecutable.exe", "--arg1", "2024-10-31T11:00:00");

    // 创建任务计划程序 API
    this->Create_Plan_Task_API();
}

WinTimeTaskAPI::~WinTimeTaskAPI()
{

}

bool WinTimeTaskAPI::Create_Plan_Task_API()
{
    // 1. 任务计划程序初始化
    if (!this->WinTimeTaskInit()) {
        qDebug() << "Failed to initialize task scheduler.";
        return false;
    }

    // 2. 创建计划任务
    if (!this->Create_Plan_Task()) {
        qDebug() << "Failed to create plan task.";
        return false;
    }

    // 3. 创建任务所需要的权限
    if (!this->Create_Plan_Principal()) {
        qDebug() << "Failed to create plan principal.";
        return false;
    }

    // 4. 创建任务的 settings
    if (!this->Create_Plan_Settings()) {
        qDebug() << "Failed to create plan settings.";
        return false;
    }

    // 5. 设置任务注册信息
    if (!this->Create_Plan_Register()) {
        qDebug() << "Failed to create plan register.";
        return false;
    }

    // 6. 设置任务的动作
    if (!this->Create_Plan_Actions()) {
        qDebug() << "Failed to create plan actions.";
        return false;
    }

    // 7. 设置触发器
    if (!this->Create_Plan_Triggers()) {
        qDebug() << "Failed to create plan triggers.";
        return false;
    }

    // 8. 注册任务最后一步
    if (!this->Create_Plan_Definition()) {
        qDebug() << "Failed to create plan definition.";
        return false;
    }

    // 9. 释放任务计划程序资源
    this->Release_WinTask();

    qDebug() << "Task created successfully.";
    return true;

}

bool WinTimeTaskAPI::Create_Plan_Task_API(QString TaskName, QString Task_Path, QString Task_Argu)
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
        qDebug()<<"Failed to create TaskService instance: " << _com_error(m_hr).ErrorMessage();
        return false;
    }

    // 连接到任务服务
    m_hr = m_pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(m_hr))
    {
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
        qDebug()<<"Failed to create new task: " << _com_error(m_hr).ErrorMessage();
        m_pService->Release();
        return false;
    }
    return true;
}

bool WinTimeTaskAPI::Create_Plan_Principal()
{

    // 定义任务所需要的权限      获取任务的 Principal
    m_hr = m_pTask->get_Principal(&m_pPrincipal);
    if (SUCCEEDED(m_hr)) {
        // 设置 Principal 为系统账户并要求最高权限 m_Task_Account.toStdWString().c_str()
        // m_pPrincipal->put_UserId(_bstr_t(L"NT AUTHORITY\\SYSTEM")); //系统用户
        m_pPrincipal->put_UserId(SysAllocString(reinterpret_cast<const wchar_t*>(m_Task_Account.utf16()))); //系统用户
        m_pPrincipal->put_LogonType(m_LOGON_TYPE);    //
        m_pPrincipal->put_RunLevel(m_RunLevel_Tyle); // 设置为最高权限

        // 释放 Principal
        m_pPrincipal->Release();
        return true;
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Settings()
{
    //设置 任务的 Settings
    m_hr = m_pTask->get_Settings(&m_pSettings);
    if (SUCCEEDED(m_hr))
    {
        m_pSettings->put_StartWhenAvailable(VARIANT_TRUE); // 可用时启动
        m_pSettings->put_AllowDemandStart(VARIANT_TRUE);   // 允许手动启动
        m_pSettings->put_StopIfGoingOnBatteries(m_Battery_State); // 如果在电池上停止
        m_pSettings->put_DisallowStartIfOnBatteries(m_DisallowStartIfBattery); // 不限制电池下启动
        // 设置最大运行时间，例如 1 小时
        m_pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0S")); // 允许手动启动 。 值为 PT0S 将使任务可以无限期运行。
        // 设置过期后删除任务的时间（例如 30 天后）
        m_pSettings->Release();
        return true;
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Register()
{
    // 设置注册信息
    m_hr = m_pTask->get_RegistrationInfo(&m_pRegInfo);
    // 设置创建者
    if (SUCCEEDED(m_hr))
    {
        m_pRegInfo->put_Author(SysAllocString(reinterpret_cast<const wchar_t*>(m_Task_Creator.utf16())));
        m_pRegInfo->put_Description(SysAllocString(reinterpret_cast<const wchar_t*>(m_Task_Desc.utf16())));
        // pRegInfo->put_Date();


        m_pRegInfo->Release();
        return true;
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Actions()
{
    // 设置任务的动作
    m_hr = m_pTask->get_Actions(&m_pActionCollection);
    if (SUCCEEDED(m_hr))
    {
        for(int i =0;i<m_globalTaskOperations.size();i++)
        {
            // 通过循环的形式创建多个执行任务
            m_hr = m_pActionCollection->Create(TASK_ACTION_EXEC, &m_pAction);
            if (SUCCEEDED(m_hr))
            {


                    // 设置要执行的程序
                    IExecAction* pExecAction = NULL;
                    m_pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
                    //pExecAction->put_Path(SysAllocString(L"C:\\Path\\To\\YourExecutable.exe")); //设置程序执行目录
                    pExecAction->put_Path(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).executable.utf16())));
                    pExecAction->put_Arguments(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).parameters.utf16()))); //设置参数
                    pExecAction->put_WorkingDirectory(SysAllocString(reinterpret_cast<const wchar_t*>(m_globalTaskOperations.at(i).startAtDirector.utf16())));  //设置起始于

                    pExecAction->Release();

            }
        }

        m_pActionCollection->Release();
        return true;
    }
    return false;
}

bool WinTimeTaskAPI::Create_Plan_Triggers()
{
    // 设置触发器
    m_hr = m_pTask->get_Triggers(&m_pTriggerCollection);
    if (SUCCEEDED(m_hr))
    {
        //设置触发器 执行 任务 类型 按预定计划，还是自启运行，还是空闲状态 也可以通过循环来创建 多个触发器

        m_hr = m_pTriggerCollection->Create(TASK_TRIGGER_TIME, &m_pTrigger);
        if (SUCCEEDED(m_hr))
        {
            //设置相关参数细节
            m_hr = m_pTrigger->QueryInterface(IID_ITimeTrigger, (void**)&m_pTimeTrigger);
            if (SUCCEEDED(m_hr))
            {
                // 设置触发时间
                m_pTimeTrigger->put_StartBoundary(SysAllocString(L"2023-10-29T10:00:00"));
                // pTimeTrigger->put_StartBoundary()
                m_pTimeTrigger->Release();
            }
            else
            {
                return false;
            }
            m_pTrigger->Release();
        }
        else
        {
            return false;
        }

        m_pTriggerCollection->Release();
    }
    else
    {
        return false;
    }
    return true;
}

bool WinTimeTaskAPI::Create_Plan_Definition()
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
            _bstr_t(m_Task_Name.toStdWString().c_str()),// _bstr_t(L"API Test"),  // 任务名称
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



