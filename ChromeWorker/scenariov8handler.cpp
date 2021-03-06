#include "scenariov8handler.h"
#include "log.h"


ScenarioV8Handler::ScenarioV8Handler()
{
    Changed = false;
    ChangedExecute = false;
    IsInitialized = false;
    NeedRestart = None;
    IsEditStart = false;
    IsEditEnd = false;
}


std::pair<std::string, bool> ScenarioV8Handler::GetResult()
{
    std::lock_guard<std::mutex> lock(mut);

    std::pair<std::string, bool> r;
    r.first = LastResult;
    r.second = Changed;

    Changed = false;

    LastResult.clear();

    return r;
}

std::pair<std::string, bool> ScenarioV8Handler::GetIsEditStart()
{
    std::lock_guard<std::mutex> lock(mut_editstart);

    std::pair<std::string, bool> r;
    r.first = EditStartScript;
    r.second = IsEditStart;

    IsEditStart = false;

    EditStartScript.clear();

    return r;
}

bool ScenarioV8Handler::GetIsEditEnd()
{
    std::lock_guard<std::mutex> lock(mut_editend);
    bool res = IsEditEnd;
    IsEditEnd = false;
    return res;
}



std::pair<std::string, bool> ScenarioV8Handler::GetExecuteCode()
{
    std::lock_guard<std::mutex> lock(mut_code);

    std::pair<std::string, bool> r;
    r.first = LastResultExecute;
    r.second = ChangedExecute;

    ChangedExecute = false;

    LastResultExecute.clear();

    return r;
}

bool ScenarioV8Handler::GetIsInitialized()
{
    std::lock_guard<std::mutex> lock(mut_initialized);

    return IsInitialized;
}

ScenarioV8Handler::RestartType ScenarioV8Handler::GetNeedRestart()
{
    std::lock_guard<std::mutex> lock(mut_restart);
    RestartType res = NeedRestart;
    NeedRestart = None;
    return res;
}


bool ScenarioV8Handler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if(name == std::string("BrowserAutomationStudio_SendCode"))
    {
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            std::lock_guard<std::mutex> lock(mut);
            LastResult = arguments[0]->GetStringValue().ToString();
            Changed = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_Initialized"))
    {
        std::lock_guard<std::mutex> lock(mut_initialized);
        IsInitialized = true;
    }else if(name == std::string("BrowserAutomationStudio_Execute"))
    {
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            std::lock_guard<std::mutex> lock(mut_code);
            LastResultExecute = arguments[0]->GetStringValue().ToString();
            ChangedExecute = true;
        }
    }
    else if(name == std::string("BrowserAutomationStudio_Restart"))
    {
        if (arguments.size() == 1)
        {
            std::lock_guard<std::mutex> lock(mut_restart);
            worker_log(std::string("BrowserAutomationStudio_Restart<<") + std::to_string(arguments[0]->GetBoolValue()));
            if(arguments[0]->GetBoolValue())
            {
                NeedRestart = Stop;
            }else
            {
                NeedRestart = Restart;
            }
        }
    }
    else if(name == std::string("BrowserAutomationStudio_EditStart"))
    {
        if (arguments.size() == 1)
        {
            std::lock_guard<std::mutex> lock(mut_editstart);
            EditStartScript = arguments[0]->GetStringValue().ToString();
            IsEditStart = true;
        }
    }else if(name == std::string("BrowserAutomationStudio_EditEnd"))
    {
        if (arguments.size() == 0)
        {
            std::lock_guard<std::mutex> lock(mut_editend);
            IsEditEnd = true;
        }
    }
    return true;
}
