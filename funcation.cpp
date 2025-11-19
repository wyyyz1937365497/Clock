#include "class.h"
#include "globals.h"
#include <graphics.h>
#include <conio.h>
#include <string>
#include <cmath>
#include <tchar.h>
#include <iostream>
using namespace std;
void update_to_screen(IMAGE *hidden)
{
}
double anglr2rad(double angle)
{
	return PI * (angle / 180.0);
}
void Pin::write_text(double angledouble, int num)
{
	TCHAR str[32] = {};
	_itot_s(num, str, _countof(str), 10);
	double rad = anglr2rad(angledouble - 87);
	int text_x = (int)((r - 20) * cos(rad)) + x;
	int text_y = (int)((r - 20) * sin(rad)) + y;
	
	// 获取文本尺寸
	int textWidth = textwidth(str);
	int textHeight = textheight(str);
	
	// 绘制黑色背景
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	fillrectangle(text_x - 2, text_y - 2, text_x + textWidth + 2, text_y + textHeight + 2);
	
	// 设置文本颜色为白色
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	
	// 绘制文本
	outtextxy(text_x, text_y, str);
}
void Pin::changr_color()
{
	setfillcolor(color);
}
Pin::Pin(int xin, int yin, int rin, COLORREF colorin)
{
	x = xin;
	y = yin;
	r = rin;
	color = colorin;
}
void Pin::point_to_dgree(double angledouble, int time)
{
	changr_color();
	if (issdf)
	{
		sdf_pie_mt(x, y, r, 270, angledouble - 90, color, 1);
	}
	else
	{
		solidpie(x - r, y - r, x + r, y + r, PI / 2 - anglr2rad(angledouble), PI / 2);
	}
	write_text(angledouble, time);
}
Dial::Dial(int xin, int yin, int rin)
{
	circle(xin, yin, rin);
	x = xin;
	y = yin;
	r = rin;
}
void Dial::refresh()
{
	clearcircle(x, y, r);
}

// Button类的实现
Button::Button(int xin, int yin, int widthin, int heightin, const std::string& textin, std::function<void()> onClickIn)
{
	x = xin;
	y = yin;
	width = widthin;
	height = heightin;
	text = textin;
	onClick = onClickIn;
	bgColor = RGB(50, 50, 50);
	textColor = RGB(255, 255, 255); // 设置文本为白色
	isHovered = false;
}

void Button::draw()
{
	// 根据是否悬停设置不同的背景色
	COLORREF currentBgColor = isHovered ? RGB(70, 70, 70) : bgColor;
	
	// 绘制按钮背景
	setfillcolor(currentBgColor);
	fillrectangle(x, y, x + width, y + height);
	
	// 绘制按钮边框
	setlinecolor(RGB(100, 100, 100));
	rectangle(x, y, x + width, y + height);
	
	// 计算文本位置使其居中
	TCHAR wText[128] = {}; // 假设文本不超过128个字符
	#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, wText, 128);
	#else
	_tcscpy_s(wText, text.c_str());
	#endif
	
	int textWidth = textwidth(wText);
	int textHeight = textheight(wText);
	int textX = x + (width - textWidth) / 2;
	int textY = y + (height - textHeight) / 2;
	
	// 设置文本背景为透明
	setbkmode(TRANSPARENT);
	
	// 绘制按钮文本
	settextcolor(textColor);
	outtextxy(textX, textY, wText);
}

bool Button::checkClick(int mouseX, int mouseY)
{
	if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
	{
		if (onClick)
		{
			onClick();
		}
		return true;
	}
	return false;
}

void Button::checkHover(int mouseX, int mouseY)
{
	bool wasHovered = isHovered;
	isHovered = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
	
	// 如果悬停状态改变，需要重绘按钮
	if (wasHovered != isHovered)
	{
		draw();
	}
}

void Button::setColors(COLORREF bgColorIn, COLORREF textColorIn)
{
	bgColor = bgColorIn;
	textColor = textColorIn;
}

// Alarm类的实现
Alarm::Alarm(const string& name, int hour, int minute, int second, int level) 
	: name(name), hour(hour), minute(minute), second(second), level(level), triggered(false) {}

string Alarm::getName() const {
	return name;
}

int Alarm::getHour() const {
	return hour;
}

int Alarm::getMinute() const {
	return minute;
}

int Alarm::getSecond() const {
	return second;
}

int Alarm::getLevel() const {
	return level;
}

void Alarm::setName(const string& newName) {
	name = newName;
}

void Alarm::setHour(int newHour) {
	hour = newHour;
}

void Alarm::setMinute(int newMinute) {
	minute = newMinute;
}

void Alarm::setSecond(int newSecond) {
	second = newSecond;
}

void Alarm::setLevel(int newLevel) {
	level = newLevel;
}

bool Alarm::isTriggered() const {
	return triggered;
}

void Alarm::setTriggered(bool state) {
	triggered = state;
}

// TextInput类的实现
TextInput::TextInput(int x, int y, int width, int height, const string& placeholder)
	: x(x), y(y), width(width), height(height), text(""), isActive(false), placeholder(placeholder) {}

void TextInput::draw() {
	// 绘制输入框背景
	COLORREF bgColor = isActive ? RGB(50, 50, 60) : RGB(40, 40, 50);
	setfillcolor(bgColor);
	fillrectangle(x, y, x + width, y + height);

	// 绘制边框
	COLORREF borderColor = isActive ? RGB(100, 150, 255) : RGB(80, 80, 100);
	setlinecolor(borderColor);
	rectangle(x, y, x + width, y + height);

	// 绘制文本
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);

	string displayText = text.empty() ? placeholder : text;
	if (!text.empty()) {
		displayText = text;
	}

	// 限制显示文本长度
	if (displayText.length() > 15) {
		displayText = displayText.substr(0, 15) + "...";
	}

	TCHAR wText[128] = {};
#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, displayText.c_str(), -1, wText, 128);
#else
	_tcscpy_s(wText, displayText.c_str());
#endif

	int textHeight = textheight(wText);
	int textY = y + (height - textHeight) / 2;
	outtextxy(x + 5, textY, wText);
}

void TextInput::handleInput(char inputChar) {
	if (!isActive) return;

	if (inputChar == '\b') { // 退格键
		if (!text.empty()) {
			text.pop_back();
		}
	}
	else if (inputChar >= 32 && inputChar <= 126) { // 可打印字符
		if (text.length() < 20) { // 限制最大长度
			text += inputChar;
		}
	}
}

void TextInput::handleClick(int mouseX, int mouseY) {
	isActive = isInside(mouseX, mouseY);
}

bool TextInput::isInside(int mouseX, int mouseY) const {
	return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

string TextInput::getText() const {
	return text;
}

void TextInput::setText(const string& newText) {
	text = newText;
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
    TextInput hourInput(dialogX + 20, dialogY + 90, 80, 30, "小时 (0-23)");
    TextInput minuteInput(dialogX + 110, dialogY + 90, 80, 30, "分钟 (0-59)");
    TextInput secondInput(dialogX + 200, dialogY + 90, 80, 30, "秒 (0-59)");

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
    secondInput.draw();

    level1Btn.draw();
    level2Btn.draw();
    level3Btn.draw();
    confirmBtn.draw();
    cancelBtn.draw();

    // 在对话框中显示当前选中的等级
    settextcolor(WHITE);
    TCHAR levelText[32] = {};
    _stprintf_s(levelText, _countof(levelText), _T("等级: %d"), selectedLevel);
    outtextxy(dialogX + 230, dialogY + 150, levelText);

    // 等待用户操作
    ExMessage msg;
    bool running = true;
    bool shouldClose = false;

    while (running) {
        // 处理消息队列中的所有消息
        while (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                nameInput.handleClick(msg.x, msg.y);
                hourInput.handleClick(msg.x, msg.y);
                minuteInput.handleClick(msg.x, msg.y);
                secondInput.handleClick(msg.x, msg.y);

                // 处理等级按钮点击
                bool levelChanged = false;
                if (level1Btn.checkClick(msg.x, msg.y)) {
                    selectedLevel = 1;
                    levelChanged = true;
                }
                if (level2Btn.checkClick(msg.x, msg.y)) {
                    selectedLevel = 2;
                    levelChanged = true;
                }
                if (level3Btn.checkClick(msg.x, msg.y)) {
                    selectedLevel = 3;
                    levelChanged = true;
                }

                // 如果等级改变，更新显示
                if (levelChanged) {
                    // 重绘所有等级按钮以反映选中状态
                    level1Btn.draw();
                    level2Btn.draw();
                    level3Btn.draw();

                    // 显示当前选中的等级
                    setfillcolor(RGB(30, 30, 40));  // 覆盖之前的文字
                    fillrectangle(dialogX + 230, dialogY + 145, dialogX + 290, dialogY + 175);
                    settextcolor(WHITE);
                    TCHAR newLevelText[32] = {};
                    _stprintf_s(newLevelText, _countof(newLevelText), _T("等级: %d"), selectedLevel);
                    outtextxy(dialogX + 230, dialogY + 150, newLevelText);
                }

                // 处理确认和取消按钮
                if (confirmBtn.checkClick(msg.x, msg.y)) {
                    // 保存闹钟逻辑
                    string name = nameInput.getText();
                    if (!name.empty()) {
                        // 解析时间输入
                        int hour = 0, minute = 0, second = 0;
                        try {
                            // 简化处理，实际应进行更严格的验证
                            if (!hourInput.getText().empty()) hour = stoi(hourInput.getText());
                            if (!minuteInput.getText().empty()) minute = stoi(minuteInput.getText());
                            if (!secondInput.getText().empty()) second = stoi(secondInput.getText());

                            if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
                                // 添加到闹钟列表
                                alarms.emplace_back(name, hour, minute, second, selectedLevel);
                                cout << "闹钟添加成功" << endl;
                            }
                        }
                        catch (...) {
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
                secondInput.handleInput((char)msg.wParam);

                // 更新显示
                nameInput.draw();
                hourInput.draw();
                minuteInput.draw();
                secondInput.draw();
            }
        }

        // 延迟避免过度刷新
        Sleep(50);
    }

    // 恢复原始屏幕内容
    putimage(0, 0, &backup);
}

// 删除闹钟确认对话框
void showDeleteAlarmDialog(int alarmIndex) {
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
    const int dialogHeight = 150;
    const int dialogX = (1000 - dialogWidth) / 2;
    const int dialogY = (800 - dialogHeight) / 2;

    // 绘制对话框背景
    setfillcolor(RGB(30, 30, 40));
    fillrectangle(dialogX, dialogY, dialogX + dialogWidth, dialogY + dialogHeight);

    // 绘制对话框边框
    setlinecolor(WHITE);
    rectangle(dialogX, dialogY, dialogX + dialogWidth, dialogY + dialogHeight);

    // 提示信息
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    TCHAR message[256] = {};
    _stprintf_s(message, _countof(message), _T("确定要删除闹钟 \"%hs\" 吗?"), alarms[alarmIndex].getName().c_str());
    outtextxy(dialogX + 20, dialogY + 30, message);

    // 创建按钮
    bool confirmed = false;
    Button confirmBtn(dialogX + 50, dialogY + 90, 80, 30, "确认", [&]() {
        confirmed = true;
    });

    Button cancelBtn(dialogX + 170, dialogY + 90, 80, 30, "取消", [&]() {
        // 取消操作
    }); 

    // 绘制按钮
    confirmBtn.draw();
    cancelBtn.draw();

    // 等待用户操作
    ExMessage msg;
    bool running = true;

    while (running) {
        while (peekmessage(&msg)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (confirmBtn.checkClick(msg.x, msg.y)) {
                    // 删除闹钟
                    alarms.erase(alarms.begin() + alarmIndex);
                    running = false;
                    Sleep(100); // 点击确认后等待0.1秒
                }

                if (cancelBtn.checkClick(msg.x, msg.y)) {
                    running = false;
                }
            }
        }
        Sleep(50);
    }

    // 恢复原始屏幕内容
    putimage(0, 0, &backup);
    cleardevice(); // 强制刷新屏幕
}