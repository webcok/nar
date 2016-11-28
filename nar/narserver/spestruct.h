#ifndef NAR_DBEXTRASTRUCTS_H
#define NAR_DBEXTRASTRUCTS_H

namespace nar {
    namespace db{
        struct Directory{
            sql::SQLString dir_id;
            std::string dir_name;
            sql::SQLString dir_size;
            sql::SQLString change_time;
        };

        struct DirectoryTo{
            sql::SQLString dir_id;
            sql::SQLString item_id;
            sql::SQLString ForD;
        };

        struct User{
            sql::SQLString  user_id;
            std::string user_name;
            sql::SQLString  quota;
            sql::SQLString  disk_space;
            std::string cryptedKey;
            sql::SQLString dir_id;
            sql::SQLString change_time
        };

        struct Machine{
            std::string machine_id;
            sql::SQLString  user_id;
            sql::SQLString  machine_quota;
            sql::SQLString machine_diskSpace;
            sql::SQLString change_time
        };

        struct UserToFile{
            sql::SQLString user_id;
            sql::SQLString file_id;
            sql::SQLString change_time
        };

        struct File{
            sql::SQLString file_id;
            std::string file_name;
            sql::SQLString file_size;
            std::string file_type;
            sql::SQLString change_time
        };

        struct Chunk{
            sql::SQLString chunk_id;
            sql::SQLString file_id;
            sql::SQLString chunk_size;
        };

        struct ChunkToMachine {
            sql::SQLString chunk_id;
            std::string machine_id;
        };
    }

}

#endif //NAR_DBEXTRASTRUCTS_H
