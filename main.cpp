#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Point
{
	long long x, y;
	Point(long long a = 0, long long b = 0)
	{
		x = a;
		y = b;
	}
};

long long direction(Point a, Point b, Point c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool isOnSegment(Point a, Point b, Point p)
{
	return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool isInsidePolygon(Point p, const vector<Point> &polygon)
{
	int n = polygon.size();
	int intersections = 0;

	for (int i = 0; i < n; ++i)
	{
		long long x1 = polygon[i].x;
		long long y1 = polygon[i].y;
		long long x2 = polygon[(i + 1) % n].x; // % n ???
		long long y2 = polygon[(i + 1) % n].y;

		if (p.y > min(y1, y2) && p.y <= max(y1, y2) && p.x <= max(x1, x2))
		{
			long long x_inter = x1 + (p.y - y1) * (x2 - x1) / (y2 - y1);
			if (x_inter > p.x)
			{
				intersections += 1;
			}
		}
	}

	return intersections % 2 == 1;
}

int getRelation(Point a, Point b, const vector<Point> &polygon)
{
	bool a_inside = isInsidePolygon(a, polygon);
	bool b_inside = isInsidePolygon(b, polygon);

	if (a_inside && b_inside)
	{
		return 0;
	}
	else if (a_inside || b_inside)
	{
		return 2;
	}
	else
	{
		int n = polygon.size();
		for (int i = 0; i < n; ++i)
		{
			Point c = Point(polygon[i].x, polygon[i].y);
			Point d = Point(polygon[(i + 1) % n].x, polygon[(i + 1) % n].y);

			long long d1 = direction(c, d, a);
			long long d2 = direction(c, d, b);
			long long d3 = direction(a, b, c);
			long long d4 = direction(a, b, d);

			if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
			{
				return 1;
			}
			else if (d1 == 0 && isOnSegment(c, d, a))
			{
				return 1;
			}
			else if (d2 == 0 && isOnSegment(c, d, b))
			{
				return 1;
			}
			else if (d3 == 0 && isOnSegment(a, b, c))
			{
				return 1;
			}
			else if (d4 == 0 && isOnSegment(a, b, d))
			{
				return 1;
			}
		}

		return 3;
	}
}

Point str_to_point(string input)
{
	int idx = 0;
	while (input[idx] != ',')
	{
		idx++;
	}
	return Point(
		stoll(
			input.substr(0, idx)),
		stoll(
			input.substr(idx + 1, input.size() - idx)));
}

int main()
{
	ifstream polysFile("polys.txt");
	ifstream linesFile("lines.txt");
	ofstream resultsFile("results.txt");

	string input = "";

	vector<Point> polygon;

	while (polysFile >> input)
	{
		polygon.push_back(str_to_point(input));
	}

	input = "";

	while (linesFile >> input)
	{
		Point p1 = str_to_point(input);
		linesFile >> input;
		Point p2 = str_to_point(input);
		resultsFile << getRelation(p1, p2, polygon) << "\n";
	}
	return 0;
}