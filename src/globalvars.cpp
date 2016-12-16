#include "globalvars.h"

#include <fstream>
#include <string>

#undef VAR
#define VAR(type, name, default) type name = default;
GLOBAL_VARS

static const std::string getOptionsFilename(const char *dir) {
    std::string path;
    if (dir) {
        path = dir;
        path = path.substr(0, 1 + path.find_last_of("\\/"));
    }
    path += FILENAME;
    return path;
}

void readOptions(const char *path) {
    std::ifstream ifs(getOptionsFilename(path));
    std::string str;

    std::getline(ifs, str);
    if (str != FIRST_LINE) return;
    std::getline(ifs, str);

    while(ifs.good() && ! ifs.eof()) {
        ifs >> str;
#undef VAR
#define VAR(type,name,default) if (str == #name) {ifs >> name; continue;}
        GLOBAL_VARS
    }

    ifs.close();
}

void saveOptions(const char *path) {
    std::ofstream ofs(getOptionsFilename(path), std::ofstream::trunc|std::ofstream::out);

    ofs << FIRST_LINE << "\n\n";
#undef VAR
#define VAR(type,name,default) ofs << #name << ' ' << name << '\n';
    GLOBAL_VARS

    ofs.close();
}

