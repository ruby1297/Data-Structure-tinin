// paiza 品川
#include <iostream>
#include <string>
using namespace std;
int main()
{
    int num;
    cin >> num;
    string bullet;
    cin.ignore();
    getline(cin, bullet);
    int ans = 0;
    string ori = bullet;
    while (bullet != ori || !ans)
    {
        ans++;
        char temp = bullet[0];
        bullet.erase(bullet.begin());
        bullet += temp;
    }
    cout << ans << endl;
    return 0;
}