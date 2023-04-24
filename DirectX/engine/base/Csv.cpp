#include "Csv.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void Csv::LoadFile(const std::string& _fileName, std::vector<std::vector<int>> _map)
{
	std::ifstream ifs(_fileName);
	std::string line;
	int y = 0;

	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string field;

		int addY = y + 1;

		_map.resize(addY);

		while (std::getline(stream, field, ','))
		{
			_map[y].push_back(stoi(field));
		}

		//ŽŸ‚Ì’i‚É‚¢‚­
		y += 1;
	}
}

int Csv::GetChipNum(std::vector<std::vector<int>> _map, float _chipSize, const XMFLOAT2& _position)
{
	int x = (UINT)(_position.x / _chipSize);
	int y = (UINT)(_position.y / _chipSize);

	int maxX = (UINT)_map[0].size();
	int maxY = (UINT)_map.size();

	if (x < 0 || x >= maxX || y < 0 || y >= maxY)
	{
		return 0;
	}

	return _map[y][x];
}