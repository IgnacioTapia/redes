/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "YASL.h"      // For Socket, ServerSocket, and SocketException
#include "json.hpp"
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include <fstream>

using namespace nlohmann;

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {
	std::cout << "Handling client ";
	try {
		std::cout << sock->getForeignAddress() << ":";
	} catch (SocketException e) {
		std::cerr << "Unable to get foreign address" << std::endl;
	}
	try {
		std::cout << sock->getForeignPort();
	} catch (SocketException e) {
		std::cerr << "Unable to get foreign port" << std::endl;
	}
	std::cout << std::endl;
	//hacer un menu, donde se pueda verificar cada pagina, es decir que sea posibe identificar que 
	//se esta solicitando.

	char eBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
	uint32_t bReceived = 0;              // Bytes read on each recv()
	std::string request=" ";
	
	bReceived = sock->recv(eBuffer, RCVBUFSIZE); //leyendo el request
		if(bReceived < 0){
			std::cerr << "no hay que leer" <<  std::endl;
		}
		else{
			eBuffer[bReceived] = '\0'; 	//termino de linea
			request = request + eBuffer + "\n"; //se junta la linea
		}

	
	std:: string pag;
	pag =request.substr(3,request.substr(3).find(" "));//se encuentra el dato que nos estan pidiendo
	
	std::ifstream archivo;
	std::string response;

	if(pag=="/pagina.html"){
		root_dir = root_dir + pag;
		archivo.open(root_dir);
		response=("HTTP/1.1 200 OK\r\n 20 Content-Type: text/html\r\n\r\n");
	}
	else if(pag =="/pagina2.html"){
		archivo.open(root_dir);
		response=("HTTP/1.1 200 OK\r\n 20 Content-Type: text/html\r\n\r\n");
	}
	else if(pag=="/pagina3.html"){
		archivo.open(root_dir);
		response=("HTTP/1.1 200 OK\r\n 20 Content-Type: text/html\r\n\r\n");
	}
	else{
		archivo.open(error_File);
		response=("HTTP/1.1 error 404 OK\r\n 20 Content-Type: text/html\r\n\r\n");
	}
	sock-> send(response.c_str(), response.length());
	//se lee archivo html
	std::string line;
	std::string body = " ";
	if (archivo.is_open()){
		while(getline(archivo,line)){
			body = body + line + "\n";
		}
		archivo.close();
	}else{
		std::cout << "No se pudo abrir"; 
	}
	//se envía el html
	sock->send(body.c_str(), body.length());
	delete sock;
}

int main(int argc, char *argv[]) {
	
	checkArgs* argumentos = new checkArgs(argc, argv);
	
	uint16_t echoServPort;   
	echoServPort  = argumentos->getArgs().PORT;

	try {
		TCPServerSocket servSock(echoServPort);     // Server Socket object
		for (;;) {   // Run forever
			HandleTCPClient(servSock.accept());       // Wait for a client to connect
		}
	} catch (SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	// NOT REACHED

	return EXIT_SUCCESS;
}
