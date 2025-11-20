#include <graphics.h>
#include <iostream>
#include <conio.h>
#include "class.h"
#include "globals.h"
#include <time.h>
#include <vector>

using namespace std;
// 定义全局变量
bool issdf = false;
vector<Alarm> alarms;

// 函数声明
void showAddAlarmDialog();
void checkAndTriggerAlarms();
void sendToastNotification(const string &alarmName);

int main()
{
    struct tm t;
    time_t now;
    initgraph(1000, 800); // 扩展窗口宽度以容纳闹钟界面
    Dial dial(400, 400, 305);
    Pin second(400, 400, 300, RGB(255, 0, 0));
    Pin minute(400, 400, 225, RGB(0, 0, 255));
    Pin hour(400, 400, 150, RGB(0, 255, 0));

    // 初始化COM
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // 创建切换按钮
    Button toggleButton(10, 10, 80, 40, "切换", []()
                        { issdf = !issdf; });

    // 创建添加闹钟按钮 (+号)
    Button addAlarmButton(820, 20, 40, 40, "+", []()
                          { showAddAlarmDialog(); });

    cleardevice();
    // 初始绘制按钮
    toggleButton.draw();
    addAlarmButton.draw();

    while (!_kbhit())
    {
        // 绘制按钮
        toggleButton.draw();
        addAlarmButton.draw();

        // 显示issdf状态
        TCHAR stateStr[32] = {};
        _stprintf_s(stateStr, _countof(stateStr), _T("issdf: %s"), issdf ? _T("true ") : _T("false"));
        outtextxy(100, 20, stateStr);

        // 显示闹钟标题
        TCHAR alarmTitle[32] = {};
        _stprintf_s(alarmTitle, _countof(alarmTitle), _T("闹钟列表"));
        outtextxy(820, 70, alarmTitle);

        // 显示闹钟列表
        for (size_t i = 0; i < alarms.size() && i < 10; i++)
        {
            string alarmInfo = alarms[i].getName() + " " +
                               to_string(alarms[i].getHour()) + ":" +
                               (alarms[i].getMinute() < 10 ? "0" : "") +
                               to_string(alarms[i].getMinute()) + ":" +
                               (alarms[i].getSecond() < 10 ? "0" : "") +
                               to_string(alarms[i].getSecond()) +
                               " (等级:" + to_string(alarms[i].getLevel()) + ")";

            TCHAR wAlarmInfo[256] = {};
#ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, alarmInfo.c_str(), -1, wAlarmInfo, 256);
#else
            _tcscpy_s(wAlarmInfo, alarmInfo.c_str());
#endif
            outtextxy(820, 100 + (int)i * 25, wAlarmInfo);
        }

        // 检查是否有闹钟需要触发
        checkAndTriggerAlarms();

        // 处理鼠标消息
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                // 检查按钮点击
                if (toggleButton.checkClick(msg.x, msg.y))
                {
                    cout << "切换按钮被点击" << endl;
                }

                if (addAlarmButton.checkClick(msg.x, msg.y))
                {
                    cout << "创建闹钟按钮" << endl;
                }

                // 检查闹钟列表项点击
                for (size_t i = 0; i < alarms.size() && i < 10; i++)
                {
                    int itemX = 820;
                    int itemY = 100 + (int)i * 25;
                    int itemWidth = 160;
                    int itemHeight = 20;

                    if (msg.x >= itemX && msg.x <= itemX + itemWidth &&
                        msg.y >= itemY && msg.y <= itemY + itemHeight)
                    {
                        showDeleteAlarmDialog(i);
                        break;
                    }
                }
            }
            else if (msg.message == WM_MOUSEMOVE)
            {
                // 检查鼠标悬停
                toggleButton.checkHover(msg.x, msg.y);
                addAlarmButton.checkHover(msg.x, msg.y);
            }
        }

        time(&now);
        localtime_s(&t, &now);
        double sec_angle = t.tm_sec * 6;
        double min_angle = t.tm_min * 6;
        double hour_angle = 0;
        if (t.tm_hour <= 12)
        {
            hour_angle = t.tm_hour * 30;
        }
        else
        {
            hour_angle = (t.tm_hour - 12) * 30;
        }
        if (second.angle != sec_angle)
        {
            if (sec_angle == 6)
            {
                dial.refresh();
            }
            // sdf_pie_mt(400,400,300,0,90,RED);
            second.point_to_dgree(sec_angle, t.tm_sec);
            minute.point_to_dgree(min_angle + (0.1 * t.tm_sec), t.tm_min);
            hour.point_to_dgree(hour_angle + (0.5 * t.tm_min) + (t.tm_sec / 120.0), t.tm_hour);
        }

        // 绘制按钮
        toggleButton.draw();
        addAlarmButton.draw();
        Sleep(100);
    }
}
// 检查并触发闹钟
void checkAndTriggerAlarms()
{
    time_t now;
    time(&now);
    struct tm t;
    localtime_s(&t, &now);

    int currentHour = t.tm_hour;
    int currentMinute = t.tm_min;
    int currentSecond = t.tm_sec;

    // 遍历所有闹钟
    for (size_t i = 0; i < alarms.size(); i++)
    {
        // 检查时间是否匹配且闹钟尚未触发
        if (alarms[i].getHour() == currentHour &&
            alarms[i].getMinute() == currentMinute &&
            alarms[i].getSecond() == currentSecond &&
            !alarms[i].isTriggered())
        {
            // 标记闹钟已触发
            alarms[i].setTriggered(true);
            switch (alarms[i].getLevel())
            {
            case 1:
                {
                // 将整个窗口设置为橙色并显示闹钟名称
                setfillcolor(RGB(255, 165, 0)); // 橙色
                fillrectangle(0, 0, 1000, 800);
                
                // 设置文字颜色为黑色以便在橙色背景上清晰显示
                settextcolor(RGB(0, 0, 0));
                setbkmode(TRANSPARENT);
                
                // 显示提示信息
                outtextxy(400, 300, _T("闹钟触发!"));
                
                // 显示闹钟名称
                TCHAR alarmName[256] = {};
#ifdef UNICODE
                MultiByteToWideChar(CP_ACP, 0, alarms[i].getName().c_str(), -1, alarmName, 256);
#else
                _tcscpy_s(alarmName, alarms[i].getName().c_str());
#endif
                
                // 计算文字位置使其居中
                int textWidth = textwidth(alarmName);
                int textHeight = textheight(alarmName);
                int textX = (1000 - textWidth) / 2;
                int textY = (800 - textHeight) / 2;
                
                outtextxy(textX, textY, alarmName);
                
                // 显示点击任意位置返回的提示
                outtextxy(350, 500, _T("点击任意位置返回"));
                
                // 强制刷新屏幕
                FlushBatchDraw();
                
                // 等待用户点击任意位置
                ExMessage msg;
                while (true) {
                    getmessage(&msg, EX_MOUSE);
                    if (msg.message == WM_LBUTTONDOWN) {
                        cleardevice();
                        break;
                    }
                }
                }
                break;
            case 2:
                {
                // 为二级闹钟运行嵌入的exe程序
                HRSRC hResource = FindResource(NULL, _T("EmbeddedExe"), _T("BINARY"));
                if (hResource != NULL) {
                    HGLOBAL hMemory = LoadResource(NULL, hResource);
                    if (hMemory != NULL) {
                        DWORD dwSize = SizeofResource(NULL, hResource);
                        LPVOID lpAddress = LockResource(hMemory);
                        
                        // 创建临时文件
                        TCHAR tempPath[MAX_PATH];
                        GetTempPath(MAX_PATH, tempPath);
                        TCHAR tempFileName[MAX_PATH];
                        GetTempFileName(tempPath, _T("alarm"), 0, tempFileName);
                        
                        // 写入资源到临时文件
                        HANDLE hFile = CreateFile(tempFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                        if (hFile != INVALID_HANDLE_VALUE) {
                            DWORD bytesWritten;
                            WriteFile(hFile, lpAddress, dwSize, &bytesWritten, NULL);
                            CloseHandle(hFile);
                            
                            // 运行临时exe文件
                            ShellExecute(NULL, _T("open"), tempFileName, NULL, NULL, SW_SHOWNORMAL);
                        }
                    }
                } else {
                    // 资源未找到时的备选方案 - 显示简单提示
                    setfillcolor(RGB(0, 255, 0)); // 绿色
                    fillrectangle(0, 0, 1000, 800);
                    
                    settextcolor(RGB(0, 0, 0));
                    setbkmode(TRANSPARENT);
                    
                    outtextxy(400, 300, _T("二级闹钟触发!"));
                    
                    TCHAR alarmName[256] = {};
#ifdef UNICODE
                    MultiByteToWideChar(CP_ACP, 0, alarms[i].getName().c_str(), -1, alarmName, 256);
#else
                    _tcscpy_s(alarmName, alarms[i].getName().c_str());
#endif
                    
                    int textWidth = textwidth(alarmName);
                    int textHeight = textheight(alarmName);
                    int textX = (1000 - textWidth) / 2;
                    int textY = (800 - textHeight) / 2;
                    
                    outtextxy(textX, textY, alarmName);
                    
                    outtextxy(350, 500, _T("点击任意位置返回"));
                    
                    FlushBatchDraw();
                    
                    ExMessage msg;
                    while (true) {
                        getmessage(&msg, EX_MOUSE);
                        if (msg.message == WM_LBUTTONDOWN) {
                            cleardevice();
                            break;
                        }
                    }
                }
                }
                break;
            case 3:

                break;

            default:
                break;
            }
            cout << "alarmed" << endl;
        }
        // 如果时间不匹配，则重置触发状态
        else if (alarms[i].getHour() != currentHour ||
                 alarms[i].getMinute() != currentMinute ||
                 alarms[i].getSecond() != currentSecond)
        {
            alarms[i].setTriggered(false);
        }
    }
}
