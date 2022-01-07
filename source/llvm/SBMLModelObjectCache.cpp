//
// Created by Ciaran on 05/11/2021.
//

#include "SBMLModelObjectCache.h"
#include "rrLogger.h"
#include <iostream>

using namespace rr;

namespace rrllvm {

    static std::mutex mtx;

    SBMLModelObjectCache &SBMLModelObjectCache::getObjectCache() {
        static SBMLModelObjectCache objectCacheInstance;
        return objectCacheInstance;
    }

    void SBMLModelObjectCache::notifyObjectCompiled(const llvm::Module *M, llvm::MemoryBufferRef ObjBuffer) {
        rrLogDebug << "module: " << M->getModuleIdentifier() << " is compiled";
        std::lock_guard<std::mutex> lock(mtx);
        cachedObjects[M->getModuleIdentifier()] = llvm::MemoryBuffer::getMemBufferCopy(
                ObjBuffer.getBuffer(), ObjBuffer.getBufferIdentifier());
    }

    std::unique_ptr<llvm::MemoryBuffer> SBMLModelObjectCache::getObject(const llvm::Module *M) {
        auto I = cachedObjects.find(M->getModuleIdentifier());
        if (I == cachedObjects.end()) {
            rrLogDebug << "No object for " << M->getModuleIdentifier() << " in cache. Compiling";
            return nullptr;
        }

        rrLogDebug << "Object for " << M->getModuleIdentifier() << " loaded from cache.\n";
        return llvm::MemoryBuffer::getMemBuffer(I->second->getMemBufferRef());
    }

    std::vector<std::string> SBMLModelObjectCache::inspect() {
        std::vector<std::string> v;
        rrLogDebug << "Number of cached models is: " << cachedObjects.size();
        std::lock_guard<std::mutex> lock(mtx);
        for (auto &x: cachedObjects) {
            rrLogDebug << "Found cached model called: " << x.first().str();
            v.push_back(x.first().str());
        }
        return v;
    }

    void SBMLModelObjectCache::addToCache(const std::string &key, std::unique_ptr<llvm::MemoryBuffer> mb) {
        std::lock_guard<std::mutex> lock(mtx);
        cachedObjects.insert({key, std::move(mb)});
    }


}