#include <iostream>
#include <thread>
#include <vector>
#include "pipe.hpp"
#include "types.hpp"
#include <fstream>
#include <string>
#include <set>
#include <future>
#include <windows.h>
using namespace std;
using namespace std::chrono;

static std::mutex write_mutex;
static bool ready;

void f(Pipe<int>& from, Pipe<int>& to)
{
	int data = from.pop();
	data += 2;
	to.push(data);
}

int compute(const Coordinate& coord, const int& r, const Map& m)
{
	int sumOfTileValues = 0;
	auto tiles = m.get_tiles_in_radius(coord.x, coord.y, r);
	for (auto itr = tiles.begin(); itr != tiles.end(); ++itr)
	{
		sumOfTileValues += tile_value(*itr);
	}
	return sumOfTileValues;
}

void mainprog()
{
	string file = "D:\\world3.map";
	ifstream ifile(file.c_str());
	/*string outputfile = "D:\\output.txt";
	ofstream ofile(outputfile.c_str(), ios::out);*/

	Map m;

	ifile >> m;

	int numOfTraders;
	ifile >> numOfTraders;
	vector<trader> traders(numOfTraders);

	for (auto i = 0; i < numOfTraders; ++i)
	{
		for (auto j = 0; j < 6; ++j)
		{
			ifile >> traders[i][j];
		}
	}

	cout << "Traderek: (" << numOfTraders << ")" << endl;
	for (auto tr : traders)
	{
		cout << "(" << tr[0];
		for (auto i = 1; i < 6; ++i)
		{
			cout << " " << tr[i];
		}
		cout << ")" << endl;
	}

	// városok megkeresése
	m.find_cities();

	auto shortest = m.get_shortest_flightpath();

	// kiírás
	/*if (ofile.is_open())
	{
		ofile << "From: " << shortest.first << '\n' << "To: " << shortest.second << '\n';
		cout << "From: " << shortest.first << '\n' << "To: " << shortest.second << endl;
	}
	if (ofile.fail()) {
		cerr << "I/O error";
	}*/

	//m.print();

	auto map = m.get_map();
	for (auto itr = map.begin(); itr != map.end(); ++itr)
	{
		auto col = *itr;
		for (auto itr2 = col.begin(); itr2 != col.end(); ++itr2)
		{
			auto field = (*itr2);
			//cout << field << " (" << field_value(field) << ")\t";
		}
		//cout << endl;
	}

	ifile.close();
	//ofile.close();

	std::vector<Pipe<int>> pipes(2);
	std::thread t(f, std::ref(pipes[0]), std::ref(pipes[1]));

	int data = 40;
	std::cout << "Data before travelling through the pipe: " << data << std::endl;
	pipes[0].push(data);

	data = pipes[1].pop();
	std::cout << "Data after travelling through the pipe: " << data << std::endl;
	t.join();

	m.calculate_trades(traders);
}

void mainshort()
{
	ready = false;
	string file = "D:\\world3.map";
	ifstream ifile(file.c_str());
	string outputfile = "D:\\output.txt";
	ofstream ofile(outputfile.c_str(), ios::out);

	Map m;

	ifile >> m;
	m.find_cities();

	auto shortest = m.get_shortest_flightpath();

	// kiírás
	if (ofile.is_open())
	{
		ofile << "From: " << shortest.first << '\n' << "To: " << shortest.second << '\n';
		//cout << "From: " << shortest.first << '\n' << "To: " << shortest.second << endl;
	}
	if (ofile.fail()) {
		cerr << "I/O error";
	}

	ready = true;

	ifile.close();
	ofile.close();
}

void printstars(int k, int from, int to)
{
	std:unique_lock<std::mutex> lk(write_mutex);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
	for (auto i = 0; i < 120; ++i) {
		for (auto j = from; j < to; ++j)
		{			
			COORD coord;
			coord.X = i;
			coord.Y = j;
			SetConsoleCursorPosition(
				GetStdHandle(STD_OUTPUT_HANDLE),
				coord
			);
			cout << "*";
		}
	}
}

int main(int argc, char const* argv[])
{	
	/*std::thread t1(printstars, 12, 0, 15);	
	std::thread t2(printstars, 10, 15, 30);	
	t1.join();
	t2.join();*/

	high_resolution_clock::time_point tp1 = high_resolution_clock::now();

	std::thread t(mainprog);
	/*for (auto i = 0; i < 10; ++i)
	{
		Sleep(100);
		cout << i << " ";
		if (ready) break;
	}*/
	t.join();
	cout << endl << "A szal futasa befejezodott." << endl;
	high_resolution_clock::time_point tp2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(tp2 - tp1).count();
	std::cout << "A program futasi ideje: " << duration / 1000.0 / 1000.0 << " sec." << endl;

	std::cout << "Kész" << endl;
	std::cout << "Kész" << endl;

	return 0;
}