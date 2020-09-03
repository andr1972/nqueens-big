#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

namespace elap {
    struct Param {
        char shortName;
        std::string name;
        int minReq;
        int maxReq;
        std::vector<std::string> options;
        int count=0;
        bool isCommand;
        Param(bool isCommand, char shortName, std::string name, int minReq, int maxReq):
                isCommand(isCommand), shortName(shortName),name(std::move(name)),minReq(minReq),maxReq(maxReq) {}
    };

    class ArgParser {
        std::vector<Param> params;
        int parseError = 0;
        Param* errorParam = nullptr;

        std::vector<Param*> getCommandsOrParams(bool isCommand){
            std::vector<Param*> result;
            for (auto &param :params)
                if (param.count>0 && param.isCommand==isCommand)
                    result.push_back(&param);
            return result;
        }

        void addCommandOrParam(bool isCommand, char shortName, std::string name, int minReq, bool maxreq) {
            if (name.empty()) name = shortName;
            params.emplace_back(isCommand, shortName, name, minReq, maxreq);
        }
    public:
        Param* getParam(std::string name) { //@todo ma porownywać isCommand
            for (auto &param :params)
                if (param.name==name) {
                    return &param;
                }
            return nullptr;
        }
        Param* getParam(char shortName) {//@todo tylko isCommand==false
            for (auto &param :params)
                if (param.shortName==shortName) {
                    return &param;
                }
            return nullptr;
        }

        void addCommand(std::string name, int minReq, bool maxreq) {
            addCommandOrParam(true, 0, name, minReq, maxreq);
        }

        void addParam(char shortName, std::string name, int minReq, bool maxreq) {
            addCommandOrParam(false, shortName, name, minReq, maxreq);
        }

        Param* getCommand() {
            for (auto &param :params)
                if (param.count>0 && param.isCommand) {
                    return &param;
                }
            return nullptr;
        }

        std::vector<Param*> getParams() {
            return getCommandsOrParams(false);
        }

        std::vector<Param*> getCommands() {
            return getCommandsOrParams(true);
        }

        bool parse(int argc, char *argv[]) {
            //argv[0] is omited as binary name
            Param* lastParam = nullptr;
            for (int i=1; i<argc; i++) {
                std::string arg = argv[i];
                if (arg.size()>1 && arg[0]=='-') {
                    if (lastParam && lastParam->minReq>lastParam->options.size()) {
                        parseError = 2;
                        errorParam = lastParam;
                        return false;
                    }
                    lastParam = nullptr;
                    if (arg.size()>2 && arg[1]=='-')
                    {
                        std::string name = arg.substr(2);
                        Param* param = getParam(name);
                        lastParam = param;
                        if (param) param->count++;
                    }
                    else {
                        for (int j=1; j<arg.size(); j++) {
                            Param* param = getParam(arg[j]);
                            if (param->minReq>0 && arg.size()>2) {
                                parseError = 1;
                                errorParam = param;
                                return false;
                            }
                            lastParam = param;
                            if (param) param->count++;
                        }
                    }
                }
                else {
                    if (lastParam) {
                        if (lastParam->maxReq>0) {
                            if (lastParam->options.size()<lastParam->maxReq)
                                lastParam->options.push_back(arg);
                            else {
                                lastParam = nullptr;
                                lastParam = getParam(arg);
                                lastParam->count++;
                            }
                        } else {
                            lastParam = nullptr;
                            lastParam = getParam(arg);
                            lastParam->count++;
                        }
                    } else {
                        lastParam = getParam(arg);
                        lastParam->count++;
                    }
                }
            }
            if (lastParam && lastParam->minReq>lastParam->options.size()) {
                parseError = 2;
                errorParam = lastParam;
                return false;
            }
            return true;
        }

        std::string getError() {
            char buf[128];
            buf[0] = 0;
            if (parseError==1) {
                sprintf(buf, "Parameter -%c (--%s) can't be merged with others because has required minimal %d option(s)",
                        errorParam->shortName, errorParam->name.c_str(), errorParam->minReq);
            } else if (parseError==2) {
                sprintf(buf, "Parameter %s has minimal required %d option(s) but is only %ld option(s)",
                        errorParam->name.c_str(), errorParam->minReq, errorParam->options.size());
            }
            std::string res = buf;
            return res;
        }
    };
}