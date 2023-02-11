#include "actuator.h"

Actuator::Actuator(QString actuatorAddress, quint16 actuatorPort){
    _actuatorAddress = actuatorAddress;
    _actuatorPort = actuatorPort;
    _actuatorSocket = new QUdpSocket;

    connectToNetwork();
}

void Actuator::connectToNetwork(){ //serviremos como cliente, servidores robôs
    //ter mais de uma instância conectado mais de um tempo, verificação para n ter crash
    if(_actuatorSocket->isOpen()){
        _actuatorSocket->close();
    }

    _actuatorSocket->connectToHost(_actuatorAddress,_actuatorPort, QIODevice::WriteOnly, QAbstractSocket::IPv4Protocol);
}

void Actuator::sendCommand(bool isYellow, int id, float vx, float vy, float vw){
    //criando grsim packet
    grSim_Packet packet;

    //informações macro, mais gerais: cor do time e time stamp
    packet.mutable_commands()->set_isteamyellow(isYellow);
    packet.mutable_commands()->set_timestamp(0.0);

    //criar um objeto de comando pro robô
    grSim_Robot_Command *command = packet.mutable_commands()->add_robot_commands();//expressa todos os comandos em uma classe só

    //setar comandos
    //set id do robô
    command->set_id(id);

    //setar a velocidade
    command->set_wheelsspeed(false);
    command->set_veltangent(vx);
    command->set_velnormal(vy);
    command->set_velangular(vw);
    //chute no z-> parabólico

    //velocidade do chute
    command->set_kickspeedx(0.0);
    command->set_kickspeedz(0.0);

    //set spinner
    command->set_spinner(false);

    //mandar para o simulador

    //transformar o pacote packet em um vetor de bytes
    std::string buffer;
    packet.SerializeToString(&buffer);//transformando o pacote transformado em uma string, um vetor de bytes

    //mandar os dados na rede
    _actuatorSocket->write(buffer.c_str(), buffer.length());


}


