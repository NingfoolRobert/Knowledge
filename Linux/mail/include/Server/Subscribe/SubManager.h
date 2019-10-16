//
// Created by aurora on 6/18/19.
//

#ifndef MDS_SUBMANAGER_H
#define MDS_SUBMANAGER_H

#include <map>
#include "SubWorker.h"

using namespace std;

class SubManager {

public:
    SubManager();
    ~SubManager();

    void LoadConfig();

    bool IsLoadConfig();

    void StartSubscribe();

    bool IsStartSubscribe();

    void CountSubscribe();

    bool CheckWorkerExist(string);

    int GetWorkerSize();

private:

    bool b_isLoadConfig;
    bool b_isStartSub;

    std::map<string, SubWorker*> map_Worker;
};


#endif //MDS_SUBMANAGER_H
