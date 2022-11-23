#define _CRT_SECURE_NO_WARNINGS
#define mp make_pair
#define ll long long
#define pb push_back
#define pii pair< int , int >
#define pll pair< ll, ll>
#define pdd pair< double, double>
#define all(x) begin(x), end(x)
#pragma GCC optimize("O3")
#pragma comment(linker, "/STACK:100000000000") 
#define _USE_MATH_DEFINES
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<map>
#include<iterator>
#include<set>
#include<queue>
#include<sstream>
#include<unordered_set>
#include<unordered_map>
#include<list>
#include<stack>
#include<math.h>
#include<fstream>
#include <time.h>
using namespace std;
const int SIZE = 500009;
const int INF = (int)1e9 + 7;

//calculate distance between two points
pdd calcDist(pdd dot1, pdd dot2)
{
    return {(dot1.first - dot2.first),(dot1.second - dot2.second) };
}

//check if enemy is in bomb radius
bool checkDot(pii dotToCheck, pii bomb, int radius)
{
    double distance = sqrt(pow((dotToCheck.first - bomb.first), 2) + pow((dotToCheck.second - bomb.second), 2));
    return distance <= radius;
}

//calculeta amount of enemies in bomb radious
int checkBomb(pii bomb, const vector<pii>& enemy, int radius)
{
    int res = 0;
    for (pii i : enemy)
    {
        res += checkDot(i, bomb, radius);
    }
    return res;
}

//calculate best of all posible results (work very very slow O(N*(maxX-minX)*(maxY-minY)))
int bestRes(const vector<pii>& enemy, int radius)
{
    int maxY = 0,minY = INT_MAX, maxX = 0, minX = INT_MAX;
    for (pii i : enemy)
    {
        maxX = max(maxX, i.first);
        minX = min(minX, i.first);
        maxY = max(maxY, i.second);
        minY = min(minY, i.second);
    }
    int bestRes = 0;
    for (int i = minX; i < maxX; ++i)
    {
        for (int j = minY; j < maxY; ++j)
        {
            bestRes = max(bestRes,checkBomb({ i,j }, enemy, radius));
        }
    }
    return bestRes;
}

//work good with big groups of enemies O((maxX-minX) + (maxY-minY) + NlogN)
pii solve1(const vector<pii>& enemy, int radius)
{
    vector <pii> enemyX = enemy; // sorted by X coordinate
    vector<pii> enemyY = enemy; // sortd by Y coordinate

    int size = enemy.size();// enemy amount

    //sort enemies arrays
    sort(all(enemyX), [](pii a, pii b) {return a.first < b.first; });
    sort(all(enemyY), [](pii a, pii b) {return a.second < b.second; });

    int maxX = 0, maxY = 0; //max coordinates on X and Y axis
    int last = 0, first = radius * 2; //coordinates of bomb
    list<int> l; // list of enemies in bomb radius

    int index = 0;//index of current enemy
    int i = 0; 
    //find the biggest group on X axis
    for (; i < size; ++i)
    {
        if (enemyX[i].first <= radius * 2)
        {
            l.push_back(i);
            index = i;
        }
        else
            break;
    }
    ++index;
    int maxKol = 0;
    for (; i < enemyX.back().first; ++i)
    {
        ++first;
        ++last;
        while (l.size() > 0 && enemyX[*l.begin()].first < last)
        {
            l.erase(l.begin());
        }
        while (index < size && enemyX[index].first < first)
        {
            l.push_back(index);
            ++index;
        }
        if (l.size() > maxKol)
        {
            maxX = (last + first) / 2;
            maxKol = l.size();
        }
    }
    //find biggest group on Y axis
    last = 0;
    first = radius * 2;
    l.clear();
    index = 0;
    i = 0;
    for (; i < size; ++i)
    {
        if (enemyY[i].second <= radius * 2)
        {
            l.push_back(i);
            index = i;
        }
        else
            break;
    }
    ++index;
    maxKol = 0;
    for (; i < enemyY.back().second; ++i)
    {
        ++first;
        ++last;
        while (l.size() > 0 && enemyY[*l.begin()].second < last)
        {
            l.erase(l.begin());
        }
        while (index < size && enemyY[index].second < first)
        {
            l.push_back(index);
            ++index;
        }
        if (l.size() > maxKol)
        {
            maxY = (last + first) / 2;
            maxKol = l.size();
        }
    }
    return { maxX,maxY };
}

//work good with evenly distributed enemies O(N) 
pii solve2(const vector<pii>& enemy, int radius)
{
    //calculate middle point of all enemies
    pll res = {0,0};
    for (pii i : enemy)
    {
        res.first += i.first;
        res.second += i.second;
    }
    res.first /= enemy.size();
    res.second /= enemy.size();

    //calculate offset for middle point
    pdd dist = { 0,0 };
    pdd offset = { 0,0 };
    for (pii i : enemy)
    {
        dist = calcDist(res, i);

        offset.first += radius / dist.first;
        offset.first = min(offset.first, dist.first);
        offset.first = max(offset.first, -dist.first);

        offset.second += radius / dist.second ;
        offset.second = min(offset.second, dist.second);
        offset.second = max(offset.second, -dist.second);
    }
    return {res.first + offset.first, res.second + offset.second};
}

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#ifdef _DEBUG
    freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
#endif


    srand(time(0));

    //randomise enemy positions and bomb radius
    vector <pii> enemy;
    int radius = 75-rand() % 25;
    for (int i = 0; i < 10; ++i)
    {
        int x, y;
        x = rand() % 1000;
        y = rand() % 1000;
        enemy.pb({ x,y });
    }

    //cout <<"Best:" << bestRes(enemy, radius) << endl;
    cout << "Solution1: " << checkBomb(solve1(enemy,radius), enemy, radius)<<endl;
    cout << "Solution2: " << checkBomb(solve2(enemy,radius), enemy, radius) << endl;
    return 0;
}  