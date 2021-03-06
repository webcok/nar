#ifndef NAR_SERVER_PEERS_H
#define NAR_SERVER_PEERS_H
#include <mutex>
#include <string>
#include <iostream>
#include "sockinfo.h"
#include "Database.h"
#include "dbstructs.h"
#include <random>
#include <set>
#include <algorithm>
#include <vector>
using std::string;
namespace nar {
    class Peers {
        private:
            std::recursive_mutex read_mtx;
            std::recursive_mutex write_mtx;
            std::map<string, nar::SockInfo*> _keepalives;
            std::vector<string> _macs;
            nar::Database * _db;


            int read_count = 0;
            void read_start();
            void read_end();
            void write_start();
            void write_end();
        public:
            Peers(nar::Database * db): read_mtx(), write_mtx(), _db(db) {}
            void insert_keepalive(string& mac_id, nar::Socket* skt);
            void delete_keepalive(string& mac_id);
            nar::SockInfo* peer_select(nar::DBStructs::User& user, unsigned long int chunk_size);
            std::vector<nar::SockInfo*>* peer_select(nar::DBStructs::User& user, unsigned long int chunk_size, int peer_count);
            nar::SockInfo* get_peer(string& machine_id);
            nar::SockInfo* random_policy(nar::DBStructs::User& user, unsigned long chunk_size);
            std::vector<nar::SockInfo*>* random_policy(nar::DBStructs::User& user, unsigned long int chunk_size, int peer_count);

    };
}


#endif
