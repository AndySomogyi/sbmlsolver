CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(poco_foundation_src)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(POCO_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(POCO_BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(target poco_foundation) 

set(poco_include_folder ${CMAKE_CURRENT_SOURCE_FOLDER}../include)



set(sources 

ASCIIEncoding.cpp
Ascii.cpp
AtomicCounter.cpp
AbstractObserver.cpp
ActiveDispatcher.cpp
ArchiveStrategy.cpp
AsyncChannel.cpp
Base64Decoder.cpp
Base64Encoder.cpp
BinaryReader.cpp
BinaryWriter.cpp
Bugcheck.cpp
ByteOrder.cpp
Channel.cpp
Checksum.cpp
Condition.cpp
Configurable.cpp
ConsoleChannel.cpp
CountingStream.cpp
DateTime.cpp
DateTimeFormat.cpp
DateTimeFormatter.cpp
DateTimeParser.cpp
Debugger.cpp
DeflatingStream.cpp
DigestEngine.cpp
DigestStream.cpp
DirectoryIterator.cpp
Environment.cpp
ErrorHandler.cpp
Event.cpp
EventArgs.cpp
Exception.cpp
FPEnvironment.cpp
File.cpp
#FIFOBufferStream.cpp
FileChannel.cpp
FileStream.cpp
FileStreamFactory.cpp
Format.cpp
Formatter.cpp
FormattingChannel.cpp
Glob.cpp
Hash.cpp
HashStatistic.cpp
HexBinaryDecoder.cpp
HexBinaryEncoder.cpp
InflatingStream.cpp
Latin1Encoding.cpp
#Latin2Encoding.cpp
Latin9Encoding.cpp
LineEndingConverter.cpp
LocalDateTime.cpp
LogFile.cpp
LogStream.cpp
Logger.cpp
LoggingFactory.cpp
LoggingRegistry.cpp
MD4Engine.cpp
MD5Engine.cpp
Manifest.cpp
MemoryPool.cpp
MemoryStream.cpp
Message.cpp
Mutex.cpp
NamedEvent.cpp
NamedMutex.cpp
NestedDiagnosticContext.cpp
Notification.cpp
NotificationCenter.cpp
NotificationQueue.cpp
TimedNotificationQueue.cpp
PriorityNotificationQueue.cpp
NullChannel.cpp
NullStream.cpp
NumberFormatter.cpp
NumberParser.cpp
Path.cpp
PatternFormatter.cpp
Pipe.cpp
PipeImpl.cpp
PipeStream.cpp
Process.cpp
PurgeStrategy.cpp
RWLock.cpp
Random.cpp
RandomStream.cpp
RefCountedObject.cpp
RegularExpression.cpp
RotateStrategy.cpp
Runnable.cpp
SHA1Engine.cpp
Semaphore.cpp
SharedLibrary.cpp
SharedMemory.cpp
SignalHandler.cpp
SimpleFileChannel.cpp
SplitterChannel.cpp
Stopwatch.cpp
StreamChannel.cpp
StreamConverter.cpp
StreamCopier.cpp
StreamTokenizer.cpp
String.cpp
StringTokenizer.cpp
SynchronizedObject.cpp
Task.cpp
TaskManager.cpp
TaskNotification.cpp
TeeStream.cpp
TemporaryFile.cpp
TextConverter.cpp
TextEncoding.cpp
TextIterator.cpp
TextBufferIterator.cpp
Thread.cpp
ThreadTarget.cpp
ThreadLocal.cpp
ThreadPool.cpp
Timer.cpp
Timespan.cpp
Timestamp.cpp
Timezone.cpp
Token.cpp
URI.cpp
URIStreamFactory.cpp
URIStreamOpener.cpp
#UTF16Encoding.cpp
#UTF32Encoding.cpp
#UTF8Encoding.cpp
#UTF8String.cpp
#UUID.cpp
#UUIDGenerator.cpp
Unicode.cpp
UnicodeConverter.cpp
#Var.cpp
#VarHolder.cpp
#Void.cpp
#Windows1250Encoding.cpp
#Windows1251Encoding.cpp
#Windows1252Encoding.cpp
#adler32.c
#compress.c
#crc32.c
#deflate.c
#gzclose.c
#gzlib.c
#gzread.c
#gzwrite.c
#uncompr.c
#infback.c
#inffast.c
#inflate.c
#inftrees.c
#pcre_chartables.c
#pcre_compile.c
#pcre_exec.c
#pcre_fullinfo.c
#pcre_globals.c
#pcre_maketables.c
#pcre_newline.c
#pcre_ord2utf8.c
#pcre_study.c
#pcre_tables.c
#pcre_try_flipped.c
#pcre_ucd.c
#pcre_valid_utf8.c
#pcre_xclass.c
#pocomsg.mc
#trees.c
#zutil.c

#Thread
#Runnable
#Exception
#Mutex
#ErrorHandler
#Bugcheck
#ThreadLocal
#Debugger
#SharedLibrary
#Path
#UnicodeConverter
UTF8Encoding
UTF16Encoding
Windows1252Encoding
TextEncoding
#TextIterator
#Ascii
#RWLock
#AtomicCounter
#RWLock
#File
#TextConverter
#TimeStamp
#UnicodeConverter
#DirectoryIterator
Environment
#String
)

if(${BORLAND})
add_definitions(
             -w-8012                     #Comparing signed /unsigned
             -w-8057                     #Parameter never used
             -w-8004                     #'var' is assigned a value that is never used
             -w-8123                     #Path not found
             -w-8066                     #Unreachable code
             -w-8104                     #Local static with constructor...
             -w-8008                     #Condition is always false..
        )
endif()


if(${MSVC})
#set(....
endif()


include_directories(
${poco_include_folder}
)
#add_library(${target} SHARED ${sources})
#
#set_property(TARGET ${target}  
#             PROPERTY  COMPILE_DEFINITIONS 
#                       WIN32
#                       SHARED_NLEQ
#             ) 
#
#Then the static version
add_library(${target}-static STATIC ${sources})
set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
                       STATIC_POCO
                       PCRE_STATIC
                       WIN32
             ) 


#INSTALLING ======================
FILE (GLOB headers ${poco_include_folder}/Poco/*.h)

#Install required files
install(FILES ${headers} 
        DESTINATION include/poco
)

#install(TARGETS ${target} 
#        LIBRARY DESTINATION lib 
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin)
#
install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

