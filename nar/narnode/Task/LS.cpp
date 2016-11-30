#include <nar/narnode/Task/LS.h>
#include <iostream>
/*

STATUS CODES
200 -> [SUCC] Request successuf.
301 -> [FAIL] Directory does not exist.
REQ
{
    "header": {
        "channel":"ps"
        "action":"get_user_file_info"
    },
    "payload": {
        "user_name": name,
        "dir_name": file,
    }

}
RES
{
    "header":{
        "channel":"sp",
        "status-code":"2xx" OR "3xx",
        "reply-to":"get_user_file_info"
    },
    "payload":{
        "file-list": ["1":{"file_id":file_id1, "file_name":file_name1,
                        "file_size":file_size1,"file_type":file_type1},
                            "2":{"file_id":file_id2, "file_name":file_name2,
                        "file_size":file_size2,"file_type":file_type2},
                    ...]
        "dir-list": ["1":{"dir_id":dir_id1, "dir_name":dir_name1,
                        "dir_size":dir_size1,"dir_type":dir_type1},
                            "2":{"dir_id":dir_id2, "dir_name":dir_name2,
                        "dir_size":dir_size2,"dir_type":dir_type2},
                    ...]
    }
}



*/
void nar::task::LS::createReq(){}
void nar::task::LS::run(int unx_sockfd, nar::Global* globals) {
    std::cout << "ls" << std::endl;
}