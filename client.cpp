#include "client.hpp"

void run_client(char *ip){
    int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
    char cbuffer[512], sname[64], cname[64];
    char *ptr = &cbuffer[0];
    char *ptr_port = (char *)&PORT;
    struct sockaddr_in serv_addr;
    struct hostent *he;
    
    int count = 0, inp, x, y, ni, inp_true = 0, toss;
    char serv_choice, cli_choice, nc;
    char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

    port_no = atoi(ptr_port);
    he = gethostbyname(ip);
    if (he == NULL)
    {
        perror("No Such Host!");
    
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Sorry. Socket could not be created!");
       
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);

    connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectfd == -1)
    {
        perror("Sorry. Could not connect to server.");
        
    }
        
    std::cout<<"Enter your name : ";
    std::cin>>cname;
    do
    {
        static int flag = 0;
        bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
        if (bytes_sent == -1 && flag == 0)
        {
            std::cout<<"PLAYER DATA NOT SENT!"<<std::endl<<"Trying Again...";
            continue;
        }
        else
        {        cli_choice = 'X';
            
            flag = 1;
            memset(&sname, 0, sizeof(sname));
            bytes_recvd = recv(sockfd, &sname, sizeof(sname), 0);
            if (bytes_recvd == -1)
                std::cout<<"COULD NOT ACQUIRE PLAYER INFORMATION!"<<std::endl<<"Trying Again..."<<std::endl;
            else
                std::cout<<"You have joined "<<sname<<" for a game of Tic-Tac-Toe."<<std::endl;
        }
    }while(bytes_sent == -1 || bytes_recvd == -1);
    
    std::cout<<"Creating game. Please wait..."<<std::endl;
    sleep(2);
    std::cout<<std::endl<<"Game created!"<<std::endl<<std::endl<<"Doing a toss...";
    
    bytes_recvd = recv(sockfd, &toss_buffer, sizeof(toss_buffer), 0);
    if (bytes_recvd == -1)
    {
        perror("TOSS BUFFER not received");
    }

    std::cout<<std::endl<<"Thank You for playing"<<std::endl;
    
    close(sockfd);
}
