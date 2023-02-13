#include <QCoreApplication>
#include <vision/vision.h>
#include <iostream>
#include <actuator/actuator.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cmath>
#include <math.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    vision *visao = new vision("224.5.23.2",10020);
    Actuator *atuador = new Actuator("127.0.0.1", 20011);
    int c;
    float targetPosition[2] = {0.0,0.0};
    float distancia[2][2];
    //fazer os floats e dobles terem apenas 1 casa decimal
    std::cout << std::fixed << std::setprecision(2);
    float vx = 1.0;
    float vy = 1.0;
    float vw = 0.0;
    float raio;
    double angulo;
    float ang;


    const double pi = std::acos(-1);

    while(true){

        visao -> run();

       /* Essa parte do código serve para perceber se os pacotes de dados estão sendo recebidos
de forma correta, para assim conseguimos aplicar controle dos robôs e ter juntamente com a info
da posição da bola. */

        //fazedo o robô ir até a bola
        SSL_DetectionBall ball = visao->getball();
        SSL_DetectionRobot blueRobot = visao->getRobot(false,3);

        //parte 1: fazer algum robô girar
//       std::cin >> c;

//      if(c == 1){
//          atuador->sendCommand(false,3,2.0,0.0,2.0);
        //  }
//      else if(c == 2){
//          atuador->sendCommand(false,3,0.0,0.0,1.5);
//          }

        //parte 2: seguir a bola
        targetPosition[0] = (ball.x() - blueRobot.x())/1000;
        targetPosition[1] = (ball.y() - blueRobot.y())/1000;

        raio = pow(targetPosition[0],2) + pow(targetPosition[1],2);
        raio = sqrt(raio);

        ang = blueRobot.orientation();

        angulo = (atan((targetPosition[1])/(targetPosition[0])));

        vx = targetPosition[0]*cos(ang) + targetPosition[1]*sin(ang);
        vy = targetPosition[1]*cos(ang) - targetPosition[0]*sin(ang);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        atuador->sendCommand(false,3,vx,vy,0.0);


        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return a.exec();
}
