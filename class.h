#include <graphics.h>
#include <conio.h>
#include <functional>
#include <string>
#include <vector>
using namespace std;
const double PI = 3.141592653;
extern bool issdf;
extern IMAGE hidden;
class Dial
{
	int x;
	int y;
	int r;

public:
	Dial(int x, int y, int r);
	void refresh();
};
// class Bottom{
//	string text;
//
// };

class Pin
{
	int x;
	int y;
	int r;
	COLORREF color;

public:
	int angle;
	void write_text(double anglein, int num);
	void point_to_dgree(double angle, int time);
	void changr_color();
	Pin(int x, int y, int len, COLORREF color);
};
double norm_angle(double a);
bool is_angle_between(double angle, double start, double end);
void sdf_pie_mt(int cx, int cy, int r, double start_deg, double end_deg, COLORREF fill_color, int aa_width = 1);
void update_to_screen(IMAGE *hidden);

class Button
{
	int x;
	int y;
	int width;
	int height;
	string text;
	function<void()> onClick;
	COLORREF bgColor;
	COLORREF textColor;
	bool isHovered;

public:
	Button(int x, int y, int width, int height, const std::string &text, std::function<void()> onClick);
	void draw();
	bool checkClick(int mouseX, int mouseY);
	void checkHover(int mouseX, int mouseY);
	void setColors(COLORREF bgColor, COLORREF textColor);
	bool getHoverState() const; // 新增：获取悬停状态
};

// 添加TextInput类用于处理文本输入
class TextInput
{
private:
	int x, y, width, height;
	string text;
	bool isActive;
	string placeholder;

public:
	TextInput(int x, int y, int width, int height, const string &placeholder);
	void draw();
	void handleInput(char inputChar);
	void handleClick(int mouseX, int mouseY);
	string getText() const;
	void setText(const string &newText);
	bool isInside(int mouseX, int mouseY) const;
};

// 添加Alarm类用于表示闹钟对象
class Alarm
{
private:
	string name;
	int hour;
	int minute;
	int second;		// 新增：秒字段
	int level;		// 闹钟等级 1-3
	bool triggered; // 用于跟踪闹钟是否已触发

public:
	Alarm(const string &name, int hour, int minute, int second, int level);
	string getName() const;
	int getHour() const;
	int getMinute() const;
	int getSecond() const; // 新增：获取秒
	int getLevel() const;
	void setName(const string &newName);
	void setHour(int newHour);
	void setMinute(int newMinute);
	void setSecond(int newSecond); // 新增：设置秒
	void setLevel(int newLevel);
	bool isTriggered() const;
	void setTriggered(bool state);
};

void showDeleteAlarmDialog(int alarmIndex); // 添加函数声明

#pragma once