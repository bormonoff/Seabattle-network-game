#pragma once

#include "handlegame.h"

#include <fstream

namespace network {

void StartServer(core::HandleGame& game, const int& port);
void StartClient(core::HandleGame& game, const int& port, const std::string& IP);
} // namespace network
