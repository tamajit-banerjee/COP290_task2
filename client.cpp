#include "client.hpp"


int run_client(SDL_Renderer *renderer, TTF_Font *font , Game *game){
    srand(1);
    int sockfd, port_no, connectfd, bytes_sent, bytes_recvd;
    char sname[64], cname[64];
    char *ptr_port = (char *)&PORT;
    struct sockaddr_in serv_addr;
    struct hostent *he;

    port_no = atoi(ptr_port);
    char *server_ip_addr = NULL;
    server_ip_addr = static_cast<char *>(malloc(16 * sizeof(char)));
    // ask_for_ip(renderer, font, server_ip_addr);
    server_ip_addr = "127.0.0.1";
    he = gethostbyname(server_ip_addr);
    if (he == NULL)
    {
        perror("No Such Host!");
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Sorry. Socket could not be created!");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_no);
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);

    connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    if (connectfd == -1)
    {
            char c[] = "Sorry. Could not connect to server.";
            SDL_RenderClear(renderer);
            disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2));
            SDL_RenderPresent(renderer);
            sleep(3);
        close(sockfd);
        return -1;
    }
        
    ask_for_name(renderer, font, cname, false);
    
    static int flag = 0;
    bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
    if (bytes_sent == -1 && flag == 0)
    {
        std::cout<<"PLAYER DATA NOT SENT!"<<std::endl<<"Trying Again...";
    }
    else
    {
        flag = 1;
        memset(&sname, 0, sizeof(sname));
        bytes_recvd = recv(sockfd, &sname, sizeof(sname), 0);
        if (bytes_recvd == -1)
            std::cout<<"COULD NOT ACQUIRE PLAYER INFORMATION!"<<std::endl<<"Trying Again..."<<std::endl;
        else{
                const char* c = "You have joined  ";
                const char* last = " for a game ";
                char* full_text;
                full_text=static_cast<char *>(malloc(strlen(c)+strlen(last)+strlen(sname)));
                strcpy(full_text,c);
                strcat(full_text,sname);
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
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/2));
        SDL_RenderPresent(renderer);
    
    sleep(2);

    game->cPlayer.name = cname;
    game->sPlayer.name = sname;
    game->isServer = false;

    for (int level = 1; level<=LEVELS; level++){

        if(!game->running()){
            break;
        }
        game->levelStart(level);

        sleep(2);
        
        int splayerInfo[4];
        int cplayerInfo[4];
        
        while (game->running() && game->isLevelRunning) {

            game->handleEvents();
            game->update();
            
            game->cPlayer.encode(cplayerInfo);
            
                bytes_sent = send(sockfd, &cplayerInfo, sizeof(cplayerInfo), 0);
                bytes_recvd = recv(sockfd, &splayerInfo, sizeof(splayerInfo), 0);

            game->sPlayer.decode(splayerInfo);

            game->render();

            

            if(game->sPlayer.time<=0 && game->cPlayer.time<=0){
                game->isLevelRunning = false;
            }
        }
        game->levelEnd();
        sleep(2);
    }
    
    close(sockfd);
    return 1;
}
