#include <QCoreApplication>
#include <vision/vision.h>
#include <iostream>
#include <actuator/actuator.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    vision *visao = new vision("224.5.23.2",10020);
    Actuator *atuador = new Actuator("127.0.0.1", 20011);

    while(true){
        visao -> run();

       /* Essa parte do código serve para perceber se os pacotes de dados estão sendo recebidos
de forma correta, para assim conseguimos aplicar controle dos robôs e ter juntamente com a info
da posição da bola. */

        // debugar posição da bola
        SSL_DetectionBall ball = visao->getball();

        // debugar posição do robô

        //posição robô amarelo
        SSL_DetectionRobot yellowRobot = visao->getRobot(true, 0);
        std::cout << "robot YELLOW 0: " << yellowRobot.x()<< " | " << yellowRobot.y() << std::endl;

        //se a posição do robô em X for menos negativa
        if(yellowRobot.x() < 0){
            //faça o robô andar em ré
            atuador->sendCommand(true, 0, -2.0, 0.0, 0.0);
            // robô é yellow?/id do robô/velocidade X/velocidade Y/velocidade do chute

        }
        else{
            //caso contrário ele andará para frente
            atuador->sendCommand(true, 0, 2.0, 0.0, 0.0);
        }

        //posição robô azul
        SSL_DetectionRobot blueRobot = visao->getRobot(false,0);
     //   std::cout << "robot BLUE 0: " << blueRobot.x()<< " | " << blueRobot.y() << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return a.exec();
}
