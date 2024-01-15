// paiza 五反田
#include <iostream>
#include <vector>
using namespace std;

void buystock(int day, int m, int a)
{
    int money = m;
    int stock = 0;
    int price;
    for (int i = 0; i < day; i++)
    {
        cin >> price;
        if (price <= a)
        {
            if (money >= price)
            {
                stock += money / price;
                money = money % price;
            }
        }
        else
        {
            money += stock * price;
            stock = 0;
        }
    }

    int total = 0;
    total = money + stock * price;
    cout << total << endl;
}

int main()
{
    int day;
    cin >> day;
    int m, a;
    cin >> m >> a;
    buystock(day, m, a);

    return 0;
}
