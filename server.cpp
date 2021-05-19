#include "server.hpp"

int run_server(SDL_Renderer *renderer,TTF_Font *font , Game *game){
    srand(1);
    //cout<<"hello\n";
    int sockfd, newsockfd, port_no, bindfd, listenfd, bytes_sent, bytes_recvd;
    char cli_ip[16], cname[64],sname[64];
    char *ptr_port;
    const char *ptr_cli_ip;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t serv_size, cli_size;

    ptr_port = (char *)&PORT;

    //creating sever side socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Server side listening Socket could not be created!");
        return -1;
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
        return -1;
    }
    
    //listening for incoming connections
    //char *sname = NULL;
    //sname = static_cast<char *>(malloc(16 * sizeof(char)));
    ask_for_name(renderer, font, sname, true);

    game->sPlayer.name = sname;
    game->cPlayer.name = cname;
    game->isServer = true;
    game->askPlayerAvatar();

    SDL_RenderClear(renderer);
    disp_text_center(renderer, "Waiting for Player 2 to join" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2));
    SDL_RenderPresent(renderer);
    listenfd = listen(sockfd, 5);
    if (listenfd == -1)
    {
        perror("Failed to listen!");
        return -1;
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
    SDL_RenderClear(renderer);
    memset(&cname, 0, sizeof(cname));

    static int flag = 0;
    bytes_recvd = recv(newsockfd, &cname, sizeof(cname), 0);
    if (bytes_recvd == -1 && flag == 0)
    {
        memset(&cname, 0, sizeof(cname));
        std::cout<<"Could not ACQUIRE Player Information!"<<std::endl<<"Trying again..."<<std::endl;
    }
    else
    {
        flag = 1;
        bytes_sent = send(newsockfd, &sname, sizeof(sname), 0);
        if (bytes_sent == -1)
            std::cout<<"Could not SEND Player Data!"<<"Trying Again..."<<std::endl;
        else{

                const char* c = "You have joined  ";
                const char* last = " for a game ";
                char* full_text;
                full_text=static_cast<char *>(malloc(strlen(c)+strlen(last)+strlen(cname)));
                strcpy(full_text,c);
                strcat(full_text,cname);
                strcat(full_text,last);
                SDL_RenderClear(renderer);
                disp_text_center(renderer, full_text , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2));
                SDL_RenderPresent(renderer);
            
            }
    }
    
    sleep(2);
    
        char* c = "Creating Game Please wait ";
        SDL_RenderClear(renderer);
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);

    

    sleep(2);
    
        c = "Thank You for playing";
        SDL_RenderClear(renderer);
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2) , int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);

    sleep(2);

    
    int splayerInfo[6];
    int cplayerInfo[6];
    
    for (int level = 1; level<=LEVELS; level++){

        if(!game->running()){
            break;
        }

        srand(std::chrono::system_clock::now().time_since_epoch().count());

        int seedi = rand();

        bytes_sent = send(newsockfd, &seedi, sizeof(seedi), 0);

        game->levelStart(level,seedi);
        
        while (game->running() && game->isLevelRunning) {


            game->handleEvents();
            game->update();
            
            game->sPlayer.encode(splayerInfo);

                bytes_recvd = recv(newsockfd, &cplayerInfo, sizeof(cplayerInfo), 0);
                bytes_sent = send(newsockfd, &splayerInfo, sizeof(splayerInfo), 0);

            game->cPlayer.decode(cplayerInfo);

            

            game->render();

            if(game->sPlayer.get_time()<=0 && game->cPlayer.get_time()<=0){
                game->isLevelRunning = false;
            }
            
        }
        game->levelEnd();
        sleep(2);
    }
    close(newsockfd);
    //close(sockfd);

    return 1;

}
