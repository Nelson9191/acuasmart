#include "am_udp.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+TCP includes. */
/* TCP/IP abstraction includes. */
//#include "aws_secure_sockets.h" 

static const int NTP_PACKET_SIZE= 48;

 
void udp_req_init(){
    #ifdef configDNS_SERVER_ADDR0
        printf("configDNS_SERVER_ADDR0: %d\n", configDNS_SERVER_ADDR0);
    #elif
        printf("configDNS_SERVER_ADDR0 NOT DEFINED\n");
    #endif    
}

Socket_t am_udp_create_socket(uint16_t src_port){
    Socket_t xSocket;
    struct freertos_sockaddr xBindAddress;

    xSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                               FREERTOS_SOCK_DGRAM,
                               FREERTOS_IPPROTO_UDP );

    if( xSocket != FREERTOS_INVALID_SOCKET ){
        xBindAddress.sin_port = FreeRTOS_htons( src_port );
        if( FreeRTOS_bind( xSocket, &xBindAddress, sizeof( &xBindAddress ) ) == 0 ){
            printf("BIND succesfull\n");
            return xSocket;
        }
        else{
            printf("BIND FAILED\n");
            return -1;
        }
    }
    else{
        printf("Unable to create socket\n");//insufficient FreeRTOS heap memory available for the socket
    }

    return -1;
}

void am_udp_close_socket(Socket_t socket){
    FreeRTOS_closesocket(socket);
}

int am_udp_send_msg(Socket_t socket, const char * host, uint16_t host_port, char * msg, uint16_t len){
    uint32_t ulIPAddress;
    struct freertos_sockaddr xDestinationAddress;
    char host_addr_str[16]; 
    int32_t iReturned;


    uint8_t ucBuffer[ 128 ] = {'h', 'o', 'l', '\n', '\0'};


    ulIPAddress = FreeRTOS_gethostbyname(host);

    if( ulIPAddress != 0 ){
        FreeRTOS_inet_ntoa(ulIPAddress, (char *)host_addr_str); //IP to string
        // Print out the IP address. 
        //printf( "www.FreeRTOS.org is at IP address %s\r\n", cb );
    }
    else{
        printf( "DNS lookup failed. " );
        return -1;
    }

    xDestinationAddress.sin_addr = FreeRTOS_inet_addr(host_addr_str);
    xDestinationAddress.sin_port = FreeRTOS_htons(host_port);

    for(int i=0; i<1; ++i){
        printf("Send %d\n", i);
        snprintf((char *)ucBuffer, 128, "Hola Probando UDP...%d\n", i);
        iReturned = FreeRTOS_sendto(
                                        /* The socket being send to. */
                                        socket,
                                        /* The data being sent. */
                                        msg,
                                        /* The length of the data being sent. */
                                        len,
                                        /* ulFlags with the FREERTOS_ZERO_COPY bit clear. */
                                        0,
                                        /* Where the data is being sent. */
                                        &xDestinationAddress,
                                        /* Not used but should be set as shown. */
                                        sizeof( xDestinationAddress )
                                );

        if( iReturned == 128 ){
            /* The data was successfully queued for sending.  128 bytes will have
            been copied out of ucBuffer and into a buffer inside the TCP/IP stack.
            ucBuffer can be re-used now. */
        }
     vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
}

int am_udp_recv_msg(Socket_t socket, const char * host, uint16_t host_port, char * buffer, uint16_t buflen){
    uint32_t ulIPAddress;
    struct freertos_sockaddr xSourceAddress;
    char host_addr_str[16];
    int8_t cIPAddressString[ 16 ]; 
    int32_t iReturned;
    uint8_t recvBuffer[ 128 ] = {'\0'};

    memset(buffer, 0, buflen);
    ulIPAddress = FreeRTOS_gethostbyname(host);


    if( ulIPAddress != 0 ){
        FreeRTOS_inet_ntoa(ulIPAddress, (char *)host_addr_str); //IP to string
    }
    else{
        printf( "DNS lookup failed. " );
        return -1;
    }

    xSourceAddress.sin_addr = FreeRTOS_inet_addr(host_addr_str);
    xSourceAddress.sin_port = FreeRTOS_htons(host_port);


    iReturned = FreeRTOS_recvfrom(
                                    /* The socket data is being received on. */
                                    socket,
                                    /* The buffer into which received data will be
                                    copied. */
                                    buffer,
                                    /* The length of the buffer into which data will be
                                    copied. */
                                    buflen,
                                    /* ulFlags with the FREERTOS_ZERO_COPY bit clear. */
                                    0,
                                    /* Will get set to the source of the received data. */
                                    &xSourceAddress,
                                    /* Not used but should be set as shown. */
                                    sizeof( xSourceAddress )
                               );

    // if( iReturned > 0 ){
    //     /* Data was received from the socket.  Prepare the IP address for
    //     printing to the console by converting it to a string. */
    //     FreeRTOS_inet_ntoa( xSourceAddress.sin_addr, ( char * ) cIPAddressString );

        
    //     // recvBuffer[41] recvBuffer[42] recvBuffer[43] ;
    //     /* Print out details of the data source. */
    //     printf( "Received %d bytes from IP address %s port number %d -- %s\r\n",
    //                 iReturned, /* The number of bytes received. */
    //                 cIPAddressString, /* The IP address that sent the data. */
    //                 FreeRTOS_ntohs( xSourceAddress.sin_port ),
    //                 recvBuffer ); /* The source port. */
    //     printf("captured timestamp: %u\n", tmp);
    // } 

    return iReturned;                           
}


void vStandardSendExample(){
uint32_t ulIPAddress;
char cb[ 16 ];
Socket_t xSocket;
struct freertos_sockaddr xBindAddress;

    /* Create a UDP socket. */
    xSocket = FreeRTOS_socket( FREERTOS_AF_INET,
                               FREERTOS_SOCK_DGRAM,
                               FREERTOS_IPPROTO_UDP );

    ulIPAddress = FreeRTOS_gethostbyname( "www.freertos.org" );

    if( ulIPAddress != 0 ){
        /* Convert the IP address to a string. */
        FreeRTOS_inet_ntoa( ulIPAddress, ( char * ) cb );
        
        /* Print out the IP address. */
        printf( "www.FreeRTOS.org is at IP address %s\r\n", cb );
    }
    else
    {
        printf( "DNS lookup failed. " );
    }

    if( xSocket != FREERTOS_INVALID_SOCKET ){

        xBindAddress.sin_port = FreeRTOS_htons( 9999 );
        if( FreeRTOS_bind( xSocket, &xBindAddress, sizeof( &xBindAddress ) ) != 0 ){
            printf("BIND FAILED\n");
            return;
        }

    }
    else
    {
        printf("Unable to create socket\n");
        return;
    } 

uint8_t ucBuffer[ 128 ] = {'h', 'o', 'l', '\n', '\0'};

struct freertos_sockaddr xDestinationAddress;
int32_t iReturned;
    
    TickType_t xSendTimeout_ms = 20000;
    xSendTimeout_ms /= portTICK_PERIOD_MS;
    

    //setsockopt(server, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
    /* Fill in the destination address and port number, which in this case is
    port 1024 on IP address 192.168.0.100. */
    //xDestinationAddress.sin_addr = FreeRTOS_inet_addr( "192.168.0.8" );//FreeRTOS_inet_addr_quick( 192, 168, 0, 8 );
    xDestinationAddress.sin_addr = FreeRTOS_inet_addr_quick( 192, 168, 0, 8 );
    xDestinationAddress.sin_port = FreeRTOS_htons( 1024 );

    //setsockopt(server, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
    /* The local buffer is filled with the data to be sent, in this case it is
    just filled with 0xff. */
    //memset( ucBuffer, 0xff, 128 );
    

    /* Send the buffer with ulFlags set to 0, so the FREERTOS_ZERO_COPY bit
    is clear. */
    if(xSocket == FREERTOS_INVALID_SOCKET){
        printf("SOCKET NOT CREATED\n");
    }
    else{
        printf("SOCKET CREATED XD\n");
    }

    for(int i=0; i<10; ++i){
        printf("Send %d\n", i);
        snprintf((char *)ucBuffer, 128, "Hola Probando UDP...%d\n", i);
        iReturned = FreeRTOS_sendto(
                                        /* The socket being send to. */
                                        xSocket,
                                        /* The data being sent. */
                                        ucBuffer,
                                        /* The length of the data being sent. */
                                        23,
                                        /* ulFlags with the FREERTOS_ZERO_COPY bit clear. */
                                        0,
                                        /* Where the data is being sent. */
                                        &xDestinationAddress,
                                        /* Not used but should be set as shown. */
                                        sizeof( xDestinationAddress )
                                );

        printf("Returned: %d\n", iReturned);
        if( iReturned == 128 )
        {
            /* The data was successfully queued for sending.  128 bytes will have
            been copied out of ucBuffer and into a buffer inside the TCP/IP stack.
            ucBuffer can be re-used now. */
        }
     vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}