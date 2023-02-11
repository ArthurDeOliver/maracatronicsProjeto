#ifndef ACTUATOR_H
#define ACTUATOR_H
/* Recebe 60 pacotes por segundo, mandaremos na mesma frequência */
#include <QUdpSocket>
#include </home/arthur/Documentos/maraca_trainee_prog/comunicacao/protobufs/grSim_Packet.pb.h>

class Actuator
{
public:
    Actuator(QString actuatorAddress, quint16 actuatorPort);
    //criando função para mandar comando pros robôs
    void sendCommand(bool isYellow, int id, float vx, float vy, float vw);


private:
    QUdpSocket *_actuatorSocket;
    QString _actuatorAddress;
    quint16 _actuatorPort;

    void connectToNetwork();
};

#endif // ACTUATOR_H
