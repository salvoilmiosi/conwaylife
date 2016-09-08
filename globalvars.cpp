#include "globalvars.h"

#include <fstream>
#include <string>

#undef VAR
#define VAR(type, name, default) type name = default;
GLOBAL_VARS

void readOptions() {
    std::ifstream ifs(FILENAME);
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

void saveOptions() {
    std::ofstream ofs(FILENAME, std::ofstream::trunc|std::ofstream::out);

    ofs << FIRST_LINE << "\n\n";
#undef VAR
#define VAR(type,name,default) ofs << #name << ' ' << name << '\n';
    GLOBAL_VARS

    ofs.close();
}

