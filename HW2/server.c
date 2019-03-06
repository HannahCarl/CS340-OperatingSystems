
// -----------------------------------
// CSCI 340 - Operating Systems
// Spring 2019
// server.h header file
// Homework 2
//
// Hannah Carl
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;
     
    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {

	

	int exit_status = OK;

	int client_socket_fd = -1;

	socklen_t client_length; 

	struct sockaddr_in client_address;

	char request[2048];

	client_length = sizeof( client_address );

	//Reponse, RequestChoice, and search char arrays
	char response[4096];
	char requestChoice[8];
	char search[] = "?";

    	client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );
		
	// -------------------------------------
	// TODO:
	// -------------------------------------
	// Modify code to fork a child process
	// -------------------------------------

	
	//Fork
	pid_t pid = fork();
	
	
	//Child Process 
	if(pid == 0){

	
		
	
		if ( client_socket_fd >= 0 ) {
		
			bzero( request, 2048 );
		
			read( client_socket_fd, request, 2047 );
			
			
			if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );
			
		
			// -------------------------------------
			// TODO:
			// -------------------------------------
			// Generate the correct http response when a GET or POST method is sent
			// from the client to the server.
			// 
			// In general, you will parse the request character array to:
			// 1) Determine if a GET or POST method was used
			// 2) Then retrieve the key/value pairs (see below)
			// -------------------------------------
		
			/*
			 ------------------------------------------------------
			 GET method key/values are located in the URL of the request message
			 ? - indicates the beginning of the key/value pairs in the URL
			 & - is used to separate multiple key/value pairs 
			 = - is used to separate the key and value
			 
			 Example:
			 
			 http://localhost/?first=tony&last=leclerc
			 
			 two &'s indicated two key/value pairs (first=tony and last=leclerc)
			 key = first, value = tony
			 key = last, value = leclerc
			 ------------------------------------------------------
			 */

			
			

			//Check request for '?' char
			char* ptr = strstr (request, search);
			
			//Set request choice
			strcpy(requestChoice, strtok(strdup(request), " "));
			
			//printf("Pointer %p\n", ptr);

			
			//Simple Post response
			if((strcmp(requestChoice, "GET") == 0) && (ptr == NULL)){

				//char* strAddr = (strtok(NULL, " "));
				//strcpy(address , strAddr);
				//char* address2 = (strtok(strdup(address), "/"));

				char entity_body_Simple[] = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Simple Post</title><link rel=\"shortcut icon\" href=\"http://www.example.com/my_empty_resource\"/></head><body><form method=\"POST\" action=\"http://127.0.0.1:8888\"><b>first name</b>&nbsp;</b><input type=\"text\" name=\"first\"><br><b>last name</b>&nbsp;</b><input type=\"text\" name=\"last\"><br><input type=\"submit\"></form></body></html>";

			sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body_Simple ), entity_body_Simple );

			}

			
			//Get response
			else if((strcmp(requestChoice, "GET") == 0) ){

				int counterChars = 0; //To count current location in char []

				//Build header and table for html GET
				char entity_body_Get[] = "<html><h2>CSCI 340 (Operating Systems) Homework 2 - Hannah Carl - [GET]</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";
				
				char* strValues = (strtok(NULL, " "));//Get string tokens
				
				char* tokenCharsGet = strtok((strValues += 2), "&=");//Remove '/?' from string tokens

				
				//Loop to parse through keys and values
				while(tokenCharsGet != NULL){
					
					//Values
					if(counterChars%2==0){
						strcat(entity_body_Get, "<tr><td>");
						strcat(entity_body_Get, tokenCharsGet);
						strcat(entity_body_Get, "</td>");
						tokenCharsGet = strtok(NULL, "&=");
						//printf("ifgetToks %s\n", tokenCharsGet);

					}//end if
					
					//Keys
					else{
						strcat(entity_body_Get, "<td>");
						strcat(entity_body_Get, tokenCharsGet);
						strcat(entity_body_Get, "</td>");
						tokenCharsGet = strtok(NULL, "&=");
						strcat(entity_body_Get, "</tr>");
						//printf("elsegetToks %s\n", tokenCharsGet);

					}

					counterChars++;
					//printf("tokenCharsGet %s\n", tokenCharsGet);


				}//end while
				
				strcat(entity_body_Get,"</table></html>");
				

				sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body_Get ), entity_body_Get );

				
			

			}//end if

			

		
			/*
			 ------------------------------------------------------
			 POST method key/value pairs are located in the entity body of the request message
			 ? - indicates the beginning of the key/value pairs
			 & - is used to delimit multiple key/value pairs 
			 = - is used to delimit key and value
			 
			 Example:
			 
			 first=tony&last=leclerc
			 
			 two &'s indicated two key/value pairs (first=tony and last=leclerc)
			 key = first, value = tony
			 key = last, value = leclerc
			 ------------------------------------------------------
			 */

			//POST 
			else if ((strcmp(requestChoice, "POST") == 0)){

				char copyTokenString[1024];

				int counterChars = 0; //To count current location in char []
				

				char entity_body_Post[] = "<html><h2>CSCI 340 (Operating Systems) Homework 2 - Hannah Carl - [POST]</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";

				//Get string tokens
				strcpy(copyTokenString, request);
				char* tokenCharsPost = (strtok(request, "\n"));
				char* copyToken = (strtok(copyTokenString, "\n"));
				

				
				//Loop to parse through request
				while(copyToken != NULL){
					strcpy(tokenCharsPost, copyToken );
					copyToken = strtok(NULL, "\n"); //Copy Token moves ahead 1 newline
					

				}//end while

				//Parse string of keys and values
				tokenCharsPost = strtok(tokenCharsPost,"&=");
				//printf("Token before second while %s\n", tokenCharsPost);


				//Loop to parse through keys and values
				while(tokenCharsPost != NULL){

					//Values
					if(counterChars%2==0){
						strcat(entity_body_Post, "<tr><td>");
						strcat(entity_body_Post, tokenCharsPost);
						strcat(entity_body_Post, "</td>");
						tokenCharsPost = strtok(NULL, "&=");

					}//end if

					//Keys
					else{
						strcat(entity_body_Post, "<td>");
						strcat(entity_body_Post, tokenCharsPost);
						strcat(entity_body_Post, "</td>");
						tokenCharsPost = strtok(NULL, "&=");
						strcat(entity_body_Post, "</tr>");

					}//end else

					counterChars++;


				}//end while
				
				strcat(entity_body_Post,"</table></html>");
				sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body_Post ), entity_body_Post );
			


				

		}//end else if
		
			
		
			
			
		
			if ( DEBUG ) printf( "%s\n", response );
		
	    		write( client_socket_fd, response, strlen( response ) );
		
			close( client_socket_fd );
		
		}
		//For neither POST or GET
		else {
		
			exit_status = FAIL;
		
		}
	
		if ( DEBUG ) printf("Exit status = %d\n", exit_status );
		exit(0);
	} //end of child process


	else{
		close( client_socket_fd );
	}
	
	//printf("after child process\n");

	return exit_status;
	
} // end accept_client function
