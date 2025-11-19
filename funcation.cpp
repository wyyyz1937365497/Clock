#include "class.h"
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
Alarm::Alarm(const string& name, int hour, int minute, int level) 
	: name(name), hour(hour), minute(minute), level(level) {}

string Alarm::getName() const {
	return name;
}

int Alarm::getHour() const {
	return hour;
}

int Alarm::getMinute() const {
	return minute;
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

void Alarm::setLevel(int newLevel) {
	level = newLevel;
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
