#include "FileKeeper.h"
#include <cstring>
#include <cstdlib>

nar::FileKeeper::FileKeeper(char * file){
	_file = file;
	_fd = openAllWays(_file);
}

nar::FileKeeper::FileKeeper(std::string file){
	_file = (char*) malloc(sizeof(char)*(file.size()+1));
    std::strcpy(_file, file.c_str());
	_fd = openAllWays(_file);
}

void nar::FileKeeper::emptyMap() {
	if(_fds.size()>0) {
		std::map<std::string ,int>::iterator it=_fds.begin();
		for(;it!= _fds.end(); ++it) {
			closeFd(it->second);
		}
	}
}

void nar::FileKeeper::closeFd(int fd) {
	if(close(fd) != 0) {			// closes the file descriptor
		printf("something is wrong with close() in FileKeeper destructor %s \n" , strerror(errno));
		return;
	}
}

int nar::FileKeeper::openFdRdonly(const char * file) {
	int fd = open(file,O_RDONLY);
	if(fd < 0) {
		printf("something is wrong with open() in FileKeeper constructor with string %s \n" , strerror(errno));
		return -1;
	}
	return fd;
}

int nar::FileKeeper::openFdWrtonly(const char * file) {
	int fd = open(file,O_WRONLY | O_CREAT,00777); 			// CREATE AND WRITE
	if( fd < 0) {
		printf("something is wrong with open() in FileKeeper constructor with string %s \n" , strerror(errno));
		return -1;
	}
	return fd;
}

int nar::FileKeeper::openAllWays(const char * file) {
    int fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd < 0) {
        printf("something is wrong with openAllWays");
        return -1;
    }
    return fd;
}

nar::FileKeeper::~FileKeeper() {
	closeFd(_fd);
	emptyMap();
    free(_file);
	_fds.clear();
}



void nar::FileKeeper::setFileName(char * file) {
	if(_fd > 0) {
		this->closeFd(_fd);
	}
	_file = file;
	_fd = openFdRdonly(_file);

}


void nar::FileKeeper::setFileName(std::string file) {
	if(_fd > 0) {
		this->closeFd(_fd);
	}
	_file = (char *)file.c_str();
	_fd = open(_file,O_RDONLY);	// creates the file descriptor
	if(_fd < 0) {
		printf("something is wrong with open() in the setFileName() with string in FileKeeper %s \n" , strerror(errno));
		return ;
	}
}



int nar::FileKeeper::getBytes(size_t start,size_t buffersize, char * buffer) {

	if( this->_file ==NULL) {			// checks whether there is a file to open or not
		printf("you need to a give a file name to FileKeeper");
		return -1;
	}

	int r_value;
	if(lseek(_fd, start, SEEK_SET) != start) {	// goes to desired location in the file descriptor
		printf("something is wrong with lseek() in FileKeeper getBytes %s \n" , strerror(errno));
		return -1;
	}
	r_value = read(_fd, buffer, buffersize);
	if(r_value < 0) {		// reads from the file desciptor
		printf("something is wrong with read() in FileKeeper getBytes %s \n" , strerror(errno));
		return -1;
	}

	return r_value;
}

int nar::FileKeeper::writeToFile(int fd, size_t buffersize, const char * buffer) {
	int w_value = write(fd,  buffer, buffersize);
	if(w_value < 0) {		// reads from the file desciptor
		printf("something is wrong with write() in FileKeeper writeToFile %s \n" , strerror(errno));
		return -1;
	}
	return w_value;
}

unsigned long nar::FileKeeper::getFileSize(){
	unsigned long fsize;
	fsize = lseek(_fd, 0, SEEK_END);
	return fsize;
}
std::string nar::FileKeeper::getFileName(){
	std::string fName(_file);
	std::size_t found = fName.find_last_of("/\\");
	return fName.substr(found+1);
}

void nar::FileKeeper::setFileMap(std::vector<std::string> & filenames) {
	std::vector<int> fds(filenames.size());

	this->emptyMap();
	_fds.clear();
	for(int  i= 0;i<filenames.size();i++) {
		fds[i] = openFdRdonly(filenames[i].c_str());
		_fds.insert(std::pair<std::string,int>(filenames[i],fds[i]));
	}
}

void nar::FileKeeper::insertOneToMap(std::string file) {
	int fd = openFdRdonly(file.c_str());
	_fds.insert(std::pair<std::string,int>(file,fd));
}

void nar::FileKeeper::removeOneFromMap(std::string file) {
	std::map<std::string ,int>::iterator it;
	it = _fds.find(file);
	if(it != _fds.end()){
		this->closeFd(it->second);
		_fds.erase(it);
	}
}

int nar::FileKeeper::getBytesMap(std::string file, size_t start, size_t buffersize, char * buffer) {
	int r_value,fd;
	std::map<std::string,int>::iterator it;
	it = _fds.find(file);
	if(it == _fds.end()) {
		printf("there is no file like that");
		return -1;
	}
	fd = it->second;
	if(lseek(fd, start, SEEK_SET) != start) {	// goes to desired location in the file descriptor
		printf("something is wrong with lseek() in FileKeeper getBytesMap %s \n" , strerror(errno));
		return -1;
	}
	r_value = read(fd, buffer, buffersize);
	if(r_value < 0) {		// reads from the file desciptor
		printf("something is wrong with read() in FileKeeper getBytesMap %s \n" , strerror(errno));
		return -1;
	}
	return r_value;
}

void nar::FileKeeper::printMap() {
	std::map<std::string,int>::iterator it;
	it = _fds.begin();
	for(;it!= _fds.end();++it) {
		std::cout<<it->first<<" - > "<<it->second<<std::endl;
	}
}

int nar::FileKeeper::getFd() {
    return _fd;
}
