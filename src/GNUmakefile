
CXX=g++
#CXXFLAGS=-c -Wall -std=c++17 -O3 -fPIC -pthread -pipe
CXXFLAGS=-c -Wall -std=c++17 -ggdb -pipe
file_ouput=-o 
RM=rm -f

LIB_DIR=-L../cryptopp-CRYPTOPP_7_0_0/
LIB=-lcryptopp

libcrypt=../cryptopp-CRYPTOPP_7_0_0/libcryptopp.a
libhttp_parser=../http-parser/libhttp_parser.o

CLIENT_OBJ=$(libcrypt) $(libhttp_parser) client_main.o password_hash.o http_client.o \
	network_client_posix.o http_parser_wrapper.o file_client.o AES_CCM_encryption.o
SERVER_OBJ=$(libcrypt) $(libhttp_parser) server_main.o password_hash.o http_server.o \
	network_server.o epoll_wrapper.o http_parser_wrapper.o AES_CCM_encryption.o

.PHONY: server
server: server.out

.PHONY: client
client: client.out

client.out: $(CLIENT_OBJ)
	$(CXX) $^ $(file_ouput)$@ $(LIB_DIR) $(LIB)

server.out: $(SERVER_OBJ)
	$(CXX) $^ $(file_ouput)$@ $(LIB_DIR) $(LIB)

client_main.o: client_main.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

server_main.o: server_main.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

epoll_wrapper.o: epoll_wrapper.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

http_server.o: http_server.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

http_parser_wrapper.o: http_parser_wrapper.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

password_hash.o: password_hash.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

network_server.o: network_server.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

network_client_posix.o: network_client_posix.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

http_client.o: http_client.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

file_client.o: file_client.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@

AES_CCM_encryption.o: AES_CCM_encryption.cpp
	$(CXX) $(CXXFLAGS) $^ $(file_ouput)$@


$(libhttp_parser):
	cd ../http-parser
	make library


clean:
	$(RM) main.o
	$(RM) password_hash.o
	$(RM) network_client.o
	$(RM) network_client_posix.o
	$(RM) http_client.o
	$(RM) *.o
	$(RM) client.out
	$(RM) server.out

