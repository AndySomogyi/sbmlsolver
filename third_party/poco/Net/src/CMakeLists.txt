CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(poco_net_src)
set(target poco_net) 

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(POCO_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(POCO_BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(sources 
	DNS HTTPResponse HostEntry Socket 
	DatagramSocket HTTPServer IPAddress SocketAddress 
	HTTPBasicCredentials HTTPCookie HTMLForm MediaType DialogSocket 
	DatagramSocketImpl FilePartSource HTTPServerConnection MessageHeader 
	HTTPChunkedStream HTTPServerConnectionFactory MulticastSocket SocketStream 
	HTTPClientSession HTTPServerParams MultipartReader StreamSocket SocketImpl 
	HTTPFixedLengthStream HTTPServerRequest HTTPServerRequestImpl MultipartWriter StreamSocketImpl 
	HTTPHeaderStream HTTPServerResponse HTTPServerResponseImpl NameValueCollection TCPServer 
	HTTPMessage HTTPServerSession NetException TCPServerConnection HTTPBufferAllocator 
	HTTPAuthenticationParams HTTPCredentials HTTPDigestCredentials 
	HTTPRequest HTTPSession HTTPSessionInstantiator HTTPSessionFactory NetworkInterface  
	HTTPRequestHandler HTTPStream HTTPIOStream ServerSocket TCPServerDispatcher TCPServerConnectionFactory 
	HTTPRequestHandlerFactory HTTPStreamFactory ServerSocketImpl TCPServerParams 
	QuotedPrintableEncoder QuotedPrintableDecoder StringPartSource 
	FTPClientSession FTPStreamFactory PartHandler PartSource NullPartHandler 
	SocketReactor SocketNotifier SocketNotification AbstractHTTPRequestHandler 
	MailRecipient MailMessage MailStream SMTPClientSession POP3ClientSession 
	RawSocket RawSocketImpl ICMPClient ICMPEventArgs ICMPPacket ICMPPacketImpl 
	ICMPSocket ICMPSocketImpl ICMPv4PacketImpl 
	RemoteSyslogChannel RemoteSyslogListener 
	WebSocket WebSocketImpl
)


#if(CMAKE_SYSTEM MATCHES "Windows")
# 
#endif(CMAKE_SYSTEM MATCHES "Windows")
#
#if (CMAKE_SYSTEM MATCHES "Linux")
#  add_definitions( -DPOCO_OS_FAMILY_UNIX )
#  # Standard 'must be' defines
#  add_definitions( -D_XOPEN_SOURCE=500 -D_REENTRANT -D_THREAD_SAFE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64)
#  set(SYSLIBS  pthread dl rt)
#endif(CMAKE_SYSTEM MATCHES "Linux")

include_directories(
../../Foundation/include
../../Net/include
)

#Then the static version
add_library(${target}-static STATIC ${sources})

#set_property(TARGET ${target}-static  
#PROPERTY  COMPILE_DEFINITIONS 
#POCO_STATIC
#PCRE_STATIC
#) 
#
#if(WIN32)
#set_property(TARGET ${target}-static  
#PROPERTY COMPILE_DEFINITIONS 
#WIN32
#) 
#
#endif()
#
#INSTALLING ======================
set(poco_net_include_folder ${CMAKE_CURRENT_SOURCE_FOLDER}../include/Poco/Net)

FILE (GLOB net_headers ${poco_net_include_folder}/*.h)

#Install required files
install(FILES ${net_headers} DESTINATION include/Poco/Net)


install(TARGETS ${target}-static 
LIBRARY DESTINATION lib 
ARCHIVE DESTINATION lib
)

