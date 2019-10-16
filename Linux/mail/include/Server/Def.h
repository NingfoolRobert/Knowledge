//
// Created by aurora on 6/18/19.
//
#pragma once
#ifndef MDS_DEF_H
#define MDS_DEF_H

#include <string>
#include <vector>
#include <unordered_map>


#define MDS_S_CONFIG_FILE "./mdss.xml"
#define DEF_SEQ_METHOD_TICKER "ByTicker"
#define DEF_SEQ_METHOD_UNIT "Unit"



#define DEF_DEFAULT_UP_STREAM_URL "defaultUpstreamUrl"
#define DEF_SOURCE_ID "sourceId"
#define DEF_URL "uri"
#define DEF_FILTER "filter"
#define DEF_GROUP "groupId"
#define DEF_GRP_NAME "groupName"
#define DEF_HANDLE "handle"
#define DEF_SEQ_NUM_METHOD "seqNumMethod"
#define DEF_UP_STREAM_URL "upstreamUrl"
#define MAX_GROUP_LEN 2

#define SERVICE_MODE_QUOTE 0x00
#define SERVICE_MODE_MSG 0x01


struct GrpItem
{
    std::string sourceId;
    std::string uri;
    std::string filter;
};

struct GrpConfig
{
    std::vector<GrpItem> items;
    std::string group;
    std::string grpName;
    std::string handle;
    std::string type;

    //For Msg ReSend
    std::string seqNumMethod;//"ByTicker"||"Unit"
    std::string upstreamUrl;
    /*Others*/
};
typedef std::unordered_map<std::string,GrpConfig*> GrpConfigMap;


struct RouterDealerConfig
{
    std::string bindUrl;//"tcp://*:8801"
    int dealerNum;
};

struct ResolveConfig
{
    std::string url;
    int waitTimes;
    int waitInterval;
};

struct MDS_S_Config
{
    int logLevel;
    int serviceMode;
    RouterDealerConfig routerDealerConfig;
    GrpConfigMap grpConfigs;
    ResolveConfig resolveConfig;
};

struct MDS_R_Publish{
    int port;
};

struct MDS_R_Monitor{
    int id;
    int port;
};

struct MDS_R_Sub_Source{
    std::string id;
    std::string uri;
    std::string filter;
};





#endif //MDS_DEF_H
