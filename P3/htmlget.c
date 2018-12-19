#include <stdio.h>
#include <stdlib.h>

#include <unistd.h> 		// close socket
#include <string.h>
#include <sys/time.h>		// datetime

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>



//* forward declarations prototypes
int 	create_tcp_socket();
char 	*get_ip( char *host );
char 	*build_get_query( char *host, char *page );
void 	usage();

#define HOST "coding.debuntu.org"
#define PAGE "/"
#define USERAGENT "HTMLGET 1.0"

int main( int argc, char **argv )
{
	struct 	sockaddr_in *remote;
	int 	sock;
	int 	tmpres;
	char 	*ip;
	char 	*get;
	char 	buf[BUFSIZ+1];
	char 	*host;
	char 	*page;
	int 	port;

	if( argc < 3 )
	{
		usage();
		exit(2);
	}

	host = argv[1];
	port = atoi(argv[2]);
	if( argc > 3 )
	{
		page = argv[3];
	}
	else
	{
		page = PAGE;
	}

	sock = create_tcp_socket();
	ip = get_ip( host );

	fprintf( stderr, "IP is %s:%d\n", ip, port ); 
	remote = ( struct sockaddr_in * ) malloc( sizeof( struct sockaddr_in * ) );
	remote->sin_family = AF_INET;
	tmpres = inet_pton( AF_INET, ip, (void *)( &(remote->sin_addr.s_addr) ) );

	if( tmpres < 0 )  
	{
		perror("Can't set remote->sin_addr.s_addr");
		exit(1);
	}
	else if( tmpres == 0 )
	{
		fprintf(stderr, "%s is not a valid IP address\n", ip);
		exit(1);
	}

	remote->sin_port = htons( port ); // host byte order to network byte order

	if( connect( sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0 ) 
	{
		perror("Could not connect");
		exit(1);
	}

	get = build_get_query( host, page );
	fprintf( stderr, "Query is:\n<<START>>\n%s<<END>>\n", get );

	//Send the query to the server
	int sent = 0;
	while(sent < strlen(get))
	{ 
		tmpres = send( sock, get+sent, strlen(get)-sent, 0 );
		if( tmpres == -1 ) 
		{
			perror("Can't send query");
			exit(1);
		}
		sent += tmpres;
	} // while

	//now it is time to receive the page
	memset( buf, 0, sizeof(buf) );
	int htmlstart = 0;
	char * htmlcontent;
	while( (tmpres = recv(sock, buf, BUFSIZ, 0)) > 0 )
	{
		if( htmlstart == 0 )
		{
			/* Under certain conditions this will not work.
				* If the \r\n\r\n part is splitted into two messages
				* it will fail to detect the beginning of HTML content
			*/
			htmlcontent = strstr(buf, "\r\n\r\n");
			if( htmlcontent != NULL )
			{
				htmlstart = 1;
				htmlcontent += 4;
			}
		}
		else
		{
			htmlcontent = buf;
		}

		if( htmlstart )
		{
			fprintf( stdout, "%s", htmlcontent );
		}
		memset( buf, 0, tmpres );
	} // while

	if( tmpres < 0 )
	{
		perror("Error receiving data");
	}

	free( get );
	free( remote );
	free( ip );
	close( sock );
	return 0;
}

void usage()
{
	fprintf(stderr, "USAGE: htmlget host port [page]\n\
		\thost: the website hostname. ex: coding.debuntu.org\n\
		\tpage: the page to retrieve. ex: index.html, default: /\n");
}


int create_tcp_socket()
{
	int sock;
	if( (sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
	{
		perror("Can't create TCP socket");
		exit(1);
	}
	return sock;
}


char *get_ip( char *host )
{
	struct hostent *hent;
	int iplen = 15; //XXX.XXX.XXX.XXX // '.' 

	char *ip = (char *) malloc( iplen + 1 );
	memset( ip, 0, iplen+1 );

	// gets hostent structure given a host name 
	if( (hent = gethostbyname(host)) == NULL )
	{
		herror("Can't get IP");
		exit(1);
	}

	// converts addresses to readable text (puts it into "ip" below).
	if( inet_ntop( AF_INET, (void *) hent->h_addr_list[0], ip, iplen) == NULL )
	{
		perror( "Can't resolve host");
		exit(1);
	}
	return ip;
}

char *build_get_query( char *host, char *page )
{
	char *query;
	char *getpage = page;
	char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	if( getpage[0] == '/' )
	{
		getpage = getpage + 1;
		fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
	}

	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *) malloc(
			strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5
			);
	sprintf(query, tpl, getpage, host, USERAGENT);
	return query;
}