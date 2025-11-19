#include <graphics.h>
#include <iostream>
#include <conio.h>
#include "class.h"
#include <time.h>
#include <vector>
using namespace std;

// 全局闹钟列表
vector<Alarm> alarms;

// 函数声明
void showAddAlarmDialog();

int main()
{
    struct tm t;
    time_t now;
    initgraph(1000, 800); // 扩展窗口宽度以容纳闹钟界面
    Dial dial(400, 400, 305);
    Pin second(400, 400, 300, RGB(255, 0, 0));
    Pin minute(400, 400, 225, RGB(0, 0, 255));
    Pin hour(400, 400, 150, RGB(0, 255, 0));
    
    // 创建切换按钮
    Button toggleButton(10, 10, 80, 40, "切换", []() {
        issdf = !issdf;
    });
    
    // 创建添加闹钟按钮 (+号)
    Button addAlarmButton(820, 20, 40, 40, "+", []() {
        showAddAlarmDialog();
    });
    
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
        for (size_t i = 0; i < alarms.size() && i < 10; i++) {
            string alarmInfo = alarms[i].getName() + " " + 
                              to_string(alarms[i].getHour()) + ":" + 
                              (alarms[i].getMinute() < 10 ? "0" : "") + 
                              to_string(alarms[i].getMinute()) + 
                              " (等级:" + to_string(alarms[i].getLevel()) + ")";
            
            TCHAR wAlarmInfo[256] = {};
#ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, alarmInfo.c_str(), -1, wAlarmInfo, 256);
#else
            _tcscpy_s(wAlarmInfo, alarmInfo.c_str());
#endif
            outtextxy(820, 100 + (int)i * 25, wAlarmInfo);
        }
        
        // 处理鼠标消息
        ExMessage msg;
        while (peekmessage(&msg, EX_MOUSE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                // 检查按钮点击
                if (toggleButton.checkClick(msg.x, msg.y))
                {
                    cout << "按钮被点击" << endl;
                    cleardevice();
                }
                
                if (addAlarmButton.checkClick(msg.x, msg.y))
                {
                    cout<<"创建闹重按钮"<<endl;
                    showAddAlarmDialog();
                }
            }
            // 检查鼠标悬停
            toggleButton.checkHover(msg.x, msg.y);
            addAlarmButton.checkHover(msg.x, msg.y);
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
            //sdf_pie_mt(400,400,300,0,90,RED);
             second.point_to_dgree(sec_angle, t.tm_sec);
             minute.point_to_dgree(min_angle + (0.1 * t.tm_sec), t.tm_min);
             hour.point_to_dgree(hour_angle + (0.5 * t.tm_min) + (t.tm_sec / 120.0), t.tm_hour);
        }

        Sleep(1000);
    }
    _getch();
    closegraph();
    return 0;
}

void showAddAlarmDialog() {
    // 保存当前屏幕内容
    IMAGE backup;
    getimage(&backup, 0, 0, 1000, 800);
    
    // 绘制半透明遮罩层
    IMAGE mask(1000, 800);
    getimage(&mask, 0, 0, 1000, 800);
    DWORD* pMaskBuffer = GetImageBuffer(&mask);
    for (int i = 0; i < 1000 * 800; ++i) {
        DWORD color = pMaskBuffer[i];
        BYTE r = GetRValue(color) / 2;
        BYTE g = GetGValue(color) / 2;
        BYTE b = GetBValue(color) / 2;
        pMaskBuffer[i] = RGB(r, g, b);
    }
    putimage(0, 0, &mask);
    
    // 对话框参数
    const int dialogWidth = 300;
    const int dialogHeight = 250;
    const int dialogX = (1000 - dialogWidth) / 2;
    const int dialogY = (800 - dialogHeight) / 2;
    
    // 绘制对话框背景
    setfillcolor(RGB(30, 30, 40));
    fillrectangle(dialogX, dialogY, dialogX + dialogWidth, dialogY + dialogHeight);
    
    // 绘制对话框边框
    setlinecolor(WHITE);
    rectangle(dialogX, dialogY, dialogX + dialogWidth, dialogY + dialogHeight);
    
    // 标题
    settextcolor(WHITE);
    TCHAR title[32] = {};
    _stprintf_s(title, _countof(title), _T("添加新闹钟"));
    outtextxy(dialogX + 20, dialogY + 10, title);
    
    // 创建界面元素
    TextInput nameInput(dialogX + 20, dialogY + 40, 260, 30, "闹钟名称");
    TextInput hourInput(dialogX + 20, dialogY + 90, 100, 30, "小时 (0-23)");
    TextInput minuteInput(dialogX + 140, dialogY + 90, 100, 30, "分钟 (0-59)");
    
    // 等级选择 (简化为按钮形式)
    int selectedLevel = 1;
    
    Button level1Btn(dialogX + 20, dialogY + 140, 60, 30, "等级 1", [&selectedLevel]() { selectedLevel = 1; });
    Button level2Btn(dialogX + 90, dialogY + 140, 60, 30, "等级 2", [&selectedLevel]() { selectedLevel = 2; });
    Button level3Btn(dialogX + 160, dialogY + 140, 60, 30, "等级 3", [&selectedLevel]() { selectedLevel = 3; });
    
    Button confirmBtn(dialogX + 40, dialogY + 190, 80, 30, "确认", [&]() {
        // 确认按钮的操作将在主循环中处理
    });
    
    Button cancelBtn(dialogX + 140, dialogY + 190, 80, 30, "取消", [&]() {
        // 取消按钮的操作将在主循环中处理
    });
    
    // 绘制界面元素
    nameInput.draw();
    hourInput.draw();
    minuteInput.draw();
    
    level1Btn.draw();
    level2Btn.draw();
    level3Btn.draw();
    confirmBtn.draw();
    cancelBtn.draw();
    
    // 等待用户操作
    ExMessage msg;
    bool running = true;
    while (running) {
        if (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                nameInput.handleClick(msg.x, msg.y);
                hourInput.handleClick(msg.x, msg.y);
                minuteInput.handleClick(msg.x, msg.y);
                
                level1Btn.checkClick(msg.x, msg.y);
                level2Btn.checkClick(msg.x, msg.y);
                level3Btn.checkClick(msg.x, msg.y);
                
                if (confirmBtn.checkClick(msg.x, msg.y)) {
                    // 保存闹钟逻辑
                    string name = nameInput.getText();
                    if (!name.empty()) {
                        // 解析时间输入
                        int hour = 0, minute = 0;
                        try {
                            // 简化处理，实际应进行更严格的验证
                            if (!hourInput.getText().empty()) hour = stoi(hourInput.getText());
                            if (!minuteInput.getText().empty()) minute = stoi(minuteInput.getText());
                            
                            if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
                                // 添加到闹钟列表
                                alarms.emplace_back(name, hour, minute, selectedLevel);
                            }
                        } catch (...) {
                            // 忽略转换错误
                        }
                    }
                    running = false;
                }
                
                if (cancelBtn.checkClick(msg.x, msg.y)) {
                    running = false;
                }
            }
            else if (msg.message == WM_CHAR) {
                nameInput.handleInput((char)msg.wParam);
                hourInput.handleInput((char)msg.wParam);
                minuteInput.handleInput((char)msg.wParam);
                
                // 更新显示
                nameInput.draw();
                hourInput.draw();
                minuteInput.draw();
            }
        }
        
        // 重绘界面元素（如果有需要）
        nameInput.draw();
        hourInput.draw();
        minuteInput.draw();
        
        // 延迟避免过度刷新
        Sleep(50);
    }
    
    // 恢复原始屏幕内容
    putimage(0, 0, &backup);
}