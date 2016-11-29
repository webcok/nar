#include <nar/narnode/global.h>
void nar::Global::read_start() {
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::Global::read_end() {
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::Global::write_start() {
    write_mtx.lock();
}
void nar::Global::write_end() {
    write_mtx.unlock();
}

std::string nar::Global::get_username() {
    read_start();
    std::string uname = username;
    read_end();
    return uname;
}
void nar::Global::set_username(std::string uname) {
    write_start();
    username = uname;
    write_end();
}


std::string nar::Global::get_curdir() {
    read_start();
    std::string dir = cur_dir;
    read_end();
    return dir;
}
void nar::Global::set_curdir(std::string cd) {
    write_start();
    cur_dir = cd;
    write_end();
}

void nar::Global::set_narServerIp(std::string ip){
    write_start();
    narServerIP = ip;
    write_end();
}
void nar::Global::set_narServerPort(int port){
    write_start();
    narServerPort = port;
    write_end();

}

std::string nar::Global::get_narServerIp(){
	read_start();
    std::string serverIp = narServerIP;
    read_end();
    return serverIp;
}
int nar::Global::get_narServerPort(){
	read_start();
    int serverPort = narServerPort;
    read_end();
    return serverPort;
}

void set_configFolder(std::string fold) {
    write_start();
    configFolder = fold;
    write_end();
}

std::string get_configFolder() {
    read_start();
    std::string fold = configFolder;
    read_end();
    return fold;
}


