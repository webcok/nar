#include <nar/narnode/Task/PushFile.h>
#include <nar/narnode/FileKeeper/FileKeeper.h>
#include <iostream>


static const char* kTypeNames[] = 
    { "Null", "False", "True", "Object", "Array", "String", "Number" };

//nar::ClientSocket *establishConnection( std::string peerIp, int peerPort);
nar::ClientSocket *nar::task::PushFile::establishServerConnection(nar::Global* globals){
	nar::ClientSocket *serverSck  = new nar::ClientSocket(globals->get_narServerIp(),globals->get_narServerPort());		// Get Connection With Server
	return serverSck;
}

void nar::task::PushFile::getJsonHeader(rapidjson::Document &header ){
	
    header.SetObject();
    rapidjson::Document::AllocatorType& allocator = header.GetAllocator();

    header.AddMember("channel", "ps", allocator);
    header.AddMember("action", "file_push_request", allocator);
	return;
}
void nar::task::PushFile::getJsonPayload(rapidjson::Document &payload, std::string fileName, unsigned long fileSize,std::string dir ){
	payload.SetObject();
    rapidjson::Document::AllocatorType& allocator = payload.GetAllocator();
	rapidjson::Value fName;
	fName.SetString(fileName.c_str(), fileName.size(),allocator);
	rapidjson::Value fSize(fileSize);

	rapidjson::Value dirName; 
	dirName.SetString(dir.c_str(), dir.size(),allocator);

    payload.AddMember("file-size", fSize, allocator);
    payload.AddMember("file-name", fName, allocator);
    payload.AddMember("directory", dirName, allocator);	
	//payload.AddMember("file-size",)

}

void nar::task::PushFile::sendJson(rapidjson::Document &msg,nar::ClientSocket *serverSck ){
std::cout << "HERE2" << std::endl;
	rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    msg.Accept(writer);
    std::string stringify(buffer.GetString());
	stringify.insert(0,std::to_string(stringify.size())+" ");

std::cout << "HERE?3" << std::endl;
	char * writable = new char[stringify.size() + 1];
	std::copy(stringify.begin(), stringify.end(), writable);
	writable[stringify.size()] = '\0';std::cout << "HERE?4" << std::endl;
	std::cout << writable << std::cout;
	serverSck->send(writable, stringify.size());
}

void nar::task::PushFile::recvJson(rapidjson::Document &msg, nar::ClientSocket *serverSck ){
	std::cout << "HERE!" << std::endl;
	char *buffer = new char[1024];
	serverSck->recv(buffer, 10);
	buffer[10] = '\0';
	std::string buff(buffer);
	int split = buff.find(" ");
	int jLen = std::stoi( buff.substr(0,split) );
	for(int i = 0; i < 10 - split-1; ++i) buffer[i] = buff[10-split-1];
	serverSck->recv(buffer+10-split-1,jLen-10+split+1 );								// RECV BROKEN
	
	std::cout << buffer << std::endl;

	/*msg.Parse(std::string(buffer).c_str());
	serverSck->send(buffer, 100);*/
}

void nar::task::PushFile::run(int unx_sockfd, nar::Global* globals) {
std::cout << "HERE?XXX" << std::endl;
	nar::FileKeeper file(file_path);
	unsigned long file_size = file.getFileSize();

	rapidjson::Document msg;	
	rapidjson::Document::AllocatorType& allocator = msg.GetAllocator();
	msg.SetObject();

	rapidjson::Document header(&msg.GetAllocator());		// Create Header
	getJsonHeader(header);

	rapidjson::Document payload(&msg.GetAllocator());		// Create Payload
	getJsonPayload(payload,file.getFileName(),file.getFileSize(),globals->get_curdir());

	msg.AddMember("header:", header, msg.GetAllocator());
	msg.AddMember("payload:", payload, msg.GetAllocator());

	nar::ClientSocket *serverSck = establishServerConnection(globals); // Connect to Server
	std::cout << "HERE?1" << std::endl;
	sendJson(msg,serverSck); 							// Send Push File REQ
	
	rapidjson::Document response;

	recvJson(response, serverSck);
		
}


