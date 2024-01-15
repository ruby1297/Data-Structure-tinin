// Paiza 涉谷
#include <iostream>
#include <vector>
#include <unordered_map> //using unordered_map O(1) better than using map O(n)
#include <cstdlib>
#include <ctime>

using namespace std;

void input(vector<pair<int, int>> &points, int n)
{
    for (int i = 0; i < n; i++)
    {
        int x, y;
        cin >> x >> y;
        points.push_back(make_pair(x, y));
    }
}

bool In(int px, int py, int cx, int cy, int r)
{
    return ((px - cx) * (px - cx) + (py - cy) * (py - cy)) <= r * r;
}

void Circles(const vector<pair<int, int>> &points, vector<pair<int, int>> &circles, unordered_map<int, int> &used, int r)
{
    srand(time(0));
    int index = 0;
    while ((used.size() < points.size()) && (index < 10000))
    {
        index++;

        /* 這樣可能會找不到點，期末考完再來想辦法 */
        // Randomly generate the center of circle (cx, cy) and check whether a point inside
        int cx = rand();
        int cy = rand();

        bool Add = false;
        for (const auto &point : points)
        {
            int px = point.first, py = point.second;
            if (used.find(px + py) == used.end())
            {
                bool inCircle = In(px, py, cx, cy, r);
                if (inCircle)
                {
                    used[px + py] = 1;
                    Add = true;
                }
            }
        }

        if (Add)
            circles.push_back(make_pair(cx, cy));
    }
}

void output(const vector<pair<int, int>> &points, const vector<pair<int, int>> &circles, const unordered_map<int, int> &used)
{
    // output points
    for (const auto &circle : circles)
        cout << circle.first << " " << circle.second << endl;

    for (const auto &point : points)
        if (used.find(point.first + point.second) == used.end())
            cout << point.first << " " << point.second << endl;
}

int main()
{
    int n, r;
    cin >> n >> r;

    vector<pair<int, int>> points;
    input(points, n);

    vector<pair<int, int>> circles;
    unordered_map<int, int> used;

    Circles(points, circles, used, r);

    // output points
    output(points, circles, used);

    return 0;
}