#ifndef NAR_FILECRYPTOR_H
#define NAR_FILECRYPTOR_H

#include <sys/types.h>

namespace nar {
    class FileCryptor {
        private:
            FileKeeper file;
        public:
            FileCryptor(FileKeeper& keep) : file(keep) {}
            int getBytes(size_t start, size_t buffersize, char* buffer);
    }
}

#endif
