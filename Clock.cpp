#include <graphics.h>
#include <iostream>
#include <conio.h>
#include "class.h"
#include "globals.h"
#include <time.h>
#include <vector>

// 添加Windows API相关头文件
#include <windows.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.Foundation.h>

using namespace std;
using namespace winrt;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;

// 定义全局变量
bool issdf = false;
vector<Alarm> alarms;

// 函数声明
void showAddAlarmDialog();
void checkAndTriggerAlarms();
void sendToastNotification(const string &alarmName);
void drawClock(); // 新增：绘制时钟的函数

// 全局按钮对象
Button *toggleButton;
Button *addAlarmButton;

int main()
{
    struct tm t;
    time_t now;
    initgraph(1000, 800); // 扩展窗口宽度以容纳闹钟界面

    // 创建离屏绘图设备用于双重缓冲
    IMAGE buffer(1000, 800);

    // 初始化COM
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // 创建按钮
    toggleButton = new Button(10, 10, 80, 40, "切换", []()
                              { issdf = !issdf; });

    addAlarmButton = new Button(820, 20, 40, 40, "+", []()
                                { showAddAlarmDialog(); });

    cleardevice();

    while (!_kbhit())
    {
        // 设置绘图目标为离屏设备
        SetWorkingImage(&buffer);

        // 清除离屏绘图设备
        cleardevice();

        // 绘制所有内容到离屏设备
        drawClock();

        // 切换回屏幕设备并一次性显示所有内容
        SetWorkingImage();
        putimage(0, 0, &buffer);

        // 检查是否有闹钟需要触发
        checkAndTriggerAlarms();

        // 处理鼠标消息
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                // 检查按钮点击
                if (toggleButton->checkClick(msg.x, msg.y))
                {
                    cout << "切换按钮被点击" << endl;
                }

                if (addAlarmButton->checkClick(msg.x, msg.y))
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
                toggleButton->checkHover(msg.x, msg.y);
                addAlarmButton->checkHover(msg.x, msg.y);
            }
        }

        time(&now);
        localtime_s(&t, &now);

        Sleep(100); // 从1000改为100以提高检查频率，支持秒级精度
    }
    _getch();
    closegraph();
    CoUninitialize(); // 清理COM
    delete toggleButton;
    delete addAlarmButton;
    return 0;
}

// 绘制时钟和界面元素
void drawClock()
{
    struct tm t;
    time_t now;
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

    // 创建表盘和指针对象
    Dial dial(400, 400, 305);
    Pin second(400, 400, 300, RGB(255, 0, 0));
    Pin minute(400, 400, 225, RGB(0, 0, 255));
    Pin hour(400, 400, 150, RGB(0, 255, 0));

    // 绘制表盘和指针
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
    toggleButton->draw();
    addAlarmButton->draw();

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
            // 触发闹钟通知
            sendToastNotification(alarms[i].getName());
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

// 发送Windows Toast通知
void sendToastNotification(const string &alarmName)
{
    try
    {
        // 构造Toast通知XML
        string xml = "<toast>"
                     "<visual>"
                     "<binding template='ToastGeneric'>"
                     "<text>闹钟提醒</text>"
                     "<text>闹钟 \"" +
                     alarmName + "\" 时间到了!</text>"
                                 "</binding>"
                                 "</visual>"
                                 "</toast>";

        // 创建XML文档
        XmlDocument doc;
        doc.LoadXml(winrt::to_hstring(xml));

        // 创建Toast通知
        ToastNotification toast(doc);

        // 创建通知管理器并显示通知
        ToastNotifier notifier = ToastNotificationManager::CreateToastNotifier(L"ClockApp");
        notifier.Show(toast);
    }
    catch (...)
    {
        // 如果无法发送Toast通知，则在控制台输出信息
        cout << "闹钟 \"" << alarmName << "\" 时间到了!" << endl;
    }
}