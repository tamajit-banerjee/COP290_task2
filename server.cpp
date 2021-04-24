#include "server.hpp"

void run_server(){
    //cout<<"hello\n";
    int sockfd, newsockfd, port_no, bindfd, listenfd, bytes_sent, bytes_recvd;
    char sbuffer[512], cli_ip[16], sname[64], cname[64];
    char *ptr_buff, *ptr_port;
    const char *ptr_cli_ip;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t serv_size, cli_size;

    int inp_true = 0, count = 0, inp, ni, x, y, toss;
    char serv_choice, cli_choice, nc;
    char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;
    

    ptr_buff = &sbuffer[0];
    ptr_port = (char *)&PORT;

    //creating sever side socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Server side listening Socket could not be created!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    port_no = atoi(ptr_port);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    //binding socket
    bindfd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bindfd == -1)
    {
        perror("Failed to bind!");
    }
    
    //listening for incoming connections
    std::cout<<"Enter your Name : ";
    std::cin>>sname;
    std::cout<<"Server created!"<<std::endl<<"Waiting for a Player..."<<std::endl;

    listenfd = listen(sockfd, 5);
    if (listenfd == -1)
    {
        perror("Failed to listen!");
    }

    serv_size = sizeof(serv_addr);
    cli_size = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_size);
        
    if (newsockfd == -1)
    {
        perror("Failed to accept from client!");
    }
        
    ptr_cli_ip = inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, cli_size);
    std::cout<<"Server received connections from "<<cli_ip<<std::endl;

    memset(&cname, 0, sizeof(cname));
    do
    {
        static int flag = 0;
        bytes_recvd = recv(newsockfd, &cname, sizeof(cname), 0);
        if (bytes_recvd == -1 && flag == 0)
        {
            memset(&cname, 0, sizeof(cname));
            std::cout<<"Could not ACQUIRE Player Information!"<<std::endl<<"Trying again..."<<std::endl;
            continue;
        }
        else
        {
            flag = 1;
            bytes_sent = send(newsockfd, &sname, sizeof(sname), 0);
            if (bytes_sent == -1)
                std::cout<<"Could not SEND Player Data!"<<"Trying Again..."<<std::endl;
            else
                std::cout<<cname<<" has joined the game."<<std::endl;
        }
    }while(bytes_recvd == -1 || bytes_sent == -1);

    std::cout<<"Creating Game. Please wait..."<<std::endl;
    sleep(2);
    std::cout<<std::endl<<"Game created!"<<std::endl<<std::endl<<"Doing a toss...";
        
    std::cout<<std::endl<<"Thank You for playing "<<std::endl;
    close(newsockfd);
    close(sockfd);
}
