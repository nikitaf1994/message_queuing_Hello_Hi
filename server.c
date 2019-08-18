#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf 
{
	long mType;
	char mText[50];
};

int main() {
	char txtBuf[50];
	int qId;
	key_t key;
	struct msgbuf msg, buf;
	struct msqid_ds msgCtlBuf;

   	if ( ( key = ftok( "/tmp", 'C' ) ) == -1 ) 
	{
      		perror( "server: ftok failed:" );
      		exit( 1 );
   	}

   	printf( "server: System V IPC key = %u\n", key );

   	if ( ( qId = msgget( key, IPC_CREAT | 0666 ) ) == -1 ) 
	{
      		perror( "server: Failed to create message queue:" );
      		exit( 2 );
   	}

   	printf( "server: Message queue id = %u\n", qId );

   	strcpy( msg.mText, "Hello!" );
   	msg.mType = 10;

   	if ( msgsnd( qId, &msg, sizeof msg.mText, 0 ) == -1 ) 
	{
      		perror( "server: msgsnd failed:" );
      		exit( 3 );
   	}

   	printf( "server: Message sent successfully\n" );

   	printf( "server: waiting..." );
   	sleep( 15 );

///////////////////////////////////////////////////////////
	if ( msgrcv( qId, &buf, sizeof msg.mText, 20, 0 ) == -1 )
      		perror( "server: msgrcv failed:" );
   	else
      		printf( "server: Message received = %s\n", buf.mText );
///////////////////////////////////////////////////////////

   	printf( "server: done waiting. removing message queue...\n" );

   	if ( msgctl( qId, IPC_RMID, &msgCtlBuf ) == -1 ) 
	{
      		perror( "server: msgctl failed:" );
      		exit( 4 );
   	}

   	printf( "server: Message queue removed OK\n" );
}
