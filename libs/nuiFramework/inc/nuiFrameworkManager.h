/** 
 * \file      nuiFrameworkManager.h
 * \author    Anatoly Lushnikov
 * \author    Scott Halstvedt
 * \author    Anatoly Churikov
 * \date      2012-2013
 * \copyright Copyright 2012 NUI Group. All rights reserved.
 */

#ifndef NUI_FRAMEWORK_MANAGER_H
#define NUI_FRAMEWORK_MANAGER_H
#define _WINSOCKAPI_

#include <string>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>

#include "nuiProperty.h"
#include "pasync.h"
#include "nuiTimer.h"
#include "nuiThread.h"
#include "nuiEndpoint.h"
#include "nuiTree.h"
#include "nuiDebugLogger.h"
#include "nuiJsonRpcApi.h"
#include "json\json.h"

class nuiModule;
class nuiModuleDescriptor;
class nuiPipelineModule;

typedef enum nuiFrameworkManagerErrorCode
{
    NUI_FRAMEWORK_MANAGER_OK,
    NUI_FRAMEWORK_PIPELINE_STRUCTURE_LOOP,
    NUI_FRAMEWORK_ERROR_SAVING_FILE,
    NUI_FRAMEWORK_ROOT_ACCESS_DENIED,
    NUI_FRAMEWORK_OBJECT_NOT_EXIST,
    NUI_FRAMEWORK_ROOT_INITIALIZATION_FAILED,
} nuiFrameworkManagerErrorCode;

struct nuiModuleRuntimeDescriptor
{
public:
    double averageFPS;
    double averageWaitTime;
    double averageProcessTime;
    bool isRunning;
};

struct nuiDataStreamRuntimeDescriptor
{
public:
    bool isRunning;
};

/** \class nuiFrameworkManager
 *  Provides API for workflow manipulations
 */
class nuiFrameworkManager
{
public:
    ~nuiFrameworkManager();
    static nuiFrameworkManager *getInstance();

    nuiFrameworkManagerErrorCode initializeFrameworkManager();
    std::vector<std::string> *listDynamicModules();
    std::vector<std::string> *listPipelines(std::string &hostedPipelineName);

    //! starts the current pipeline workflow
    nuiFrameworkManagerErrorCode workflowStart();

    //! starts module index workflow at the current pipeline
    nuiFrameworkManagerErrorCode workflowStart(int moduleIndex);

    //! stops the current pipeline workflow
    nuiFrameworkManagerErrorCode workflowStop();

    //! starts module index workflow at the current pipeline
    nuiFrameworkManagerErrorCode workflowStop(int moduleIndex);
    nuiFrameworkManagerErrorCode workflowQuit();

    nuiModuleDescriptor *createPipeline(std::string &pipelineName);
    nuiModuleDescriptor *createModule(std::string &pipelineName,std::string &moduleName);
    nuiDataStreamDescriptor *createConnection(std::string &pipelineName, int sourceModuleID, int destinationModuleID,int sourcePort, int destinationPort);
    int setInputEndpointCount(std::string &pipelineName, int count);
    int setOutputEndpointCount(std::string &pipelineName, int count);

    nuiModuleDescriptor *getCurrentPipeline();
    nuiModuleDescriptor *getWorkflowRoot();
    nuiModuleDescriptor *getPipeline(std::string &pipelineName);
    nuiModuleDescriptor *getModule(std::string &pipelineName, int index);
    nuiModuleDescriptor *getModule(std::string &moduleName);
    nuiEndpointDescriptor *getInputEndpoint(std::string &pipelineName, int index);
    nuiEndpointDescriptor *getOutputEndpoint(std::string &pipelineName, int index);
    nuiDataStreamDescriptor *getConnection(std::string &pipelineName, int sourceModuleID, int destinationModuleID,int sourcePort, int destinationPort);

    nuiFrameworkManagerErrorCode deletePipeline(std::string &pipelineName);
    nuiModuleDescriptor *deleteModule(std::string &pipelineName,int moduleIndex);
    nuiModuleDescriptor *deleteInputEndpoint(std::string &pipelineName,int index);
    nuiModuleDescriptor *deleteOutputEndpoint(std::string &pipelineName,int index);
    nuiModuleDescriptor *deleteConnection(std::string &pipelineName, int sourceModuleID, int destinationModuleID, int sourcePort, int destinationPort);

    nuiModuleDescriptor *updatePipeline(std::string &pipelineName, nuiModuleDescriptor* moduleDescriptor);
    nuiModuleDescriptor *updateModule(std::string &pipelineName, int index, nuiModuleDescriptor* moduleDescriptor);
    nuiEndpointDescriptor *updateInputEndpoint(std::string &pipelineName,int index, nuiEndpointDescriptor* endpointDescriptor);
    nuiEndpointDescriptor *updateOutputEndpoint(std::string &pipelineName,int index, nuiEndpointDescriptor* endpointDescriptor);
    nuiDataStreamDescriptor *updateConnection(std::string &pipelineName, int sourceModuleID, int destinationModuleID,int sourcePort, int destinationPort, nuiDataStreamDescriptor *connectionDescriptor);

    /** dives into moduleIndex pipeline of the currect pipeline
     *  \returns new current pipeline descriptor, NULL if action failed
     */
    nuiModuleDescriptor *navigatePush(int moduleIndex);

    /** returns back to upper pipeline
     *  \return new current pipeline descriptor, NULL if action failed
     */
    nuiModuleDescriptor *navigatePop( );

    nuiFrameworkManagerErrorCode loadSettingsFromJson(const char *fileName);
    nuiFrameworkManagerErrorCode loadSettingsFromJson(Json::Value *root);

private:
    nuiFrameworkManager();

    //! gets currently selected pipeline
    nuiPipelineModule *getCurrent();
    //! list of child indexes to the current pipeline
    std::list<int> pathToCurrent;

    nuiModule* currentModule;
    nuiPipelineModule *rootPipeline;
    nuiTree<int,nuiModule*> *dataObjectTree;

    nuiFrameworkManagerErrorCode saveSettingsToJson(const char *fileName, std::string &pipelineName);
    nuiFrameworkManagerErrorCode saveSettingsToJson(const char *fileName, std::list<nuiModuleDescriptor*>* descriptors);
    nuiFrameworkManagerErrorCode saveSettingsToJson(Json::Value *root, std::list<nuiModuleDescriptor*>* descriptors);
    nuiFrameworkManagerErrorCode saveSettingsToJson(Json::Value *root, std::string &pipelineName);
    nuiModuleDescriptor *parseModuleDescriptor(Json::Value *root);
    void parseModuleDescriptorParameters(nuiModuleDescriptor &moduleDescriptor, Json::Value *root);
};

#endif//NUI_FRAMEWORK_MANAGER_H