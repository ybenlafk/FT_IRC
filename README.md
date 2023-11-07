# IRC Server

This project is an IRC (Internet Relay Chat) server implemented in C++ using socket programming. The server allows multiple IRC clients to connect and communicate with each other through channels and private messages.

## Features

- Multiple clients: The server supports multiple clients connecting simultaneously. Each client can join channels and participate in group conversations.

- Channel creation and management: Clients can create new channels, join existing channels, and leave channels. Channels provide a way for clients to communicate with each other in a group setting.

- Private messaging: Clients can send private messages to other clients connected to the server. This allows for one-on-one conversations.

- Server-client protocol: The server follows the IRC protocol, ensuring compatibility with IRC clients such as LimeChat. Clients can connect to the server using LimeChat or any other IRC client that supports the IRC protocol.

## Bonus Features

In addition to the core IRC server functionality, this project also includes some bonus features:

### File Transfer

The server supports file transfer between clients. Clients can send and receive files through private messages or channels.

### IRC Bot

An IRC bot is implemented in this project, providing some additional fun and utility. The bot can perform various tasks, including playing a random song on Spotify. To interact with the bot, connect to the server using an irc client (LimeChat is recommonded) ,use the following command:

````
PRIVMSG tchipa play // to play the music.
````
````
PRIVMSG tchipa stop // to stop it.
````
## Installation

1. Clone the repository to your local machine:

   ````
   git clone https://github.com/ybenlafk/FT_IRC.git
   ````

2. Navigate to the project directory:

   ````
   cd FT_IRC
   ````

3. Compile the source code using a C++ compiler. For example, using g++:

   - To run the mandatory part:
   ````
   make
   ````
   - To run the bonus part:
   ````
   make bonus
   ````
   
## Usage

1. Start the server by running the compiled executable:
   - To execute the mandatory part:
   ````
   ./ircserv <port> <password> // <port>: the port that the server is gonna runing on it, <password> irc server password.
   ````
   - To execute the mandatory part:
   ````
   ./ircbot <port> <hostname> <password> // <port>: the port that the bot is gonna runing on it, <hostname> the hostname of the machine that the server is runing on it, <password> irc server password.
   ````
1. Configure your IRC client (e.g., LimeChat) to connect to the server. Use the following settings:

   - Server address: localhost (or the IP address of the machine running the server)
   - Port: <port> (default IRC port)
   - password: <password> use the password that you used to run the server

2. Connect your IRC client to the server.

3. Use your IRC client to join channels, send messages, and interact with other connected clients.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please follow these steps:

1. Fork the repository.

2. Create a new branch for your feature or bug fix:

   ````
   git checkout -b feature/your-feature-name
   ````

3. Make the necessary changes and commit them:

   ````
   git commit -m "Add your commit message"
   ````

4. Push your changes to your forked repository:

   ````
   git push origin feature/your-feature-name
   ````

5. Open a pull request on the original repository and provide a clear description of your changes.

