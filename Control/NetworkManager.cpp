#include "NetworkManager.h"

#include "../Control/Game.h"
#include "../GameObjects/Player.h"
#include "../Control/ObjectManager.h"


// SERVIDOR

// Funci�n que acepta a los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::acceptPlayers()
{
	while (!exitThread) {
		TCPsocket csd;
		if ((csd = SDLNet_TCP_Accept(socket)))
		{
			IPaddress* remoteIP;

			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				std::cout << ("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port)) << std::endl;
			else
				std::cout << ("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError()) << std::endl;

			int id = getClientID(*remoteIP);

			PacketAccept pkt;
			pkt.packet_type = EPT_ACCEPT;

			if (id_count >= MAX_PLAYERS) { // TODO: CHECK IF A PLAYER IS DISCONNECTED
				pkt.packet_type = EPT_DENY;
			}
			else {
				if (id == -1) {
					Player* p = addPlayerHost();

					game_->getObjectManager()->addPlayer(p);
					id = p->getId();
				}

				player_sockets[id] = csd;
				player_ips[id] = *remoteIP;
			}

			pkt.player_id = id;

			if (SDLNet_TCP_Send(csd, &pkt, sizeof(PacketAccept)) < sizeof(PacketAccept))
			{
				std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		SDL_Delay(accept_frequency);
	}
}

// Funci�n que se encarga de actualizar los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::receivePlayers()
{
	while (!exitThread) {
		PacketRecv packet;

		for (int i = 1; i < id_count; i++) { // Comenzamos en uno porque el 0 somos nosotros mismos
			if (SDLNet_TCP_Recv(player_sockets[i], &packet, sizeof(PacketRecv)) > 0)
			{
				switch (packet.packet_type) {
				case EPT_UPDATE:
					game_->getObjectManager()->getPlayers()[i]->handleInput(Vector2D<double>(packet.player_horizontal, packet.player_vertical));
					break;
				case EPT_QUIT:
					std::cout << ("Client disconnected: ID(%d)\n", i) << std::endl;

					SDLNet_TCP_Close(player_sockets[i]);
					player_sockets[i] = NULL;
					break;
				}
			} // TODO: CHECK WHEN TIRAR DEL CABLE
			  // TODO: SYNC PLAYERS?
		}

		SDL_Delay(recv_frequency);
	}
}


// Funci�n que se encarga de mandar la informaci�n de todos los jugadores a todos los jugadores (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::sendPlayers()
{
	while (!exitThread) {
		// UPDATE CLIENTS
		Player* p;
		PacketSend packet;
		packet.packet_type = EPT_UPDATE;

		// Send each player to the other players
		for (int i = 0; i < id_count; i++) {
			p = players[i];
			packet.player_id = i;
			packet.player_horizontal = p->getAxis().getX();
			packet.player_vertical = p->getAxis().getY();

			for (int j = 1; j < id_count; j++) { // Empezamos en uno porque el servidor ya tiene su propio hilo
												 // que recibe los datos de los clientes, por tanto no hace falta mandar nada.
				if (i != j) {
					if (SDLNet_TCP_Send(player_sockets[j], &packet, sizeof(PacketSend)) < sizeof(PacketSend))
					{
						std::cout << (stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
						exit(EXIT_FAILURE);
					}
				}
			}
		}

		SDL_Delay(send_frequency);
	}
}


// Funci�n que tienen los clientes (se encuentra en un hilo)
// Problemas que da: ---
void NetworkManager::updateClient()
{
	while (!exitThread) {
		PacketSend server_pkt;
		
		// Receive info from server
		if (SDLNet_TCP_Recv(socket, &server_pkt, sizeof(PacketSend)) > 0) {
			Player* p = players[server_pkt.player_id];

			if (p == nullptr) {
				p = addPlayerClient(server_pkt.player_id);
				game_->getObjectManager()->addPlayer(p);
			}

			switch (server_pkt.packet_type) {
			case EPT_UPDATE:
				p->handleInput(Vector2D<double>(server_pkt.player_horizontal, server_pkt.player_vertical));
				break;
			//case EPT_SYNCPLAYER:
			//	// parsear pkt
			//	PacketSyncPlayer newPkt = *(PacketSyncPlayer*)(void*)&server_pkt;
			//	p->setPosition(newPkt.posX, newPkt.posY);

			//	break;
			}
		}

		// Send info
		PacketRecv client_pkt;
		client_pkt.packet_type = EPT_UPDATE;
		
		client_pkt.player_horizontal = players[0]->getAxis().getX(); // Ponemos 0 porque somos nosotros mismos
		client_pkt.player_vertical = players[0]->getAxis().getY();

		if (SDLNet_TCP_Send(socket, &client_pkt, sizeof(PacketRecv)) < sizeof(PacketRecv))
		{
			std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
			exit(EXIT_FAILURE);
		}

		SDL_Delay(client_frequency);
	}
}

// Funci�n para asignar IDs
int NetworkManager::getClientID(const IPaddress& addr)
{
	int id = -1;

	for (int i = 0; i < id_count; i++) {
		if (compareAddress(player_ips[i], addr)) {
			id = i;
			break;
		}
	}

	return id;
}

bool NetworkManager::compareAddress(const IPaddress& addr1, const IPaddress& addr2)
{
	return addr1.host == addr2.host && addr1.port == addr2.port;
}

NetworkManager::NetworkManager(Game* game)
{
	exitThread = false;

	nType = '0';
	id_count = 0;

	game_ = game;

	accept_frequency = 150;
	recv_frequency = 50;
	send_frequency = 100;

	client_frequency = 50;
}

NetworkManager::~NetworkManager()
{
	SDLNet_TCP_Close(socket);
	SDLNet_Quit();
}

// Funci�n que inicializa SDL_Net y tu funci�n (servidor o cliente)
// Problemas que da: ---

bool NetworkManager::init(char type, const char* ip_addr)
{
	if (SDLNet_Init() < 0)
	{
		std::cout << ("SDLNet_Init: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}

	nType = type;

	if (SDLNet_ResolveHost(&ip, ip_addr, 2000) < 0)
	{
		std::cout << ("SDLNet_ResolveHost: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!(socket = SDLNet_TCP_Open(&ip)))
	{
		std::cout << ("SDLNet_TCP_Open: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}
	
	PacketAccept pkt;

	if (type == 'c') { // Si somos un cliente
		while (SDLNet_TCP_Recv(socket, &pkt, sizeof(PacketAccept)) == 0); // Esperamos a que el servidor nos acepte

		if (pkt.packet_type == EPT_ACCEPT) { // Cuando nos acepte, se crea el personaje, etc.
			game_->getObjectManager()->addPlayer(addPlayerClient(pkt.player_id));
			
			client_id = pkt.player_id;

			id_count = MAX_PLAYERS;

			updateclient_t = new std::thread(&NetworkManager::updateClient, this);
		}
		else if (pkt.packet_type == EPT_DENY) { // Si nos rechazan porque la partida est� llena
			return false;
		}
	}
	else { // Si somos un host
		game_->getObjectManager()->addPlayer(addPlayerHost());

		// Hilos
		accept_t = new std::thread(&NetworkManager::acceptPlayers, this);
		receiveplayers_t = new std::thread(&NetworkManager::receivePlayers, this);
		sendplayers_t = new std::thread(&NetworkManager::sendPlayers, this);

		player_sockets[0] = socket;
		player_ips[0] = ip;

		client_id = 0;
	}

	
	return true;
}


// Duda: Aqu� es donde se sincronizar�an las cosas?
void NetworkManager::update() // SINCRONIZAR ESTADO DE JUEGO CADA 0.5 SEGS
{
	if (lastUpdate_ + updateTime_ > sdlutils().currRealTime()) { //si no pasan
		return;
	}

	lastUpdate_ = sdlutils().currRealTime();
	
	//PacketSyncPlayer pkt_send;
	// Send status
	//for (int i = 0; i < id_count; i++) {
	//	
	//	pkt_send.packet_type = EPT_SYNCPLAYER;
	//	pkt_send.player_id = players[i]->getId();
	//	pkt_send.posX = players[i]->getPosition().getX();
	//	pkt_send.posY = players[i]->getPosition().getY();
	//	
	//	for (int j = 0; j < id_count; j++) {
	//		if (i != j) {
	//			if (SDLNet_TCP_Send(player_sockets[j], (void*)&pkt_send, sizeof(PacketSyncPlayer)) < sizeof(PacketSyncPlayer))
	//			{
	//				fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
	//				exit(EXIT_FAILURE);
	//			}
	//		}
	//	}
	//}

}

void NetworkManager::close()
{
	Player* player = players[client_id];

	PacketSend pkt;
	pkt.packet_type = EPT_QUIT;
	pkt.player_id = player->getId();

	if (SDLNet_TCP_Send(socket, &pkt, sizeof(PacketSend)) < sizeof(PacketSend))
	{
		std::cout << ("SDLNet_TCP_Send: %s\n", SDLNet_GetError()) << std::endl;
		exit(EXIT_FAILURE);
	}

	exitThread = true;

	if (nType == 'h') {
		accept_t->join();
		receiveplayers_t->join();
		sendplayers_t->join();

		delete accept_t;
		delete receiveplayers_t;
		delete sendplayers_t;
	}
	else {
		updateclient_t->join();
		delete updateclient_t;
	}
	
	SDLNet_TCP_Close(socket);
}

Player* NetworkManager::addPlayerHost()
{
	Player* p = new Player(game_);
	
	p->setId(id_count);

	players[id_count] = p;

	id_count++;

	return p;
}

Player* NetworkManager::addPlayerClient(int id)
{
	Player* p = new Player(game_);
	
	p->setId(id);

	players[id] = p;

	return p;
}
