#include <nar/lib/Exception/Exception.h>
#include <nar/lib/Messaging/MessageTypes/KeepAlive.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPush.h>
#include <nar/lib/Messaging/MessageTypes/WaitChunkPull.h>
#include <nar/lib/Messaging/messaging_utility.h>
#include <time.h>
#include "reactive.h"
#include <thread>
#include <string>
#include <nar/lib/Messaging/MessageTypes/KeepAliveCheck.h>

void nar::keep_alive(nar::Socket* sck, nar::Global* globals) {
    //while(!nar::DaemonTasks::handshake(sck, globals));
    std::string macid = globals->get_machine_id();
    nar::MessageTypes::KeepAlive::Request req(macid);
    nar::MessageTypes::KeepAlive::Response resp(200);
    int tried = 5;

    while (tried--) {
        try {
            req.send_mess(sck,resp);
        }
        catch (nar::Exception::MessageTypes::ServerSocketAuthenticationError exp) {
            std::cout << exp.what() << std::endl;
            sleep(1);
            continue;
        }
        break;
    }
    if(!tried) {
        std::cout << "Server may be down" << std::endl;
        exit(0);
    }
    return;
}




void nar::chunk_push_replier(long long int stream_id, nar::Global* globals, long long int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket* cli_sck = new nar::USocket(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    std::cout << "HERE THREAD, chunk size: "<< chunk_size << std::endl;
    cli_sck->connect();
    std::cout << "Ready To Read" << std::endl;


    std::string path(globals->get_file_folder() + std::string("/c"));
    path = path + std::to_string(chunk_id);
    nar::File recvfile(path, "w", false);

    int total_read = 0;
    char buf[1024];
    while(total_read < chunk_size) {
        std::cout << stream_id << " before" << std::endl;
         int len = cli_sck->recv(buf, 1024);
        std::cout << stream_id << " after " << len << std::endl;
         recvfile.write(buf, len);
         total_read += len;
    }
    recvfile.close();
    return;
}


void nar::chunk_pull_replier(unsigned int stream_id, nar::Global* globals, int chunk_size, unsigned short rand_port, long long int chunk_id) {
    nar::USocket cli_sck(globals->get_ioserv(), globals->get_server_ip(), rand_port, stream_id);
    cli_sck.connect();
    std::string path(globals->get_file_folder() + std::string("/c"));
    nar::File f( (path+std::to_string(chunk_id) ).c_str(), "r", false);
    cli_sck.send(f,0,f.size());
}

void nar::reactive_dispatcher(nar::Global *globals) {
    nar::Socket* server_socket = globals->establish_server_connection();
    keep_alive(server_socket, globals);


    //resp.receive_message(transform(message))
    for(;;) {
        std::cout<<"MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" <<std::endl;
        std::string message = nar::get_message( *server_socket);
        std::cout<<"MEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" <<message<<std::endl;
        std::string action = Messaging::get_action(message);
        std::cout << "Action::    " <<  std::endl << action << std::endl;
        if(action == std::string("wait_chunk_push_request")) {
            nar::MessageTypes::WaitChunkPush::Request req;
            req.receive_message(Messaging::transform(message));

            long long int stream_id = req.get_stream_id();
            long long int chunk_id = req.get_chunk_id();
            long long int chunk_size = req.get_chunk_size();
            unsigned short rand_port = req.get_randevous_port();

            std::cout << "STREAM ID: ************ " << stream_id << std::endl;

            // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

            nar::MessageTypes::WaitChunkPush::Response resp(200);
            resp.send_mess(server_socket);

            std::thread thr(&nar::chunk_push_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
            thr.detach();
        } else if (action == std::string("wait_chunk_pull_request")) {
          nar::MessageTypes::WaitChunkPull::Request req;
          req.receive_message(Messaging::transform(message));
          unsigned int stream_id = req.get_stream_id();
          long long int chunk_id = req.get_chunk_id();
          int chunk_size = req.get_chunk_size();
          unsigned short rand_port = req.get_randevous_port();

          // DO CHECKS IF THERE ARE ANY B4 SENDING SUCCESS

          nar::MessageTypes::WaitChunkPull::Response resp(200);
          resp.send_mess(server_socket);

          std::thread thr(nar::chunk_pull_replier, stream_id, globals, chunk_size, rand_port, chunk_id);
          thr.detach();
        } else if(action == std::string("keepalive_check")) {
            nar::MessageTypes::KeepAliveCheck::Response resp(200);
            resp.send_mess(server_socket);
        }
    }
}
