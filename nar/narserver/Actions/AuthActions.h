#ifndef NAR_AUTHACTIONS_H
#define NAR_AUTHACTIONS_H
#include <nar/lib/Socket/Socket.h>
#include <nar/narserver/ServerGlobal.h>
#include <nar/lib/Messaging/MessageTypes/UserAuthenticationInit.h>
#include <nar/lib/Messaging/MessageTypes/MachineRegister.h>
#include <nar/lib/Messaging/MessageTypes/UserRegister.h>
#include <nar/lib/Messaging/MessageTypes/FilePush.h>
#include <nar/narserver/dbstructs.h>

namespace nar {
    namespace AuthAction {
        void authentication_dispatcher(nar::ServerGlobal* s_global, nar::Socket* skt, nar::DBStructs::User& user);
        void push_file_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::FilePush::Request& req, nar::DBStructs::User&  user);
        void machine_register_action(nar::ServerGlobal* s_global, nar::Socket* skt, nar::MessageTypes::MachineRegister::Request& req, nar::DBStructs::User& user);
    }
}

#endif