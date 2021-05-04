#include "client.hpp"


void run_client(SDL_Renderer *renderer, TTF_Font *font , Game *game){
    int sockfd, newsockfd, port_no, n, connectfd, bytes_sent, bytes_recvd;
    char cbuffer[512], sname[64],cname[64];
    char *ptr = &cbuffer[0];
    char *ptr_port = (char *)&PORT;
    struct sockaddr_in serv_addr;
    struct hostent *he;
    
    int count = 0, inp, x, y, ni, inp_true = 0, toss;
    char serv_choice, cli_choice, nc;
    char choice_buffer[2], co_ordinates_buffer[2], toss_buffer;

    port_no = atoi(ptr_port);
    char *server_ip_addr = NULL;
    server_ip_addr = static_cast<char *>(malloc(16 * sizeof(char)));
    // ask_for_ip(renderer, font, server_ip_addr);
    server_ip_addr = "127.0.0.1";
    he = gethostbyname(server_ip_addr);
    if (he == NULL)
    {
        perror("No Such Host!");
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Sorry. Socket could not be created!");
        return;
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
            disp_text(renderer, c , font, 200, 200);
            SDL_RenderPresent(renderer);
            sleep(3);
        close(sockfd);
        return;
    }
        
    //char *cname = NULL;
    //cname = static_cast<char *>(malloc(16 * sizeof(char)));
    ask_for_name(renderer, font, cname);
    
    static int flag = 0;
    bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
    if (bytes_sent == -1 && flag == 0)
    {
        std::cout<<"PLAYER DATA NOT SENT!"<<std::endl<<"Trying Again...";
    }
    else
    {        cli_choice = 'X';
        
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
                disp_text(renderer, full_text , font, 200, 200);
                SDL_RenderPresent(renderer);
        }
    }

    
    sleep(2);
    
        char* c = "Creating Game Please wait ";
        SDL_RenderClear(renderer);
        disp_text(renderer, c , font, 200, 200);
        SDL_RenderPresent(renderer);

    
    
    sleep(2);
        
        c = "Thank You for playing";
        SDL_RenderClear(renderer);
        disp_text(renderer, c , font, 200, 200);
        SDL_RenderPresent(renderer);
    
    sleep(2);

    game->cPlayer.name = cname;
    game->sPlayer.name = sname;
    game->isServer = false;

    for (int level = 1; level<=LEVELS; level++){

        if(!game->running()){
            break;
        }
        game->counter = 0;
        game->mazeInit();
        game->cPlayer.time = 1000;
        game->sPlayer.time = 1200;
        
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
            // char splayerInfo[100];
            // char cplayerInfo[100];
            
            // game->cPlayer.encode(cplayerInfo, 100);
            
            // do{
            //     bytes_sent = send(newsockfd, &cplayerInfo, sizeof(cplayerInfo), 0);
            //     {
            //         std::cout<<"Could not SEND Player Data!"<<"Trying Again..."<<std::endl;
            //     }
            //     bytes_recvd = recv(newsockfd, &splayerInfo, sizeof(splayerInfo), 0);
            //     if (bytes_recvd == -1)
            //     {
            //         memset(&sname, 0, sizeof(sname));
            //         std::cout<<"Could not ACQUIRE Player Information!"<<std::endl<<"Trying again..."<<std::endl;
            //     }
            // }while(bytes_recvd == -1 || bytes_sent == -1);

            // game->sPlayer.decode(splayerInfo);

            game->handleEvents();
            game->update();
            game->render();

            if(game->cPlayer.time<=0){
                game->isLevelRunning = false;
            }
        }
        game->clean();
        sleep(2);
    }
    
    close(sockfd);
}
