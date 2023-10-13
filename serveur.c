#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8080

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    int socketfd, newsocketfd, portNumber, n;
    char buffer[255]; // to store msg to send

    struct sockaddr_in server_address, client_address;
    socklen_t clilen;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        error("Error opening socket");
    }

    bzero((char *)&server_address, sizeof(server_address)); // it clears all the data to what it it reference to

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(socketfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        error("Binding Failed");
    }

    // next step is our server is trying to connect

    listen(socketfd, 5);
    clilen = sizeof(client_address);

    newsocketfd = accept(socketfd, (struct sockaddr *)&client_address, &clilen);
    if (newsocketfd < 0)
    {
        error("Error on Accept");
    }

    int number1, number2, result, operation;
S:
    number1 = write(newsocketfd, "Enter First Number  : ", strlen("Enter First Number "));
    if (number1 < 0)
    {
        error("Error Writing to socket");
    }
    read(newsocketfd, &number1, sizeof(int));
    printf("First Number  is : %d\n", number1);

    number2 = write(newsocketfd, "Enter Second Number  : ", strlen("Enter Second Number "));
    if (number2 < 0)
    {
        error("Error Writing to socket");
    }
    read(newsocketfd, &number2, sizeof(int));
    printf("Second Number  is : %d\n", number2);

    operation = write(newsocketfd, "Enter Your operation\n1.Add\n2.Subtract\n3.Division\n4.Multiplication\n5.Exit", strlen("Enter Your operation\n1.Add\n2.Subtract\n3.Division\n4.Multiplication\n5.Exit"));
    if (operation < 0)
    {
        error("Error on Writing");
    }
    read(newsocketfd, &operation, sizeof(int));
    printf("- operation is : %d\n", operation);

    switch (operation)
    {
    case 1:
        result = number1 + number2;
        break;
    case 2:
        result = number1 - number2;
        break;
    case 3:
        result = number1 / number2;
        break;
    case 4:
        result = number1 * number2;
        break;

    case 5:
        goto Q;
        break;
    }

    write(newsocketfd, &result, sizeof(int));
    if (operation != 5)
    {
        goto S;
    }

Q:
    close(newsocketfd);
    close(socketfd);
    return 0;
}
