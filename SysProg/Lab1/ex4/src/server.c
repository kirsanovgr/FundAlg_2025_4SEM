#include "../include/server.h"

statuses mqueueCreate() {
    mq_unlink("/server");
    mq = mq_open("/server", O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        return MQ_CREATE_ERROR;
    }
    return OK;
}

size_t connectionCheck(char* client_str, river clients[100]) {
    size_t client_id = -1;
    for (size_t i = 0; i < 100; i++) {
        if (strcmp(clients[i].client, client_str) == 0) {
            client_id = i;
            break;
        }
    }
    return client_id;
}

// "connect /client1"
// "take goat /client1"


// take - меняем пустоту лодки и положение на сайде реки
// put - меняем пустоту лодки и положение на сайде реки
// move - меняем ривер сайд + проверка состояния

statuses Task() {
    char buff[128];
    river clients[100];
    size_t count;
    size_t clients_problems = 0;
    if (mqueueCreate() == MQ_CREATE_ERROR) {
        return MQ_CREATE_ERROR;
    }
    while(1) {
        count = mq_receive(mq, buff, sizeof(buff), NULL);
        char* token = strtok(buff, " ");
        size_t client_id;
        if (strcmp(token, "connect") == 0) {
            token = strtok(NULL, " ");
            strcpy(clients[clients_problems].client, token);
            clients[clients_problems].client_ds = mq_open(clients[clients_problems].client, O_RDWR, 0644, NULL);
            client_id = clients[clients_problems].client_ds;
            if (mq_send(clients[clients_problems].client_ds, "connection done", sizeof("connection done"), 0) == -1) {
                printf("server: problem with connection %s", clients[clients_problems].client);
                continue;
            }
            else {
                // если соединение успешно, то мы дальше работаем с клиентом
                for (size_t i = 0; i < 3; i++) {
                    clients[clients_problems].left_side[i] = 1;
                    clients[clients_problems].right_side[i] = 0;
                }
                clients[clients_problems].river_side = 0;
                clients[clients_problems].boat = -1;
                clients_problems++;
                continue;
            }

        }
        else if (strcmp(token, "take") == 0) {
            char* object = strtok(NULL, " ");
            char* client_str = strtok(NULL, " ");
            client_id = connectionCheck(client_str, clients);
            if (client_id == -1) {
                printf("server: incorrect user\n");
                continue;
            }

            else {
                // проверяем пустоту лодки
                if (clients[client_id].boat != -1) {
                    mq_send(clients[client_id].client_ds, "boat is full", sizeof("boat is full"), 0);
                }
                else {
                    // проверяем объект
                    int object_num = -1;
                    if (strcmp(object, "wolf") == 0) {
                        object_num = 0;
                    }
                    else if(strcmp(object, "goat") == 0) {
                        object_num = 1;
                    }
                    else if(strcmp(object, "cabbage") == 0) {
                        object_num = 2;
                    }

                    if (object_num == -1) {
                        mq_send(clients[client_id].client_ds, "incorrect object", sizeof("incorrect object"), 0);
                    }
                    // выполняем взятие
                    else {
                        if (clients[client_id].river_side == 0 && clients[client_id].left_side[object_num] == 1) {
                            clients[client_id].boat = object_num;
                            clients[client_id].left_side[object_num] = 0;
                        }
                        else if(clients[client_id].river_side == 1 && clients[client_id].right_side[object_num] == 1) {
                            clients[client_id].boat = object_num;
                            clients[client_id].right_side[object_num] = 0;
                            
                        }
                        else {
                            mq_send(clients[client_id].client_ds, "incorrect option", sizeof("incorrect option"), 0);
                        }
                    }
                }
            }
            mq_send(clients[client_id].client_ds, "take to boat", sizeof("take to boat"), 0);
        }
        else if(strcmp(token, "put") == 0) {
            char* client_str = strtok(NULL, " ");
            client_id = connectionCheck(client_str, clients);
            if (client_id == -1) {
                printf("server: incorrect user\n");
                continue;
            }

            else {
                // проверяем пустоту лодки
                if (clients[client_id].boat == -1) {
                    mq_send(clients[client_id].client_ds, "boat is empty", sizeof("boat is empty"), 0);
                }
                // кладем объект
                else {
                    if (clients[client_id].river_side == 0) {
                        clients[client_id].left_side[clients[client_id].boat] = 1;
                        clients[client_id].boat = -1;

                    }
                    else {
                        clients[client_id].right_side[clients[client_id].boat] = 1;
                        clients[client_id].boat = -1;
                    }
                }
            }
            mq_send(clients[client_id].client_ds, "put to side", sizeof("put to side"), 0);


        }
        else if (strcmp(token, "move") == 0 || strcmp(token, "done") == 0) {
            char* client_str = strtok(NULL, " ");
            client_id = connectionCheck(client_str, clients);
            if (client_id == -1) {
                printf("server: incorrect user\n");
                continue;
            }
            else {
                // меняем положение лодки
                clients[client_id].river_side = (clients[client_id].river_side + 1) % 2;
                // проверяем получившиеся состояние только после move, т.к. если проверять после каждой команды, то не получится выиграть вообще
                // выигрыщ
                if (clients[client_id].right_side[0] == 1 && clients[client_id].right_side[1] == 1 
                        && clients[client_id].right_side[2] == 1) {
                    mq_send(clients[client_id].client_ds, "YOU WIN", sizeof("YOU WIN"), 0);
                    clients_problems--;

                }
                // проигрыши
                else if ((clients[client_id].left_side[0] == 1 && clients[client_id].left_side[1] == 1) || 
                    (clients[client_id].right_side[0] == 1 && clients[client_id].right_side[1] == 1)) {
                     mq_send(clients[client_id].client_ds, "YOU LOSE, wolf kill goat", sizeof("YOU LOSE, wolf kill goat"), 0);
                     clients_problems--;
                }
                else if ((clients[client_id].left_side[2] == 1 && clients[client_id].left_side[1] == 1) || 
                        (clients[client_id].right_side[2] == 1 && clients[client_id].right_side[1] == 1)) {
                    mq_send(clients[client_id].client_ds, "YOU LOSE, goat eat cabbage", sizeof("YOU LOSE, goat eat cabbage"), 0);
                    clients_problems--;
                }
                else {
                    mq_send(clients[client_id].client_ds, "move to the other side", sizeof("move to the other side"), 0);
                }
            }
        }
        else {
            printf("server: incorrect command\n");
            continue;
        }


    }
    return OK;

}


int main() {
    switch(Task()) {
        case MQ_CREATE_ERROR:
            printf("mq_create_error\n");
            return MQ_CREATE_ERROR;
    }
    mq_unlink("/server");

}