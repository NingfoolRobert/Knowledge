//
// Created by aurora on 6/18/19.
//

#include "MDSRConfigHelper.h"
#include "tinyxml2.h"

using namespace tinyxml2;

MDSRConfigHelper* MDSRConfigHelper::instance = nullptr;

void MDSRConfigHelper::Load() {

    XMLDocument doc;
    doc.LoadFile(CONFIG_FILE_NAME.c_str());

    XMLElement *root = doc.FirstChildElement("root");
    str_name = root->FirstChildElement("appname")->GetText();

    XMLElement *groups = root->FirstChildElement("groups");
    XMLElement *group = groups->FirstChildElement("group");

    while(group){
        GrpConfig *grpConfig = new GrpConfig();
        XMLElement *child=group->FirstChildElement();
        while(child){
            string key = child->Name();
            if(key == "name"){
                grpConfig->grpName = child->GetText();
            }
            if(key == "handle"){
                grpConfig->handle = child->GetText();
            }
            if(key == "type"){
                grpConfig->type = child->GetText();
            }
            if(key == "source"){
                XMLElement *grandchild = child->FirstChildElement();
                GrpItem item;
                while(grandchild){
                    string key = grandchild->Name();
                    if(key == "id"){
                        item.sourceId = grandchild->GetText();
                    }
                    if(key == "uri"){
                        item.uri = grandchild->GetText();
                    }
                    if(key == "filter"){
                        item.filter = grandchild->GetText();
                    }
                    grandchild = grandchild->NextSiblingElement();
                }
                grpConfig->items.push_back(item);
            }
            child = child->NextSiblingElement();
        }
        grpConfigMap->insert(make_pair(grpConfig->grpName, grpConfig));
        group = group->NextSiblingElement();
    }

    XMLElement *pub = root->FirstChildElement("publish");
    publish.port = stoi(pub->FirstChildElement("port")->GetText());

    XMLElement *mon = root->FirstChildElement("monitor");
    monitor.port = stoi(mon->FirstChildElement("port")->GetText());
    monitor.id = stoi(mon->FirstChildElement("id")->GetText());

    XMLElement *log = root->FirstChildElement("log");
    i_logLevel = stoi(log->FirstChildElement("level")->GetText());


    b_isLoad = true;
}

void MDSRConfigHelper::ShowConfig() {
    for(auto itr = grpConfigMap->begin(); itr!= grpConfigMap->end(); itr++){
        printf("name: %s\n", itr->second->grpName.c_str());
        printf("handle: %s\n", itr->second->handle.c_str());
        printf("type: %s\n", itr->second->type.c_str());
        for(auto v = itr->second->items.begin(); v!= itr->second->items.end(); v++){
            printf("source\n");
            printf("--id:%s\n", v->sourceId.c_str());
            printf("--uri:%s\n", v->uri.c_str());
            printf("--filter:%s\n", v->filter.c_str());
        }
    }

    printf("publish port : %d \n", publish.port);
    printf("monitor id : %d \n", monitor.id);
    printf("monitor port : %d \n", monitor.port);
    printf("log level : %d \n", i_logLevel);

}

GrpConfigMap *MDSRConfigHelper::GetGrpConfigMap() {
    return grpConfigMap;
}

int MDSRConfigHelper::GetPublishPort() {
    return publish.port;
}

int MDSRConfigHelper::GetMonitorPort() {
    return monitor.port;
}

int MDSRConfigHelper::GetMonitorId() {
    return monitor.id;
}

int MDSRConfigHelper::GetLogLevel() {
    return i_logLevel;
}

bool MDSRConfigHelper::CheckValid() {
    //TODO
    return true;
}

string MDSRConfigHelper::GetPublishAddress() {
    return "tcp://*:" + to_string(GetPublishPort());
}

string MDSRConfigHelper::GetMonitorAddress() {
    return "tcp://*:" + to_string(GetMonitorPort());
}
