#include "javascriptextensions.h"
#include <fstream>
#include "log.h"
#include "picojson.h"
#include "replaceall.h"

JavaScriptExtensions::JavaScriptExtensions()
{

}


std::string JavaScriptExtensions::GetUserAgentExtension(const std::string& UserAgent)
{
    std::string rescode;
    if(!UserAgent.empty())
    {
        rescode += std::string("Object.defineProperty(window.navigator, 'userAgent', {"
        "    get: function() {"
         "        return ") + picojson::value(UserAgent).serialize() + std::string(";"
         "    }"
         "});");

        rescode += std::string("Object.defineProperty(window.navigator, 'appVersion', {"
        "    get: function() {"
         "        return ") + picojson::value(ReplaceAll( UserAgent, "Mozilla/", "")).serialize() + std::string(";"
         "    }"
         "});");

        rescode += std::string("Object.defineProperty(window.navigator, 'vendor', {"
        "    get: function() {"
         "        return ") + picojson::value("").serialize() + std::string(";"
         "    }"
         "});");

        rescode += std::string("Object.defineProperty(window.navigator, 'platform', {"
        "    get: function() {"
         "        return ") + picojson::value("").serialize() + std::string(";"
         "    }"
         "});");
    }
    return rescode;
}

std::string JavaScriptExtensions::GetLanguage(const std::string& Language)
{
    std::string rescode;
    if(!Language.empty())
    {
        rescode += std::string("Object.defineProperty(window.navigator, 'language', {"
        "    get: function() {"
         "        return ") + picojson::value(Language).serialize() + std::string(";"
         "    }"
         "});");

        rescode += std::string("Object.defineProperty(window.navigator, 'languages', {"
        "    get: function() {"
         "        return [") + picojson::value(Language).serialize() + std::string("];"
         "    }"
         "});");
    }
    return rescode;
}


std::string JavaScriptExtensions::GetBasicExtension()
{
    std::string additional;
    try
    {
        std::ifstream ifs("html/main/css_path.js");
        additional = std::string((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    }catch(...){}
    worker_log(additional);

    return
    additional +
    std::string(
    "var BrowserAutomationStudio_CssSelectorGenerator = new CssSelectorGenerator();"
    "function BrowserAutomationStudio_InspectElement(x,y)"
    "{"
        "var el = document.elementFromPoint(x,y);"
        "if(el){"
            "var rect = el.getBoundingClientRect();"
            "var css = '';"
            "var match = '';"
            "try"
            "{"
                "css = BrowserAutomationStudio_CssSelectorGenerator.getSelector(el);"
                "match = el.outerHTML.substr(0,40).replace(/(?:\\r\\n|\\r|\\n)/g, ' ');"
            "}catch(e){}"
            "browser_automation_studio_inspect_result(parseInt(rect.left),parseInt(rect.top),parseInt(rect.width),parseInt(rect.height),css,css,match,x + window.scrollX,y + window.scrollY,true);"
        "}else{"
            "browser_automation_studio_inspect_result(0,0,0,0,'','','',x,y,false);"
        "}"
    "}"
    "function BrowserAutomationStudio_ScrollTo(x,y,el)"
    "{"
        "x = Math.floor(x);y = Math.floor(y);"
        "if(x > window.scrollX && x < window.scrollX + window.innerWidth && y > window.scrollY && y < window.scrollY + window.innerHeight && typeof(el) == 'undefined')"
        "{"
            "browser_automation_studio_result(x.toString() + ',' + y.toString());"
            "return;"
        "}"
        "window.scrollTo(x - window.innerWidth/2,y - window.innerHeight/2);"

        "x-=window.scrollX;"
        "if(x<0)x=0;"
        "if(x>window.innerWidth-2)x=window.innerWidth-2;"
        "x+=window.scrollX;"

        "y-=window.scrollY;"
        "if(y<0)y=0;"
        "if(y>window.innerHeight-2)y=window.innerHeight-2;"
        "y+=window.scrollY;"

        "var res = x.toString() + ',' + y.toString();"
        "if(typeof(el)!='undefined')"
        "{"
             "var rect = el.getBoundingClientRect();"
             "var left = rect.left;"
             "if(left<0)left=0;"
             "if(left>window.innerWidth-2)left=window.innerWidth-2;"
             "left+=window.scrollX;"

             "var top = rect.top;"
             "if(top<0)top=0;"
             "if(top>window.innerHeight-2)top=window.innerHeight-2;"
             "top+=window.scrollY;"

             "var right = rect.right;"
             "if(right<=rect.left)right=rect.left+1;"
             "if(right>window.innerWidth-1)right=window.innerWidth-1;"
             "right+=window.scrollX;"

             "var bottom = rect.bottom;"
             "if(bottom<=rect.top)bottom=rect.top+1;"
             "if(bottom>window.innerHeight-1)bottom=window.innerHeight-1;"
             "bottom+=window.scrollY;"

             "res += ',' + left.toString() + ',' + top.toString()+ ',' + right.toString() + ',' + bottom.toString();"
        "}"
        "browser_automation_studio_result(res);"
    "}"
    "function BrowserAutomationStudio_ScrollToRelative(x,y,el)"
    "{"
        "BrowserAutomationStudio_ScrollTo(x + window.scrollX, y + window.scrollY,el);"
    "}"
    "function BrowserAutomationStudio_ScrollToNoResult(x,y)"
    "{"
        "if(x > window.scrollX && x < window.scrollX + window.innerWidth && y > window.scrollY && y < window.scrollY + window.innerHeight)"
        "{"
            "return;"
        "}"
        "window.scrollTo(x - window.innerWidth/2,y - window.innerHeight/2);"
    "}"
    "function BrowserAutomationStudio_ScrollUp()"
    "{"
        "window.scrollTo(window.scrollX,window.scrollY - 100);"
    "}"
    "function BrowserAutomationStudio_ScrollDown()"
    "{"
        "window.scrollTo(window.scrollX,window.scrollY + 100);"
    "}"
    "function BrowserAutomationStudio_ScrollRight()"
    "{"
        "window.scrollTo(window.scrollX + 100,window.scrollY);"
    "}"
    "function BrowserAutomationStudio_ScrollLeft()"
    "{"
        "window.scrollTo(window.scrollX - 100,window.scrollY);"
    "}"
    "function BrowserAutomationStudio_MatchAllIteration(res,el,mask)"
    "{"
        "if(el == document)"
            "el = document.body;"
        "if(el.outerHTML.indexOf(mask)<0)"
            "return [];"
        "var no_child_match = true;"
        "try{"
            "for(var i = 0;i<el.childNodes.length;i++)"
            "{"
                "var e = el.childNodes[i];"
                "var content = '';"
                "try{"
                    "content = e.outerHTML;"
                "}catch(e)"
                "{"
                    "continue;"
                "}"
                "if(!content || content.length === 0)"
                    "continue;"
                "if(content.indexOf(mask)>=0)"
                "{"
                    "no_child_match = false;"
                    "res.concat(BrowserAutomationStudio_MatchAllIteration(res,e,mask));"
                "}"
            "}"
            "if(no_child_match)"
            "{"
                "res.push(el);"
            "}"
        "}"
        "catch(e){"
        "}"
        "return res;"
    "}"
    "function BrowserAutomationStudio_MatchIteration(el,mask)"
    "{"
        "if(el == document)"
            "el = document.body;"
        "if(el.outerHTML.indexOf(mask)<0)"
            "return null;"
        "for(var i = 0;i<el.childNodes.length;i++)"
        "{"
            "var e = el.childNodes[i];"
            "var content = '';"
            "try{"
                "content = e.outerHTML;"
            "}catch(e)"
            "{"
                "continue;"
            "}"
            "if(!content || content.length === 0)"
                "continue;"
            "if(content.indexOf(mask)>=0)"
            "{"
                "return BrowserAutomationStudio_MatchIteration(e,mask);"
            "}"
        "}"
        "return el;"
    "}"
    "function BrowserAutomationStudio_FindElement(search)"
    "{"
        "var json = JSON.parse(search);"
        "var res = document;"
        "for(var i = 0;i+1<json.length;i+=2)"
        "{"
            "var select_type = json[i];"
            "var select_value = json[i+1];"
            "if(select_type == 'css')"
            "{"
                "res = res.querySelector(select_value);"
            "}"
            "if(select_type == 'all')"
            "{"
                "res = res.querySelectorAll(select_value);"
            "}"
            "if(select_type == 'at')"
            "{"
                "res = res[parseInt(select_value)];"
            "}"
            "if(select_type == 'match')"
            "{"
                "res = BrowserAutomationStudio_MatchIteration(res,select_value);"
            "}"
            "if(select_type == 'match_all')"
            "{"
                "res = BrowserAutomationStudio_MatchAllIteration([],res,select_value);"
            "}"
            "if(select_type == 'position')"
            "{"
                "var x = 0;"
                "var y = 0;"
                "try{"
                    "x = parseInt(select_value.split(',')[0].trim());"

                    "y = parseInt(select_value.split(',')[1].trim());"
                "}catch(e){};"
                "BrowserAutomationStudio_ScrollToNoResult(x,y);"
                "res = document.elementFromPoint(x - window.scrollX,y - window.scrollY);"
            "}"
            "if(!res)"
                "return null;"
        "}"
        "if(res == document)"
            "return document.body;"
        "return res;"
    "}")
    ;
}

std::string JavaScriptExtensions::GetJqueryExtension()
{
    return "";
}
