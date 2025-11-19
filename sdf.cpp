#include <graphics.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <thread>
#include <future>
#include "class.h"
using namespace std;
double norm_angle(double a)
{
    a = fmod(a, 360.0);
    return a < 0 ? a + 360.0 : a;
}

bool is_angle_between(double angle, double start, double end)
{
    start = norm_angle(start);
    end = norm_angle(end);
    angle = norm_angle(angle);
    if (start <= end)
        return angle >= start && angle <= end;
    else
        return angle >= start || angle <= end;
}
void sdf_pie_mt(int cx, int cy, int r, double start_deg, double end_deg, COLORREF fill_color, int aa_width)
{
    if (r <= 0 || aa_width <= 0)
        return;
    int left = max(0, cx - r - aa_width);
    int top = max(0, cy - r - aa_width);
    int right = min(getwidth() - 1, cx + r + aa_width);
    int bottom = min(getheight() - 1, cy + r + aa_width);
    int w = right - left + 1;
    int h = bottom - top + 1;
    if (w <= 0 || h <= 0)
        return;
    double start_rad = start_deg * PI / 180.0;
    double end_rad = end_deg * PI / 180.0;
    IMAGE tmp(w, h);
    SetWorkingImage(&tmp);
    vector<COLORREF> buffer(w * h, 0);
    unsigned int n_threads = thread::hardware_concurrency();
    n_threads = max(1u, min(n_threads, (unsigned int)h));
    vector<future<void>> futures;
    for (unsigned int t = 0; t < n_threads; ++t)
    {
        int y_start = top + (t * h) / n_threads;
        int y_end = top + ((t + 1) * h) / n_threads - 1;
        y_end = min(y_end, bottom);
        futures.push_back(async(launch::async, [=, &buffer]()
                                {
                BYTE fg_r = GetRValue(fill_color);
                BYTE fg_g = GetGValue(fill_color);
                BYTE fg_b = GetBValue(fill_color);
                for (int y = y_start; y <= y_end; ++y) {
                    for (int x = left; x <= right; ++x) {
                        double dx = (double)(x - cx);
                        double dy = (double)(y - cy);
                        double dist = sqrt(dx * dx + dy * dy);
                        double angle = atan2(dy, dx);
                        if (angle < 0) angle += 2 * PI;
                        double start_norm = start_rad;
                        double end_norm = end_rad;
                        if (start_norm < 0) start_norm += 2 * PI;
                        if (end_norm < 0) end_norm += 2 * PI;
                        bool in_angle = false;
                        if (start_norm <= end_norm) {
                            in_angle = (angle >= start_norm && angle <= end_norm);
                        } else {
                            in_angle = (angle >= start_norm || angle <= end_norm);
                        }
                        if (!in_angle) continue;
                        double sdf = r - dist;
                        double alpha = 0.0;
                        if (sdf >= aa_width) alpha = 1.0;
                        else if (sdf > 0) alpha = sdf / aa_width;
                        else continue;
                        BYTE out_r = (BYTE)(fg_r * alpha);
                        BYTE out_g = (BYTE)(fg_g * alpha);
                        BYTE out_b = (BYTE)(fg_b * alpha);
                        int idx = (y - top) * w + (x - left);
                        buffer[idx] = RGB(out_r, out_g, out_b);
                    }
                } }));
    }
    for (auto &fut : futures)
    {
        fut.wait();
    }
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            COLORREF c = buffer[y * w + x];
            if (c != 0)
            {
                putpixel(x, y, c);
            }
        }
    }
    SetWorkingImage();
    putimage(left, top, &tmp, SRCPAINT);
}
