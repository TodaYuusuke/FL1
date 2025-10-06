#include "BlackBoard.h"

bool BlackBoard::HasKey(const std::string& key) const {
	return data.find(key) != data.end();
}
