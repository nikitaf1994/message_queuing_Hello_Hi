#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct msgbuf {
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
      		perror( "client: ftok failed:" );
      		exit( 1 );
   	}

   	printf( "client: System V IPC key = %u\n", key );

	if ( ( qId = msgget( key, IPC_CREAT | 0666 ) ) == -1 ) 
	{
      		perror( "client: Failed to create message queue:" );
      		exit( 2 );
   	}

   	printf( "client: Message queue id = %u\n", qId );

   	if ( msgrcv( qId, &buf, sizeof msg.mText, 10, 0 ) == -1 )
      		perror( "client: msgrcv failed:" );
   	else
	{
		printf( "client: Message received = %s\n", buf.mText );
		strcpy( msg.mText, "Hi!" );
   		msg.mType = 20;

		if ( msgsnd( qId, &msg, sizeof msg.mText, 0 ) == -1 )
		{
      			perror( "client: msgsnd failed:" );
      			exit( 3 );
   		}

		printf( "client: Message sent successfully\n" );
	}
}

