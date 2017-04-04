#include "IPCLs.h"

std::string nar::MessageTypes::IPCLs::Request::get_dir_name(){
    return _dir_name;
}

void nar::MessageTypes::IPCLs::Request::set_dir_name(std::string dn){
    this->_dir_name = dn;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::get_myrequestjson() {
    nlohmann::json json_to_sent;
    json_to_sent["header"]["action"] = "ls";
    json_to_sent["payload"]["dir_name"] = _dir_name;
    return json_to_sent;
}

nlohmann::json nar::MessageTypes::IPCLs::Request::generate_json() {
    nlohmann::json jsn = IPCBaseRequest::generate_json();
    jsn["payload"]["dir_name"] = this->_dir_name;
    return jsn;
}

void nar::MessageTypes::IPCLs::Request::populate_object(std::string& jsn_str) {
    auto jsn = nlohmann::json::parse(jsn_str);
    IPCBaseRequest::populate_object(jsn);
    this->_dir_name = jsn["payload"]["dir_name"];
}

void nar::MessageTypes::IPCLs::Request::send_action(nar::Socket* skt) {
    nlohmann::json json_to_sent = this->generate_json();
    send_message(skt, json_to_sent.dump());
    return;
}

void nar::MessageTypes::IPCLs::Request::print_loop(nar::Socket* skt) {
	std::vector<std::string> v_items;
	std::string ename=std::string(""), esize=std::string(""), ctime=std::string(""), type=std::string("");
    while(true){
		std::string items = std::string("");
        std::string tmp = get_message(*skt);
        nlohmann::json received = nlohmann::json::parse(tmp);
        if(received["header"]["reply_to"] == std::string("END")){
            break;
        }
		else if(received["header"]["reply_to"] == std::string("ls")) {
			ename =	received["payload"]["entity_name"];
			esize = received["payload"]["entity_size"];
			ctime = received["payload"]["change_time"];
			type = received["payload"]["type"];
			items += (std::string(">>>")+ename + std::string(" ") + esize + std::string(" ") + ctime + std::string(" ") + type + std::string("\n")) ;
			v_items.push_back(items);
		}
    }
	for(int i=0;i<v_items.size();i++) {
		std::cout << v_items[i];	
	}
    return;
}

/*
char* nar::MessageTypes::IPCLs::Request::masctime(const struct tm *timeptr)
{
	static const char wday_name[][4] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[][4] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char result[26];
	sprintf(result, "%.3s%2d %.2d:%.2d", mon_name[timeptr->tm_mon], timeptr->tm_mday, timeptr->tm_hour, timeptr->tm_min);
	return result;
}
*/

std::string nar::MessageTypes::IPCLs::Response::get_entity_name() {
	return this -> _entity_name;
}
std::string nar::MessageTypes::IPCLs::Response::get_entity_size() {
	return this -> _entity_size;
}
std::string nar::MessageTypes::IPCLs::Response::get_change_time() {
	return this -> _change_time;
}
std::string nar::MessageTypes::IPCLs::Response::get_type() {
	return this -> _type;
}

void nar::MessageTypes::IPCLs::Response::set_entity_name(std::string en) {
	this -> _entity_name = en;
}
void nar::MessageTypes::IPCLs::Response::set_entity_size(std::string es) {
	this -> _entity_size = es;
}
void nar::MessageTypes::IPCLs::Response::set_change_time(std::string ct) {
	this -> _change_time = ct;
}
void nar::MessageTypes::IPCLs::Response::set_type(std::string t) {
	this -> _type = t;
}

nlohmann::json nar::MessageTypes::IPCLs::Response::give_myresponsejson() {
    nlohmann::json resp_json;
    resp_json["header"]["reply_to"] = get_reply_to();
    resp_json["payload"]["progress"] = get_progress();
    resp_json["payload"]["status_code"] = get_status_code();
	resp_json["payload"]["entity_name"] = get_entity_name();
	resp_json["payload"]["entity_size"] = get_entity_size();
	resp_json["payload"]["change_time"] = get_change_time();
	resp_json["payload"]["type"] = get_type(); 
    return resp_json;
}

