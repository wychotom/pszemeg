#include "DRB.h"
#include "../common_header.h"
#include "Dedicated_bearer.h"
#include "UE.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

DRB::DRB(int port, std::vector<UE*> &clients) : Dedicated_bearer(port, clients), clients(clients)
{
}

ssize_t DRB::recv_message(int event_fd)
{
    struct FILE_DATA file_data = {};

    ssize_t received_bytes = recv(event_fd, &file_data, sizeof(struct FILE_DATA), 0);

    if(received_bytes > 0)
    {
        Log::info("DRB", "received " + Log::colors[Colors::Blue] + "FILE" + Log::colors[Colors::Default] + "named " + file_data.file_name);

        std::ofstream file(file_data.file_name);
        file.write(file_data.data, file_data.size);
        file.close();
    }

    return received_bytes;
}