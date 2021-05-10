#include "server.hpp"

void run_server(SDL_Renderer *renderer,TTF_Font *font , Game *game){
    srand(1);
    //cout<<"hello\n";
    int sockfd, newsockfd, port_no, bindfd, listenfd, bytes_sent, bytes_recvd;
    char sbuffer[512], cli_ip[16], cname[64],sname[64];
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
        return;
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
        return;
    }
    
    //listening for incoming connections
    //char *sname = NULL;
    //sname = static_cast<char *>(malloc(16 * sizeof(char)));
    ask_for_name(renderer, font, sname);

    SDL_RenderClear(renderer);
    char ci[] = "Waiting for a Client to connect to the server";
    disp_text(renderer, ci , font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2));
    SDL_RenderPresent(renderer);
    listenfd = listen(sockfd, 5);
    if (listenfd == -1)
    {
        perror("Failed to listen!");
        return;
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
                disp_text(renderer, full_text , font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2));
                SDL_RenderPresent(renderer);
            
            }
    }
    
    sleep(2);
    
        char* c = "Creating Game Please wait ";
        SDL_RenderClear(renderer);
        disp_text(renderer, c , font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);

    

    sleep(2);
    
        c = "Thank You for playing";
        SDL_RenderClear(renderer);
        disp_text(renderer, c , font, int(SCREEN_WIDTH/2) - 100, int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);

    sleep(2);

    game->cPlayer.name = cname;
    game->sPlayer.name = sname;
    game->isServer = true;
    int splayerInfo[4];
    int cplayerInfo[4];
    
    for (int level = 1; level<=LEVELS; level++){

        if(!game->running()){
            break;
        }
        game->counter = 0;
        game->mazeInit();
        game->maze_gen();
        game->cPlayer.time = 1000;
        game->sPlayer.time = 1000;

        game->isLevelRunning = true;
        
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        disp_text(renderer, "Level: ", font, 290, 220);
        std::string temp_str = std::to_string(level);
        char* char_type = (char*) temp_str.c_str();
        disp_text(renderer, char_type, font, 340, 220);
        SDL_RenderPresent(renderer);

        sleep(2);
        
        while (game->running() && game->isLevelRunning) {


             game->sPlayer.encode(splayerInfo);

                 bytes_recvd = recv(newsockfd, &cplayerInfo, sizeof(cplayerInfo), 0);
                 bytes_sent = send(newsockfd, &splayerInfo, sizeof(splayerInfo), 0);

            game->cPlayer.decode(cplayerInfo);

            game->handleEvents();
            game->update();
            game->render();

            if(game->sPlayer.time<=0){
                game->isLevelRunning = false;
            }
        }
        game->clean();
        sleep(2);
    }
    close(newsockfd);
    close(sockfd);

    

}
