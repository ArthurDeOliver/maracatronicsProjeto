#include "vision.h"

vision::vision(QString visionAddress, quint16 visionPort){
    _visionAddres = visionAddress;
    _visionPort = visionPort;
    _visionSocket = new QUdpSocket();

    connecToNetwork();
// Receber o endereço e porta para fazar a conexão e chamar o método da conexão com a rede
}

//criação da conexão da rede
void vision::connecToNetwork(){
    _visionSocket->bind(QHostAddress(_visionAddres), _visionPort, QUdpSocket::ShareAddress);
    //QHostAddress -> Converte a string para o tipo QHostadrress

// multicast group: endereços da rede que podem se conectar e receber pacotes e grupos, mesmo conceito de bind
    _visionSocket->joinMulticastGroup(QHostAddress(_visionAddres));
}

void vision::run(){
    //enquanto tiver datagramas pendentes no socket, vai pegar o datagramas e processar eles
    while(_visionSocket->hasPendingDatagrams()){

        QNetworkDatagram datagram; //objetos que pegará os datagramas, conjunto de bytes
        datagram = _visionSocket->receiveDatagram(); //recebe o datagrama

        //processar e transformar ele em uma classe para protobuf
        SSL_WrapperPacket packet; //contém detecção e geometria

        //pegar um array de bytes e dar um parse para o protocolo protobuf
        packet.ParseFromArray(datagram.data().data(), datagram.data().size());

        // checando se existe detecção
        if(packet.has_detection()){

            //etapa de captação dos dados de bola, robôs azuis e amarelos
            SSL_DetectionFrame visionFrame = packet.detection(); //mensagem que contem dentro dela pacotes de informações dos robôs: posição X,Y robô X,Y bola

            //checando se tem bola
            if(visionFrame.balls_size() > 0){
                //ele tem uma bola
                _ball = visionFrame.balls(0);//classe q armazena a posição xy da bola

            }

            //checar se existem robôs azuis (time tem robôs azuis e amarelos)
            if(visionFrame.robots_blue_size() > 0){ //quantidade de azuis detectados > 0
                //armazenar cada robô detectado
                for( int i = 0; i < visionFrame.robots_blue_size(); i++){
                    SSL_DetectionRobot robot = visionFrame.robots_blue(i); //tem todas as informações de um robô com índice i
                    _blueRobots.insert(robot.robot_id(), robot);//entrando no map do robôs azuis, toda vez que gera um novo, apago o antigo
                }
            }

            if(visionFrame.robots_yellow_size() > 0){ //quantidade de amarelos detectados > 0
                //armazenar cada robô detectado
                for( int i = 0; i < visionFrame.robots_yellow_size(); i++){
                    SSL_DetectionRobot robot = visionFrame.robots_yellow(i); //tem todas as informações de um robô com índice i
                    _yellowRobots.insert(robot.robot_id(), robot);
                }
            }
        }
    }
}

SSL_DetectionBall vision::getball(){
    return _ball;
}

SSL_DetectionRobot vision::getRobot(bool isYellow, int id){
    //garantir que o id tá registrado
    if(isYellow){
        if(_yellowRobots.contains((id))){
            return _yellowRobots.value(id);
    }
        else{
            return SSL_DetectionRobot();
        }
    }
    else{
        if(_blueRobots.contains((id))){
            return _blueRobots.value(id);
    }
        else{
            return SSL_DetectionRobot(); //objeto default
        }
    }
}
