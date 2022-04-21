//
// Created by Ciaran on 05/11/2021.
//

#ifndef ROADRUNNER_SBMLMODELOBJECTCACHE_H
#define ROADRUNNER_SBMLMODELOBJECTCACHE_H

#define NOMINMAX

#include <memory>
#ifdef _MSC_VER
#pragma warning(disable: 4146)
#pragma warning(disable: 4141)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#endif

#include "llvm/ADT/StringMap.h"
#include "llvm/ExecutionEngine/ObjectCache.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#ifdef _MSC_VER
#pragma warning(default: 4146)
#pragma warning(default: 4141)
#pragma warning(default: 4267)
#pragma warning(default: 4624)
#endif


namespace rrllvm {

    /**
     * @brief Stores SBML model as object files for later use.
     * @see llvm example called LLJitWithObjectCache.cpp
     * @details This object is a singleton. It cannot be copied, moved or assigned
     * and all interactions with this object must occur through the static call
     * SBMLModelObjectCache::getObjectCache();
     */
    class SBMLModelObjectCache : public llvm::ObjectCache {
    public:

        static SBMLModelObjectCache& getObjectCache();

        SBMLModelObjectCache(const SBMLModelObjectCache& objectCache) = delete;

        SBMLModelObjectCache& operator=(const SBMLModelObjectCache&) = delete;

        SBMLModelObjectCache(SBMLModelObjectCache&& objectCache) noexcept = delete;

        SBMLModelObjectCache& operator=(SBMLModelObjectCache&&) noexcept= delete;

        void notifyObjectCompiled(const llvm::Module *M, llvm::MemoryBufferRef ObjBuffer) override;

        std::unique_ptr<llvm::MemoryBuffer> getObject(const llvm::Module *M) override;

        std::vector<std::string> inspect();

        void addToCache(const std::string & key, std::unique_ptr<llvm::MemoryBuffer> mb);

    private:
        SBMLModelObjectCache()= default;

        llvm::StringMap<std::unique_ptr<llvm::MemoryBuffer>> cachedObjects;

    };

    /**
     * @brief Instantiate a global instance of object cache.
     * @details This is experimental and may possible be better than
     * the one historically home rolled with roadrunner since
     * its actually an llvm construct. Notably, the compile layer
     * looks in the object cache prior to compiling. It also means that
     * we can access the object for load/save state. This is in contrast
     * to the old system whereby the IR is compiled to object file,
     * and manually stored as string before compiling the object file into
     * machine code.
     */
//    static ObjectCache objectCache;

}

#endif //ROADRUNNER_SBMLMODELOBJECTCACHE_H
