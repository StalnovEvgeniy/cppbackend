#include "path_utils.h"
#include "response_utuls.h"

namespace path_utils {

using namespace response_utuls;

// Возвращает true, если каталог p содержится внутри base_path.
bool IsSubPath(fs::path path, fs::path base) {
    // Приводим оба пути к каноничному виду (без . и ..)
    path = fs::weakly_canonical(path);
    base = fs::weakly_canonical(base);

    // Проверяем, что все компоненты base содержатся внутри path
    for (auto b = base.begin(), p = path.begin(); b != base.end(); ++b, ++p) {
        if (p == path.end() || *p != *b) {
            return false;
        }
    }
    return true;
}

fs::path GetPath(fs::path  requestPath, fs::path rootPath)
{
    using namespace std::literals;
    if (requestPath.empty() /*|| requestPath.c_str() == "/"sv*/) {
        requestPath.append(INDEX_HTML);
        //requestPath.append(SLESH);
    }
    if (requestPath.c_str() == SLESH) {
       requestPath.append(INDEX_HTML);
    }


    //auto filePath = fs::weakly_canonical(wwwRootPath_ / requestPath);

    return fs::weakly_canonical(rootPath / requestPath);
}


}
