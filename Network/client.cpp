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
            disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3));
            SDL_RenderPresent(renderer);
            sleep(3);
        close(sockfd);
        return -1;
    }
        
    if(ask_for_name(renderer, font, cname, false)== -1){
        close(sockfd);
        return 0;
    }
    game->cPlayer.name = cname;
    game->isServer = false;
    int playerId;
    // Receiving Player id
    bytes_recvd = recv(sockfd, &playerId, sizeof(playerId), 0);
    game->sPlayer.playerId = playerId;
    game->askPlayerAvatar();

    
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
                game->sPlayer.name = sname;
                const char* c = "You have joined  ";
                const char* last = " for a game ";
                char* full_text;
                full_text=static_cast<char *>(malloc(strlen(c)+strlen(last)+strlen(sname)));
                strcpy(full_text,c);
                strcat(full_text,sname);
                strcat(full_text,last);
                for(int i = 0; i<SLEEP_UNIT; i++){
                    SDL_RenderClear(renderer);
                    disp_text_center(renderer, full_text , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3));
                    SDL_RenderPresent(renderer);
                    if(game->toQuit()){
                        close(sockfd);
                        return 0;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
        }
    }

    for(int i = 0; i<SLEEP_UNIT; i++){
        char* c = "Creating Game Please wait ";
        SDL_RenderClear(renderer);
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3));
        SDL_RenderPresent(renderer);
        if(game->toQuit()){
            close(sockfd);
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    

    int splayerInfo[6];
    int cplayerInfo[6];


    int isClientRunning;
    int isServerRunning;

    int badNetworkCounter = 0;

    for (int level = 1; level<=LEVELS; level++){

        if(!game->running()){
            break;
        }

        int seedi;

        bytes_recvd = recv(sockfd, &seedi, sizeof(seedi), 0);
        
        game->levelStart(level,seedi);

            isClientRunning = game->running();
            bytes_sent = send(sockfd, &isClientRunning, sizeof(isClientRunning), 0);
            bytes_recvd = recv(sockfd, &isServerRunning, sizeof(isServerRunning), 0);
            game->isRunning = isServerRunning && isClientRunning;

        
        while (game->running() && game->isLevelRunning) {

            game->handleEvents();
            game->update();

            
            game->cPlayer.encode(cplayerInfo);

            int time1 = std::chrono::system_clock::now().time_since_epoch().count(); 
                bytes_sent = send(sockfd, &cplayerInfo, sizeof(cplayerInfo), 0);
            int time2 = std::chrono::system_clock::now().time_since_epoch().count(); 
                bytes_recvd = recv(sockfd, &splayerInfo, sizeof(splayerInfo), 0);
            int time3 = std::chrono::system_clock::now().time_since_epoch().count(); 
            
            if(time2 - time1 > 1000*NETWORK_THRESHOLD || time3 - time2 > 1000*NETWORK_THRESHOLD){
                badNetworkCounter++;
                if(badNetworkCounter > 0){
                    for(int i = 0; i< SLEEP_UNIT; i++){
                        SDL_RenderClear(renderer);
                        disp_text_center(renderer, "Slow Network. Taking too long to connect!" , font, int(SCREEN_WIDTH/2) , int(SCREEN_HEIGHT/3));
                        SDL_RenderPresent(renderer);
                        if(game->toQuit()){
                            close(sockfd);
                            return 0;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    }

                }
            }

            game->sPlayer.decode(splayerInfo);

            game->render();

            

            if(game->sPlayer.get_time()<=0 && game->cPlayer.get_time()<=0){
                game->isLevelRunning = false;
            }
            
            isClientRunning = game->running();
            bytes_sent = send(sockfd, &isClientRunning, sizeof(isClientRunning), 0);
            bytes_recvd = recv(sockfd, &isServerRunning, sizeof(isServerRunning), 0);
            game->isRunning = isServerRunning && isClientRunning;

        }
        game->levelEnd();

            isClientRunning = game->running();
            bytes_sent = send(sockfd, &isClientRunning, sizeof(isClientRunning), 0);
            bytes_recvd = recv(sockfd, &isServerRunning, sizeof(isServerRunning), 0);
            game->isRunning = isServerRunning && isClientRunning;

    }
    
    close(sockfd);

    for(int i = 0; i< SLEEP_UNIT; i++){
        SDL_RenderClear(renderer);
        disp_text_center(renderer, "Thank You for playing", font, int(SCREEN_WIDTH/2) , int(SCREEN_HEIGHT/3));
        SDL_RenderPresent(renderer);
        if(game->toQuit()){
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }


    for(int i = 0; i< SLEEP_UNIT; i++){
        
        const char* c = "The winner is: ";
        char* full_text;
        if(game->sPlayer.score > game->cPlayer.score){
            full_text=static_cast<char *>(malloc(strlen(c)+strlen(game->sPlayer.name)));
            strcpy(full_text,c);
            strcat(full_text,game->sPlayer.name);
        }
        else if(game->sPlayer.score < game->cPlayer.score){
            full_text=static_cast<char *>(malloc(strlen(c)+strlen(game->cPlayer.name)));
            strcpy(full_text,c);
            strcat(full_text,game->cPlayer.name);
        }
        else{
            full_text = "Its a Tie!";
        }

        SDL_RenderClear(renderer);
        disp_text_center(renderer, "RESULTS", font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3));
        disp_text_center(renderer, full_text, font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)+50);
        SDL_RenderPresent(renderer);
        if(game->toQuit()){
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }


    return 1;
}
