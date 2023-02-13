#ifndef VISION_H
#define VISION_H

#include <QUdpSocket>
#include </home/arthur/Documentos/maraca_trainee_prog/comunicacao/protobufs/ssl_vision_wrapper.pb.h>
#include <QNetworkDatagram>
/*
    Classe responsável por faazer a captura dos dados do campo e salvar
    em uma variável, vetor...
    Captura de dados na rede pra deixar salvo e ser usada em momentos que for
    instâncida.
*/
class vision{

public:
    vision(QString visionAddres, quint16 port);
    void run(); //método responsável por fazer a captura dos dados e fazer a interpretação desses dados

    //consultar os objetos do campo que foram capiturados
    SSL_DetectionBall getball();
    SSL_DetectionRobot getRobot(bool isYellow,int id);
private:
    QUdpSocket *_visionSocket; //ponteiro *
    //variáveis para receber os pacotes
    //endereço e porta que o socket será conectado
    QString _visionAddres;
    quint16 _visionPort;

    void connecToNetwork(); //método para conexão com a rede

    SSL_DetectionBall _ball;
    //mapear uma variável de um tipo para outro tipo, associar o identificador a um robô
    QMap<int, SSL_DetectionRobot> _blueRobots;
    QMap<int, SSL_DetectionRobot> _yellowRobots;

};

#endif // VISION_H
