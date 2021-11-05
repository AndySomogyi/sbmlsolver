//
// Created by Ciaran on 05/11/2021.
//

#include "SBMLModelObjectCache.h"
#include "rrLogger.h"
#include <iostream>

using namespace rr;

namespace rrllvm {

    SBMLModelObjectCache &SBMLModelObjectCache::getObjectCache() {
        static SBMLModelObjectCache objectCacheInstance;
        return objectCacheInstance;
    }

    void SBMLModelObjectCache::notifyObjectCompiled(const llvm::Module *M, llvm::MemoryBufferRef ObjBuffer) {
        // TODO module identifier will be the same for different roadrunner objects.
        // we either need to change this to the md5 string, like the original or
        // change the identifier to the md5.
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

    std::vector<std::string> SBMLModelObjectCache::inspect(){
        std::vector<std::string> v;
        rrLogDebug << "Number of cached models is: " << cachedObjects.size();
        for (auto& x : cachedObjects){
            rrLogDebug << "Found cache model called: " << x.first().str();
            v.push_back(x.first().str());
        }
        return v;
    }



}