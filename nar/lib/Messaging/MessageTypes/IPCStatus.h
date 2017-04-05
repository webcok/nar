#ifndef NAR_MESSIPCSTATUS_H
#define NAR_MESSIPCSTATUS_H

#include <string>
#include "IPCBaseRequest.h"
#include "IPCBaseResponse.h"
#include <nar/narnode/utility.h>
#include <nar/lib/nlohJson/json.hpp>

#include <vector>

namespace nar {
    namespace MessageTypes {
        namespace IPCStatus {
            class Request : public IPCBaseRequest {
                public:
                    Request() : IPCBaseRequest(std::string("status")) {}
					Request(std::string username, std::string password, std::string curdir) : IPCBaseRequest(std::string("pull"), username, password, curdir) {}
                    nlohmann::json get_myrequestjson();
                    void send_action(nar::Socket* skt);

					nlohmann::json generate_json();
					void populate_object(std::string& jsn_str);
            };

            class Response : public IPCBaseResponse {
                public:
                    Response(long int prog, long int sc) : IPCBaseResponse(prog, std::string("status"), sc) {}
                    nlohmann::json give_myresponsejson();
            };
        }
    }
}

#endif
