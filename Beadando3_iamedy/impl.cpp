#include "types.hpp"
#include "pipe.hpp"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <array>
#include <iterator>
#include <cmath>
#include <future>
#include <string>
#include <chrono>

static std::mutex write_mutex;
std::string outputfile = "D:\\output_x.txt";
std::ofstream ofile(outputfile.c_str(), std::ios::out);

FIELD field_from_int(const int i)
{
	/*switch (i)
	{
	case 0: return FIELD::CANNIBALS;
	case 1: return FIELD::DESERT;
	case 2: return FIELD::FOREST;
	case 3: return FIELD::GOLD_MINE;
	case 4: return FIELD::GRASSLAND;
	case 6: return FIELD::LAKE;
	case 7: return FIELD::SEA;
	case 8: return FIELD::SWAMP;
	case 9: return FIELD::VILLAGE;
	case 10: return FIELD::WHEAT;
	case 11: return FIELD::CITY;
	default:
		return FIELD::SEA;
	}*/
	// vagy 	
	return static_cast<FIELD>(i);
}


int tile_value(const Tile& f)
{
	return field_value(std::get<1>(f));
	// vagy return field_value(f.second);
}

int field_value(const FIELD& f)
{
	switch (f)
	{
	case FIELD::CANNIBALS:
		return -2;
	case FIELD::DESERT:
		return 0;
	case FIELD::FOREST:
		return 3;
	case FIELD::GOLD_MINE:
		return 5;
	case FIELD::GRASSLAND:
		return 2;
	case FIELD::IRON:
		return 2;
	case FIELD::LAKE:
		return 3;
	case FIELD::SEA:
		return 1;
	case FIELD::SWAMP:
		return 1;
	case FIELD::VILLAGE:
		return 6;
	case FIELD::WHEAT:
		return 3;
	case FIELD::CITY:		
		return 10;
	default:
		return 10;
	}
}

std::ostream& operator<<(std::ostream& s, const FIELD& f)
{
	switch (f)
	{
	case FIELD::CANNIBALS:
		s << "cannibals"; break;
	case FIELD::DESERT:
		s << "desert"; break;
	case FIELD::FOREST:
		s << "forest"; break;
	case FIELD::GOLD_MINE:
		s << "gold mine"; break;
	case FIELD::GRASSLAND:
		s << "grassland"; break;
	case FIELD::IRON:
		s << "iron"; break;
	case FIELD::LAKE:
		s << "lake"; break;
	case FIELD::SEA:
		s << "sea"; break;
	case FIELD::SWAMP:
		s << "swamp"; break;
	case FIELD::VILLAGE:
		s << "village"; break;
	case FIELD::WHEAT:
		s << "wheat"; break;
	case FIELD::CITY:
		s << "city"; break;
	}
	return s;
}

std::ostream& operator<<(std::ostream& s, const DIRECTION& d)
{
	switch (d)
	{
	case DIRECTION::NORTH_WEST:
		s << "NW"; break;
	case DIRECTION::NORTH:
		s << "N"; break;
	case DIRECTION::NORTH_EAST:
		s << "NE"; break;
	case DIRECTION::EAST:
		s << "E"; break;
	case DIRECTION::SOUTH_EAST:
		s << "SE"; break;
	case DIRECTION::SOUTH:
		s << "S"; break;
	case DIRECTION::SOUTH_WEST:
		s << "SW"; break;
	case DIRECTION::WEST:
		s << "W"; break;
	}
	return s;
}


bool operator<(const Coordinate& a, const Coordinate& b)
{
	if (a.x < b.x) return true;
	if (a.x > b.x) return false;

	return a.y <= b.y;
}

std::ostream& operator<<(std::ostream& s, const Coordinate& p)
{
	s << "(" << p.x << "," << p.y << ")";
	return s;
}
std::istream& operator>>(std::istream& s, Coordinate& p)
{
	s >> p.x >> p.y;
	return s;
}


std::ostream& operator<<(std::ostream& s, const Tile& t)
{
	s << std::get<0>(t) << "-" << std::get<1>(t);
	return s;
}


std::istream& operator>>(std::istream& s, Map& m)
{
	int r, c;
	s >> r >> c;
	m = Map(r, c);
	for (int i = 0; i < r; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			int f;
			s >> f;
			m.set_tile(i, j, field_from_int(f));
		}
	}
	return s;
}

Map::Map() : rows_(0), cols_(0)
{

}

Map::Map(const int r, const int c)
{
	rows_ = r > 0 ? r : 0;
	cols_ = c > 0 ? c : 0;
	map_ = std::vector<std::vector<FIELD>>(rows_, std::vector<FIELD>(cols_, FIELD::SEA));
}

bool Map::in_range(const int x, const int y) const
{
	return x >= 0 && x < rows_ && y >= 0 && y < cols_;
}


Tile Map::tile_at(const int i, const int j) const
{
	FIELD f = in_range(i, j) ? map_[i][j] : FIELD::SEA;
	return std::make_pair(Coordinate(i, j), f);
}

void Map::set_tile(const int i, const int j, const FIELD f)
{
	if (in_range(i, j))
	{
		map_[i][j] = f;
		if (f == FIELD::CITY)
		{
			cities_.push_back(City(i, j));
		}
	}
}

std::set<Tile> Map::get_tiles_in_radius(const int i, const int j, const int r) const
{
	std::array<DIRECTION, 6> dirs = {
		DIRECTION::SOUTH_EAST,
		DIRECTION::SOUTH,
		DIRECTION::SOUTH_WEST,
		DIRECTION::NORTH_WEST,
		DIRECTION::NORTH,
		DIRECTION::NORTH_EAST,
	};
	std::set<Tile> s;
	int x = i;

	s.insert(tile_at(x, j)); //origo

	for (int i = 1; i <= r; ++i)
	{
		--x;
		Tile actual = tile_at(x, j);
		for (const auto& dir : dirs)
		{
			for (int j = 0; j < i; ++j)
			{
				actual = tile_in_direction(actual, dir);
				s.insert(actual);
			}
		}
	}
	return s;
}

//"odd-q" vertical layout, odd columns shoved down
Tile Map::tile_in_direction(int x, int y, const DIRECTION d) const
{
	switch (d)
	{
	case DIRECTION::NORTH_WEST:
		--y;
		if (y & 1)
		{
			--x;
		}
		break;
	case DIRECTION::NORTH:
		--x;
		break;
	case DIRECTION::NORTH_EAST:
		++y;
		if (y & 1)
		{
			--x;
		}
		break;
	case DIRECTION::SOUTH_EAST:
		if (y & 1)
		{
			++x;
		}
		++y;
		break;
	case DIRECTION::SOUTH:
		++x;
		break;
	case DIRECTION::SOUTH_WEST:
		if (y & 1)
		{
			++x;
		}
		--y;
		break;
	default:
		break;
	}
	return tile_at(x, y);
}


int Map::rows() const
{
	return rows_;
}

int Map::cols() const
{
	return cols_;
}
/**
  * new functions for 2nd assignment
  */
void Map::find_cities()
{
	cities_.clear();
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			if (map_[i][j] == FIELD::CITY)
			{
				cities_.push_back(City(i, j));
			}
		}
	}
}

void Map::print()
{
	for (int i = 0; i < rows_; ++i)
	{
		for (int j = 0; j < cols_; ++j)
		{
			std::cout << map_[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

std::vector<std::vector<FIELD>> Map::get_map() const
{
	return map_;
}

std::vector<City> Map::get_cities() const
{
	return cities_;
}

double Map::get_length(const FlightPath& p) const
{
	return get_distance(std::get<0>(p), std::get<1>(p));
}

double Map::get_distance(const City& a, const City& b) const
{
	int x = b.x - a.x;
	int y = b.y - a.y;
	return std::sqrt(x * x + y * y);
}


FlightPath Map::closest_brute_force(const std::vector<City>& cities) const
{
	if (cities.size() == 0)
	{
		return { {0,0}, {0,0} };
	}
	else if (cities.size() == 1)
	{
		return { cities[0], cities[0] };
	}
	else if (cities.size() == 2)
	{
		return { cities[0], cities[1] };
	}

	double abDist = get_distance(cities[0], cities[1]);
	double acDist = get_distance(cities[0], cities[2]);
	double bcDist = get_distance(cities[1], cities[2]);

	double min_dist = std::min(std::min(abDist, acDist), bcDist);
	if (min_dist == abDist)
	{
		return { cities[0], cities[1] };
	}
	else if (min_dist == acDist)
	{
		return { cities[0], cities[2] };
	}
	else
	{
		return { cities[1], cities[2] };
	}
}

FlightPath Map::find_shortest(const std::vector<City>& x_cities, const std::vector<City>& y_cities) const
{
	int city_count = x_cities.size();
	if (city_count <= 3)
	{
		return closest_brute_force(x_cities);
	}

	auto iter = x_cities.begin();
	std::advance(iter, (city_count / 2));
	std::vector<City> on_left_x(x_cities.begin(), iter);
	std::vector<City> on_right_x(iter, x_cities.end());

	int middleX = on_right_x[0].x;

	std::vector<City> on_left_y;
	std::vector<City> on_right_y;

	for (auto it : y_cities)
	{
		if (it.x <= middleX) {
			on_left_y.push_back(it);
		}
		else {
			on_right_y.push_back(it);
		}
	}

	std::future<FlightPath> fp_asnyc = std::async(std::launch::async, [&]() {
		return find_shortest(on_left_x, on_left_y);
		});

	auto fp_1 = find_shortest(on_right_x, on_right_y);
	auto fp_2 = fp_asnyc.get();

	FlightPath min_path = get_length(fp_1) <= get_length(fp_2) ? fp_1 : fp_2;


	std::vector<City> stripe;
	std::copy_if(y_cities.begin(), y_cities.end(), std::back_inserter(stripe),
		[&](const City& c) {
			return std::abs(middleX - c.x) < get_length(min_path);
		});

	auto result = min_path;

	if (stripe.size() > 0)
	{
		for (iter = stripe.begin(); iter != stripe.end(); ++iter)
		{
			for (auto it = iter + 1; it != stripe.end() && ((it->y - iter->y) < get_length(min_path)); ++it)
			{
				auto new_dist = get_distance(*it, *iter);
				if (new_dist < get_length(result))
				{
					result = { *it, *iter };
				}
			}
		}
	}
	return result;
}

FlightPath Map::get_shortest_flightpath() const
{
	std::vector<City> x_cities(cities_.begin(), cities_.end());
	std::sort(x_cities.begin(), x_cities.end(),
		[](const City& a, const City& b) {
			return a.x < b.x;
		});

	std::vector<City> y_cities(cities_.begin(), cities_.end());
	std::sort(y_cities.begin(), y_cities.end(),
		[](const City& a, const City& b) {
			return a.y < b.y;
		});

	return find_shortest(x_cities, y_cities);
}

//3rd assignment

void outputthread(Pipe<trader>& p, int count)
{
	//ofile << "count: " << count << " thread: " << std::this_thread::get_id() << " " << '\n';
	for (auto i = 0; i < count; ++i)
	{
		trader tr = p.pop();

		ofile << "(" << tr[0];
		for (auto i = 1; i < tr.size() - 1; ++i)
		{
			ofile << " " << tr[i];
		}
		ofile << ")" << '\n';
	}
}

void Map::calculate_trades(std::vector<trader>& traders)
{
	auto fi = FIELD::CANNIBALS;
	std::cout << "Field: " << fi << " is useful? " << (is_useful_field(fi) ? "true" : "false") << std::endl;
	auto index = get_field_index(fi);
	std::cout << ((index > -1) ? std::to_string(index) : "not useful field") << std::endl;

	find_cities();
	auto C = get_cities().size();
	std::cout << "Cities: " << C << std::endl;	
	std::vector<Pipe<trader>> pipes(C + 1);
	//pipes.resize(C + 1);
	std::vector<std::thread> working_cities;
	
	auto c = traders.size();
	//Pipe<trader>& p = pipes[0];
	auto& p = pipes[C];
	auto ft = [&]() { outputthread(p, c); };
	std::thread t(ft);
	
	std::cout << "Press any key to continue . . ." << std::endl;
	_getch();		// vagy std::cin.ignore();
	auto f = [&](int j) { city_trade(cities_[j], traders.size(), pipes[j], pipes[j + 1]); };
	for (auto i = 0; i < C; ++i)
	{
		//std::thread t(&Map::city_trade, std::ref(cities_.at(i)), std::ref(s), std::ref(pipes.at(i)), std::ref(pipes.at(i + 1)));
		//working_cities.push_back(std::move(t));
		//working_cities.push_back(std::thread(&Map::city_trade, std::ref(cities_.at(i)), std::ref(s), std::ref(pipes.at(i)), std::ref(pipes.at(i + 1))));
		//working_cities.push_back(f, i);
		
		working_cities.push_back(std::thread(f, i));
	}

	for (trader& tr : traders)
	{
		pipes[0].push(tr);
	}
	
	for (std::thread& th : working_cities)
	{
		if (th.joinable())
			th.join();
	}
	
	t.join();
	ofile.close();
}

bool is_useful_field(FIELD f)
{
	//if (f >= FIELD::CANNIBALS && f <= FIELD::LAKE)
		//return true;
	// értékes mezõk: FIELD::FOREST, FIELD::GOLD_MINE, FIELD::IRON, FIELD::LAKE, FIELD::WHEAT
	if (f == FIELD::FOREST || f == FIELD::GOLD_MINE || f == FIELD::IRON || f == FIELD::LAKE || f == FIELD::WHEAT) return true;
	return false;
}

int get_field_index(FIELD f)
{
	// fa - 0, arany - 1, vas - 2, hal - 3, gabona - 4
	std::vector<FIELD> fields{ FIELD::FOREST, FIELD::GOLD_MINE, FIELD::IRON, FIELD::LAKE, FIELD::WHEAT };
	auto it = std::find(fields.begin(), fields.end(), f);
	if (it != fields.end())
	{
		return std::distance(fields.begin(), it);
	}
	else
	{
		return -1;
	}
}

bool have_sellable(stock& s)
{
	for (auto it = s.begin(); it != s.end(); ++it)
	{
		if (*it > 0) return true;
	}
	return false;
}

void Map::city_trade(const City& city, int count, Pipe<trader>& from, Pipe<trader>& to) const
{
	//std::cout << "City: " << city << " count: " << count << " thread: " << std::this_thread::get_id() << " " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	auto tiles = get_tiles_in_radius(city, 2);
	stock Stock = { 0, 0, 0, 0, 0 };

	for (auto tile : tiles)
	{
		if (is_useful_field(tile.second))
			Stock[get_field_index(tile.second)]++;
	}

	write_mutex.lock();
	std::cout << city << " A varos hasznos keszlete: ";
	for (auto t : Stock)
	{
		std::cout << t << " ";
	}
	std::cout << std::endl;
	write_mutex.unlock();

	trader tr;

	for (auto i = 0; i < count; ++i)
	{		
		tr = from.pop();
		write_mutex.lock();
		std::cout << city << ": A trader hasznos keszlete: ";
		for (auto t : tr)
		{
			std::cout << t << " ";
		}
		std::cout << std::endl;
		write_mutex.unlock();

		auto mi = tr[tr.size() - 1];
		for (auto i = 0; i < Stock.size(); ++i)
		{
			auto city_raw_material = Stock[i];
			if (city_raw_material == 0)
			{
				auto tr_raw_material = tr[i];
				if (tr_raw_material > 0)
				{
					for (auto k = 0; k < Stock.size(); ++k)
					{
						if (Stock[k] > mi)
						{
							// csere: 
							Stock[i]++; tr[i]--; Stock[k] -= mi; tr[k] += mi;
							break;
						}
					}
				}
			}
		}

		write_mutex.lock();
		std::cout << city << " A varos uj keszlete: ";
		for (auto t : Stock)
		{
			std::cout << t << " ";
		}
		std::cout << std::endl;
		write_mutex.unlock();

		to.push(tr);
	}
}
